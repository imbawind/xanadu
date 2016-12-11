//

#include "../player.hpp"
#include "../job_constants.hpp"

void Player::handle_pirate_job_advancer()
{
	switch (job_)
	{

	case job_ids::kBeginner:
	{
		set_state(1000);

		if (level_ >= 10)
		{
			set_job(job_ids::kPirate);
		}

		break;
	}

	case job_ids::kPirate:
	{
		if (level_ >= 30)
		{
			if (get_state() == 0)
			{
				send_simple("Choose the path you wish to follow. \r\n#L0#lBrawler#1 \r\n#L1#Gunslinger#l");
			}

			else if (get_state() == 1)
			{
				switch (get_selected())
				{
				case 0:
					set_job(job_ids::kBrawler);

					break;

				case 1:
					set_job(job_ids::kGunslinger);

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

	case job_ids::kBrawler:
	case job_ids::kGunslinger:
	{
		set_state(1000);

		if (level_ >= 70)
		{
			switch (job_)
			{
			case job_ids::kBrawler:
				set_job(job_ids::kMarauder);
				break;

			case job_ids::kGunslinger:
				set_job(job_ids::kOutlaw);
				break;
			}
		}

		break;
	}

	case job_ids::kMarauder:
	case job_ids::kOutlaw:
	{
		set_state(1000);

		if (level_ >= 120)
		{
			switch (job_)
			{
			case job_ids::kMarauder:
				set_job(job_ids::kBuccaneer);
				break;

			case job_ids::kOutlaw:
				set_job(job_ids::kCorsair);
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
