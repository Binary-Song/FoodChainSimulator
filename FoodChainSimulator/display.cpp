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

	static CHAR_INFO* buffer = nullptr;

	static char Hex(int number)
	{
		if (number < 10)
		{
			return '0' + number;
		}
		return 'A' + number - 10;
	}

	auto get_cell_text(const Cell& cell) -> int
	{
		using namespace EcoSim::UI;
		if (cell.Content() == nullptr)
		{
			return ' ';
		}
		else
		{
			if (Display::displayMode == 0)
			{
				auto grass = sp_dynamic_cast<Grass>(cell.Content());
				if (grass)
				{
					return '.';
				}
				return '*';
			}
			else if (Display::displayMode == 1)
			{
				auto mortal = sp_dynamic_cast<IAnimal>(cell.Content());
				if (mortal)
				{
					return (Hex(mortal->Health()));
				}
				return ('.');
			}
			else
			{ 
				auto gend = sp_dynamic_cast<IAnimal>(cell.Content());
				if (gend)
				{
					if (gend->Gender() == LivingThingGender::Male)
					{
						return ('x');
					}
					else
					{
						return ('o');
					}
				}
				else
				{ 
					return ('.');
				}
			} 
		}
	}

	auto Display::DisplayBuffer() -> void
	{
		COORD sizeToWrite = { (SHORT)Width,(SHORT)Height };
		COORD startPos = { 0,0 };
		SMALL_RECT rcRegion = { 0, 0, (SHORT)( Width - 1),(SHORT)( Height - 1 )};
		WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), buffer, sizeToWrite, startPos, &rcRegion);
	}

	auto Display::BufferCell(const Cell& cell)-> void
	{
		buffer[cell.position.y * Width + cell.position.x*2].Char.AsciiChar = get_cell_text(cell);
		if (cell.Content())
		{ 
			buffer[cell.position.y * Width + cell.position.x*2].Attributes = cell.Content()->DisplayColor();
		}
	}

	auto Display::MapUpdatedEventHandler(const CellMatrix& map, std::vector<Vector2> updatedPositions) -> void
	{
		// DrawMap(map);
		for (auto& pos : updatedPositions)
		{
			BufferCell(map.Access(pos));
		}
		DisplayBuffer();
	}


	/// <summary>
	/// 初始化显示设置
	/// </summary>

	auto Display::Initialize(Game& game) -> void
	{
		game.event_mapUpdated.Subscribe(MapUpdatedEventHandler);

		// 最大化显示
		HWND hwnd = GetConsoleWindow();
		ShowWindow(hwnd, SW_SHOWMAXIMIZED);

		// 禁止改变大小
		HWND consoleWindow = GetConsoleWindow();
		SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZE & ~WS_SIZEBOX);

		// 计算行列数
		/*CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		Width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		Height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;*/

	}

	auto Display::DrawMap(const CellMatrix& map) -> void
	{
		Width = map.width *2;
		Height = map.height;

		assert(buffer == nullptr);
		buffer = new CHAR_INFO[Height * Width]{};

		using namespace EcoSim::UI;
		system("cls");
		for (auto&& cell : map)
		{
			BufferCell(cell);
		}
	}

	int Display::Width = 0;
	int Display::Height = 0;
	int Display::displayMode = 0;
} // namespace EcoSim