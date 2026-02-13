#include <iostream>


#include <engine.h>
#include "main_game_object.h"


int main()
{
    Engine engine;
    engine.SetRootGameObject<MainGameObject>();
    engine.Start();
    return 0;
}


