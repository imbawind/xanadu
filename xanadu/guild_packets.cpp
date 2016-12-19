//

#include "packetcreator.hpp"

#include "guild.hpp"
#include "guild_member.hpp"
#include "player.hpp"
#include "send_packet_opcodes.hpp"

void PacketCreator::GuildMemberData(GuildMember *member)
{
	write_string(member->get_name(), 13);
	write<int>(member->get_job());
	write<int>(member->get_level());
	write<int>(member->get_guild_rank());
	write<int>(member->get_online_status());
	write<int>(member->get_signature());
	write<int>(member->get_alliance_rank());
}

void PacketCreator::CreateGuild()
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(1); // action
}

void PacketCreator::change_guild_emblem()
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(17); // action
}

void PacketCreator::GuildInfo(Guild *guild)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(26); // action
	write<bool>(true); // 0/false = empty guild window, 1/true = in guild
	write<int>(guild->get_id());
	write<std::string>(guild->get_name());
	write<std::string>(guild->GetRank1());
	write<std::string>(guild->GetRank2());
	write<std::string>(guild->GetRank3());
	write<std::string>(guild->GetRank4());
	write<std::string>(guild->GetRank5());
	{
		// guild member data
		auto members = guild->get_members();

		write<signed char>(static_cast<unsigned char>(members->size()));

		for (auto &it : *members)
		{
			write<int>(it.second->get_id());
		}
		for (auto &it : *members)
		{
			GuildMemberData(it.second.get());
		}
	}
	write<int>(guild->get_capacity());
	write<short>(guild->get_logo_background());
	write<signed char>(guild->get_logo_background_color());
	write<short>(guild->get_logo());
	write<signed char>(guild->get_logo_color());
	write<std::string>(guild->GetNotice());
	write<int>(guild->get_points());
	write<int>(guild->get_alliance_id());
}

void PacketCreator::AddGuildPlayer(int guild_id, GuildMember *member)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(39); // action
	write<int>(guild_id);
	write<int>(member->get_id());
	GuildMemberData(member);
}

void PacketCreator::GuildPlayerLeave(int guild_id, int player_id, std::string charname, bool expelled)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(expelled ? 47 : 44); // action; expelled = 47, leaving = 44
	write<int>(guild_id);
	write<int>(player_id);
	write<std::string>(charname);
}

void PacketCreator::InviteGuild(Player *inviter)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(5); // action
	write<int>(inviter->get_guild()->get_id());
	write<std::string>(inviter->get_name());
}

void PacketCreator::ChangeRank(int guild_id, int player_id, int rank)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(64); // action
	write<int>(guild_id);
	write<int>(player_id);
	write<signed char>(rank);
}

void PacketCreator::UpdateGuildRanks(Guild *guild)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(62); // action
	write<int>(guild->get_id());
	write<std::string>(guild->GetRank1());
	write<std::string>(guild->GetRank2());
	write<std::string>(guild->GetRank3());
	write<std::string>(guild->GetRank4());
	write<std::string>(guild->GetRank5());
}

void PacketCreator::UpdateGuildNotice(Guild *guild)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(68); // action
	write<int>(guild->get_id());
	write<std::string>(guild->GetNotice());
}

void PacketCreator::DisbandGuild(int guild_id)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(50); // action
	write<int>(guild_id);
	write<signed char>(1);
}

void PacketCreator::ShowGuildEmblem(Guild *guild)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(66); // action
	write<int>(guild->get_id());
	write<short>(guild->get_logo_background());
	write<signed char>(guild->get_logo_background_color());
	write<short>(guild->get_logo());
	write<signed char>(guild->get_logo_color());
}

void PacketCreator::GuildMemberOnline(int guild_id, int player_id, bool online)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(61); // action
	write<int>(guild_id);
	write<int>(player_id);
	write<bool>(online);
}

void PacketCreator::guild_bbs_add_thread()
{
	write<int>(0); // local_thread_id
	write<int>(0); // poster cid
	write<std::string>(""); // name
	write<long long>(0); // timestamp
	write<int>(0); // icon
	write<int>(0); // reply count
}

void PacketCreator::guild_bbs_thread_list(int start_thread)
{
	write<short>(send_headers::kGUILD_BBS_OPERATION);
	write<signed char>(0x06);

	// if there is no result
	bool has_entries = false;
	if (!has_entries)
	{
		write<signed char>(0);
		write<int>(0);
		write<int>(0);
		return;
	}

	int thread_count = 0; // get it from sql or from cache

	int local_thread_id = 0;
	bool has_notice = (local_thread_id == 0);
	write<bool>(has_notice);

	if (has_notice)
	{
		guild_bbs_add_thread();
		thread_count--; // one thread didn't count (because it's a notice)
	}

	/*if (!rs.absolute(start + 1)) { //seek to the thread before where we start

		rs.first(); // we're trying to start at a place past possible

		start = 0;
	}*/

	write<int>(thread_count);
	//write<int>(Math.min(10, threadCount - start));

	/*for (int i = 0; i < Math.min(10, threadCount - start); i++)
	{
		addThread(mplew, rs);
		rs.next();
	}*/
}

void PacketCreator::guild_bbs_show_thread(int local_thread_id)
{

}

/*
public static MaplePacket showThread(int localthreadid, ResultSet threadRS, ResultSet repliesRS) throws SQLException, RuntimeException {
MaplePacketLittleEndianWriter mplew = new MaplePacketLittleEndianWriter();

mplew.writeShort(SendPacketOpcode.BBS_OPERATION.getValue());
mplew.write(0x07);
mplew.writeInt(localthreadid);
mplew.writeInt(threadRS.getInt("postercid"));
mplew.writeLong(getKoreanTimestamp(threadRS.getLong("timestamp")));
mplew.writeMapleAsciiString(threadRS.getString("name"));
mplew.writeMapleAsciiString(threadRS.getString("startpost"));
mplew.writeInt(threadRS.getInt("icon"));
if (repliesRS != null) {
int replyCount = threadRS.getInt("replycount");
mplew.writeInt(replyCount);
int i;
for (i = 0; i < replyCount && repliesRS.next(); i++) {
mplew.writeInt(repliesRS.getInt("replyid"));
mplew.writeInt(repliesRS.getInt("postercid"));
mplew.writeLong(getKoreanTimestamp(repliesRS.getLong("timestamp")));
mplew.writeMapleAsciiString(repliesRS.getString("content"));
}
if (i != replyCount || repliesRS.next()) {
//in the unlikely event that we lost count of replyid
throw new RuntimeException(String.valueOf(threadRS.getInt("threadid")));
//we need to fix the database and stop the packet sending
//or else it'll probably error 38 whoever tries to read it

//there is ONE case not checked, and that's when the thread
//has a replycount of 0 and there is one or more replies to the
//thread in bbs_replies
}
} else {
mplew.writeInt(0); //0 replies

}
return mplew.getPacket();
}
*/
