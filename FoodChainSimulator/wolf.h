#ifndef _WOLF_H_
#define _WOLF_H_

#include "common_includes.h"
#include "cell.h"
#include "mortal.h"
#include "gendered.h"
namespace EcoSim
{
	/// <summary>
	/// 狼
	/// </summary>
	class Wolf final : public ILivingThing, public IMortal, public IGendered
	{
		int health;

		LivingThingGender gender;

	public:
		Wolf();

		static int starvationHealthHarm;

		static int consumptionHealthBenifit;

		static int targetOffspringCount;

		static int maximumHealth;

		static int initialHealth;

		static int minimumReproduceHealth;

		auto DecideDestination(const CellMatrix& map, Vector2 pos)->Vector2 override;

		auto DecideChildrenLocation(const CellMatrix& map, Vector2 pos)->std::vector<Vector2> override;

		auto DisplayColor() const -> int override { return ColorCode::Red; }

		auto Reproduce() -> std::shared_ptr<ILivingThing> override { return std::shared_ptr<ILivingThing>(new Wolf()); }

		auto Health() -> int& override { return health; }

		auto StarvationHealthHarm() -> int override { return starvationHealthHarm; }

		auto ConsumptionHealthBenifit() -> int override { return consumptionHealthBenifit; }

		auto MaximumHealth() -> int override { return maximumHealth; }

		auto MinimumReproduceHealth() -> int override { return minimumReproduceHealth; }

		auto InitialHealth() -> int override { return initialHealth; }

		auto	 Gender() -> LivingThingGender override { return gender; }

		auto TypeIdentifier() const -> std::string override { return "Wolf"; }
	};
} // namespace EcoSim
#endif // _GRASS_H_