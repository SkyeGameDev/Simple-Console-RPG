//----------------------------------
//---------------- CI411 Assignment
//---------------- Skye Barrowman
//----------------------------------

#include <iostream>
#include "game.h"

using namespace std;

int main()
{
    bool replay = true;

    while (replay)
    {
        system("cls");

        Game* game = new Game;
        game->mainLoop();
        replay = game->replay;
        delete game;
    }

    return 0;
}

