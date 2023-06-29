#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#define MAXTIME 3600
#define MAXMISTAKES 100
#define StartBtn_id 1
#define Difficulty_id 2
#define Time_id 3
#define Mistakes_id 4
#define pointerHeight 40
#define pointerWidth 15

RECT pointer;

struct FIELD 
{
    int left;
    int top;
    int right;
    int bottom;
} 
winrect = {0, 0, 1080, 720};

RECT textfield;

HWND time_txt, mistakes_txt, hwnd_set, hwnd;

HDC dc;

LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);