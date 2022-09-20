#include "CursorControl.h"
#include "FrameControl.h"

class Snail
{
public:
	Snail(int size_X, int size_Y, int cursorSpeed)
	{
		_dir = 1;		// 숫자에 따라서 커서의 이동방향 변경 [1: 오른쪽, 2: 아래, 3: 왼쪽, 4: 위]
		_pos_X = -2;	// 커서의 움직임을 출력할 때 먼저 +2를 하고 출력하기 때문에 -2부터 시작 
						// (▣ 문자의 크기가 커서 2칸을 차지하기 때문에 +2를 해준다.)
		_pos_Y = 0;

		_cursorSpeed = cursorSpeed;
		_moveCount = 0;	// 한 방향으로 이동 중인 커서가 이동한 수

		_blankSpace_X = size_X;	// 커서가 이동할 수 있는 X축 거리
		_blankSpace_Y = size_Y;	// 커서가 이동할 수 있는 Y축 거리
		_height = size_Y;

		_move = false;	// 커서가 움직이는지 확인하는 변수
		_full = false;	// 움직일 공간이 있는지 확인하는 변수
	}

	~Snail()
	{

	}

	void MoveCursor();
	void PrintCursor();
	void Pause();
	void BlinkCursor();
	void PrintBlinkCursor();
	void Unpause();
	void EndCursorPos();
	bool getFull();

private:
	int _dir, _pos_X, _pos_Y;
	int _cursorSpeed, _moveCount;
	int _blankSpace_X, _blankSpace_Y, _height;
	bool _move, _full;
};



// 커서의 좌표 확인
void Snail::MoveCursor()
{
	// 오른쪽으로 이동
	if (_dir == 1 && _blankSpace_X != 0)
	{
		_pos_X += 2;
		_moveCount++;

		// X축으로 이동할 수 있는 거리와 이동한 수가 같을 경우
		if (_moveCount == _blankSpace_X)
		{
			_moveCount = 0;
			_dir = 2;
			_blankSpace_X--;
			_blankSpace_Y--;
		}
	}
	// 아래로 이동
	else if (_dir == 2 && _blankSpace_Y != 0)
	{
		_pos_Y++;
		_moveCount++;

		// Y축으로 이동할 수 있는 거리와 이동한 수가 같을 경우
		if (_moveCount == _blankSpace_Y)
		{
			_moveCount = 0;
			_dir = 3;
		}
	}
	// 왼쪽으로 이동
	else if (_dir == 3 && _blankSpace_X != 0)
	{
		_pos_X -= 2;
		_moveCount++;

		// X축으로 이동할 수 있는 거리와 이동한 수가 같을 경우
		if (_moveCount == _blankSpace_X)
		{
			_moveCount = 0;
			_dir = 4;
			_blankSpace_X--;
			_blankSpace_Y--;
		}
	}
	// 위로 이동
	else if (_dir == 4 && _blankSpace_Y != 0)
	{
		_pos_Y--;
		_moveCount++;

		// Y축으로 이동할 수 있는 거리와 이동한 수가 같을 경우
		if (_moveCount == _blankSpace_Y)
		{
			_moveCount = 0;
			_dir = 1;
		}
	}

	// X축 방향으로 이동해야 되는데 빈공간이 없을 경우
	if ((_dir == 1 || _dir == 3) && _blankSpace_X == 0)
	{
		_full = true;
	}
	// Y축 방향으로 이동해야 되는데 빈공간이 없을 경우
	else if ((_dir == 2 || _dir == 4) && _blankSpace_Y == 0)
	{
		_full = true;
	}
}



// 커서 출력
void Snail::PrintCursor()
{
	COORD pos = { static_cast<SHORT>(_pos_X), static_cast<SHORT>(_pos_Y) };
	::SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	cout << "▣";

	SpeedControl(_cursorSpeed);
}



// 일시정지
void Snail::Pause()
{
	if (_kbhit())
	{
		char keyInput = _getch();
		int temp = _cursorSpeed;

		_cursorSpeed = 4; // 1초에 4번 깜빡이는 속도

		// 스페이스바를 누를 경우 일시 정지
		if (keyInput == 32)
		{
			BlinkCursor();
		}
		_cursorSpeed = temp;
	}
}



// 일시정지일 때 커서 점멸
void Snail::BlinkCursor()
{
	bool blink = true;

	while (_move != true)
	{
		// 정지한 위치에 커서를 제거
		if (blink == true)
		{
			PrintBlinkCursor();
			blink = false;
		}
		// 정지한 위치에 커서를 생성
		else
		{
			PrintCursor();
			blink = true;
		}
		Unpause();
	}

	// 정지한 위치에 커서를 생성
	PrintCursor();
	_move = false;
}



// 커서 제거
void Snail::PrintBlinkCursor()
{
	COORD pos = { static_cast<SHORT>(_pos_X), static_cast<SHORT>(_pos_Y) };
	::SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	cout << "  ";

	SpeedControl(_cursorSpeed);
}



// 일시정지 해제
void Snail::Unpause()
{
	if (_kbhit())
	{
		char keyInput = _getch();

		// 스페이스바를 누를 경우 일시 정지 해제
		if (keyInput == 32)
		{
			_move = true;
		}
	}
}



// 커서 이동이 끝난 후 커서가 이동할 위치
void Snail::EndCursorPos()
{
	COORD pos = { static_cast<SHORT>(_pos_X), static_cast<SHORT>(_height) };
	::SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}



bool Snail::getFull()
{
	return _full;
}



// 커서의 움직임을 제어
void ControlCursorMove(int x, int y, int speed)
{
	Snail snail(x, y, speed);

	printf("\x1b[?25l");	// 커서를 숨김
	system("cls");			// 콘솔 화면 초기화

	while (snail.getFull() != true)
	{
		snail.MoveCursor();
		snail.PrintCursor();
		snail.Pause();
	}
	snail.EndCursorPos();
}