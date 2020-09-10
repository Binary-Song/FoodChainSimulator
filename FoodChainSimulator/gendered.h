#pragma once
namespace EcoSim
{
	enum class LivingThingGender { Male, Female };
	class IGendered
	{
	public:

		virtual ~IGendered() = default;
		virtual LivingThingGender Gender() = 0;
	};
}