#include <iostream> 
#include "game.h"
#include "display.h"
#include "cell.h"
#include <Windows.h>

 
int main(int, char**) {
      
    using namespace EcoSim; 
    Game game(80,50,0.9f);
 
    Display::Initialize(game);
    Display::DrawMap(game.Map());
    while (true)
    { 
        game.NextStep(); 
    }
    return 0;
}
