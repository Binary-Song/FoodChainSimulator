#pragma once
#include "utils_memory.h"
#include <memory>
namespace EcoSim
{
	/// <summary>
	/// std::shared_ptr专用的dynamic_cast
	/// </summary> 
	template<class Derived, class Base>
	std::shared_ptr<Derived> sp_dynamic_cast(std::shared_ptr<Base> bp)
	{
		return std::dynamic_pointer_cast<Derived>(bp);
	}
}