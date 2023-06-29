#include "winproc.h"

extern int symbols, entered_symb;
extern int Difficulty, Time, Mistakes;
extern bool isMove, isStarted;

extern HWND time_txt, mistakes_txt;

LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) 
    {
        case WM_DESTROY: //удаление окна
            PostQuitMessage(0);
            break;
        case WM_CHAR: 
            isMove = true;
            symbols++;
            entered_symb = wparam;
            break;
        case WM_COMMAND:
            switch (LOWORD(wparam))
            {
                case StartBtn_id:
                    isStarted = true;
                    break;
                case Difficulty_id:
                    if(HIWORD(wparam) == CBN_SELCHANGE)
                        Difficulty = SendMessage(lparam, CB_GETCURSEL, 0, 0);
                    break;
                case Time_id:
                    char get_time[100];
                    GetWindowText(lparam, get_time, 100);
                    Time = atoi(get_time);
                    if (Time > MAXTIME)
                    {
                        SetWindowText(time_txt, "so long");
                    }
                    break;
                case Mistakes_id:
                    char get_mist[100];
                    GetWindowText(lparam, get_mist, 100);
                    Mistakes = atoi(get_mist);
                    if (Mistakes > MAXMISTAKES)
                    {
                        SetWindowText(mistakes_txt, "so mach");
                    }
                    break;
            }
            break;
        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
            break;
    }
}