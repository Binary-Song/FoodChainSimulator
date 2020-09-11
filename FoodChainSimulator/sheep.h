#ifndef _SHEEP_H_
#define _SHEEP_H_

#include "common_includes.h"
#include "cell.h"
#include "mortal.h"
#include "gendered.h"
namespace EcoSim
{
	/// <summary>
	/// 羊
	/// </summary>
	class Sheep final : public ILivingThing, public IMortal, public IGendered
	{
		int health;

		LivingThingGender gender;

	public:
		Sheep();

		static int starvationHealthHarm;

		static int consumptionHealthBenifit;

		static int targetOffspringCount;

		static int maximumHealth;

		static int initialHealth;

		static int minimumReproduceHealth;

		auto DecideDestination(const CellMatrix& map, Vector2 pos)->Vector2 override;

		auto DecideChildrenLocation(const CellMatrix& map, Vector2 pos)->std::vector<Vector2> override;

		auto DisplayColor() const -> int override{ return ColorCode::Red | ColorCode::Blue | ColorCode::Green; }

		auto Reproduce() -> std::shared_ptr<ILivingThing> override { return std::shared_ptr<ILivingThing>(new Sheep()); }

		auto Health() -> int& override { return health; }

		auto StarvationHealthHarm() -> int override { return starvationHealthHarm; }

		auto ConsumptionHealthBenifit() -> int override { return consumptionHealthBenifit; }

		auto MaximumHealth() -> int override { return maximumHealth; } 

		auto MinimumReproduceHealth() -> int override { return minimumReproduceHealth; }

		auto InitialHealth() -> int override { return initialHealth; } 

		auto	 Gender() -> LivingThingGender override { return gender; }

		auto TypeIdentifier() const -> std::string override { return "Sheep"; }
	};
} // namespace EcoSim
#endif // _GRASS_H_