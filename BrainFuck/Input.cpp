#include "Input.h"

Input::Input()
{
    //ctor
}

Input::~Input()
{
    //dtor
}

RESULT Input::Initialize()
{
    return 0;
}

RESULT Input::frame()
{
    return 0;
}

RESULT Input::Release()
{
    delete this;
    return 0;
}

RESULT Input::keydown(WPARAM wparam, LPARAM)
{
    if (wparam == VK_ESCAPE)
        PostQuitMessage(0);
    return 0;
}

RESULT Input::keyup(WPARAM, LPARAM)
{
    return 0;
}

RESULT Input::mousemove(WPARAM, LPARAM)
{
    return 0;
}
