#pragma once
#include <thread>
namespace EcoSim
{
	class Recorder
	{
	public:
		int recordPause = 100; 
		std::thread Start();
	}; 
}
