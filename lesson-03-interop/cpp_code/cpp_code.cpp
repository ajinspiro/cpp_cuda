// Provided by Praseed Pai K T
// https://github.com/praseedpai/NativeInterfaceJavaCsharp/blob/master/cpp_code.cpp
// Original cpp_code.cpp has been modified for x64 use here.

#include <stdio.h>
#include <cstring>
#include <windows.h>

using namespace std;

////////////////////////////////////////
//
// Add
//
extern "C" __declspec(dllexport) int __cdecl Add(int a, int b)
{
    return a + b;
}

////////////////////////////////////////
//
// ANSI version of Length function
//
extern "C" __declspec(dllexport) size_t __cdecl StrLenA(char *r)
{
    return strlen(r);
}

////////////////////////////////////////
//
// Unicode version of Strlen
//
extern "C" __declspec(dllexport) size_t __cdecl StrLenW(wchar_t *r)
{
    return wcslen(r);
}
//////////////////////////////
//
// Computes the minimum value in an int array
//
extern "C" __declspec(dllexport) int __cdecl MinArray(int *pData, int length)
{
    int minData = pData[0];
    for (int pos = 1; pos < length; pos++)
    {
        if (pData[pos] < minData)
        {
            minData = pData[pos];
        }
    }

    return minData;
}

///////////////////////////////////////////
//
// Computes the minimum for a double array
//
extern "C" __declspec(dllexport) double __cdecl MinArrayD(double *pData, int length)
{
    double minData = pData[0];
    for (int pos = 1; pos < length; pos++)
    {
        if (pData[pos] < minData)
        {
            minData = pData[pos];
        }
    }
    return minData;
}

//////////////////////////////////////////
//
// Computes the arithematic mean of an array
//
extern "C" __declspec(dllexport) double __cdecl Average(double *pData, int length)
{
    double minData = pData[0];
    for (int pos = 1; pos < length; pos++)
    {
        minData += pData[pos];
    }
    return minData / length;
}

/////////////////////////////////////////
//
// This structure will be passed between C# and C++
//
struct EventData
{
    int I;
    char *Message;
};

extern "C" __declspec(dllexport) bool __cdecl PutEventData(EventData *ptr)
{
    printf("%s: %s\n", "PutEventData", ptr->Message);
    printf("%s: %d\n", "PutEventData", ptr->I);
    return false;
}

// This is not possible because when GetEventData exits, it will destroy the callstack
// and msg and ed will go out of scope.
// extern "C" __declspec(dllexport) EventData *__cdecl GetEventData()
// {
//     EventData ed = {};
//     char msg[] = "Let's dare to segfault\0";
//     ed.I = 321;
//     ed.Message = msg;
//     return &ed;
// }

extern "C" __declspec(dllexport) void __cdecl NumberTransformer(long(__cdecl *square_callback)(int rs))
{
    for (int i = 0; i < 10; ++i)
    {
        double ret = (*square_callback)(i);
        printf("NumberTransformer: f(%d)=%g\n", i, ret);
    }
}

extern "C" __declspec(dllexport) bool __cdecl StringCopy(char *dest, int dest_size, const char *src)
{
    if (dest == nullptr || dest_size <= 0)
        return false;

    if (src == nullptr)
    {
        dest[0] = '\0';
        return false;
    }

    size_t src_len = strlen(src);
    if (src_len >= (size_t)dest_size)
    {
        dest[0] = '\0'; // fail cleanly rather than truncate silently
        return false;
    }

    strcpy(dest, src);
    return true;
}