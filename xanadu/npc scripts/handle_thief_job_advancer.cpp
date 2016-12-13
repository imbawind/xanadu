//

#include "../player.hpp"
#include "../job_constants.hpp"

void Player::handle_thief_job_advancer()
{
	switch (job_)
	{

	case job_ids::kBeginner:
	{

		if (level_ >= 10 && get_dex() >= 25)
		{
			if (get_state() == 0)
			{
				send_simple("Do you want to become a Rogue? \r\n#L0#Yes.#l \r\n#L1#No.#l");
			}
			else if (get_state() == 1)
			{
				switch (get_selected())
				{
				case 0:
					set_job(job_ids::kRogue);
					give_item(1472000, 1);
					give_item(2070006, 800);
					give_item(2070006, 800);
					give_item(2070006, 800);
					send_ok("You have chosen wisely. Now go, go with pride.");
					break;

				case 1:
					send_ok("Come to me again when you made up your mind.");
					break;
				}
			}
		}

		else {
			send_ok("Train a bit more and I can show you the way of the #rRogue#k.");
			set_state(1000);
		}

		break;
	}

	case job_ids::kRogue:
	{
		if (level_ >= 30 && get_sp() == 0)
		{
			if (get_state() == 0)
			{
				send_simple("Choose the path you wish to follow. \r\n#L0#Assassin#l \r\n#L1#Bandit#l");
			}

			else if (get_state() == 1)
			{
				switch (get_selected())
				{
				case 0:
					set_job(job_ids::kAssassin);
					break;

				case 1:
					set_job(job_ids::kBandit);
					break;
				}
			}
		}
		else
		{
			if (level_ >= 30 && get_sp() > 0)
				send_ok("Your time has yet to come.");
			else
				send_ok("You have chosen wisely");
			set_state(1000);
		}

		break;
	}

	case job_ids::kAssassin:
	case job_ids::kBandit:
	{

		if (level_ >= 70 && get_sp() == 0)
		{
			send_ok("You are really a strong one.");
			switch (job_)
			{
			case job_ids::kAssassin:
				set_job(job_ids::kHermit);
				break;

			case job_ids::kBandit:
				set_job(job_ids::kChiefBandit);
				break;
			}
		}
		else
		{
			if (level_ >= 70 && get_sp() > 0)
				send_ok("Your time has yet to come.");
			else
				send_ok("You have chosen wisely");
			set_state(1000);
		}

		break;
	}

	case job_ids::kHermit:
	case job_ids::kChiefBandit:
	{

		if (level_ >= 120 && get_sp() == 0)
		{
			send_ok("I knew this day would come.");
			switch (job_)
			{
			case job_ids::kHermit:
				set_job(job_ids::kNightLord);
				break;

			case job_ids::kChiefBandit:
				set_job(job_ids::kShadower);
				break;
			}
		}
		else
		{
			if (level_ >= 120 && get_sp() > 0)
				send_ok("Your time has yet to come.");
			else
				send_ok("You have chosen wisely");
			set_state(1000);
		}

		break;
	}

	case job_ids::kNightLord:
	case job_ids::kShadower:
	{
		send_ok("You have chosen wisely");
		set_state(1000);
	}

	default:
		set_state(1000);
		break;
	}
}