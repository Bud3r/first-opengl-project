#include <iostream>

#include <engine.h>
#include "main_game_object.h"
#include <GLFW/glfw3.h>
#include <stb_image.h>

int main()
{
    Engine engine;
    engine.set_root_game_object<MainGameObject>();
    engine.start();
    return 0;
}


