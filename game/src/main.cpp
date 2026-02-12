#include <iostream>


#include <engine.h>
#include "main_game_object.h"


int main()
{
    Engine engine;
    engine.set_root_game_object<MainGameObject>();
    engine.start();
    return 0;
}


