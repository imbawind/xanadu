//

#include "player.hpp"

#include "map.hpp"
#include "packetcreator.hpp"
#include "inventory.hpp"
#include "world.hpp"
#include "cash_shop_item_data_provider.hpp"
#include "cash_shop_package_data_provider.hpp"
#include "cash_item_data.hpp"
#include "cashshop_constants.hpp"

void Player::handle_cash_shop_enter()
{
	in_cash_shop_ = true;
	map_->remove_player(this);

	{
		// send a packet
		PacketCreator packet;
		packet.EnterCashShop(this);
		send_packet(&packet);
	}

	{
		// send a packet
		PacketCreator packet;
		packet.GetCashShopInventory(cashshop_storage_items_, user_id_, storage_slots_, character_slots_);
		send_packet(&packet);
	}

	{
		// send a packet
		PacketCreator packet;
		packet.ShowCashPoints(nx_cash_credit_);
		send_packet(&packet);
	}
}

void Player::handle_leave_cash_shop()
{
	in_cash_shop_ = false;

	{
		// send a packet
		PacketCreator packet;
		packet.change_map(this, true);
		send_packet(&packet);
	}

	map_->add_player(this);
}

void Player::handle_update_cash_shop()
{
	{
		// send a packet
		PacketCreator packet;
		packet.ShowCashPoints(nx_cash_credit_);
		send_packet(&packet);
	}
}

void Player::handle_cash_shop_action()
{
	signed char action = read<signed char>();

	switch (action)
	{
	case CashShopReceivePacketActions::kBuyCashItem:
	{
		skip_bytes(1);
		skip_bytes(4); // type of cash used
		int serial_number = read<int>();

		CashItemData *cash_item = CashShopItemDataProvider::get_instance()->get_cash_item_data(serial_number);
		if (!cash_item)
		{
			return;
		}

		int price = cash_item->price;

		if (nx_cash_credit_ < price)
		{
			return;
		}

		int item_id = cash_item->item_id;
		short amount = cash_item->count;

		auto item = std::shared_ptr<Item>(new Item(item_id));
		item->set_amount(amount);
		item->set_commodity_id_sn(serial_number);

		cashshop_storage_items_.push_back(item);

		// to-do check if cash storage still has space for it
		// to-do send the cashshop error packet then

		nx_cash_credit_ -= price;

		{
			// send a packet
			PacketCreator packet;
			packet.ShowCashPoints(nx_cash_credit_);
			send_packet(&packet);
		}

		{
			// send a packet
			PacketCreator packet;
			packet.ShowBoughtCashItem(item, user_id_);
			send_packet(&packet);
		}

		break;
	}
	case CashShopReceivePacketActions::kBuyInventorySlots:
	{
		skip_bytes(1);
		skip_bytes(4); // type of cash used
		skip_bytes(1);
		signed char inventory_id = read<signed char>();
		Inventory *inventory = get_inventory(inventory_id);

		if (!inventory)
		{
			return;
		}

		if (inventory->get_slots() > 48)
		{
			return;
		}

		if (nx_cash_credit_ < 4000)
		{
			return;
		}

		nx_cash_credit_ -= 4000;
		inventory->add_slots(4);

		{
			// send a packet
			PacketCreator packet;
			packet.ShowCashPoints(nx_cash_credit_);
			send_packet(&packet);
		}

		// send a packet
		{
			PacketCreator packet;
			packet.IncreaseInventorySlots(inventory_id, inventory->get_slots());
			send_packet(&packet);
		}

		break;
	}
	case CashShopReceivePacketActions::kBuyStorageSlots:
	{
		skip_bytes(1);
		skip_bytes(4); // type of cash used
		skip_bytes(1);

		if (storage_slots_ > 48)
		{
			return;
		}

		if (nx_cash_credit_ < 4000)
		{
			return;
		}

		nx_cash_credit_ -= 4000;
		storage_slots_ += 4;

		{
			// send a packet
			PacketCreator packet;
			packet.ShowCashPoints(nx_cash_credit_);
			send_packet(&packet);
		}

		{
			// send a packet
			PacketCreator packet;
			packet.IncreaseStorageSlots(storage_slots_);
			send_packet(&packet);
		}

		break;
	}
	case CashShopReceivePacketActions::kBuyCharacterSlot:
	{
		skip_bytes(1);
		skip_bytes(4); // type of cash used
		int cash_sn = read<int>(); // ? sn of the character slot item maybe?

		if (character_slots_ < 21 && nx_cash_credit_ > 6700)
		{
			nx_cash_credit_ -= 6700;
			character_slots_ += 1;

			// to-do use original packet for it?
			{
				// send a packet
				PacketCreator packet;
				packet.ShowMessage("1 slot has been added to your account character slots.", 1);
				send_packet(&packet);
			}
		}

		break;
	}
	case CashShopReceivePacketActions::kRetrieveCashItem:
	{
		int cash_item_unique_id_sn = read<int>();

		for (auto it : cashshop_storage_items_)
		{
			if (it->get_unique_id() == cash_item_unique_id_sn)
			{

				Inventory *inventory = get_inventory(it->get_inventory_id());

				if (!inventory)
				{
					continue;
				}

				if (!inventory->add_item_find_slot(it))
				{
					// to-do send the cashshop error packet instead of those packets down there

					{
						// send a packet
						PacketCreator packet;
						packet.ShowCashPoints(nx_cash_credit_);
						send_packet(&packet);
					}

					{
						// send a packet
						PacketCreator packet;
						packet.EnableAction();
						send_packet(&packet);
					}

					continue;
				}

				// send a packet
				PacketCreator packet;
				packet.TakeOutFromCashShopInventory(it.get(), 1);
				send_packet(&packet);
			}
		}

		cashshop_storage_items_.clear();

		break;
	}
	/*case CashShopReceivePacketActions::kStoreCashItem:
		{
			//int cash_sn = read<int>();

			//send_packet(PacketCreator().transfer_to_cash_shop_inventory(user_id_, 0, 0, 0));

			break;
		}*/
	case CashShopReceivePacketActions::kBuyPackage:
	{
		skip_bytes(1);
		skip_bytes(4); // type of cash used
		int package_serial_number = read<int>();
		CashItemData *package_cash_item = CashShopItemDataProvider::get_instance()->get_cash_item_data(package_serial_number);
		if (!package_cash_item)
		{
			return;
		}

		int package_id = package_cash_item->item_id;
		std::vector<int> serial_numbers = CashShopPackageDataProvider::get_instance()->get_serial_numbers_in_cash_package(package_id);

		for (int serial_number : serial_numbers)
		{
			CashItemData *cash_item = CashShopItemDataProvider::get_instance()->get_cash_item_data(serial_number);
			if (!cash_item)
			{
				return;
			}
			int price = cash_item->price;
			if (nx_cash_credit_ < price)
			{
				return;
			}
			int item_id = cash_item->item_id;
			short amount = cash_item->count;

			auto item = std::shared_ptr<Item>(new Item(item_id));
			item->set_amount(amount);
			item->set_commodity_id_sn(serial_number);

			cashshop_storage_items_.push_back(item);

			// to-do check if cash storage still has space for it
			// to-do send the cashshop error packet then

			nx_cash_credit_ -= price;
			{
				// send a packet
				PacketCreator packet;
				packet.ShowCashPoints(nx_cash_credit_);
				send_packet(&packet);
			}

			// to-do maybe package has it's own packet?

			{
				// send a packet
				PacketCreator packet;
				packet.ShowBoughtCashItem(item, user_id_);
				send_packet(&packet);
			}
		}

		break;
	}
	}
}
