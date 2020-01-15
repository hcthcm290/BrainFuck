#ifndef INPUT_H
#define INPUT_H

#include "useful_stuff.h"

class Input : private NonCopyable
{
    public:
        Input();
        ~Input();

        RESULT Initialize();
        RESULT frame();
        RESULT Release();

        RESULT keydown(WPARAM, LPARAM);
        RESULT keyup(WPARAM, LPARAM);
        RESULT mousemove(WPARAM, LPARAM);
    protected:

    private:
};

#endif // INPUT_H
