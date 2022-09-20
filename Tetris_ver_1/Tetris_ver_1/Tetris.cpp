#include "Tetris.h"

// -------------------------- 생성자  소멸자 --------------------------
Tetris::Tetris(int speed)
{
	_tetroDescentSpeed = speed;

	_boardW = WIDTH;
	_boardH = HEIGHT;

	_tetrisBoard.clear();

	_checkTetroPosChange = true; // 테트로미노가 처음 보드에 생성되는 것도 위치 변화로 간주하여 true로 초기화

	_tetroPattern.clear();

	_pressSpaceBarKey = false;
	_pressAKey = false;
	_pressDKey = false;

	_tetroPosX = 6; // 테트로미노가 생성되는 위치의 X좌표
	_tetroPosY = 2; // 테트로미노가 생성되는 위치의 Y좌표

	_tetroRotationNum = 0;

	_checkWall = false;

	_timeElapsed = 0.0f;
	_lastTime = timeGetTime();

	_contactTetroOrFloor = false;

	_timeElapsedSinceContact = 0.0f;
	_lastTimeSinceContact = 0;

	_preventInit = false;

	_isGameOver = false;
}

Tetris::~Tetris()
{

}



// ------------------------------ 초기화 ------------------------------
void Tetris::Init()
{
	// 보드 초기화
	BoardInit();

	// 보드 출력
	DrawBoard();

	// 테트로미노 선택
	SelectTetro();

	// 테트로미노 출력
	DrawTetro();
}



// ------------------------------  루프  ------------------------------
void Tetris::ProcessInput()
{
	if (_kbhit())
	{
		int key = _getch();
		if (key == GETKEY_SPACE)
		{
			PressSpaceBarKey();
		}
		else if (key == GETKEY_A || key == GETKEY_a)
		{
			PressAKey();
		}
		else if (key == GETKEY_D || key == GETKEY_d)
		{
			PressDKey();
		}
	}
}

void Tetris::Update()
{
	// _checkTetroPosChange 변수를 false로 초기화
	CheckTetroPosChangeInit();

	// 테트로미노를 시계방향으로 회전
	RotateTetro();

	// 테트로미노를 왼쪽으로 이동
	MoveTetro_Left();

	// 테트로미노를 오른쪽으로 이동
	MoveTetro_Right();

	// 테트로미노 하강
	TetroDescending();
}

void Tetris::Render()
{
	// 보드 출력
	DrawBoard();
	
	// 테트로미노 출력
	DrawTetro();

	// _isGameOver가 true일 경우 GameOver를 출력
	PrintGameOver();
}



// -------------------------- 커서 위치 변경 --------------------------
void Tetris::CursorPos(int cursorPosX, int cursorPosY)
{
	COORD cursorPos = { static_cast<SHORT>(cursorPosX), static_cast<SHORT>(cursorPosY) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
}



// --------------------------- 보드  초기화 ---------------------------
void Tetris::BoardInit()
{
	// 보드 크기만큼 공백으로 초기화
	for (int height = 0; height < _boardH; height++)
	{
		vector<string> inputValue;

		for (int width = 0; width < _boardW; width++)
		{
			inputValue.push_back("  ");
		}
		_tetrisBoard.push_back(inputValue);
	}

	// 벽에 해당하는 부분을 ■으로 초기화
	for (int width = 0; width < _boardW; width++)
	{
		_tetrisBoard[0][width] = "■";
		_tetrisBoard[1][width] = "■";
		_tetrisBoard[_boardH - 2][width] = "■";
		_tetrisBoard[_boardH - 1][width] = "■";
	}

	for (int height = 1; height < _boardH - 1; height++)
	{
		_tetrisBoard[height][0] = "■";
		_tetrisBoard[height][1] = "■";
		_tetrisBoard[height][_boardW - 2] = "■";
		_tetrisBoard[height][_boardW - 1] = "■";
	}
}



// ---------------------------- 보드  출력 ----------------------------
void Tetris::DrawBoard()
{
	if (_checkTetroPosChange == true)
	{
		CursorPos(0, 0);

		for (int height = 0; height < _boardH; height++)
		{
			for (int width = 0; width < _boardW; width++)
			{
				cout << _tetrisBoard[height][width];
			}
			cout << endl;
		}
	}
}



// ------------------------- 테트로미노  선택 -------------------------
void Tetris::SelectTetro()
{
	int randValue = rand() % 7;

	switch (randValue)
	{
	case 0:
		_tetroPattern = pattern_I;
		break;
	case 1:
		_tetroPattern = pattern_O;
		break;
	case 2:
		_tetroPattern = pattern_T;
		break;
	case 3:
		_tetroPattern = pattern_J;
		break;
	case 4:
		_tetroPattern = pattern_L;
		break;
	case 5:
		_tetroPattern = pattern_S;
		break;
	case 6:
		_tetroPattern = pattern_Z;
		break;
	}
}



// ------------------------- 테트로미노  출력 -------------------------
void Tetris::DrawTetro()
{
	// 테트로미노의 위치가 변경됬을 때 출력
	if (_checkTetroPosChange == true)
	{
		for (int tetroNum = 0; tetroNum < 4; tetroNum++)
		{
			int cursorPosX = _tetroPattern[_tetroRotationNum][tetroNum][0] + (_tetroPosX * 2); // □, ■, ▣ 문자가 커서 2칸을 차지함으로 * 2
			int cursorPosY = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY;
			CursorPos(cursorPosX, cursorPosY);
			cout << "□";
		}
	}
}



// --------------------------- 키 입력 확인 ---------------------------
void Tetris::PressSpaceBarKey()
{
	_pressSpaceBarKey = true;
}

void Tetris::PressAKey()
{
	_pressAKey = true;
}

void Tetris::PressDKey()
{
	_pressDKey = true;
}



// ------------ _checkTetroPosChange 변수를 false로 초기화 ------------
void Tetris::CheckTetroPosChangeInit()
{
	_checkTetroPosChange = false;
}



// ------------ 조작키로 테트로미노를  회전 및 좌우로 이동 ------------
void Tetris::RotateTetro()
{
	if (_pressSpaceBarKey == true)
	{
		_pressSpaceBarKey = false;

		int temp = _tetroRotationNum; // 회전할 수 없는 상황을 방지하여 회전하기 전 회전 번호를 temp에 저장
		_tetroRotationNum = (_tetroRotationNum + 1) % 4;
		CheckSpaceWhenRotating();

		// 회전할 수 없는 경우
		if (_checkWall == true) 
		{
			_tetroRotationNum = temp;
			_checkWall = false;
		}
	}
}

void Tetris::MoveTetro_Left()
{
	if (_pressAKey == true)
	{
		_pressAKey = false;

		_tetroPosX--;
		CheckSpaceWhenMove();

		// 왼쪽으로 이동할 수 없는 경우
		if (_checkWall == true)
		{
			_tetroPosX++;
			_checkWall = false;
		}
	}
}

void Tetris::MoveTetro_Right()
{
	if (_pressDKey == true)
	{
		_pressDKey = false;

		_tetroPosX++;
		CheckSpaceWhenMove();

		// 오른쪽으로 이동할 수 없는 경우
		if (_checkWall == true)
		{
			_tetroPosX--;
			_checkWall = false;
		}
	}
}



// - 테트로미노가 이동할 공간에 저장된 테트로미노 및 벽이 있는지 확인 -
void Tetris::CheckSpaceWhenRotating()
{
	int rotationPosNum = 0;

	while (rotationPosNum != 25)
	{
		for (int tetroNum = 0; tetroNum < 4; tetroNum++)
		{
			int pos_X = (_tetroPattern[_tetroRotationNum][tetroNum][0] / 2) + _tetroPosX + tetroRotationPos[rotationPosNum][0];
			int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY + tetroRotationPos[rotationPosNum][1];

			// 테트로미노가 벽을 넘어갔을 경우
			if (pos_X < 2 || pos_X > 11 || pos_Y < 2 || pos_Y > 21)
			{
				_checkWall = true;
				rotationPosNum++;
				break;
			}

			// 테트로미노가 다른 테트로미노와 겹쳤을 경우
			if (_tetrisBoard[pos_Y][pos_X].compare("▣") == 0)
			{
				_checkWall = true;
				rotationPosNum++;
				break;
			}

			_checkWall = false;
		}

		// 벽 또는 다른 테트로미노와 겹치지 않았을 경우
		if (_checkWall == false)
		{
			_tetroPosX += tetroRotationPos[rotationPosNum][0];
			_tetroPosY += tetroRotationPos[rotationPosNum][1];
			_checkTetroPosChange = true;
			break;
		}
	}

	
}

void Tetris::CheckSpaceWhenMove()
{
	for (int tetroNum = 0; tetroNum < 4; tetroNum++)
	{
		int pos_X = (_tetroPattern[_tetroRotationNum][tetroNum][0] / 2) + _tetroPosX;
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY;

		// 테트로미노가 벽 또는 다른 테트로미노와 겹쳤을 경우
		if (_tetrisBoard[pos_Y][pos_X].compare("■") == 0 || _tetrisBoard[pos_Y][pos_X].compare("▣") == 0)
		{
			_checkWall = true;
			break;
		}
	}

	// 테트로미노가 벽 또는 다른 테트로미노와 겹치지 않았을 경우
	if (_checkWall == false)
	{
		_checkTetroPosChange = true;
	}
}



// ------------------------- 테트로미노  하강 -------------------------
void Tetris::TetroDescending()
{
	DWORD curTime = timeGetTime(); // 현재 시간
	float timeDelta = (curTime - _lastTime) * 0.001f; // 시간을 1초 단위로 변환

	_timeElapsed += timeDelta;
	_lastTime = curTime;

	// 1.0f / _tetroDescentSpeed만큼 시간이 흐르면 AutoMoveTetro_Down()을 실행
	if (_timeElapsed >= 1.0f / _tetroDescentSpeed)
	{
		_timeElapsed = 0.0f;
		_lastTime = timeGetTime();

		AutoMoveTetro_Down();
	}
}

void Tetris::AutoMoveTetro_Down()
{
	_tetroPosY++;
	CheckSpaceWhenDescent();

	// 아래로 이동할 수 없는 경우
	if (_checkWall == true)
	{
		_tetroPosY--;
		_checkWall = false;
	}

	CheckContactTime();
}

void Tetris::CheckSpaceWhenDescent()
{
	for (int tetroNum = 0; tetroNum < 4; tetroNum++)
	{
		int pos_X = (_tetroPattern[_tetroRotationNum][tetroNum][0] / 2) + _tetroPosX;
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY;

		// 테트로미노가 벽 또는 다른 테트로미노와 겹쳤을 경우
		if (_tetrisBoard[pos_Y][pos_X].compare("■") == 0 || _tetrisBoard[pos_Y][pos_X].compare("▣") == 0)
		{
			_checkWall = true;
			_contactTetroOrFloor = true; // 테트로미노 또는 바닥과 접촉이 true
			break;
		}
	}

	// 테트로미노가 벽 또는 다른 테트로미노와 겹치치 않았을 경우
	if (_checkWall == false)
	{
		_contactTetroOrFloor = false; // 테트로미노 또는 바닥과 접촉이 false
		_checkTetroPosChange = true;
	}
}



// --- 테트로미노가 다른 테트로미노 또는  바닥에 접촉한 시간을 확인 ---
void Tetris::CheckContactTime()
{
	if (_contactTetroOrFloor == true)
	{
		// 해당 함수에 들어올 때마다 초기화되는 것을 방지
		if (_preventInit == false)
		{
			_preventInit = true;
			_timeElapsedSinceContact = 0.0f;
			_lastTimeSinceContact = timeGetTime();
		}

		DWORD curTimeSinceContact = timeGetTime(); // 접촉 후 현재 시간
		float timeDeltaSinceContact = (curTimeSinceContact - _lastTimeSinceContact) * 0.001f; // 프레임을 1초 단위로 변환

		_timeElapsedSinceContact += timeDeltaSinceContact;

		// 1.0f / 4 만큼 시간이 흐르면 TetroStorage()을 실행
		if (_timeElapsedSinceContact >= 1.0f / 4)
		{
			_contactTetroOrFloor = false;
			_preventInit = false;

			TetroStorage();
		}
		_lastTimeSinceContact = curTimeSinceContact;
	}
	else
	{
		_preventInit = false;
	}
}



// --------------------- 테트로미노를 보드에 저장 ---------------------
void Tetris::TetroStorage()
{
	for (int tetroNum = 0; tetroNum < 4; tetroNum++)
	{
		int pos_X = (_tetroPattern[_tetroRotationNum][tetroNum][0] / 2) + _tetroPosX;
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY;
		_tetrisBoard[pos_Y][pos_X] = "▣";
	}
	_checkTetroPosChange = true;

	TetroPosInit();
	CheckLine();
	CheckGameOver();
}





// ---------------------- 테트로미노 위치 초기화 ----------------------
void Tetris::TetroPosInit()
{
	_tetroPattern.clear();
	_tetroPosX = 6;
	_tetroPosY = 2;
	_tetroRotationNum = 0;

	SelectTetro();
}



// ------- 빈틈없이 채워진 가로줄이 있는지 확인하고 있으면 제거 -------
void Tetris::CheckLine()
{
	for (int height = 2; height < _boardH - 2; height++)
	{
		int checkTetroCount = 0;

		for (int width = 2; width < _boardW - 2; width++)
		{
			if (_tetrisBoard[height][width].compare("▣") == 0)
			{
				checkTetroCount++;
			}

			if (checkTetroCount == 10)
			{
				LineSwap(height);
			}
		}
	}
}

void Tetris::LineSwap(int lineNum)
{
	// lineNum 번째 줄을 공백으로 변경
	for (int width = 2; width < _boardW - 2; width++)
	{
		_tetrisBoard[lineNum][width] = "  ";
	}

	// lineNum 번째 줄을 lineNum - 1 번째 줄하고 바꿈
	for (int row = lineNum; row > 2; row--)
	{
		vector<string> temp;

		for (int width = 2; width < _boardW - 2; width++)
		{
			temp.push_back(_tetrisBoard[row][width]);
		}

		for (int width = 2; width < _boardW - 2; width++)
		{
			_tetrisBoard[row][width] = _tetrisBoard[row - 1][width];
		}
		
		for (int width = 2; width < _boardW - 2; width++)
		{
			_tetrisBoard[row - 1][width] = temp[width - 2];
		}
	}
}



// -------------------------- 게임오버  확인 --------------------------
void Tetris::CheckGameOver()
{
	for (int tetroNum = 0; tetroNum < 4; tetroNum++)
	{
		int pos_X = (_tetroPattern[_tetroRotationNum][tetroNum][0] / 2) + _tetroPosX;
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY;

		// 테트로미노가 생성되자마자 다른 테트로미노와 겹쳐있을 경우
		if (_tetrisBoard[pos_Y][pos_X].compare("▣") == 0)
		{
			_isGameOver = true;
			break;
		}
	}
}

bool Tetris::IsGameOver()
{
	return _isGameOver;
}



// -------------------------- 게임오버  출력 --------------------------
void Tetris::PrintGameOver()
{
	if (_isGameOver == true)
	{
		CursorPos(0, 25);
		cout << "GameOver~~~~~" << endl;
	}
}



// ----------------------------- 클래스밖 -----------------------------
// 게임 실행
void Run(int speed)
{
	CursorHide();

	int tetroDescentSpeed = TetroDescentSpeedLimit(speed);

	Tetris tetris(tetroDescentSpeed);
	tetris.Init();

	while (tetris.IsGameOver() == false)
	{
		tetris.ProcessInput();
		tetris.Update();
		tetris.Render();
	}
}

// 커서 숨기기
void CursorHide()
{
	CONSOLE_CURSOR_INFO consoleCursor;
	consoleCursor.bVisible = false;
	consoleCursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleCursor);
}

// 테트로미노 하강 속도 제한
int TetroDescentSpeedLimit(int speed)
{
	// 하강 속도가 1보다 작을 경우
	if (speed < 1)
	{
		speed = 1;
	}

	// 하강 속도가 10보다 클 경우
	if (speed > 10)
	{
		speed = 10;
	}

	return speed;
}