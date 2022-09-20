#pragma once
#include <iostream>
#include <conio.h>
#include <vector>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

void ControlCursorMove(int x, int y, int speed); // 커서의 움직임을 제어