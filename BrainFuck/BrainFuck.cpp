#include <iostream>
#include "Main.h"

using std::cerr;

int main()
{
    Main* game = new Main();
    if (game->Initialize())
    {
        cerr << "--- INITIALIZE FAILED ---\n";
        DESTROY(game);
        return 1;
    }
    if (game->Run())
    {
        cerr << "--- RUNTIME ERROR ---\n";
        DESTROY(game);
        return 1;
    }
    if (game->Release())
    {
        cerr << "--- RELEASE FAILED ---\n";
        DESTROY(game);
        return 1;
    }
    else cerr << "RELEASE SUCCEED";
    return 0;
}
