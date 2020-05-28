
#include <actor_fixtures.hpp>

namespace hapnmc { namespace dedcom {

	bool ActorRepoFoes::HasActor(int max_threat_lvl)
	{
		for(auto const& actor: _actors) {
			if(actor.GetThreatLvl() <= max_threat_lvl) return true;
		}

		return false;
	}

	void ActorRepoFoes::AddActor(Actor a)
	{
		_actors.push_back(a);
	}

	Actor ActorRepoFoes::GetRandomActorCopy(int max_threat_lvl)
	{
		std::vector<const Actor * > viable_actors;

		for(auto const& actor: _actors) {
			if(actor.GetThreatLvl() <= max_threat_lvl) {
				viable_actors.push_back(&actor);
			}
		}

		std::uniform_int_distribution<> dis_int(0, viable_actors.size()-1);

		const Actor* actor_ptr = viable_actors.at(dis_int(_gen));

		Actor actor_inst = *actor_ptr;

		randomize(actor_inst);
		
		return actor_inst;
	}

	void ActorRepoFoes::randomize(Actor& actor)
	{
		std::uniform_real_distribution<double> dis_double(0.6, std::nextafter(1.4, std::numeric_limits<double>::max()));
		double randratio;

		randratio = dis_double(_gen);
		actor.ResetIntegrity(actor.GetBaseIntegrity()*randratio);

		randratio = dis_double(_gen);
		actor.SetArmor(actor.GetArmor()*randratio);

		randratio = dis_double(_gen);
		actor.SetAmmo(actor.GetAmmo()*randratio);

		randratio = dis_double(_gen);
		actor.SetShieldAmount(actor.GetShieldAmount()*randratio);

		randratio = dis_double(_gen);
		actor.SetPower(actor.GetPower()*randratio);

		randratio = dis_double(_gen);
		actor.SetBaseSpeed(actor.GetBaseSpeed()*randratio);
	}


	void ActorRepoFoes::Init()
	{
		Actor a_zapper("Zapper", "Small and agressive bot, electrocutes targets",
			5, 0, 0, 0, 0,
			2, 0, 15, 3);
		a_zapper.AddAction("Electrocute");
		a_zapper.AddTag("mech");
		a_zapper.AddTag("elctr");
		AddActor(a_zapper);

		Actor a_secbot("Secbot", "Security bot armed with a basic projectile weapon",
			10, 0, 3, 0, 0,
			1, 0, 5, 6);
		a_secbot.AddAction("Slugshot");
		a_secbot.AddTag("mech");
		a_secbot.AddTag("elctr");
		AddActor(a_secbot);

		Actor a_shambler("Shambler", "Unidentified humanoid",
			20, 0, 0, 0, 0,
			0, 0, 10, 9);
		a_shambler.AddAction("Assault");
		a_shambler.AddTag("orgnc");
		AddActor(a_shambler);

		Actor a_decontaminator("Decontaminator", "Decontamination specialist bot",
			15, 0, 6, 0, 0,
			0, 0, 5, 15);
		a_decontaminator.AddAction("Flamer");
		a_decontaminator.AddTag("mech");
		a_decontaminator.AddTag("elctr");
		AddActor(a_decontaminator);

		Actor a_mechashambler("Mechashambler", "Unidentified mechanised humanoid",
			20, 0, 3, 0, 0,
			0, 0, 10, 20);
		a_mechashambler.AddAction("Assault");
		a_mechashambler.AddAction("Slugshot");
		a_mechashambler.AddTag("mech");
		a_mechashambler.AddTag("orgnc");
		AddActor(a_mechashambler);

		Actor a_terminator("Terminator", "Dedicated combat bot",
			30, 10, 8, 0, 0,
			0, 1, 10, 40);
		a_terminator.AddAction("Slugshot");
		a_terminator.AddAction("Plasmashot");
		a_terminator.AddTag("mech");
		AddActor(a_terminator);

		Actor a_dominator("Dominator", "Advanced combat bot with a powerful generator and a shield projector",
			35, 15, 12, 5, 2,
			4, 2, 10, 80);
		a_dominator.AddAction("Slugshot");
		a_dominator.AddAction("Plasmashot");
		a_dominator.AddTag("mech");
		a_dominator.AddTag("elctr");
		AddActor(a_dominator);


	}

}} // hapnmc::dedcom