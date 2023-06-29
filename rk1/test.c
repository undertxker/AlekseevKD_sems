#include <stdio.h>
#include <stdlib.h>
#include "test.h"

int symbols = 0; // кол-во введенных символов
int entered_symb = 0;
int Difficulty, Time, Mistakes;
bool isStarted = false, isMove = false, isMistake = false;

static int words = 0;
static int mist_count = 0;
static bool isEnded = false, isTheEnd = false, isClose = false;

void InitTextField (int line)
{
    textfield.left = 20;
    textfield.top = 20 + 40 * line;
    textfield.right = textfield.left + 1040;
    textfield.bottom = textfield.top + 400; 
}


int main () {

    // Создаем структуру для класса окна 
    WNDCLASSA wcl; 
    // Заполняем структуру значениями по умолчанию 
        memset(&wcl, 0, sizeof(WNDCLASSA)); 
    // Устанавливаем имя класса окна 
        wcl.lpszClassName = "speedTester"; 
    // Указываем процедуру обработки сообщений для окна 
        wcl.lpfnWndProc = WindowProc; 
    // Регистрируем класс окна 
    RegisterClassA(&wcl);

    // Создаем окно с заданными параметрами 
    hwnd_set = CreateWindow("speedTester", "Speed Tester", WS_OVERLAPPEDWINDOW, 500, 250, 330, 250, NULL, NULL, NULL, NULL); 
    // Создаем второе окно с другими параметрами 
    hwnd = CreateWindow("speedTester", "Speed Tester", WS_OVERLAPPEDWINDOW, 10, 10, 1080, 720, NULL, NULL, NULL, NULL); 
    // Получаем контекст устройства для второго окна 
    dc = GetDC(hwnd);

    // Создаем шрифт для окна 
    HFONT fontRectangle = CreateFont(40, 15, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_MODERN, "Font");

    // Показываем первое окно 
    ShowWindow(hwnd_set, SW_SHOWNORMAL);

    // Инициализируем объект указателя 
    InitPointer();

    // Создаем надписи и поля ввода на первом окне 
    HWND difficulty_txt = CreateWindow("static", "Choose difficulty", WS_VISIBLE | WS_CHILD, 10, 10, 180, 20, hwnd_set, NULL, NULL, NULL); time_txt = CreateWindow("static", "Put the time in seconds", WS_VISIBLE | WS_CHILD, 10, 50, 180, 20, hwnd_set, NULL, NULL, NULL); mistakes_txt = CreateWindow("static", "Put the number of mistakes", WS_VISIBLE | WS_CHILD, 10, 90, 180, 20, hwnd_set, NULL, NULL, NULL); HWND difficulty = CreateWindow("combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 200, 10, 100, 150, hwnd_set, Difficulty_id, NULL, NULL); SendMessage(difficulty, CB_ADDSTRING, 0, "easy"); SendMessage(difficulty, CB_ADDSTRING, 0, "medium"); SendMessage(difficulty, CB_ADDSTRING, 0, "hard"); SendMessage(difficulty, CB_SETCURSEL, 0, 0); HWND times = CreateWindow("edit", "120", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_NUMBER, 200, 50, 100, 20, hwnd_set, Time_id, NULL, NULL); HWND mistakes = CreateWindow("edit", "15", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_NUMBER, 200, 90, 100, 20, hwnd_set, Mistakes_id, NULL, NULL);

    // Создаем кнопку "Start" на первом окне 
    HWND bt_start = CreateWindow("button", "Start", WS_VISIBLE | WS_CHILD, 200, 150, 100, 50, hwnd_set, StartBtn_id, NULL, NULL);
    
    char buffer2[256][128];
    int line_len[256];
    int counter = 0, total_symb = 0;

    char *text_str[counter];
        
    int shown_lines = 0;
    
    MSG msg;  //переменная сообщения
    while (GetMessage(&msg, NULL, 0 , 0)) //getmessage вытаскивает сообщения и сохраняет их в msg
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg); //передает собщение в наше окно

        clock_t start, current_time, end;

        if(isStarted)
        {
        // Код загружает текстовые данные из файла "textlib.txt" и рандомно выбирает текст определенной сложности
            if(!isClose)
            {
                char lib_buf[128][32];
                int lib_len = 0;

                FILE *lib = fopen("textlib.txt", "r"); // открыть файл на чтение
                for(lib_len; !feof(lib); lib_len++) // прочитать строки из файла
                    fscanf(lib, "%s", &lib_buf[lib_len]);
                fclose(lib); // закрыть файл

                char difficlty_folder[3][8] = {"easy/", "medium/", "hard/"}; //массив уровней сложности
                char dif_buf[32] = "";

                srand(time(NULL));

                strcat(dif_buf, difficlty_folder[Difficulty]); // выбрать сложность из массива
                strcat(dif_buf, lib_buf[rand() % (lib_len)]); // рандомно выбрать строку из файла

                char buffer[1024][32];
                int len = 0, total_len = 0;
                
                FILE *text = fopen(dif_buf, "r"); //открыть файл на чтение
                for(len; !feof(text); len++) // прочитать строки из файла
                    fscanf(text, "%s", &buffer[len]);
                fclose(text); // закрыть файл 
                
                // Разбить текст на строки, чтобы поместить его на экран

                for(int i = 0; i < len; i++) 
                {
                    int strlength = strlen(buffer[i]);

                    if (total_len + strlength < 64)
                    {
                        for (int j = 0; j < strlength; j++)
                            buffer2[counter][total_len++] = buffer[i][j];
                        buffer2[counter][total_len++] = ' ';
                    } 
                    else 
                    {
                        line_len[counter] = total_len;
                        counter++;
                        total_len = 0;
                    }
                }
                line_len[counter] = total_len;
                for (int i = 0; i <= counter; i++)
                    total_symb += line_len[i];
                for (int i = 0; i <= counter; i++)
                    text_str[i] = buffer2[i];

                // Закрыть окно настроек и открыть игровое окно
                CloseWindow(hwnd_set); //закрывает окно 1
                ShowWindow(hwnd, SW_SHOWNORMAL); //показывает окно 2
                ShowPointer(dc); //показывает прямоугольник указателя
                SetBkMode(dc, TRANSPARENT);
                SetTextColor(dc, RGB(0, 0, 0));
                SelectObject(dc, fontRectangle);	

                //отображение текста на экране
                for (int i = 0; i <= 5 ; i++)
                {
                    InitTextField(i);
                    DrawText(dc, text_str[i], -1, &textfield, DT_SINGLELINE);
                }
                isClose = true; // Флаг, что окно уже было закрыто
                start = clock(); // Запустить таймер
            }
            
            // Код, который выполняется во время игрового процесса
            if (!isEnded) 
            {
                int line_count, current_total_symb = 0;

                // Если курсор движется
                if (isMove)
                {

                    for (int i = 0; i < line_count; i++)
                        current_total_symb += line_len[i];

                    int current_symb = buffer2[line_count][symbols - current_total_symb - 1];
                        // Проверить, была ли введена правильная буква
                    if(entered_symb != current_symb)
                    {
                        isMistake = true;
                        mist_count++;
                    }

                        // Проверить, был ли введен пробел
                    if(current_symb == ' ') words++;
                    
                    ClearField(dc, pointer.left, pointer.top, pointer.right, pointer.bottom);
                    ClearField(dc, winrect.left, winrect.bottom / 2, winrect.right, winrect.bottom);
                    MovePointer(line_len[line_count], &line_count);
                    ShowPointer(dc);

                        // Отображать текст на экране
                    for (int j = 0; j <= 5; j++) 
                    {
                        int div = line_count - shown_lines;

                        if(div == 6)
                        {
                            shown_lines += 6;
                            ClearField(dc, winrect.left, winrect.top, winrect.right, winrect.bottom); 
                            InitPointer();
                            ShowPointer(dc);
                        }

                        InitTextField(j);
                        DrawText(dc, text_str[j + shown_lines], -1, &textfield, DT_SINGLELINE);

                        if (line_count % 5 == 0 && symbols >= current_total_symb)   j == 0;

                                // Проверить на окончание игры
                        bool needEnd = ((Mistakes - mist_count) == 0) || ((Time - ((current_time - start) / CLK_TCK)) <= 0);
                        if ((line_count == counter && symbols >= (total_symb - 1)) || needEnd) 
                        {
                            end = clock();
                            isEnded = true;
                            break;
                        }
                    }

                    char buf[10] = "";
                    char table[4][20] = {"Symbols: ", "Mistakes left: ", "Time left: ", "Speed: "};

                    itoa(symbols, buf, 10);
                    char *symb = strcat(table[0], buf);
                    itoa(Mistakes - mist_count, buf, 10);
                    char *mist = strcat(table[1], buf);
                    itoa(Time - ((current_time - start) / CLK_TCK), buf, 10);
                    char *time = strcat(table[2], buf);
                    itoa(symbols * (60 / ((current_time - start) / CLK_TCK)), buf, 10);
                    char *speed = strcat(table[3], buf);

                    InitTextField(9);
                    DrawText(dc, symb, -1, &textfield, DT_SINGLELINE | DT_CENTER);
                    InitTextField(10);
                    DrawText(dc, mist, -1, &textfield, DT_SINGLELINE | DT_CENTER);
                    InitTextField(11);
                    DrawText(dc, time, -1, &textfield, DT_SINGLELINE | DT_CENTER);
                    InitTextField(12);
                    DrawText(dc, speed, -1, &textfield, DT_SINGLELINE | DT_CENTER);

                    isMove = false;
                    current_time = clock();
                }

            } 
            else 
            {
                if (!isTheEnd)
                {
                    ClearField(dc, winrect.left, winrect.top, winrect.right, winrect.bottom);
                    char buf[10] = "";
                    char table[6][20] = {"Total symbols: ","Right: ", "Mistakes: ", "Total time: ", "Average speed: ", "Words in min: "};

                    itoa(symbols, buf, 10);
                    char *tsymb = strcat(table[0], buf);
                    itoa((symbols - mist_count), buf, 10);
                    char *rsymb = strcat(table[1], buf);
                    itoa(mist_count, buf, 10);
                    char *tmist = strcat(table[2], buf);
                    itoa((end - start) / CLK_TCK, buf, 10);
                    char *ttime = strcat(table[3], buf);
                    itoa(symbols * (60 / ((end - start) / CLK_TCK)), buf, 10);
                    char *avspd = strcat(table[4], buf);
                    

                    InitTextField(3);
                    DrawText(dc, "GAME OVER", -1, &textfield, DT_SINGLELINE | DT_CENTER);
                    InitTextField(4);
                    DrawText(dc, tsymb, -1, &textfield, DT_SINGLELINE | DT_CENTER);
                    InitTextField(5);
                    DrawText(dc, rsymb, -1, &textfield, DT_SINGLELINE | DT_CENTER);
                    InitTextField(6);
                    DrawText(dc, tmist, -1, &textfield, DT_SINGLELINE | DT_CENTER);
                    InitTextField(7);
                    DrawText(dc, ttime, -1, &textfield, DT_SINGLELINE | DT_CENTER);
                    InitTextField(8);
                    DrawText(dc, avspd, -1, &textfield, DT_SINGLELINE | DT_CENTER);
                   
                    isTheEnd = true;
                }
            }
        }
    }
    return 0;
}