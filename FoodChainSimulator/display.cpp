#include "display.h"
#include <Windows.h>
#include <iostream>
#include "mortal.h"
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
    
     
    auto Display::DrawCell(const Cell& cell)-> void
    {
        using namespace EcoSim::UI;
        SetCursorPosition(cell.position.x * 2, cell.position.y);

        if (cell.content == nullptr)
        {
            std::cout << " ";
        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cell.content->DisplayColor()); 
            auto mortal = dynamic_cast<IMortal*>(cell.content.get());
            if (mortal)
            {
                //putchar('0' + mortal->Health() % 10);
                putchar('o');
            }
            else
            {
                putchar('o');
            }
        }
    }

    auto Display::MapUpdatedEventHandler(const CellMatrix& map, std::vector<Vector2> updatedPositions) -> void
    {
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

    auto Display::DrawMap(const CellMatrix &map) -> void
    {
        using namespace EcoSim::UI;
        system("cls");
        for (auto &&cell : map)
        {
            DrawCell(cell);
        }
    }
} // namespace EcoSim