
#include "Scene.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Scene scene;

    if (!scene.init())
    {
        std::cerr << "Error al inicializar la escena."
                  << std::endl;
        return 1;
    }

    scene.loadModels(argc, argv);
    scene.render();

    return 0;
}
