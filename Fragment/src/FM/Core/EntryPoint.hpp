#include "FM/Core/Game.hpp"

extern fm::Game* getGame();

int main(int argc, char* argv[])
{   
    fm::Game* game = getGame();
    game->run();

    return 0;
}

