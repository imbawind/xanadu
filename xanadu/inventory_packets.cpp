//

#include "packetcreator.hpp"

#include "item_data_provider.hpp"
#include "item_data.hpp"
#include "item.hpp"
#include "send_packet_opcodes.hpp"
#include "constants.hpp"

/*
from v0.62 GMS extract:

void *__thiscall equip_iteminfo_sub_4B5F21(void *this, int a2)
{
int v2; // edi@1
void *v3; // esi@1
int v4; // ecx@1
int v5; // ecx@1
int v6; // eax@1
int v7; // eax@1
int v8; // eax@1
int v9; // eax@1
int v10; // eax@1
int v11; // eax@1
int v12; // eax@1
int v13; // eax@1
int v14; // eax@1
int v15; // eax@1
int v16; // eax@1
int v17; // eax@1
int v18; // eax@1
int v19; // eax@1
int v20; // eax@1
_DWORD *v21; // eax@1
int v22; // eax@3
void *result; // eax@3

v2 = a2;
v3 = this;
sub_4B5A68(this, a2);
LOBYTE(v4) = CInPacket::Decode1(v2);
*(_DWORD *)((char *)v3 + 42) = sub_4B8C01(v4, (char *)v3 + 40);
LOBYTE(v5) = CInPacket::Decode1(v2);
*((_DWORD *)v3 + 12) = sub_4B8C01(v5, (char *)v3 + 46);
LOWORD(v6) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 14) = sub_4B8C34(v6, (char *)v3 + 52);
LOWORD(v7) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 16) = sub_4B8C34(v7, (char *)v3 + 60);
LOWORD(v8) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 18) = sub_4B8C34(v8, (char *)v3 + 68);
LOWORD(v9) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 20) = sub_4B8C34(v9, (char *)v3 + 76);
LOWORD(v10) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 22) = sub_4B8C34(v10, (char *)v3 + 84);
LOWORD(v11) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 24) = sub_4B8C34(v11, (char *)v3 + 92);
LOWORD(v12) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 26) = sub_4B8C34(v12, (char *)v3 + 100);
LOWORD(v13) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 28) = sub_4B8C34(v13, (char *)v3 + 108);
LOWORD(v14) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 30) = sub_4B8C34(v14, (char *)v3 + 116);
LOWORD(v15) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 32) = sub_4B8C34(v15, (char *)v3 + 124);
LOWORD(v16) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 34) = sub_4B8C34(v16, (char *)v3 + 132);
LOWORD(v17) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 36) = sub_4B8C34(v17, (char *)v3 + 140);
LOWORD(v18) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 38) = sub_4B8C34(v18, (char *)v3 + 148);
LOWORD(v19) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 40) = sub_4B8C34(v19, (char *)v3 + 156);
LOWORD(v20) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 42) = sub_4B8C34(v20, (char *)v3 + 164);
v21 = (_DWORD *)CInPacket::DecodeStr(&a2);
lstrcpy((char *)v3 + 188, *v21);
if ( a2 )
sub_434E38((volatile LONG *)(a2 - 12));
LOWORD(v22) = CInPacket::Decode2(v2);
*((_DWORD *)v3 + 44) = sub_4B8C34(v22, (char *)v3 + 172);
result = (void *)(*((_DWORD *)v3 + 7) | *((_DWORD *)v3 + 6));
if ( *((_QWORD *)v3 + 3) )
{
*((_DWORD *)v3 + 45) = 0;
*((_DWORD *)v3 + 46) = 0;
}
else
{
result = CInPacket::DecodeBuffer(v2, (char *)v3 + 180, 8u);
}
return result;
}

from v0.95 GMS extract:

void __thiscall GW_ItemSlotEquip::RawEncode(GW_ItemSlotEquip *this, COutPacket *oPacket)
{
GW_ItemSlotEquip *v2; // ebx@1
char v3; // al@1
char v4; // al@1
unsigned __int16 v5; // ax@1
unsigned __int16 v6; // ax@1
unsigned __int16 v7; // ax@1
unsigned __int16 v8; // ax@1
unsigned __int16 v9; // ax@1
unsigned __int16 v10; // ax@1
unsigned __int16 v11; // ax@1
unsigned __int16 v12; // ax@1
unsigned __int16 v13; // ax@1
unsigned __int16 v14; // ax@1
unsigned __int16 v15; // ax@1
unsigned __int16 v16; // ax@1
unsigned __int16 v17; // ax@1
unsigned __int16 v18; // ax@1
unsigned __int16 v19; // ax@1
char *v20; // eax@2
char v21; // cl@3
int v22; // ebp@4
char *v23; // eax@4
unsigned __int16 v24; // ax@5
char v25; // al@5
char v26; // al@5
unsigned int v27; // eax@5
unsigned int v28; // eax@5
unsigned int v29; // eax@5
char v30; // al@5
char v31; // al@5
unsigned __int16 v32; // ax@5
unsigned __int16 v33; // ax@5
unsigned __int16 v34; // ax@5
unsigned __int16 v35; // ax@5
unsigned __int16 v36; // ax@5
ZXString<char> v37; // [sp-4h] [bp-14h]@1

v2 = this;
GW_ItemSlotBase::RawEncode((GW_ItemSlotBase *)&this->vfptr, oPacket);
v3 = _ZtlSecureFuse<unsigned char>(v2->item._ZtlSecureTear_nRUC, v2->item._ZtlSecureTear_nRUC_CS);
COutPacket::Encode1(oPacket, v3);
v4 = _ZtlSecureFuse<unsigned char>(v2->item._ZtlSecureTear_nCUC, v2->item._ZtlSecureTear_nCUC_CS);
COutPacket::Encode1(oPacket, v4);
v5 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niSTR, v2->item._ZtlSecureTear_niSTR_CS);
COutPacket::Encode2(oPacket, v5);
v6 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niDEX, v2->item._ZtlSecureTear_niDEX_CS);
COutPacket::Encode2(oPacket, v6);
v7 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niINT, v2->item._ZtlSecureTear_niINT_CS);
COutPacket::Encode2(oPacket, v7);
v8 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niLUK, v2->item._ZtlSecureTear_niLUK_CS);
COutPacket::Encode2(oPacket, v8);
v9 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niMaxHP, v2->item._ZtlSecureTear_niMaxHP_CS);
COutPacket::Encode2(oPacket, v9);
v10 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niMaxMP, v2->item._ZtlSecureTear_niMaxMP_CS);
COutPacket::Encode2(oPacket, v10);
v11 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niPAD, v2->item._ZtlSecureTear_niPAD_CS);
COutPacket::Encode2(oPacket, v11);
v12 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niMAD, v2->item._ZtlSecureTear_niMAD_CS);
COutPacket::Encode2(oPacket, v12);
v13 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niPDD, v2->item._ZtlSecureTear_niPDD_CS);
COutPacket::Encode2(oPacket, v13);
v14 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niMDD, v2->item._ZtlSecureTear_niMDD_CS);
COutPacket::Encode2(oPacket, v14);
v15 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niACC, v2->item._ZtlSecureTear_niACC_CS);
COutPacket::Encode2(oPacket, v15);
v16 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niEVA, v2->item._ZtlSecureTear_niEVA_CS);
COutPacket::Encode2(oPacket, v16);
v17 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niCraft, v2->item._ZtlSecureTear_niCraft_CS);
COutPacket::Encode2(oPacket, v17);
v18 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niSpeed, v2->item._ZtlSecureTear_niSpeed_CS);
COutPacket::Encode2(oPacket, v18);
v19 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_niJump, v2->item._ZtlSecureTear_niJump_CS);
COutPacket::Encode2(oPacket, v19);
v37._m_pStr = 0;
if ( v2 != (GW_ItemSlotEquip *)-48 )
{
v20 = v2->sTitle;
do
v21 = *v20++;
while ( v21 );
v22 = v20 - &v2->sTitle[1];
v23 = ZXString<char>::GetBuffer(&v37, v22, 0);
memcpy(v23, v2->sTitle, v22);
ZXString<char>::ReleaseBuffer(&v37, v22);
}
COutPacket::EncodeStr(oPacket, v37);
v24 = _ZtlSecureFuse<short>(v2->item._ZtlSecureTear_nAttribute, v2->item._ZtlSecureTear_nAttribute_CS);
COutPacket::Encode2(oPacket, v24);
v25 = _ZtlSecureFuse<unsigned char>(v2->item._ZtlSecureTear_nLevelUpType, v2->item._ZtlSecureTear_nLevelUpType_CS);
COutPacket::Encode1(oPacket, v25);
v26 = _ZtlSecureFuse<unsigned char>(v2->item._ZtlSecureTear_nLevel, v2->item._ZtlSecureTear_nLevel_CS);
COutPacket::Encode1(oPacket, v26);
v27 = _ZtlSecureFuse<long>(v2->item._ZtlSecureTear_nEXP, v2->item._ZtlSecureTear_nEXP_CS);
COutPacket::Encode4(oPacket, v27);
v28 = _ZtlSecureFuse<long>(v2->item._ZtlSecureTear_nDurability, v2->item._ZtlSecureTear_nDurability_CS);
COutPacket::Encode4(oPacket, v28);
v29 = _ZtlSecureFuse<long>(v2->item._ZtlSecureTear_nIUC, v2->item._ZtlSecureTear_nIUC_CS);
COutPacket::Encode4(oPacket, v29);
v30 = _ZtlSecureFuse<unsigned char>(v2->option._ZtlSecureTear_nGrade, v2->option._ZtlSecureTear_nGrade_CS);
COutPacket::Encode1(oPacket, v30);
v31 = _ZtlSecureFuse<unsigned char>(v2->option._ZtlSecureTear_nCHUC, v2->option._ZtlSecureTear_nCHUC_CS);
COutPacket::Encode1(oPacket, v31);
v32 = _ZtlSecureFuse<short>(v2->option._ZtlSecureTear_nOption1, v2->option._ZtlSecureTear_nOption1_CS);
COutPacket::Encode2(oPacket, v32);
v33 = _ZtlSecureFuse<short>(v2->option._ZtlSecureTear_nOption2, v2->option._ZtlSecureTear_nOption2_CS);
COutPacket::Encode2(oPacket, v33);
v34 = _ZtlSecureFuse<short>(v2->option._ZtlSecureTear_nOption3, v2->option._ZtlSecureTear_nOption3_CS);
COutPacket::Encode2(oPacket, v34);
v35 = _ZtlSecureFuse<short>(v2->option._ZtlSecureTear_nSocket1, v2->option._ZtlSecureTear_nSocket1_CS);
COutPacket::Encode2(oPacket, v35);
v36 = _ZtlSecureFuse<short>(v2->option._ZtlSecureTear_nSocket2, v2->option._ZtlSecureTear_nSocket2_CS);
COutPacket::Encode2(oPacket, v36);
if ( !v2->liCashItemSN.QuadPart )
COutPacket::EncodeBuffer(oPacket, &v2->liSN, 8u);
COutPacket::EncodeBuffer(oPacket, &v2->ftEquipped, 8u);
COutPacket::Encode4(oPacket, v2->nPrevBonusExpRate);
}

*/

void PacketCreator::ItemInfo(Item *item, bool show_position)
{
	int item_id = item->get_item_id();
	ItemData *item_data = ItemDataProvider::get_instance()->get_item_data(item_id);
	if (!item_data)
	{
		return;
	}

	enum ItemTypes
	{
		kEquip = 1,
		kItem = 2,
		kPet = 3
	};

	signed char item_type = (item->is_pet() ? kPet : item->is_equip() ? kEquip : kItem);

	bool is_cash = false;

	if (item_type == kPet)
	{
		is_cash = true;
	}

	if (item_id / 1000000 == 5)
	{
		is_cash = true;
	}

	// some checks for cash items
	// though if it's a pet, it already has the correct values due to the check above
	if (item_data->is_cash && item_type != kPet)
	{
		if (item->get_inventory_id() == kInventoryConstantsTypesEquip)
		{
			item_type = kEquip;
		}

		if (item->get_inventory_id() == kInventoryConstantsTypesCash)
		{
			item_type = kItem;
		}

		is_cash = true;
	}

	if (show_position)
	{
		char slot = item->get_slot();

		// equipped

		if (slot < 0)
		{
			slot *= -1;
			
			if (slot > 100)
			{
				is_cash = true;

				write<signed char>(slot - 100);
			}
			else
			{
				write<signed char>(slot);
			}
		}
		else // not equipped
		{
			write<signed char>(slot);
		}
	}

	write<signed char>(item_type);
	write<int>(item_id);
	write<bool>(is_cash);

	if (is_cash)
	{
		write<long long>(item->get_unique_id());
	}

	write<long long>(item->get_expiration_time());

	switch (item_type)
	{
	case kEquip:
	{
		write<signed char>(item->get_free_slots());
		write<signed char>(item->get_used_scrolls());
		write<short>(item->get_str());
		write<short>(item->get_dex());
		write<short>(item->get_int());
		write<short>(item->get_luk());
		write<short>(item->get_hp());
		write<short>(item->get_mp());
		write<short>(item->get_weapon_attack());
		write<short>(item->get_magic_attack());
		write<short>(item->get_weapon_defense());
		write<short>(item->get_magic_defense());
		write<short>(item->get_acc());
		write<short>(item->get_avoid());
		write<short>(item->get_hand());
		write<short>(item->get_speed());
		write<short>(item->get_jump());
		write<std::string>(item->get_owner_name());
		write<short>(item->get_flag());

		if (!is_cash)
		{
			write<long long>(0);
		}

		break;
	}

	case kItem:
	{
		write<short>(item->get_amount());
		write<std::string>(item->get_owner_name());
		write<short>(item->get_flag());

		if (item->is_star())
		{
			write<long long>(-1);
		}

		break;
	}

	case kPet:
	{
		write_string(item->get_name(), 13);
		write<signed char>(item->get_pet_level());
		write<short>(item->get_pet_closeness());
		write<signed char>(item->get_pet_fullness());
		write<long long>(item->get_expiration_time());
		write<int>(0);

		break;
	}
	}
}

void PacketCreator::UpdateSlot(std::shared_ptr<Item> item)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(1); // how many items to upgrade
	write<signed char>(1); // bag
	write<signed char>(item->get_inventory_id());
	write<short>(item->get_slot());
	write<short>(item->get_amount());
}

void PacketCreator::MoveItem(signed char inventory_id, short source_slot, short destination_slot)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(1); // how many items to update
	write<signed char>(2);
	write<signed char>(inventory_id);
	write<short>(source_slot);
	write<short>(destination_slot);
	write<signed char>(0);
}

void PacketCreator::NewItem(std::shared_ptr<Item> item, bool from_drop)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(from_drop); // unstuck the client
	write<signed char>(1); // how many items to add
	write<signed char>(0);
	write<signed char>(item->get_inventory_id());
	write<short>(item->get_slot());
	ItemInfo(item.get(), false);
}

void PacketCreator::remove_item(signed char inventory_id, int slot, bool from_drop)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(from_drop); // unstuck the client
	write<signed char>(1); // how many items to remove
	write<signed char>(3);
	write<signed char>(inventory_id);
	write<short>(slot);

	if (slot < 0)
	{
		write<signed char>(1);
	}
}

void PacketCreator::MoveItemMerge(signed char inventory_id, short source_slot, short destination_slot, short amount)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(2); // how many items to update
	write<signed char>(3);
	write<signed char>(inventory_id);
	write<short>(source_slot);
	write<signed char>(1);
	write<signed char>(inventory_id);
	write<short>(destination_slot);
	write<short>(amount);
}

void PacketCreator::MoveItemMergeTwo(signed char inventory_id, short source_slot, short source_amount, short destination_slot, short destination_amount)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(2); // how many items to update
	write<signed char>(1);
	write<signed char>(inventory_id);
	write<short>(source_slot);
	write<short>(source_amount);
	write<signed char>(1);
	write<signed char>(inventory_id);
	write<short>(destination_slot);
	write<short>(destination_amount);
}

void PacketCreator::ScrolledItem(std::shared_ptr<Item> scroll, std::shared_ptr<Item> equip, bool destroyed)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(destroyed ? 2 : 3);
	write<signed char>((scroll->get_amount() > 1) ? 1 : 3);
	write<signed char>(scroll->get_inventory_id());
	write<short>(scroll->get_slot());

	if (scroll->get_amount() > 1)
	{
		write<short>(scroll->get_amount() - 1);
	}

	write<signed char>(3);
	write<signed char>(kInventoryConstantsTypesEquip);
	write<short>(equip->get_slot());

	if (!destroyed)
	{
		write<signed char>(0);
		write<signed char>(kInventoryConstantsTypesEquip);
		write<short>(equip->get_slot());
		ItemInfo(equip.get(), false);
	}

	write<signed char>(1);
}
