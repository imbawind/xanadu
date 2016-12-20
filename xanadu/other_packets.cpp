//

#include "packetcreator.hpp"

#include <string>

#include "buddy.hpp"
#include "effect.hpp"
#include "send_packet_opcodes.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "map.hpp"
#include "mob.hpp"
#include "mapnpc.hpp"
#include "party.hpp"
#include "party_member.hpp"
#include "attackinfo.hpp"
#include "quest.hpp"
#include "summon.hpp"
#include "skill_data_provider.hpp"
#include "skill_data.hpp"
#include "shop_data.hpp"
#include "shop_item_data.hpp"
#include "item_data.hpp"
#include "hiredmerchant_item.hpp"
#include "item_data_provider.hpp"
#include "channel.hpp"
#include "player.hpp"
#include "key.hpp"
#include "hiredmerchant.hpp"
#include "guild.hpp"
#include "guild_member.hpp"
#include "world.hpp"
#include "character.hpp"
#include "tools.hpp"
#include "buffstat_constants.hpp"
#include "tools.hpp"
#include "constants.hpp"

// monster carnival party quest packets

void PacketCreator::StartCarnivalPartyQuest(unsigned char team)
{
	write<short>(send_headers::kMONSTER_CARNIVAL_START);
	write<unsigned char>(team);
	write<short>(0); // Obtained CP - Used CP
	write<short>(0); // Total Obtained CP
	write<short>(0); // Obtained CP - Used CP of the team
	write<short>(0); // Total Obtained CP of the team
	write<short>(0); // Obtained CP - Used CP of the enemy team
	write<short>(0); // Total Obtained CP of the enemy team
	write_null(10); // one byte for each cpq mob?
}

void PacketCreator::obtain_cp()
{
	write<short>(send_headers::kMONSTER_CARNIVAL_OBTAINED_CP);
	write<short>(100); // Obtained CP - Used CP
	write<short>(100); // Total Obtained CP
}

void PacketCreator::obtain_party_cp(unsigned char team)
{
	write<short>(send_headers::kMONSTER_CARNIVAL_PARTY_CP);
	write<unsigned char>(team); // Team where the points are given to.
	write<short>(200); // Obtained CP - Used CP
	write<short>(200); // Total Obtained CP
}

void PacketCreator::carnival_pq_message(signed char message)
{
	write<short>(send_headers::kMONSTER_CARNIVAL_MESSAGE);
	write<signed char>(message);
}

void PacketCreator::carnival_pq_summon(signed char tab, signed char summon_number, std::string player_name)
{
	write<short>(send_headers::kMONSTER_CARNIVAL_SUMMON);
	write<signed char>(tab);
	write<signed char>(summon_number); // each mob in cpq has a specific summon number
	write<std::string>(player_name);
}

void PacketCreator::carnival_pq_died(signed char lost_cp, unsigned char team, std::string player_name)
{
	write<short>(send_headers::kMONSTER_CARNIVAL_DIED);
	write<unsigned char>(team);
	write<std::string>(player_name);
	write<signed char>(lost_cp);
}

// player_is_leader != 6: [teamname] of Team [playername] has quit the Monster Carnival.
// player_is_leader == 6: Since the leader of the Team [teamname] quit the Monster Carnival, [playername] has been appointed as the new leader of the team.

void PacketCreator::leave_carnival_pq(bool player_is_leader, unsigned char team, std::string player_name)
{
	write<short>(send_headers::kMONSTER_CARNIVAL_LEAVE);
	write<signed char>(player_is_leader ? 6 : 0);
	write<unsigned char>(team);
	write<std::string>(player_name);
}

// cpq_show_game_result
// 8: You have won the Monster Carnival. Please wait as you'll be transported out of here shortly.
// 9: Unfortunately, you have lost the Monster Carnival. Please wait as you'll be transported out of here shortly.
// 10: Despite the Overtime, the carnival ended in a draw. Please wait as you'll be transported out of here shortly.
// 11: Monster Carnival has ended abruptly due to the opposing team leaving the game too early. Please wait as you'll be transported out of here shortly.

void PacketCreator::cpq_show_game_result(signed char result)
{
	write<short>(send_headers::kMONSTER_CARNIVAL_SHOW_GAME_RESULT);
	write<signed char>(result);
}

// end of monster carnival party quest packets

// start of other packets

// kSHOW_STATUS_INFO
// CWvsContext::OnMessage
// not confirmed yet, but probably correct:
// 0 = droppickup
// 1 = questrecord
// 2 = cashitemexpire
// 3 = increase exp
// 4 = increase sp
// 5 = increase fame
// 6 = increase money
// 7 = increase guild points
// 8 = give buff message
// 9 = OnGeneralItemExpireMessage
// 10 = OnSystemMessage
// 11 = OnQuestRecordExMessage
// 12 = OnItemProtectExpireMessage
// 13 = OnItemExpireReplaceMessage
// 14 = OnSkillExpireMessage

void PacketCreator::GainExp(int exp, bool in_chat, bool white, int party_bonus)
{
	write<short>(send_headers::kSHOW_STATUS_INFO);
	write<signed char>(3); // 3 = increase exp, there are also much other types
	write<bool>(white); // white or yellow
	write<int>(exp); // amount of exp
	write<bool>(in_chat); // in chat or on screen
	write<int>(0); // Bonus Event EXP (+value)
	write<signed char>(0); // A bonus EXP value% is awarded for every 3rd monster defeated.
	write<signed char>(static_cast<signed char>(party_bonus)); // party bonus EXP
	write<int>(0); // Bonus Wedding EXP (+value)
	write<short>(0);
}

void PacketCreator::GainItem(int itemid, short amount)
{
	write<short>(send_headers::kSHOW_STATUS_INFO);
	write<signed char>(0); // 0 = drop pickup, there are also much other types
	write<signed char>(0); // 0 = item, 1 = mesos
	write<int>(itemid);
	write<int>(amount);
}

void PacketCreator::GainMesos(int amount)
{
	write<short>(send_headers::kSHOW_STATUS_INFO);
	write<signed char>(0); // 0 = drop pickup, there are also much other types
	write<signed char>(1); // 0 = item, 1 = mesos
	write<int>(amount);
	write<short>(0);
}

void PacketCreator::ShowAvatarMega(Player *player, unsigned char ear, int item_id, std::string message, std::string message2, std::string message3, std::string message4)
{
	write<short>(send_headers::kSHOW_AVATAR_MEGA);
	write<int>(item_id);
	write<std::string>(player->get_name());
	write<std::string>(message);
	write<std::string>(message2);
	write<std::string>(message3);
	write<std::string>(message4);
	write<int>(player->get_channel_id());
	write<signed char>(ear);
	AddCharLook(player, true);
}

void PacketCreator::HandleCloseChalkboard(int player_id)
{
	write<short>(send_headers::kCHALKBOARD);
	write<int>(player_id);
	write<signed char>(0);
}

void PacketCreator::UseChalkBoard(int player_id, const std::string &message)
{
	write<short>(send_headers::kCHALKBOARD);
	write<int>(player_id);
	write<signed char>(1);
	write<std::string>(message);
}

void PacketCreator::UseScroll(int player_id, bool success, bool cursed, bool legendary_spirit)
{
	write<short>(send_headers::kSHOW_SCROLL_EFFECT);
	write<int>(player_id);
	write<signed char>(success ? 1 : 0);
	write<signed char>(cursed ? 1 : 0);
	write<bool>(legendary_spirit);
	write<signed char>(0);
}

void PacketCreator::UseItemEffect(int player_id, int item_id)
{
	write<short>(send_headers::kSHOW_ITEM_EFFECT);
	write<int>(player_id);
	write<int>(item_id);
}

void PacketCreator::ShowChair(int player_id, int item_id)
{
	write<short>(send_headers::kSHOW_CHAIR);
	write<int>(player_id);
	write<int>(item_id);
}

void PacketCreator::CancelChair()
{
	write<short>(send_headers::kCANCEL_CHAIR);
	write<signed char>(0);
}

void PacketCreator::ShowPlayer(Player *player)
{
	write<short>(send_headers::kSPAWN_PLAYER);
	write<int>(player->get_id());
	write<std::string>(player->get_name());

	// guild info

	Guild *guild = player->get_guild();

	if (guild)
	{
		write<std::string>(guild->get_name());
		write<short>(guild->get_logo_background());
		write<signed char>(guild->get_logo_background_color());
		write<short>(guild->get_logo());
		write<signed char>(guild->get_logo_color());
	}
	else
	{
		write<std::string>("");
		write<short>(0);
		write<signed char>(0);
		write<short>(0);
		write<signed char>(0);
	}

	// end of guild info

	// buff info

	write<int>(0);
	write<int>(1);
	write<signed char>(0); // morph
	write<short>(0);
	write<unsigned char>(0xF8);

	long long buff_mask = 0;
	signed char buff_value = 0;

	if (player->is_buff_stat_active(buffstat_constants::kCombo))
	{
		buff_mask |= buffstat_constants::kCombo;
		buff_value = player->get_crusader_combo_value();
	}

	if (player->is_buff_stat_active(buffstat_constants::kShadowPartner))
	{
		int skill_id = 4111002;
		// 4211008
		// 14111000
		buff_mask |= buffstat_constants::kShadowPartner;
	}

	if (player->is_buff_stat_active(buffstat_constants::kDarksight))
	{
		buff_mask |= buffstat_constants::kDarksight;
	}

	if (player->get_mount_item_id() != 0)
	{
		buff_mask |= buffstat_constants::kMonsterRiding;
	}

	write<int>(static_cast<int>((buff_mask >> 32) & 0xffffffffL));

	if (buff_value != 0)
	{
		write<signed char>(buff_value);

		// morph has another byte
	}

	write<int>(static_cast<int>(buff_mask & 0xffffffffL));

	int CHAR_MAGIC_SPAWN = 4562313;

	write<int>(0);
	write<short>(0);
	write<int>(CHAR_MAGIC_SPAWN);
	write<int>(0);
	write<int>(0);
	write<short>(0);
	write<int>(CHAR_MAGIC_SPAWN);
	write<int>(0);
	write<int>(0);
	write<short>(0);
	write<int>(CHAR_MAGIC_SPAWN);
	write<short>(0);

	// mount info

	if (player->get_mount_item_id() != 0)
	{
		write<int>(player->get_mount_item_id());
		write<int>(player->get_mount_skill_id());
	}
	else
	{
		write<long long>(0);
	}

	write<int>(CHAR_MAGIC_SPAWN);
	write<long long>(0);
	write<int>(CHAR_MAGIC_SPAWN);
	write<long long>(0);
	write<int>(0);
	write<short>(0);
	write<int>(CHAR_MAGIC_SPAWN);
	write<int>(0);

	// end of buff info

	// player information, mainly looks

	write<short>(player->get_job());
	AddCharLook(player);
	write<int>(player->get_item_amount(5110000)); // hearts
	write<int>(player->get_item_effect());
	write<int>(player->get_chair());
	write<short>(player->get_position_x());
	write<short>(player->get_position_y());
	write<signed char>(player->get_stance());
	write<short>(0); // foothold
	write<signed char>(0);

	// pet info

	enum
	{
		kEndPetInfo = 0,
		kStartPetInfo = 1
	};

	auto pets = player->get_pets();
	for (auto &pet : *pets)
	{
		write<signed char>(kStartPetInfo);
		write<int>(pet->get_item_id());
		write<std::string>(pet->get_name());
		write<int>(pet->get_unique_id());
		write<int>(0);
		write<short>(pet->get_position_x());
		write<short>(pet->get_position_y());
		write<signed char>(pet->get_stance());
		write<short>(0); // foothold
		write<short>(0);
	}

	write<signed char>(kEndPetInfo);

	// mount info

	write<int>(1); // level
	write<int>(0); // exp
	write<int>(0); // tiredness/fatigue

	// playershop/minigame info

	bool has_minigame = false;

	write<bool>(has_minigame);

	if (has_minigame)
	{
		write<int>(0); // map object id
		write<std::string>("hello"); // description text
		write<signed char>(0); // specific if game if private?
		write<signed char>(10); // type
		write<signed char>(1); // amount of players that are already inside the minigame
		write<signed char>(2); // max players inside the minigame
		write<signed char>(0); // determines wether joinable or not? 1/0? unsure
	}

	// end of playershop/minigame info

	// chalkboard info

	std::string chalkboard_text = player->get_chalk_board();
	bool has_chalkboard = (chalkboard_text != "");

	write<bool>(has_chalkboard);

	if (has_chalkboard)
	{
		write<std::string>(chalkboard_text);
	}

	// end of chalkboard info

	// rings info

	bool has_couple_ring = false;

	write<bool>(has_couple_ring); // couple ring

	if (has_couple_ring)
	{
		// to-do
	}

	bool has_friendship_ring = false;

	write<bool>(has_friendship_ring); // friendship ring

	if (has_friendship_ring)
	{
		// to-do
	}

	bool has_marriage_ring = false;

	write<bool>(has_marriage_ring); // marriage ring

	if (has_marriage_ring)
	{
		// to-do
	}

	// end of rings info

	/*
	from lithium v111:

			mplew.write(chr.getStat().Berserk ? 1 : 0); // 0x1 = dark force, 0x2 = dragon, 0x4 = swallow (wild hunter?), for (0x8, 0x10 and 0x20, extra int)
		mplew.writeInt(0);
		mplew.write(0); // new year cards boolean

		mplew.writeInt(0); //no clue
		final boolean pvp = chr.inPVP();
		if (pvp) {
			mplew.write(Integer.parseInt(chr.getEventInstance().getProperty("type")));
		}
		if (chr.getCarnivalParty() != null) {
			mplew.write(chr.getCarnivalParty().getTeam());
		} else if (GameConstants.isTeamMap(chr.getMapId())) {
			mplew.write(chr.getTeam() + (pvp ? 1 : 0)); //is it 0/1 or is it 1/2?
		}
		*/

	write<signed char>(0); // carnival party quest team for some fields maybe?
}

void PacketCreator::RemovePlayer(Player *player)
{
	write<short>(send_headers::kREMOVE_PLAYER_FROM_MAP);
	write<int>(player->get_id());
}

// this packet shows effects in the map
// mode specifies the type of effect: 2 = object, 3 = effect, 4 = sound, 6 = map music
// name specifies the effect to be shown

void PacketCreator::MapEffect(signed char mode, std::string name)
{
	write<short>(send_headers::kMAP_EFFECT);
	write<signed char>(mode);
	write<std::string>(name);
}

void PacketCreator::ShowTimer(int seconds)
{
	write<short>(send_headers::kCLOCK);
	write<signed char>(2);
	write<int>(seconds);
}

void PacketCreator::PlayerAttack(signed char attack_type, PlayerAttackInfo &attack)
{
	switch (attack_type)
	{
	case attack_type_constants::kCloseRange:
		write<short>(send_headers::kCLOSE_RANGE_ATTACK);
		break;
	case attack_type_constants::kRanged:
		write<short>(send_headers::kRANGED_ATTACK);
		break;
	case attack_type_constants::kMagic:
		write<short>(send_headers::kMAGIC_ATTACK);
		break;
	case attack_type_constants::kEnergy:
		write<short>(send_headers::kENERGY_ATTACK);
		break;
	}

	write<int>(attack.player_id_);
	write<signed char>(attack.info_byte_);
	write<signed char>(attack.skill_level_);

	if (attack.skill_id_ > 0)
	{
		write<int>(attack.skill_id_);
	}

	write<signed char>(attack.direction_);
	write<signed char>(attack.stance_);
	write<signed char>(attack.weapon_speed_);
	write<signed char>(10);
	write<int>(attack.item_id_);

	for (auto &it : attack.damages_)
	{
		int mob_object_id = it.first;

		write<int>(mob_object_id);
		write<unsigned char>(0xFF);

		auto &damage = attack.damages_[mob_object_id];

		for (auto &damage : damage)
		{
			write<int>(damage);
		}
	}

	if (attack_type == attack_type_constants::kMagic)
	{
		// mage Big Bang skills
		if (attack.skill_id_ == 2121001 ||
			attack.skill_id_ == 2221001 ||
			attack.skill_id_ == 2321001)
		{
			write<int>(attack.charge_);
		}
	}
}

void PacketCreator::ForfeitQuest(short quest_id)
{
	write<short>(send_headers::kSHOW_STATUS_INFO);
	write<signed char>(1); // 1 = quest message, there are also much other types
	write<short>(quest_id);
	write<short>(0);
}

void PacketCreator::UpdateQuest(Quest *quest, int npc_id)
{
	write<short>(send_headers::kUPDATE_QUEST_INFO);
	write<signed char>(10);
	write<short>(quest->get_id());
	write<int>(npc_id);
	write<short>(0);
	write<signed char>(0);
}

void PacketCreator::UpdateQuestInfo(Quest *quest)
{
	write<short>(send_headers::kSHOW_STATUS_INFO);
	write<signed char>(1); // 1 = quest message, there are also much other types
	write<short>(quest->get_id());

	if (quest->is_completed())
	{
		write<signed char>(2);
		write<long long>(quest->get_completion_time());
	}
	else
	{
		write<signed char>(1);
		write<std::string>(quest->get_killed_mobs1());
	}
}

void PacketCreator::ItemGainChat(int itemid, int amount, signed char items_size)
{
	write<short>(send_headers::kSHOW_ITEM_GAIN_INCHAT);
	write<signed char>(3);
	write<signed char>(items_size);

	for (signed char i = 0; i < items_size; ++i)
	{
		write<int>(itemid);
		write<int>(amount);
	}
}

void PacketCreator::MesosGainChat(int amount)
{
	write<short>(send_headers::kSHOW_STATUS_INFO);
	write<signed char>(5); // 5 = increase mesos, there are also much other types
	write<int>(amount);
	write<short>(0);
}

void PacketCreator::FameGainChat(int amount)
{
	write<short>(send_headers::kSHOW_STATUS_INFO);
	write<signed char>(4); // 4 = increase fame, there are also much other types
	write<int>(amount);
}

void PacketCreator::AddCharStats(Player *player)
{
	write<int>(player->get_id());
	write_string(player->get_name(), 13);
	write<signed char>(player->get_gender());
	write<signed char>(player->get_skin_color());
	write<int>(player->get_face());
	write<int>(player->get_hair());
	write<long long>(0); // pet unique id 1
	write<long long>(0); // pet unique id 2
	write<long long>(0); // pet unique id 3
	write<signed char>(player->get_level());
	write<short>(player->get_job());
	write<short>(player->get_str());
	write<short>(player->get_dex());
	write<short>(player->get_int());
	write<short>(player->get_luk());
	write<short>(player->get_hp());
	write<short>(player->get_max_hp());
	write<short>(player->get_mp());
	write<short>(player->get_max_mp());
	write<short>(player->get_ap());
	write<short>(player->get_sp());
	write<int>(player->get_exp());
	write<short>(player->get_fame());
	write<int>(0);
	write<int>(player->get_map()->get_id());
	write<signed char>(player->get_spawn_point());
	write<int>(0);
}

void PacketCreator::writeCharacterData(Player *player)
{
	write<long long>(-1);
	AddCharStats(player);
	write<signed char>(player->get_buddy_list_capacity());
	AddInventoryInfo(player);
	AddSkillInfo(player);
	AddCoolDownInfo(player);
	AddQuestInfo(player);

	// minigame data?
	// not sure if this one belongs to rings, and it might be size for something
	// extract from client:
	/*
	v64 = (unsigned __int16)CInPacket::Decode2(a2);
	if ( (signed int)(unsigned __int16)v64 > 0 )
	{
	v65 = v64;
	do
	{
	v134 = 0;
	LOBYTE(v142) = 14;
	sub_4B8B70(&v133);
	v66 = v134;
	*v66 = CInPacket::Decode4(a2);
	v67 = (int)v134;
	*(_DWORD *)(v67 + 4) = CInPacket::Decode4(a2);
	v68 = (int)v134;
	*(_DWORD *)(v68 + 8) = CInPacket::Decode4(a2);
	v69 = (int)v134;
	*(_DWORD *)(v69 + 12) = CInPacket::Decode4(a2);
	v70 = (int)v134;
	*(_DWORD *)(v70 + 16) = CInPacket::Decode4(a2);
	sub_4B93C8(v134, &v133);
	LOBYTE(v142) = 1;
	if ( v134 )
	{
	sub_4B8BD2(0);
	v134 = 0;
	}
	--v65;
	}
	while ( v65 );
	}
	*/
	write<short>(0); // amount?

	AddRingInfo();
	AddTeleportRockInfo();
	write<int>(0);
}

void PacketCreator::AddInventoryInfo(Player *player)
{
	write<int>(player->get_mesos());

	write<signed char>(player->get_equip_slots());
	write<signed char>(player->get_use_slots());
	write<signed char>(player->get_setup_slots());
	write<signed char>(player->get_etc_slots());
	write<signed char>(player->get_cash_slots());

	enum Constants
	{
		kEndInventory
	};

	// equipped Inventory (no cash)
	auto items = player->get_inventory(kInventoryConstantsTypesEquipped)->get_items();

	for (auto &it : *items)
	{
		std::shared_ptr<Item> item = it.second;

		if (item->get_slot() > -100)
		{
			ItemInfo(item.get());
		}
	}

	// equipped inventory (only cash)
	for (auto &it : *items)
	{
		std::shared_ptr<Item> item = it.second;

		if (item->get_slot() <= -100)
		{
			ItemInfo(item.get());
		}
	}

	write<short>(kEndInventory);

	// equip inventory
	items = player->get_inventory(kInventoryConstantsTypesEquip)->get_items();

	for (auto &it : *items)
	{
		ItemInfo(it.second.get());
	}

	write<signed char>(kEndInventory);

	// use inventory
	items = player->get_inventory(kInventoryConstantsTypesUse)->get_items();
	for (auto &it : *items)
	{
		ItemInfo(it.second.get());
	}

	write<signed char>(kEndInventory);

	// setup inventory
	items = player->get_inventory(kInventoryConstantsTypesSetup)->get_items();
	for (auto &it : *items)
	{
		ItemInfo(it.second.get());
	}

	write<signed char>(kEndInventory);

	// etc inventory
	items = player->get_inventory(kInventoryConstantsTypesEtc)->get_items();
	for (auto &it : *items)
	{
		ItemInfo(it.second.get());
	}

	write<signed char>(kEndInventory);

	// cash inventory
	items = player->get_inventory(kInventoryConstantsTypesCash)->get_items();
	for (auto &it : *items)
	{
		ItemInfo(it.second.get());
	}

	write<signed char>(kEndInventory);
}

void PacketCreator::AddSkillInfo(Player *player)
{
	auto skills = player->get_skills();
	short amount = static_cast<short>(skills->size());
	write<short>(amount);

	for (auto it : *skills)
	{
		int skill_id = it.first;
		Skill &skill = it.second;

		write<int>(skill_id);
		write<int>(skill.level_);

		/*
		// extract from client
		// v137 = skill_id
		if ( v137 / 10000 % 100 && v137 / 10000 % 10 == 2 )
        {
          v138 = CInPacket::Decode4(a2);
          sub_4B8F75(&v137, &v138);
        }
		*/
		if (tools::is_fourth_job(skill_id))
		{
			write<int>(skill.master_level_);
		}
	}
}

void PacketCreator::AddCoolDownInfo(Player *player)
{
	write<short>(0); // size
	
	// (followed 1. by skillid (int)
	// and 2. time (short))
}

void PacketCreator::AddQuestInfo(Player *player)
{
	// quests in progress data

	auto quests_in_progress = player->get_quests_in_progress();

	write<short>(static_cast<short>(quests_in_progress->size()));

	for (auto &it : *quests_in_progress)
	{
		Quest *quest = it.second.get();
		write<short>(quest->get_id());
		write<std::string>(quest->get_killed_mobs1());
	}

	// completed quests data

	auto completed_quests = player->get_completed_quests();

	write<short>(static_cast<short>(completed_quests->size()));

	for (auto &it : *completed_quests)
	{
		Quest *quest = it.second.get();
		write<short>(quest->get_id());
		write<long long>(quest->get_completion_time());
	}
}

void PacketCreator::AddRingInfo()
{
	// crush rings
	write<short>(0); // amount

	// to-do crush rings info

	// friendship rings
	write<short>(0); // amount

	// to-do friendship rings info

	// marriage rings
	write<short>(0); // amount

	// to-do marriage rings info
}

void PacketCreator::AddTeleportRockInfo()
{
	int loop_counter = 0;
	// regular rock maps
	for (; loop_counter < 5; ++loop_counter)
	{
		write<int>(kNoMap);
	}
	// vip rock maps
	for (loop_counter = 0; loop_counter < 10; ++loop_counter)
	{
		write<int>(kNoMap);
	}
}

void PacketCreator::change_map(Player *player, bool is_connect_packet)
{
	write<short>(send_headers::kWARP_TO_MAP);
	write<int>(player->get_channel_id());
	write<signed char>(1); // portal count (probably starts with 1 upon login, increases each when entering portal)
	write<bool>(is_connect_packet);
	write<short>(0); // amount for some kind of message on the screen

	if (is_connect_packet)
	{
		writeRngSeeds();
		writeCharacterData(player);
	}
	else
	{
		write<int>(player->get_map()->get_id());
		write<signed char>(player->get_spawn_point());
		write<short>(player->get_hp());
		write<signed char>(0);
	}

	write<long long>(tools::time_to_tick());
}

void PacketCreator::writeRngSeeds()
{
	write<int>(36556356); // random number generator seed 1
	write<int>(233868); // random number generator seed 2
	write<int>(98358998); // random number generator seed 3
}

void PacketCreator::AddCharLook(Player *player, bool megaphone)
{
	write<signed char>(player->get_gender());
	write<signed char>(player->get_skin_color());
	write<int>(player->get_face());
	write<bool>(megaphone);
	write<int>(player->get_hair());

	std::unordered_map<signed char, int> visible_equips;
	std::unordered_map<signed char, int> invisible_equips;
	int cash_weapon_id = 0;
	auto equips = player->get_inventory(kInventoryConstantsTypesEquipped)->get_items();

	for (auto it : *equips)
	{
		std::shared_ptr<Item> item = it.second;
		int item_id = item->get_item_id();
		signed char pos = -item->get_slot();

		// cash weapon
		if (pos == 111)
		{
			cash_weapon_id = item_id;
			continue;
		}

		if (pos < 100)
		{
			if (visible_equips.find(pos) == visible_equips.end())
			{
				visible_equips[pos] = item_id;
			}
			else
			{
				invisible_equips[pos] = item_id;
			}
		}
		else
		{
			// cash equips

			pos -= 100;

			if (visible_equips.find(pos) != visible_equips.end())
			{
				invisible_equips[pos] = visible_equips[pos];
			}

			visible_equips[pos] = item_id;
		}
	}

	enum
	{
		EndEquipInfo = -1
	};

	// visible equips info

	for (auto &it : visible_equips)
	{
		write<signed char>(it.first);
		write<int>(it.second);
	}

	write<signed char>(EndEquipInfo);

	// invisible equips info

	for (auto &it : invisible_equips)
	{
		write<signed char>(it.first);
		write<int>(it.second);
	}

	write<signed char>(EndEquipInfo);

	// cash weapon itemid
	write<int>(cash_weapon_id);

	{
		// show pets

		auto pets = *player->get_pets();
		write<int>(pets.size() > 0 ? pets[0]->get_item_id() : 0);
		write<int>(pets.size() > 1 ? pets[1]->get_item_id() : 0);
		write<int>(pets.size() > 2 ? pets[2]->get_item_id() : 0);
	}
}

void PacketCreator::UpdatePlayer(Player *player)
{
	write<short>(send_headers::kUPDATE_CHAR_LOOK);
	write<int>(player->get_id());
	write<signed char>(1);
	AddCharLook(player);

	// rings info

	bool has_couple_ring = false;

	write<bool>(has_couple_ring); // couple ring

	if (has_couple_ring)
	{
		// to-do
	}

	bool has_friendship_ring = false;

	write<bool>(has_friendship_ring); // friendship ring

	if (has_friendship_ring)
	{
		// to-do
	}

	bool has_marriage_ring = false;

	write<bool>(has_marriage_ring); // marriage ring

	if (has_marriage_ring)
	{
		// to-do
	}

	// end of rings info

	write<int>(player->get_chair());
}

void PacketCreator::ShowKeymap(Player *player)
{
	write<short>(send_headers::kKEYMAP);
	write<signed char>(0); // bool if it is empty?

	for (int pos = kMinKeymapPos; pos < kMaxKeymapPos; ++pos)
	{
		Key &key = player->get_key(pos);
		write<signed char>(key.type);
		write<int>(key.action);
	}
}

void PacketCreator::ShowPlayerMovement(int player_id, unsigned char *buffer, int buffer_size)
{
	write<short>(send_headers::kMOVE_PLAYER);
	write<int>(player_id);

	// copy movement data
	memcpy(buffer_ + length_, buffer, buffer_size);
	length_ += buffer_size;
}

void PacketCreator::FaceExpression(int player_id, int face)
{
	write<short>(send_headers::kFACIAL_EXPRESSION);
	write<int>(player_id);
	write<int>(face);
}

void PacketCreator::ShowInfo(Player *player)
{
	write<short>(send_headers::kCHAR_INFO);
	write<int>(player->get_id());
	write<signed char>(player->get_level());
	write<short>(player->get_job());
	write<short>(player->get_fame());

	// marriage info

	bool is_maried = player->is_married();

	write<bool>(is_maried);

	// guild info

	Guild *guild = player->get_guild();

	if (guild)
	{
		write<std::string>(guild->get_name());
	}
	else
	{
		write<std::string>("-");
	}

	write<std::string>(""); // guild alliance name

	write<signed char>(0); // medal info/MedalAchievementInfo according to client data

	// pets info

	enum Constants
	{
		EndPetInfo = 0,
		StartPetInfo = 1
	};

	auto pets = player->get_pets();
	signed char index = 0;

	for (auto pet : *pets)
	{
		write<signed char>(StartPetInfo);

		write<int>(pet->get_item_id());
		write<std::string>(pet->get_name());
		write<signed char>(pet->get_pet_level());
		write<short>(pet->get_pet_closeness());
		write<signed char>(pet->get_pet_fullness());
		write<short>(0);

		Inventory *inventory = player->get_inventory(kInventoryConstantsTypesEquipped);
		if (!inventory)
		{
			return;
		}

		signed char slot = (index == 0 ? -114 : (index == 1 ? -130 : -138));
		auto item = inventory->get_item_by_slot(slot);

		if (item)
		{
			write<int>(item->get_item_id());
		}
		else
		{
			write<int>(0);
		}

		++index;
	}

	write<signed char>(EndPetInfo);

	// mount info
	bool has_tamed_mob = (player->get_mount_item_id() != 0);
	write<bool>(has_tamed_mob);
	if (has_tamed_mob)
	{
		write<int>(1); // level
		write<int>(0); // exp
		write<int>(0); // tiredness
	}

	// wishlist info
	write<signed char>(0); // size

	// monster book info
	write<int>(1);
	write<int>(0);
	write<int>(0);
	write<int>(0);
	write<int>(0);
	// end of monster book info

	// equipped medal info
	Inventory *inventory = player->get_inventory(kInventoryConstantsTypesEquipped);
	if (!inventory)
	{
		write<int>(0);
	}
	else
	{
		auto medal = inventory->get_item_by_slot(kItemConstantsEquippedSlotsMedal);
		write<int>(medal ? medal->get_item_id() : 0);
	}

	// collected medals info
	write<short>(0); // size
}

void PacketCreator::SendFame(std::string name, signed char type)
{
	write<short>(send_headers::kFAME);
	write<signed char>(5);
	write<std::string>(name);
	write<signed char>(type);
}

void PacketCreator::SendFamee(std::string name2, signed char type, int newFame)
{
	write<short>(send_headers::kFAME);
	write<signed char>(0);
	write<std::string>(name2);
	write<signed char>(type);
	write<int>(newFame);
}

void PacketCreator::EnableAction(bool unstuck)
{
	write<short>(send_headers::kUPDATE_STATS);
	write<bool>(unstuck); // 1 / true = fix client lock
	write<int>(0); // stat
}

void PacketCreator::UpdateLevel(unsigned char level)
{
	write<short>(send_headers::kUPDATE_STATS);
	write<bool>(true); // 1 / true = fix client lock
	write<int>(kCharacterStatsLevel);
	write<unsigned char>(level);
}

void PacketCreator::UpdateSp(Player * player, short value)
{
	write<short>(send_headers::kUPDATE_STATS);
	write<bool>(true); // 1 / true = fix client lock
	write<int>(kCharacterStatsSp);
	write<short>(value); // remaining sp
}

void PacketCreator::UpdateApStats(short str, short dex, short intt, short luk, short ap)
{
	write<short>(send_headers::kUPDATE_STATS);
	write<bool>(true); // 1 / true = fix client lock
	write<int>(kCharacterStatsStr | kCharacterStatsDex | kCharacterStatsInt | kCharacterStatsLuk | kCharacterStatsAp); // update mask

	// values
	write<short>(str);
	write<short>(dex);
	write<short>(intt);
	write<short>(luk);
	write<short>(ap);
}

// skin, job, str, dex, int, luk, ap, sp (has own packet)
void PacketCreator::UpdateStatShort(int stat, short value)
{
	write<short>(send_headers::kUPDATE_STATS);
	write<bool>(true); // 1 / true = fix client lock
	write<int>(stat);
	write<short>(value);
}

// face, hair, hp, maxhp, mp, maxmp, fame, exp, mesos
void PacketCreator::UpdateStatInt(int stat, int value)
{
	write<short>(send_headers::kUPDATE_STATS);
	write<bool>(true); // 1 / true = fix client lock
	write<int>(stat);
	write<int>(value);
}

void PacketCreator::get_inventory_full()
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<signed char>(1);
	write<signed char>(0);
}

/*
* mode can be as follows:
* 0xFF = "You can't get anymore items."
* 0xFE = "This item is unavailable for the pick-up."
*/
void PacketCreator::CantGetAnymoreItems()
{
	write<short>(send_headers::kSHOW_STATUS_INFO);
	write<signed char>(0); // 0 = drop pickup, there are also much other types
	write<unsigned char>(0xFF); // mode
}
