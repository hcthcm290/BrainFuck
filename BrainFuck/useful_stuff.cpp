#include "useful_stuff.h"

NonCopyable::NonCopyable()
{
    //ctor
}

NonCopyable::~NonCopyable()
{
    //dtor
}

NonCopyable::NonCopyable(NonCopyable&)
{

}

//NonCopyable& NonCopyable::operator=(const NonCopyable&)
//{
//
//}

Invokable::Invokable(FUNCTION(void, func, void*), void* param)
{
    this->func = func;
    this->param = param;
}

Invokable::~Invokable()
{
    Invoke();
}

void Invokable::Invoke()
{
    func(param);
}
