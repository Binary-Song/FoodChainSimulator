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
#include <map>
#include <time.h> 
 

int main(int argc, char** args) { 
    using namespace EcoSim;  
  
    ConfigLoader loader;
    auto config_file_path = GetExeDir() + L"\\rules.ini";
    std::wcout << L"Loading config file:" << config_file_path << std::endl;
    try {
        loader.Load(std::ifstream(config_file_path));
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what(); 
        return 1;
    }
    std::cout << "Config loaded:"<<std::endl;
    std::cout << loader.StrInfo(); 
    std::cout << "Press enter to continue.";
    getchar(); 
    ApplySeed();
    Game game(80, 50, 0.9f);
    Display::Initialize(game);
    Display::DrawMap(game.Map());

    Recorder rec; 
    auto thread = rec.Start();

    Game::cycleTime = Game::cycleTime > 0 ? Game::cycleTime : 0;
    while (true)
    {
        auto t_start = clock();
        game.NextStep(); 
        auto t_end = clock();
        double diff = Game::cycleTime - ((double)t_end - (double)t_start) / (double)(CLOCKS_PER_SEC * 1000 )   ;
        if (diff>0) {
            Sleep(diff);
        }
    }
    return 0;
}
