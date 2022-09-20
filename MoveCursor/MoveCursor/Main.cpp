#include <iostream>
#include <vector>
#include "CursorInfo.h"
#include "CursorControl.h"
using namespace std;

int main()
{
	// 커서가 움직일 공간의 가로 길이, 세로 길이와 커서의 이동 속도 입력
	vector<int> cursorData;
	cursorData = SetCursorData();

	// 커서의 움직임 제어
	ControlCursorMove(cursorData[0], cursorData[1], cursorData[2]);
}