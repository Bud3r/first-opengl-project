#include <iostream>


#include <engine.h>
#include "main_game_object.h"


int main(int argc, char* args[])
{
    Engine engine;

    try {
        engine.SetCmdLineArguments(argc, args);
        engine.Start<MainGameObject>();
    }
    catch (std::logic_error& e) {
        std::cerr << "Logic error exeption: " << e.what() << std::endl;
    }
    
    return 0;
}


