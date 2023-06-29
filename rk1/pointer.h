#ifndef POINTER_H
#define POINTER_h

#include <stdbool.h>
#include <windows.h>

#define pointerHeight 40
#define pointerWidth 15

void InitPointer();
void MovePointer(int len, int* line);
void ShowPointer(HDC dc);
void ClearField (HDC dc, int left, int top, int right, int bottom);

#endif // POINTER_H