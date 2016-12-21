//

#include "summon.hpp"

#include "world.hpp"
#include "tools.hpp"

// constructor

Summon::Summon(int skill_id, signed char skill_level, int hp, short position_x, short position_y)
{
	skill_id_ = skill_id;
	skill_level_ = skill_level;
	hp_ = hp;
	position_x_ = position_x;
	position_y_ = position_y;
	object_id_ = World::get_instance()->get_summon_id();
}

signed char summon_get_movement_type(int skill_id)
{
	enum movement_types
	{
		kNoMovement = 0,
		kFollow = 1,
		kFlyingFollow = 4
	};

	switch (skill_id)
	{
	case 3111002: // Puppet (Ranger)
	case 3211002: // Puppet (Sniper)
	case 5211001: // Octopus (Outlaw)
	case 5220002: // Wrath of the Octopi (Corsair)
	case 4341006: // Mirrored Target (Blade Master)
	case 4111007: // Dark Flare
	case 4211007: // Dark Flare
		return kNoMovement;
	case 2311006: // Summon Dragon
	case 3111005: // Silver Hawk
	case 3211005: // Golden Eagle
	case 3121006: // Phoenix
	case 3221005: // Frostprey
	case 5211002: // Gaviota
		return kFlyingFollow;
	default:
		return kFollow;
	}
}

signed char summon_get_summon_type(int skill_id)
{
	if ((skill_id != 3120012 && skill_id != 3220012 && tools::skill_is_puppet(skill_id)))
	{
		return 0;
	}

	switch (skill_id)
	{
	case 1321007: // Beholden
		return 2; // buffs and stuff
	case 4111007: // Dark Flare
	case 4211007: // Dark Flare
		return 7; // attacks what you get hit by
	default:
		return 1;
	}
}
