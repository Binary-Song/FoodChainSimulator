#include <iostream> 
#include "game.h"
#include "display.h"
#include "cell.h"
#include <Windows.h>
#include "config_loader.h"
#include <fstream>
#include <stdexcept>
#include "random_gen.h"
#include "recorder.h"
#include "utils.h"
int main(int argc, char** args) { 
    using namespace EcoSim;  
    ApplySeed();

    ConfigLoader loader;
    try { 
        loader.Load(std::ifstream(GetExeDir() + L"\\rules.ini")); 
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what(); 
        return 1;
    } 

    Game game(80, 50, 0.9f);
    Display::Initialize(game);
    Display::DrawMap(game.Map());

    Recorder rec; 
    auto thread = rec.Start();
    while (true)
    { 
        game.NextStep(); 
    }
    return 0;
}
