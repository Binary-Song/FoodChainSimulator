#include <thread>
#include <Windows.h>
#include <fstream>  
#include "recorder.h" 
#include "grass.h"
#include "sheep.h"
#include "wolf.h"
namespace EcoSim
{
	std::thread Recorder::Start()
	{
		return std::thread([=]() {
			SYSTEMTIME time;
			GetSystemTime(&time); 

			auto path_str = GetExeDir() + L"\\game_records\\record(" 
				+ std::to_wstring(time.wHour + 8) 
				+ L"-" 
				+ std::to_wstring(time.wMinute)
				+ L"-"
				+ std::to_wstring(time.wSecond)
				+ L").txt";
			
			std::ofstream file; 
			file.open(path_str, std::ios::app);
			if (file.is_open())
			{ 
				file << "Grass\t" << "Sheep\t" << "Wolf\t" << std::endl;
			}
			file.close();
			while (true)
			{
				std::ofstream file;
				file.open(path_str, std::ios::app);
				if (file.is_open())
				{
					file << Grass::population << "\t"
						<< Sheep::population << "\t"
						<< Wolf::population << "\t" << std::endl;
				}
				file.close();
				Sleep(recordPause);
			}
		});
	}
}