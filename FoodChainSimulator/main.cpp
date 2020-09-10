#include <iostream> 
#include "game.h"
#include "display.h"
#include "cell.h"
#include <Windows.h>

 
int main(int, char**) {
      
    using namespace EcoSim; 
    Game game(80,30,0.01f);
    Display::Initialize(game);
    Display::DrawMap(game.Map());
    while (true)
    { 
        Sleep(1000);
        game.NextStep(); 
    }
    return 0;
}
