#ifndef USEFUL_STUFF_H
#define USEFUL_STUFF_H

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <sal.h>
#include <windows.h>

/**
    Typo:
        All function and class should start uppercase and be camelcase
        All members of class should start lowercase and be camelcase
        All constants and macro should be all uppercase
        All macros should be defined here. Try not to pollute the namespace.
    Rules:
        All class should have Initialize function in place of constructor
        All class should have Release function in place of destructor
        Unless it CANNOT fail or it has a return value, functions should return RESULT for error-checking.
        If a function succeed, return 0. Else, return whatever else (should be 1 if unspecified).
        Constants should be defined in relevant HEADERS.
*/

#define FUNCTION(returntype,name,arguement) returntype (*name) (arguement)
/** Function pointer quick-define*/
#define DESTROY(p_COM_object) if (p_COM_object) {p_COM_object->Release();p_COM_object = NULL;}
/** Call Release of target object. No error checking.*/
#define RESULT LONG
/** Result of command*/
#define BLOCKALLOC(type, pname) if (!(pname = new type)) {cerr << "out of memory\n";return 1;}
/** Allocate the variable. If out of memory, return 1 */
#define BLOCKCALL(COM_command) if (FAILED(COM_command)) return 1
/** Call the COM command. If it fails, return 1*/
#define CALL(command, errorString) if (command) {cerr << errorString; return 1;}
/** Call the command. If it fails, return 1*/
struct DXGI_RATIONAL;
typedef DXGI_RATIONAL RefreshRate;

using std::cerr;

class NonCopyable
{
    public:
        NonCopyable();
        virtual ~NonCopyable();

    protected:

    private:
        NonCopyable(NonCopyable&);
        NonCopyable& operator = (const NonCopyable&);
};

class Invokable
{
public:
    Invokable(FUNCTION(void, func, void*), void* param);
    ~Invokable();
    void Invoke();
private:
    FUNCTION(void, func, void*);
    void* param;
};


#endif // USEFUL_STUFF_H
