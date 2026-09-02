#include <iostream>

extern "C" __declspec(dllimport) int __cdecl Add(int a, int b);
extern "C" __declspec(dllimport) size_t __cdecl StrLenA(char *r);
extern "C" __declspec(dllimport) size_t __cdecl StrLenW(wchar_t *r);
extern "C" __declspec(dllimport) int __cdecl MinArray(int *pData, int length);

struct EventData
{
    int I;
    char *Message;
};
extern "C" __declspec(dllimport) bool __cdecl PutEventData(EventData *ptr);
extern "C" __declspec(dllimport) void __cdecl NumberTransformer(long(__cdecl *square_callback)(int rs));
extern "C" __declspec(dllimport) bool __cdecl StringCopy(char *dest, int dest_size, const char *src);

int main(int, char **)
{
    // Add
    int a = 5, b = 6, c = Add(a, b);
    std::cout << "Sum of " << a << ", " << b << " = " << c << std::endl;

    // StrLenA
    char name_ansi[] = "arun kumar";
    size_t name_len_ansi = StrLenA(name_ansi);
    std::cout << "ANSI string length of '" << name_ansi << "' = " << name_len_ansi << std::endl;

    // StrLenW
    wchar_t name_unicode[] = L"praseed pai 😎";
    size_t name_len_unicode = StrLenW(name_unicode);
    std::wcout << L"UNICODE string length = " << name_len_unicode << std::endl;

    // MinArray
    int int_arr[] = {0, 5, 2, 8, -1, 4, 5, 3}, int_arr_min = MinArray(int_arr, sizeof(int_arr) / sizeof(int));
    std::cout << "Smallest element in array [";
    for (size_t i = 0; i < sizeof(int_arr) / sizeof(int); i++)
    {
        std::cout << int_arr[i] << ",";
    }
    std::cout << "\b \b" << "] = " << int_arr_min << std::endl;

    // PutEventData
    char event_data_message[] = "MicroSoft Visual C++";
    EventData ed = {};
    ed.I = 123;
    ed.Message = event_data_message;
    PutEventData(&ed);

    // NumberTransformer
    auto create_squares = [](int x)
    {
        return long(x * x);
    };
    auto create_cubes = [](int x)
    {
        return long(x * x * x);
    };
    NumberTransformer(create_squares);
    NumberTransformer(create_cubes);

    // StringCopy
    char source[] = "arun kumar", destination[sizeof(source)] = {};
    bool result = StringCopy(destination, sizeof(destination), source);
    std::cout << "StringCopy: Result=" << result << " Destination=" << destination << std::endl;
    
    return EXIT_SUCCESS;
}