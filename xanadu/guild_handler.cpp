//

#include "player.hpp"

#include "packetcreator.hpp"
#include "guild.hpp"
#include "guild_member.hpp"
#include "map.hpp"
#include "world.hpp"

void Player::handle_guild_action()
{
	signed char action = read<signed char>();

	switch (action)
	{
	case 2: // Create a guild
	{
		if (guild_)
		{
			return;
		}
		std::string &name = read<std::string>();
		if (name.length() < 4 || name.length() > 12 || World::get_instance()->guild_name_used(name))
		{
			return;
		}
		guild_rank_ = 1;
		World::get_instance()->create_guild(this, name);

		break;
	}
	case 5: // Invite a player
	{
		if (!guild_ || guild_rank_ > 2)
		{
			return;
		}
		std::string &target = read<std::string>();
		Player *invited = World::get_instance()->GetPlayerByName(target);
		if (!invited || invited->get_guild())
		{
			return;
		}
		// send a packet
		PacketCreator packet21;
		packet21.InviteGuild(this);
		invited->send_packet(&packet21);

		break;
	}
	case 6: // Join a guild
	{
		if (guild_)
		{
			return;
		}

		int guildid = read<int>();
		World::get_instance()->join_guild(this, guildid);

		break;
	}
	case 7: // Leave a guild
	{
		if (!guild_ || guild_rank_ == 1)
		{
			return;
		}
		// send a packet
		PacketCreator packet22;
		packet22.GuildPlayerLeave(guild_->get_id(), id_, name_);
		guild_->send_packet(&packet22);
		//
		guild_->DeleteMember(id_);
		guild_ = nullptr;
		guild_rank_ = 5;

		break;
	}
	case 8: // Expell a player
	{
		int target_player_id = read<int>();
		std::string target_player_name = read<std::string>();

		if (!guild_ || guild_rank_ > 2)
		{
			return;
		}

		World::get_instance()->expel_guild_member(guild_, target_player_id, target_player_name);

		break;
	}
	case 13: // Change the title's
	{
		if (!guild_ || guild_rank_ != 1)
		{
			return;
		}

		std::string rank1 = read<std::string>();
		std::string rank2 = read<std::string>();
		std::string rank3 = read<std::string>();
		std::string rank4 = read<std::string>();
		std::string rank5 = read<std::string>();

		guild_->SetRank1(rank1, false);
		guild_->SetRank2(rank2, false);
		guild_->SetRank3(rank3, false);
		guild_->SetRank4(rank4, false);
		guild_->SetRank5(rank5, false);

		// send a packet
		PacketCreator packet24;
		packet24.UpdateGuildRanks(guild_);
		guild_->send_packet(&packet24);

		break;
	}
	case 14: // Change someones rank
	{
		int target_player_id = read<int>();
		signed char new_rank = read<signed char>();

		if (!guild_ || guild_rank_ > 2 || (new_rank == 2 && guild_rank_ != 1) || new_rank < 2 || new_rank > 5)
		{
			return;
		}

		GuildMember *member = guild_->GetMember(target_player_id);
		if (!member)
		{
			return;
		}
		member->set_guild_rank(new_rank);

		Player *target_player = World::get_instance()->GetPlayerById(target_player_id);
		if (target_player)
		{
			target_player->set_guild_rank(new_rank);
		}

		// send a packet
		PacketCreator packet25;
		packet25.ChangeRank(guild_->get_id(), target_player_id, new_rank);
		guild_->send_packet(&packet25);

		break;
	}
	case 15: // Change emblem
	{
		if (!guild_ || guild_rank_ != 1)
		{
			return;
		}

		guild_->SetLogoBackground(read<short>(), false);
		guild_->SetLogoBackgroundColor(read<signed char>(), false);
		guild_->SetLogo(read<short>(), false);
		guild_->SetLogoColor(read<signed char>(), false);

		// send a packet
		PacketCreator packet26;
		packet26.ShowGuildEmblem(guild_);
		guild_->send_packet(&packet26);

		break;
	}
	case 16: // Change the notice
	{
		if (!guild_ || guild_rank_ > 2)
		{
			return;
		}

		std::string &notice = read<std::string>();
		guild_->SetNotice(notice, false);

		// send a packet
		PacketCreator packet27;
		packet27.UpdateGuildNotice(guild_);
		guild_->send_packet(&packet27);

		break;
	}
	}
}

void Player::handle_guild_bbs_action()
{
	signed char action = read<signed char>();
	int local_thread_id = 0;

	enum actions
	{
		kCreateOrEditThread,
		kDeleteThread,
		kListThreads,
		kListThreadAndReply,
		kReply,
		kDeleteReply
	};

	switch (action)
	{
	case kCreateOrEditThread:
	{
		bool b_edit = (read<signed char>() == 1 ? true : false);
		if (b_edit)
		{
			local_thread_id = read<int>();
		}
		
		bool b_notice = (read<signed char>() == 1 ? true : false);
		std::string title = read<std::string>(); // String title = correctLength(slea.readMapleAsciiString(), 25);
		std::string text = read<std::string>(); // String text = correctLength(slea.readMapleAsciiString(), 600);
	
		int icon = read<int>();
		if (icon >= 0x64 && icon <= 0x6a)
		{
			int item_test_id = (5290000 + icon - 0x64);
			int item_test_amount = get_item_amount(item_test_id);
			if (item_test_amount == 0)
			{
				return;
			}
		}
		else if (!(icon >= 0 && icon <= 2))
		{
			return;
		}
	
		if (!b_edit)
		{
			// newBBSThread(c, title, text, icon, bNotice);
		}
		else
		{
			// editBBSThread(c, title, text, icon, localthreadid);
		}
		break;
	}
	case kDeleteThread:
	{
		local_thread_id = read<int>();
		// deleteBBSThread(c, localthreadid);
		break;
	}
	case kListThreads:
	{
		int start = read<int>();

		// to-do get guild bbs data from cache or mysql and use it in the packet

		PacketCreator packet50;
		packet50.guild_bbs_thread_list(start * 10);
		send_packet(&packet50);
		break;
	}
	case kListThreadAndReply: // List thread + reply, followed by id (int).
	{
		local_thread_id = read<int>();
		// displayThread(c, localthreadid);
		break;
	}
	case kReply:
	{
		local_thread_id = read<int>();
		std::string text = read<std::string>(); // text = correctLength(slea.readMapleAsciiString(), 25);
		// newBBSReply(c, localthreadid, text);
		break;
	}
	case kDeleteReply:
	{
		local_thread_id = read<int>();
		int reply_id = read<int>();
		// deleteBBSReply(c, replyid);
		break;
	}
	}
}

/*    @Override
	public void handlePacket(SeekableLittleEndianAccessor slea, MapleClient c) {
		c.getPlayer().resetAfkTime();
		if (!MapleGuild.ENABLE_BBS) {
			c.getSession().write(MaplePacketCreator.serverNotice(1, "Your server administrator has currently disabled Guild BBS."));
			return;
		}

		if (c.getPlayer().getGuildId() <= 0) {
			return;
		}
		byte mode = slea.readByte();
		int localthreadid = 0;

		switch (mode) {
			case 0:
				boolean bEdit = slea.readByte() == 1 ? true : false;
				if (bEdit) {
					localthreadid = slea.readInt();
				}
				boolean bNotice = slea.readByte() == 1 ? true : false;
				String title = correctLength(slea.readMapleAsciiString(), 25);
				String text = correctLength(slea.readMapleAsciiString(), 600);
				int icon = slea.readInt();
				if (icon >= 0x64 && icon <= 0x6a) {
					if (!c.getPlayer().haveItem(5290000 + icon - 0x64, 1, false, true)) {
						return;
					}
				} else if (!(icon >= 0 && icon <= 2)) {
					return;
				}
				if (!bEdit) {
					newBBSThread(c, title, text, icon, bNotice);
				} else {
					editBBSThread(c, title, text, icon, localthreadid);
				}
				break;
		*/