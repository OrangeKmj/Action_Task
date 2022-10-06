#include "Tetris.h"

// -------------------------- 생성자  소멸자 --------------------------
Tetris::Tetris(int speed)
{
	_tetroDescentSpeed = speed;

	_borderLineBoardW = BorderLineBoardSize::WIDTH_BORDER;
	_borderLineBoardH = BorderLineBoardSize::HEIGHT_BORDER;

	_tetrisBorderLineBoard.clear();

	_boardW = ScreenAndLogicBoardSize::WIDTH;
	_boardH = ScreenAndLogicBoardSize::HEIGHT;

	_tetrisScreenBoard.clear();
	_tetrisLogicBoard.clear();

	// [만약 테트리스 경계선 보드 크기를 키워서 테트리스 화면 보드를 옮기고 싶으면 _screenBoardStartPos를 변경]
	_screenBoardStartPosX = 1;
	_screenBoardStartPosY = 1;

	_checkTetroPosChange = true; // 테트로미노가 처음 보드에 생성되는 것도 위치 변화로 간주하여 true로 초기화

	_tetroPattern.clear();

	_pressSpaceBarKey = false;
	_pressAKey = false;
	_pressDKey = false;

	_pressCKey = false;

	_tetroPosX = ((_boardW / 2) - 1) + _screenBoardStartPosX; // 테트로미노가 생성되는 위치의 X좌표
	_tetroPosY = 0 + _screenBoardStartPosY; // 테트로미노가 생성되는 위치의 Y좌표

	_tetroRotationNum = 0;

	_checkWall = false;

	_timeElapsed = 0.0f;
	_lastTime = timeGetTime();

	_contactTetroOrFloor = false;

	_timeElapsedSinceContact = 0.0f;
	_lastTimeSinceContact = 0;

	_preventInit = false;

	_isGameOver = false;

	rotstop = 0;

	max_weight_idx = 0;

	memset(map, 0, sizeof(map));
}

Tetris::~Tetris()
{

}



// ------------------------------ 초기화 ------------------------------
void Tetris::Init()
{
	// 테트리스 보드 초기화
	BoardInit();

	// 테트리스 경계선 보드 출력
	DrawBorderLineBoard();

	// 테트리스 화면 보드 출력
	DrawScreenBoard();

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
		if (key == GetKey::GETKEY_SPACE)
		{
			PressSpaceBarKey();
		}
		else if (key == GetKey::GETKEY_A || key == GetKey::GETKEY_a)
		{
			PressAKey();
		}
		else if (key == GetKey::GETKEY_D || key == GetKey::GETKEY_d)
		{
			PressDKey();
		}
		//키보드 이벤트 추가
		else if (key == GetKey::GETKEY_C || key == GetKey::GETKEY_c)
		{
			PressCKey();
		}
		else if (key == GetKey::GETKEY_P || key == GetKey::GETKEY_p)
		{
			PressPKey();
		}
	}
}

void Tetris::Update()
{
	// _checkTetroPosChange 변수를 false로 초기화
	CheckTetroPosChangeInit();

	controlAi();

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
	// 테트리스 화면 보드 출력
	DrawScreenBoard();

	// 테트로미노 출력
	DrawTetro();

	// _isGameOver가 true일 경우 GameOver를 출력
	PrintGameOver();

	showKeySet(); //조작키 출력
}



// -------------------------- 커서 위치 변경 --------------------------
void Tetris::CursorPos(int cursorPosX, int cursorPosY)
{
	COORD cursorPos = { static_cast<SHORT>(cursorPosX), static_cast<SHORT>(cursorPosY) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
}



// ----------------------- 테트리스 보드 초기화 -----------------------
void Tetris::BoardInit()
{
	// 테트리스 경계선 보드 초기화
	for (int height = 0; height < _borderLineBoardH; height++)
	{
		vector<string> inputValue;

		for (int width = 0; width < _borderLineBoardW; width++)
		{
			inputValue.push_back("■");
		}
		_tetrisBorderLineBoard.push_back(inputValue);
	}

	// 테트리스 화면, 로직 보드 초기화
	for (int height = 0; height < _boardH; height++)
	{
		vector<string> inputValue;

		for (int width = 0; width < _boardW; width++)
		{
			inputValue.push_back("  ");
		}
		_tetrisScreenBoard.push_back(inputValue);
		_tetrisLogicBoard.push_back(inputValue);
	}
}



// ------------------------ 테트리스 보드 출력 ------------------------
void Tetris::DrawBorderLineBoard()
{
	CursorPos(0, 0);

	for (int height = 0; height < _borderLineBoardH; height++)
	{
		for (int width = 0; width < _borderLineBoardW; width++)
		{
			cout << _tetrisBorderLineBoard[height][width];
		}
		cout << endl;
	}
}

void Tetris::DrawScreenBoard()
{
	if (_checkTetroPosChange == true)
	{
		for (int height = 0; height < _boardH; height++)
		{
			// UI 보드 안에 화면 보드를 넣기 위해 커서 위치를 Y 좌표가 변경될 때마다 재설정
			// ■, ▣ 문자가 커서를 2칸 차지함으로 시작 X 좌표에 * 2 
			int cursorPosX = _screenBoardStartPosX * 2;
			int cursorPosY = _screenBoardStartPosY + height;
			CursorPos(cursorPosX, cursorPosY);

			for (int width = 0; width < _boardW; width++)
			{
				cout << _tetrisScreenBoard[height][width];
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
			int cursorPosX = (_tetroPattern[_tetroRotationNum][tetroNum][0] * 2) + (_tetroPosX * 2); // □ 문자가 커서를 2칸 차지함으로 벡터와 변수에 모두 * 2
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
			// 원래는 테트리스 경계선 벡터 기준으로 테트로미노가 생성되나
			// 테트리스 로직 벡터 기준으로 테트로미노를 보기 위해서 _tetroPosX - _screenBoardStartPosX, _tetroPosY - _screenBoardStartPosY
			int pos_X = _tetroPattern[_tetroRotationNum][tetroNum][0] + (_tetroPosX - _screenBoardStartPosX) + tetroRotationPos[rotationPosNum][0];
			int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + (_tetroPosY - _screenBoardStartPosY) + tetroRotationPos[rotationPosNum][1];

			// 테트로미노가 테트리스 로직 벡터 밖으로 넘어갔을 경우
			if (pos_X < 0 || pos_X >(_boardW - 1) || pos_Y < 0 || pos_Y >(_boardH - 1))
			{
				_checkWall = true;
				rotationPosNum++;
				break;
			}

			// 테트로미노가 다른 테트로미노와 겹쳤을 경우
			if (_tetrisLogicBoard[pos_Y][pos_X].compare("▣") == 0)
			{
				_checkWall = true;
				rotationPosNum++;
				break;
			}

			_checkWall = false;
		}

		// 테트로미노가 움직일 수 있는 경우
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
		// 원래는 테트리스 경계선 벡터 기준으로 테트로미노가 생성되나
		// 테트리스 로직 벡터 기준으로 테트로미노를 보기 위해서 _tetroPosX - _screenBoardStartPosX, _tetroPosY - _screenBoardStartPosY
		int pos_X = _tetroPattern[_tetroRotationNum][tetroNum][0] + (_tetroPosX - _screenBoardStartPosX);
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + (_tetroPosY - _screenBoardStartPosY);

		// 테트로미노가 테트리스 로직 벡터 밖으로 넘어갔을 경우
		if (pos_X < 0 || pos_X >(_boardW - 1))
		{
			_checkWall = true;
			break;
		}

		// 테트로미노가 다른 테트로미노와 겹쳤을 경우
		if (_tetrisLogicBoard[pos_Y][pos_X].compare("▣") == 0)
		{
			_checkWall = true;
			break;
		}
	}

	// 테트로미노가 움직일 수 있는 경우
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
		// 원래는 테트리스 경계선 벡터 기준으로 테트로미노가 생성되나
		// 테트리스 로직 벡터 기준으로 테트로미노를 보기 위해서 _tetroPosX - _screenBoardStartPosX, _tetroPosY - _screenBoardStartPosY
		int pos_X = _tetroPattern[_tetroRotationNum][tetroNum][0] + (_tetroPosX - _screenBoardStartPosX);
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + (_tetroPosY - _screenBoardStartPosY);

		// 테트로미노가 테트리스 로직 벡터 밖으로 넘어갔을 경우
		if (pos_Y < 0 || pos_Y >(_boardH - 1))
		{
			_checkWall = true;
			_contactTetroOrFloor = true; // 테트로미노 또는 바닥과 접촉을 확인
			break;
		}

		// 테트로미노가 다른 테트로미노와 겹쳤을 경우
		if (_tetrisLogicBoard[pos_Y][pos_X].compare("▣") == 0)
		{
			_checkWall = true;
			_contactTetroOrFloor = true; // 테트로미노 또는 바닥과 접촉을 확인
			break;
		}
	}

	// 테트로미노가 움직일 수 있는 경우
	if (_checkWall == false)
	{
		_contactTetroOrFloor = false; // 테트로미노 또는 바닥과 떨어짐을 확인
		_checkTetroPosChange = true;
	}
}



// --- 테트로미노가 다른 테트로미노 또는  바닥에 접촉한 시간을 확인 ---
void Tetris::CheckContactTime()
{
	if (_contactTetroOrFloor == true)
	{
		// CheckContactTime() 메소드에 들어올 때마다 초기화되는 것을 방지
		if (_preventInit == false)
		{
			_preventInit = true;
			_timeElapsedSinceContact = 0.0f;
			_lastTimeSinceContact = timeGetTime();
		}

		DWORD curTimeSinceContact = timeGetTime(); // 접촉 후 현재 시간
		float timeDeltaSinceContact = (curTimeSinceContact - _lastTimeSinceContact) * 0.001f; // 시간을 1초 단위로 변환

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



// ---------- 테트로미노를 테트리스 로직 및  화면 보드에 저장 ----------
void Tetris::TetroStorage()
{
	for (int tetroNum = 0; tetroNum < 4; tetroNum++)
	{
		// 원래는 테트리스 경계선 벡터 기준으로 테트로미노가 생성되나
		// 테트리스 로직 벡터 기준으로 테트로미노를 보기 위해서 _tetroPosX - _screenBoardStartPosX, _tetroPosY - _screenBoardStartPosY
		int pos_X = _tetroPattern[_tetroRotationNum][tetroNum][0] + (_tetroPosX - _screenBoardStartPosX);
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + (_tetroPosY - _screenBoardStartPosY);

		_tetrisLogicBoard[pos_Y][pos_X] = "▣";
	}

	TetroPosInit();
	_checkTetroPosChange = true;

	CheckLine();
	_tetrisScreenBoard.clear();
	_tetrisScreenBoard = _tetrisLogicBoard; // 채워진 줄이 있는지 확인이 끝나면 테트리스 로직 보드를 테트리스 화면 보드에 삽입

	CheckGameOver();
}





// ---------------------- 테트로미노 위치 초기화 ----------------------
void Tetris::TetroPosInit()
{
	_tetroPattern.clear();
	_tetroPosX = ((_boardW / 2) - 1) + _screenBoardStartPosX;
	_tetroPosY = 0 + _screenBoardStartPosY;
	_tetroRotationNum = 0;

	rotstop = 0;

	tetro_move_weight.clear();
	tetro_rotation_weight.clear();
	tetro_movex.clear();

	SelectTetro();
}



// ------- 빈틈없이 채워진 가로줄이 있는지 확인하고 있으면 제거 -------
void Tetris::CheckLine()
{
	for (int height = 0; height < _boardH; height++)
	{
		int checkTetroCount = 0;

		for (int width = 0; width < _boardW; width++)
		{
			if (_tetrisLogicBoard[height][width].compare("▣") == 0)
			{
				checkTetroCount++;
			}

			if (checkTetroCount == _boardW)
			{
				LineSwap(height);
			}
		}
	}
}

void Tetris::LineSwap(int lineNum)
{
	// lineNum 번째 줄을 공백으로 변경
	for (int width = 0; width < _boardW; width++)
	{
		_tetrisLogicBoard[lineNum][width] = "  ";
	}

	// lineNum 번째 줄을 lineNum - 1 번째 줄하고 바꿈
	for (int row = lineNum; row > 0; row--)
	{
		vector<string> temp;

		for (int width = 0; width < _boardW; width++)
		{
			temp.push_back(_tetrisLogicBoard[row][width]);
		}

		for (int width = 0; width < _boardW; width++)
		{
			_tetrisLogicBoard[row][width] = _tetrisLogicBoard[row - 1][width];
		}

		for (int width = 0; width < _boardW; width++)
		{
			_tetrisLogicBoard[row - 1][width] = temp[width];
		}
	}
}



// -------------------------- 게임오버  확인 --------------------------
void Tetris::CheckGameOver()
{
	for (int tetroNum = 0; tetroNum < 4; tetroNum++)
	{
		// 원래는 테트리스 경계선 벡터 기준으로 테트로미노가 생성되나
		// 테트리스 로직 벡터 기준으로 테트로미노를 보기 위해서 _tetroPosX - _screenBoardStartPosX, _tetroPosY - _screenBoardStartPosY
		int pos_X = _tetroPattern[_tetroRotationNum][tetroNum][0] + (_tetroPosX - _screenBoardStartPosX);
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + (_tetroPosY - _screenBoardStartPosY);

		// 테트로미노가 생성되자마자 다른 테트로미노와 겹쳐있을 경우
		if (_tetrisLogicBoard[pos_Y][pos_X].compare("▣") == 0)
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
		int cursorPosY = _borderLineBoardH + 1;
		CursorPos(0, cursorPosY);

		cout << "GameOver~~~~~" << endl;
	}
}


// ----------------------------- 추가함수 -----------------------------
void Tetris::PressCKey()
{
	_pressCKey = true;
}

void Tetris::PressPKey()
{
	_pressCKey = false;
}

void Tetris::controlAi()
{
	if (_pressCKey == false)
	{
		return;
	}

	setMapWeight();
	maxWeight();

	if (_timeElapsed >= 0.2f / _tetroDescentSpeed)
	{
		rotateAi();
		moveAi();
	}
}

void Tetris::rotateAi()
{
	if (rotstop == 0) {

		for (int i = 0; i < 4; i++) 
		{
			if (tetro_rotation_weight[max_weight_idx] == i) 
			{
				rotstop = 1;
				break;
			}

			PressSpaceBarKey();
			RotateTetro();
		}
	}
}

void Tetris::moveAi()
{
	for (int i = 0; i < 20; i++)
	{

		if (tetro_movex[max_weight_idx] < 4)
		{
			PressAKey();
			MoveTetro_Left();
		}

		else if (tetro_movex[max_weight_idx]  >  4)
		{
			PressDKey();
			MoveTetro_Right();
		}
		else if (tetro_movex[max_weight_idx]  == 4)
		{
			break;
		}
	}
}


void Tetris::maxWeight() {

	int weightsum = 0;
	int weightsumtemp = 0;
	int rotation = 0;
	int x = 0;

	vector<vector<vector<int>>> _tetroPattern_temp;
	_tetroPattern_temp = _tetroPattern;

	for (int i = 19; i > 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int rotate = 0; rotate < 4; rotate++)
			{
				weightsum = 0;

				for (int tetroNum = 0; tetroNum < 4; tetroNum++)
				{
					int pos_X = _tetroPattern_temp[rotate][tetroNum][0] + j;
					int pos_Y = _tetroPattern_temp[rotate][tetroNum][1] + i;

					if (pos_Y < 0 || pos_Y >= 20)
					{
						break;
					}

					if (pos_X < 0 || pos_X >= 10)
					{
						break;
					}

					weightsum = weightsum + map[pos_Y][pos_X];
					x = pos_X;
				}

				if (weightsum > weightsumtemp)
				{
					weightsumtemp = weightsum;
					rotation = rotate;
				}

				tetro_move_weight.push_back(weightsumtemp);
				tetro_movex.push_back(x);
				tetro_rotation_weight.push_back(rotation);
			}
		}
	}
	max_weight_idx = int(max_element(tetro_move_weight.begin(), tetro_move_weight.end()) - tetro_move_weight.begin());
}


void Tetris::setMapWeight() { //map에 가중치 저장
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{

			if (_tetrisLogicBoard[i][j].compare("  ") == 0)
			{
				if (i == 19) {
					map[i][j] = i * 2;
				}
				else if (j == 0 || j == 9) {
					map[i][j] = i ;
				}
				else {
					map[i][j] = -20 + i;
				}
			}
			else {
				map[i][j] = -20;
			}
		}
	}
}

void Tetris::showKeySet()
{
	CursorPos(30, 10);
	cout << "press 'C' play Ai";
	CursorPos(30, 12);
	cout << "press 'P' play Player";
}


// ----------------------------- 클래스밖 -----------------------------
// 게임실행
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

// 테트로미노 시작 하강 속도 제한
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