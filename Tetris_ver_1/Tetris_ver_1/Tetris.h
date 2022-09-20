#pragma once
#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

// ------------------------------ ����  ũ�� ------------------------------
enum BoardSize
{
	WIDTH = 14,
	HEIGHT = 24
};



// ---------------------------------- Ű ----------------------------------
enum GetKey
{
	GETKEY_SPACE = 32,
	GETKEY_A = 65,
	GETKEY_D = 68,
	GETKEY_a = 97,
	GETKEY_d = 100
};



// ------------------------------ ��Ʈ�ι̳� ------------------------------
const vector<vector<vector<int>>> pattern_I = {
	{{-2, 0}, {0, 0}, {2, 0}, {4, 0}},
	{{0, -2}, {0, -1}, {0, 0}, {0, 1}},
	{{-2, 0}, {0, 0}, {2, 0}, {4, 0}},
	{{0, -2}, {0, -1}, {0, 0}, {0, 1}}
};

const vector<vector<vector<int>>> pattern_O = {
	{{0, 0}, {2, 0}, {0, 1}, {2, 1}},
	{{0, 0}, {2, 0}, {0, 1}, {2, 1}},
	{{0, 0}, {2, 0}, {0, 1}, {2, 1}},
	{{0, 0}, {2, 0}, {0, 1}, {2, 1}}
};

const vector<vector<vector<int>>> pattern_T = {
	{{-2, 0}, {0, 0}, {2, 0}, {0, 1}},
	{{0, -1}, {-2, 0}, {0, 0}, {0, 1}},
	{{0, -1}, {-2, 0}, {0, 0}, {2, 0}},
	{{0, -1}, {0, 0}, {2, 0}, {0, 1}}
};

const vector<vector<vector<int>>> pattern_J = {
	{{-2, 0}, {0, 0}, {2, 0}, {2, 1}},
	{{0, -1}, {0, 0}, {-2, 1}, {0, 1}},
	{{-2, -1}, {-2, 0}, {0, 0}, {2, 0}},
	{{0, -1}, {2, -1}, {0, 0}, {0, 1}}
};

const vector<vector<vector<int>>> pattern_L = {
	{{-2, 0}, {0, 0}, {2, 0}, {-2, 1}},
	{{-2, -1}, {0, -1}, {0, 0}, {0, 1}},
	{{2, -1}, {-2, 0}, {0, 0}, {2, 0}},
	{{0, -1}, {0, 0}, {0, 1}, {2, 1}}
};

const vector<vector<vector<int>>> pattern_S = {
	{{0, 0}, {2, 0}, {-2, 1}, {0, 1}},
	{{-2, -1}, {-2, 0}, {0, 0}, {0, 1}},
	{{0, 0}, {2, 0}, {-2, 1}, {0, 1}},
	{{-2, -1}, {-2, 0}, {0, 0}, {0, 1}}
};

const vector<vector<vector<int>>> pattern_Z = {
	{{-2, 0}, {0, 0}, {0, 1}, {2, 1}},
	{{0, -1}, {-2, 0}, {0, 0}, {-2, 1}},
	{{-2, 0}, {0, 0}, {0, 1}, {2, 1}},
	{{0, -1}, {-2, 0}, {0, 0}, {-2, 1}}
};



// ------------------------- ��Ʈ�ι̳� ȸ�� ��ġ -------------------------
const vector<vector<int>> tetroRotationPos = {
	{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1},
	{1, 2}, {-1, 0}, {-1, 1}, {-1, 2}, {2, 0},
	{-2, 0}, {2, 1}, {-2, 1}, {2, 2}, {-1, 2},
	{0, -1}, {1, -1}, {-1, -1}, {2, -1}, {-2, -1},
	{0, -2}, {1, -2}, {-1, -2}, {2, -2}, {-2, -2}
};



class Tetris
{
public:
	// -------------------------- ������  �Ҹ��� --------------------------
	Tetris(int speed);
	~Tetris();



	// ------------------------------ �ʱ�ȭ ------------------------------
	void Init();



	// ------------------------------  ����  ------------------------------
	void ProcessInput();
	void Update();
	void Render();



	// -------------------------- Ŀ�� ��ġ ���� --------------------------
	void CursorPos(int cursorPosX, int cursorPosY);



	// --------------------------- ����  �ʱ�ȭ ---------------------------
	void BoardInit();



	// ---------------------------- ����  ��� ----------------------------
	void DrawBoard();



	// ------------------------- ��Ʈ�ι̳�  ���� -------------------------
	void SelectTetro();



	// ------------------------- ��Ʈ�ι̳�  ��� -------------------------
	void DrawTetro();



	// --------------------------- Ű �Է� Ȯ�� ---------------------------
	void PressSpaceBarKey();

	void PressAKey();

	void PressDKey();



	// ------------ _checkTetroPosChange ������ false�� �ʱ�ȭ ------------
	void CheckTetroPosChangeInit();



	// ------------ ����Ű�� ��Ʈ�ι̳븦  ȸ�� �� �¿�� �̵� ------------
	void RotateTetro();
	
	void MoveTetro_Left();

	void MoveTetro_Right();

	

	// - ��Ʈ�ι̳밡 �̵��� ������ ����� ��Ʈ�ι̳� �� ���� �ִ��� Ȯ�� -
	void CheckSpaceWhenRotating();
	
	void CheckSpaceWhenMove();



	// ------------------------- ��Ʈ�ι̳�  �ϰ� -------------------------
	void TetroDescending();

	void AutoMoveTetro_Down();

	void CheckSpaceWhenDescent();



	// --- ��Ʈ�ι̳밡 �ٸ� ��Ʈ�ι̳� �Ǵ�  �ٴڿ� ������ �ð��� Ȯ�� ---
	void CheckContactTime();



	// --------------------- ��Ʈ�ι̳븦 ���忡 ���� ---------------------
	void TetroStorage();

	

	// ---------------------- ��Ʈ�ι̳� ��ġ �ʱ�ȭ ----------------------
	void TetroPosInit();



	// ------- ��ƴ���� ä���� �������� �ִ��� Ȯ���ϰ� ������ ���� -------
	void CheckLine();

	void LineSwap(int lineNum);


	
	// -------------------------- ���ӿ���  Ȯ�� --------------------------
	void CheckGameOver();

	bool IsGameOver();



	// -------------------------- ���ӿ���  ��� --------------------------
	void PrintGameOver();

private:
	// ---------------------------- ���  ���� ----------------------------
	int _tetroDescentSpeed; // ��Ʈ�ι̳� �ϰ� �ӵ�

	int _boardW; // ���� ���� ����
	int _boardH; // ���� ���� ����

	vector<vector<string>> _tetrisBoard; // ��Ʈ���� ����

	bool _checkTetroPosChange; // ��Ʈ�ι̳� ��ġ�� ����Ǿ����� Ȯ��

	vector<vector<vector<int>>> _tetroPattern; // ���忡 ��µǴ� ��Ʈ�ι̳�

	bool _pressSpaceBarKey; // �����̽��ٸ� �������� Ȯ��
	bool _pressAKey; // A�� �������� Ȯ��
	bool _pressDKey; // D�� �������� Ȯ��

	int _tetroPosX; // ������ ��Ʈ�ι̳��� �߽��� X��ǥ
	int _tetroPosY; // ������ ��Ʈ�ι̳��� �߽��� Y��ǥ

	int _tetroRotationNum; // ������ ��Ʈ�ι̳��� ȸ�� ��ȣ

	bool _checkWall; // ��Ʈ�ι̳밡 �̵��� ��ġ�� ���� �ִ��� Ȯ��

	float _timeElapsed; // �帥 �ð�
	DWORD _lastTime; // ���� �ð�

	bool _contactTetroOrFloor; // ��Ʈ�ι̳밡 �ٸ� ��Ʈ�ι̳� �Ǵ� �ٴڿ� ��Ҵ��� Ȯ��

	float _timeElapsedSinceContact; // ���� �� �帥 �ð�
	DWORD _lastTimeSinceContact; // ���� �� ���� �ð�

	bool _preventInit; // �ʱ�ȭ�� �����ϱ� ���� ����

	bool _isGameOver; // ���ӿ��� Ȯ��
};



// ------------------------------- Ŭ������ -------------------------------
void Run(int speed); // ���ӽ���

void CursorHide(); // Ŀ�� �����

int TetroDescentSpeedLimit(int speed); // ��Ʈ�ι̳� �ϰ� �ӵ� ����