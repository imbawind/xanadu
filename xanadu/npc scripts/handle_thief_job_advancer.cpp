//

#include "../player.hpp"
#include "../job_constants.hpp"

void Player::handle_thief_job_advancer()
{
	switch (job_)
	{

	case job_ids::kBeginner:
	{
		set_state(1000);

		if (level_ >= 10)
		{
			set_job(job_ids::kRogue);
		}

		break;
	}

	case job_ids::kRogue:
	{
		if (level_ >= 30)
		{
			if (get_state() == 0)
			{
				send_simple("Choose the path you wish to follow. \r\n#L0#lAssassin#1 \r\n#L1#Bandit#l");
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
			set_state(1000);
		}

		break;
	}

	case job_ids::kAssassin:
	case job_ids::kBandit:
	{
		set_state(1000);

		if (level_ >= 70)
		{
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

		break;
	}

	case job_ids::kHermit:
	case job_ids::kChiefBandit:
	{
		set_state(1000);

		if (level_ >= 120)
		{
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

		break;
	}

	default:
		set_state(1000);
		break;
	}
}
