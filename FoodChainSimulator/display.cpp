#include "display.h"
#include <Windows.h>
#include <iostream>
#include "mortal.h"
#include "gendered.h"
namespace EcoSim
{
	namespace UI
	{
		void SetCursorPosition(int x, int y)
		{
			COORD coord;
			coord.X = x;
			coord.Y = y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		}
	} // namespace UI


	static char Hex(int number)
	{
		if (number < 10)
		{
			return '0' + number;
		} 
		return 'A' + number - 10; 
	}

	auto Display::DrawCell(const Cell& cell)-> void
	{
		using namespace EcoSim::UI;
		SetCursorPosition(cell.position.x * 2, cell.position.y);

		if (cell.Content() == nullptr)
		{
			std::cout << " ";
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cell.Content()->DisplayColor());
		/*	auto gend = sp_dynamic_cast<IGendered>(cell.Content());
			if (gend)
			{ 
				if(gend->Gender()== LivingThingGender::Male)
				{
					putchar('x');
				}
				else
				{
					putchar('o');
				} 
			}
			else
			{
				putchar('.');
			} */

			auto mortal = sp_dynamic_cast<IMortal>(cell.Content());
			if (mortal)
			{
				putchar( Hex(mortal->Health()));
			}
			else
			{
				putchar('.');
			}
		}
	}

	auto Display::MapUpdatedEventHandler(const CellMatrix& map, std::vector<Vector2> updatedPositions) -> void
	{
		//DrawMap(map);
		for (auto& pos : updatedPositions)
		{
			DrawCell(map.Access(pos));
		}
	}


	/// <summary>
	/// 初始化显示设置
	/// </summary>

	auto Display::Initialize(Game& game) -> void
	{
		game.event_mapUpdated.Subscribe(MapUpdatedEventHandler);

		HWND hwnd = GetConsoleWindow();
		ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	}

	auto Display::DrawMap(const CellMatrix& map) -> void
	{
		using namespace EcoSim::UI;
		system("cls");
		for (auto&& cell : map)
		{
			DrawCell(cell);
		}
	}
} // namespace EcoSim