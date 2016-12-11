//

#include "../player.hpp"

// in 251010404, warps the party into pirate pq

void Player::handle_npc_ppq_guon()
{
	set_state(1000);

	set_map(925100000);
}

// handles ppq inside

void Player::handle_npc_ppq_guon2()
{
	set_state(1000);
}
