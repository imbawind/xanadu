//NPC ID: 2100
#include "../player.hpp"

void Player::handle_npc_sera()
{
	send_ok("Hi, welcome to MapleStory! I will reset your AP so that you can choose which to add. Have fun!");
	short temp = (str_ - 4 + dex_ - 4 + int_ - 4 + luk_ - 4);
	set_ap(ap_ + temp);
	set_str(4);
	set_dex(4);
	set_luk(4);
	set_int(4);
	set_map(100000000);
}