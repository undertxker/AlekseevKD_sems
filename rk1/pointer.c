#include "pointer.h"

extern RECT pointer; //курсор прямоугольник
extern bool isMistake; // ошибся или нет 

void InitPointer () //создаем прямоугольник курсора
{
    pointer.left = 20; 
    pointer.top = 20;
    pointer.right = pointer.left + pointerWidth;
    pointer.bottom = pointer.top + pointerHeight;  //координаты краев 
}

void MovePointer (int len, int* line) // функция передвижения курсора
{
    pointer.left += pointerWidth + 1;
    if (pointer.left > (20 + (pointerWidth + 1) * (len - 1))) 
    {
        pointer.left = 20;
        pointer.top += 40;
        (*line)++;
    }
    pointer.right = pointer.left + pointerWidth;
    pointer.bottom = pointer.top + pointerHeight; 
}

void ShowPointer (HDC dc) // Объявляем функцию для отображения указателя на экране
{
    SelectObject(dc, GetStockObject(DC_BRUSH)); // Выбираем кисть по умолчанию
    SetDCBrushColor(dc, RGB(0, 255, 0)); // Устанавливаем цвет кисти зеленым
    SelectObject(dc, GetStockObject(DC_PEN)); // Выбираем перо по умолчанию
    if (isMistake) // Если была допущена ошибка
        SetDCBrushColor(dc, RGB(255, 0, 0)); // Устанавливаем цвет кисти красным
    else
        SetDCBrushColor(dc, RGB(0, 255, 0)); // Устанавливаем цвет кисти зеленым
    Rectangle(dc, pointer.left, pointer.top, pointer.right, pointer.bottom); // Рисуем прямоугольник указателя
    isMistake = false; // Обнуляем флаг ошибки после отображения указателя
}

void ClearField (HDC dc, int left, int top, int right, int bottom) // Объявляем функцию для очистки поля
{
    SelectObject(dc, GetStockObject(DC_PEN)); // Выбираем перо по умолчанию
    SetDCPenColor(dc,RGB(255, 255, 255)); // Устанавливаем цвет пера белым
    SelectObject(dc, GetStockObject(DC_BRUSH)); // Выбираем кисть по умолчанию
    SetDCBrushColor(dc, RGB(255, 255, 255)); // Устанавливаем цвет кисти белым
    Rectangle(dc, left, top, right, bottom); // Рисуем прямоугольник, заполняя его белым цветом
}