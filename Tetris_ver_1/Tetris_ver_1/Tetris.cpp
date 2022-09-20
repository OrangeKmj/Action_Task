#include "Tetris.h"

// -------------------------- ������  �Ҹ��� --------------------------
Tetris::Tetris(int speed)
{
	_tetroDescentSpeed = speed;

	_boardW = WIDTH;
	_boardH = HEIGHT;

	_tetrisBoard.clear();

	_checkTetroPosChange = true; // ��Ʈ�ι̳밡 ó�� ���忡 �����Ǵ� �͵� ��ġ ��ȭ�� �����Ͽ� true�� �ʱ�ȭ

	_tetroPattern.clear();

	_pressSpaceBarKey = false;
	_pressAKey = false;
	_pressDKey = false;

	_tetroPosX = 6; // ��Ʈ�ι̳밡 �����Ǵ� ��ġ�� X��ǥ
	_tetroPosY = 2; // ��Ʈ�ι̳밡 �����Ǵ� ��ġ�� Y��ǥ

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



// ------------------------------ �ʱ�ȭ ------------------------------
void Tetris::Init()
{
	// ���� �ʱ�ȭ
	BoardInit();

	// ���� ���
	DrawBoard();

	// ��Ʈ�ι̳� ����
	SelectTetro();

	// ��Ʈ�ι̳� ���
	DrawTetro();
}



// ------------------------------  ����  ------------------------------
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
	// _checkTetroPosChange ������ false�� �ʱ�ȭ
	CheckTetroPosChangeInit();

	// ��Ʈ�ι̳븦 �ð�������� ȸ��
	RotateTetro();

	// ��Ʈ�ι̳븦 �������� �̵�
	MoveTetro_Left();

	// ��Ʈ�ι̳븦 ���������� �̵�
	MoveTetro_Right();

	// ��Ʈ�ι̳� �ϰ�
	TetroDescending();
}

void Tetris::Render()
{
	// ���� ���
	DrawBoard();
	
	// ��Ʈ�ι̳� ���
	DrawTetro();

	// _isGameOver�� true�� ��� GameOver�� ���
	PrintGameOver();
}



// -------------------------- Ŀ�� ��ġ ���� --------------------------
void Tetris::CursorPos(int cursorPosX, int cursorPosY)
{
	COORD cursorPos = { static_cast<SHORT>(cursorPosX), static_cast<SHORT>(cursorPosY) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
}



// --------------------------- ����  �ʱ�ȭ ---------------------------
void Tetris::BoardInit()
{
	// ���� ũ�⸸ŭ �������� �ʱ�ȭ
	for (int height = 0; height < _boardH; height++)
	{
		vector<string> inputValue;

		for (int width = 0; width < _boardW; width++)
		{
			inputValue.push_back("  ");
		}
		_tetrisBoard.push_back(inputValue);
	}

	// ���� �ش��ϴ� �κ��� ������ �ʱ�ȭ
	for (int width = 0; width < _boardW; width++)
	{
		_tetrisBoard[0][width] = "��";
		_tetrisBoard[1][width] = "��";
		_tetrisBoard[_boardH - 2][width] = "��";
		_tetrisBoard[_boardH - 1][width] = "��";
	}

	for (int height = 1; height < _boardH - 1; height++)
	{
		_tetrisBoard[height][0] = "��";
		_tetrisBoard[height][1] = "��";
		_tetrisBoard[height][_boardW - 2] = "��";
		_tetrisBoard[height][_boardW - 1] = "��";
	}
}



// ---------------------------- ����  ��� ----------------------------
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



// ------------------------- ��Ʈ�ι̳�  ���� -------------------------
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



// ------------------------- ��Ʈ�ι̳�  ��� -------------------------
void Tetris::DrawTetro()
{
	// ��Ʈ�ι̳��� ��ġ�� ������� �� ���
	if (_checkTetroPosChange == true)
	{
		for (int tetroNum = 0; tetroNum < 4; tetroNum++)
		{
			int cursorPosX = _tetroPattern[_tetroRotationNum][tetroNum][0] + (_tetroPosX * 2); // ��, ��, �� ���ڰ� Ŀ�� 2ĭ�� ���������� * 2
			int cursorPosY = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY;
			CursorPos(cursorPosX, cursorPosY);
			cout << "��";
		}
	}
}



// --------------------------- Ű �Է� Ȯ�� ---------------------------
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



// ------------ _checkTetroPosChange ������ false�� �ʱ�ȭ ------------
void Tetris::CheckTetroPosChangeInit()
{
	_checkTetroPosChange = false;
}



// ------------ ����Ű�� ��Ʈ�ι̳븦  ȸ�� �� �¿�� �̵� ------------
void Tetris::RotateTetro()
{
	if (_pressSpaceBarKey == true)
	{
		_pressSpaceBarKey = false;

		int temp = _tetroRotationNum; // ȸ���� �� ���� ��Ȳ�� �����Ͽ� ȸ���ϱ� �� ȸ�� ��ȣ�� temp�� ����
		_tetroRotationNum = (_tetroRotationNum + 1) % 4;
		CheckSpaceWhenRotating();

		// ȸ���� �� ���� ���
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

		// �������� �̵��� �� ���� ���
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

		// ���������� �̵��� �� ���� ���
		if (_checkWall == true)
		{
			_tetroPosX--;
			_checkWall = false;
		}
	}
}



// - ��Ʈ�ι̳밡 �̵��� ������ ����� ��Ʈ�ι̳� �� ���� �ִ��� Ȯ�� -
void Tetris::CheckSpaceWhenRotating()
{
	int rotationPosNum = 0;

	while (rotationPosNum != 25)
	{
		for (int tetroNum = 0; tetroNum < 4; tetroNum++)
		{
			int pos_X = (_tetroPattern[_tetroRotationNum][tetroNum][0] / 2) + _tetroPosX + tetroRotationPos[rotationPosNum][0];
			int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY + tetroRotationPos[rotationPosNum][1];

			// ��Ʈ�ι̳밡 ���� �Ѿ�� ���
			if (pos_X < 2 || pos_X > 11 || pos_Y < 2 || pos_Y > 21)
			{
				_checkWall = true;
				rotationPosNum++;
				break;
			}

			// ��Ʈ�ι̳밡 �ٸ� ��Ʈ�ι̳�� ������ ���
			if (_tetrisBoard[pos_Y][pos_X].compare("��") == 0)
			{
				_checkWall = true;
				rotationPosNum++;
				break;
			}

			_checkWall = false;
		}

		// �� �Ǵ� �ٸ� ��Ʈ�ι̳�� ��ġ�� �ʾ��� ���
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

		// ��Ʈ�ι̳밡 �� �Ǵ� �ٸ� ��Ʈ�ι̳�� ������ ���
		if (_tetrisBoard[pos_Y][pos_X].compare("��") == 0 || _tetrisBoard[pos_Y][pos_X].compare("��") == 0)
		{
			_checkWall = true;
			break;
		}
	}

	// ��Ʈ�ι̳밡 �� �Ǵ� �ٸ� ��Ʈ�ι̳�� ��ġ�� �ʾ��� ���
	if (_checkWall == false)
	{
		_checkTetroPosChange = true;
	}
}



// ------------------------- ��Ʈ�ι̳�  �ϰ� -------------------------
void Tetris::TetroDescending()
{
	DWORD curTime = timeGetTime(); // ���� �ð�
	float timeDelta = (curTime - _lastTime) * 0.001f; // �ð��� 1�� ������ ��ȯ

	_timeElapsed += timeDelta;
	_lastTime = curTime;

	// 1.0f / _tetroDescentSpeed��ŭ �ð��� �帣�� AutoMoveTetro_Down()�� ����
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

	// �Ʒ��� �̵��� �� ���� ���
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

		// ��Ʈ�ι̳밡 �� �Ǵ� �ٸ� ��Ʈ�ι̳�� ������ ���
		if (_tetrisBoard[pos_Y][pos_X].compare("��") == 0 || _tetrisBoard[pos_Y][pos_X].compare("��") == 0)
		{
			_checkWall = true;
			_contactTetroOrFloor = true; // ��Ʈ�ι̳� �Ǵ� �ٴڰ� ������ true
			break;
		}
	}

	// ��Ʈ�ι̳밡 �� �Ǵ� �ٸ� ��Ʈ�ι̳�� ��ġġ �ʾ��� ���
	if (_checkWall == false)
	{
		_contactTetroOrFloor = false; // ��Ʈ�ι̳� �Ǵ� �ٴڰ� ������ false
		_checkTetroPosChange = true;
	}
}



// --- ��Ʈ�ι̳밡 �ٸ� ��Ʈ�ι̳� �Ǵ�  �ٴڿ� ������ �ð��� Ȯ�� ---
void Tetris::CheckContactTime()
{
	if (_contactTetroOrFloor == true)
	{
		// �ش� �Լ��� ���� ������ �ʱ�ȭ�Ǵ� ���� ����
		if (_preventInit == false)
		{
			_preventInit = true;
			_timeElapsedSinceContact = 0.0f;
			_lastTimeSinceContact = timeGetTime();
		}

		DWORD curTimeSinceContact = timeGetTime(); // ���� �� ���� �ð�
		float timeDeltaSinceContact = (curTimeSinceContact - _lastTimeSinceContact) * 0.001f; // �������� 1�� ������ ��ȯ

		_timeElapsedSinceContact += timeDeltaSinceContact;

		// 1.0f / 4 ��ŭ �ð��� �帣�� TetroStorage()�� ����
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



// --------------------- ��Ʈ�ι̳븦 ���忡 ���� ---------------------
void Tetris::TetroStorage()
{
	for (int tetroNum = 0; tetroNum < 4; tetroNum++)
	{
		int pos_X = (_tetroPattern[_tetroRotationNum][tetroNum][0] / 2) + _tetroPosX;
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY;
		_tetrisBoard[pos_Y][pos_X] = "��";
	}
	_checkTetroPosChange = true;

	TetroPosInit();
	CheckLine();
	CheckGameOver();
}





// ---------------------- ��Ʈ�ι̳� ��ġ �ʱ�ȭ ----------------------
void Tetris::TetroPosInit()
{
	_tetroPattern.clear();
	_tetroPosX = 6;
	_tetroPosY = 2;
	_tetroRotationNum = 0;

	SelectTetro();
}



// ------- ��ƴ���� ä���� �������� �ִ��� Ȯ���ϰ� ������ ���� -------
void Tetris::CheckLine()
{
	for (int height = 2; height < _boardH - 2; height++)
	{
		int checkTetroCount = 0;

		for (int width = 2; width < _boardW - 2; width++)
		{
			if (_tetrisBoard[height][width].compare("��") == 0)
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
	// lineNum ��° ���� �������� ����
	for (int width = 2; width < _boardW - 2; width++)
	{
		_tetrisBoard[lineNum][width] = "  ";
	}

	// lineNum ��° ���� lineNum - 1 ��° ���ϰ� �ٲ�
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



// -------------------------- ���ӿ���  Ȯ�� --------------------------
void Tetris::CheckGameOver()
{
	for (int tetroNum = 0; tetroNum < 4; tetroNum++)
	{
		int pos_X = (_tetroPattern[_tetroRotationNum][tetroNum][0] / 2) + _tetroPosX;
		int pos_Y = _tetroPattern[_tetroRotationNum][tetroNum][1] + _tetroPosY;

		// ��Ʈ�ι̳밡 �������ڸ��� �ٸ� ��Ʈ�ι̳�� �������� ���
		if (_tetrisBoard[pos_Y][pos_X].compare("��") == 0)
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



// -------------------------- ���ӿ���  ��� --------------------------
void Tetris::PrintGameOver()
{
	if (_isGameOver == true)
	{
		CursorPos(0, 25);
		cout << "GameOver~~~~~" << endl;
	}
}



// ----------------------------- Ŭ������ -----------------------------
// ���� ����
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

// Ŀ�� �����
void CursorHide()
{
	CONSOLE_CURSOR_INFO consoleCursor;
	consoleCursor.bVisible = false;
	consoleCursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleCursor);
}

// ��Ʈ�ι̳� �ϰ� �ӵ� ����
int TetroDescentSpeedLimit(int speed)
{
	// �ϰ� �ӵ��� 1���� ���� ���
	if (speed < 1)
	{
		speed = 1;
	}

	// �ϰ� �ӵ��� 10���� Ŭ ���
	if (speed > 10)
	{
		speed = 10;
	}

	return speed;
}