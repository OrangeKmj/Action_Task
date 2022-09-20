#pragma once
#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

// ------------------------------ 보드  크기 ------------------------------
enum BoardSize
{
	WIDTH = 14,
	HEIGHT = 24
};



// ---------------------------------- 키 ----------------------------------
enum GetKey
{
	GETKEY_SPACE = 32,
	GETKEY_A = 65,
	GETKEY_D = 68,
	GETKEY_a = 97,
	GETKEY_d = 100
};



// ------------------------------ 테트로미노 ------------------------------
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



// ------------------------- 테트로미노 회전 위치 -------------------------
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
	// -------------------------- 생성자  소멸자 --------------------------
	Tetris(int speed);
	~Tetris();



	// ------------------------------ 초기화 ------------------------------
	void Init();



	// ------------------------------  루프  ------------------------------
	void ProcessInput();
	void Update();
	void Render();



	// -------------------------- 커서 위치 변경 --------------------------
	void CursorPos(int cursorPosX, int cursorPosY);



	// --------------------------- 보드  초기화 ---------------------------
	void BoardInit();



	// ---------------------------- 보드  출력 ----------------------------
	void DrawBoard();



	// ------------------------- 테트로미노  선택 -------------------------
	void SelectTetro();



	// ------------------------- 테트로미노  출력 -------------------------
	void DrawTetro();



	// --------------------------- 키 입력 확인 ---------------------------
	void PressSpaceBarKey();

	void PressAKey();

	void PressDKey();



	// ------------ _checkTetroPosChange 변수를 false로 초기화 ------------
	void CheckTetroPosChangeInit();



	// ------------ 조작키로 테트로미노를  회전 및 좌우로 이동 ------------
	void RotateTetro();
	
	void MoveTetro_Left();

	void MoveTetro_Right();

	

	// - 테트로미노가 이동할 공간에 저장된 테트로미노 및 벽이 있는지 확인 -
	void CheckSpaceWhenRotating();
	
	void CheckSpaceWhenMove();



	// ------------------------- 테트로미노  하강 -------------------------
	void TetroDescending();

	void AutoMoveTetro_Down();

	void CheckSpaceWhenDescent();



	// --- 테트로미노가 다른 테트로미노 또는  바닥에 접촉한 시간을 확인 ---
	void CheckContactTime();



	// --------------------- 테트로미노를 보드에 저장 ---------------------
	void TetroStorage();

	

	// ---------------------- 테트로미노 위치 초기화 ----------------------
	void TetroPosInit();



	// ------- 빈틈없이 채워진 가로줄이 있는지 확인하고 있으면 제거 -------
	void CheckLine();

	void LineSwap(int lineNum);


	
	// -------------------------- 게임오버  확인 --------------------------
	void CheckGameOver();

	bool IsGameOver();



	// -------------------------- 게임오버  출력 --------------------------
	void PrintGameOver();

private:
	// ---------------------------- 멤버  변수 ----------------------------
	int _tetroDescentSpeed; // 테트로미노 하강 속도

	int _boardW; // 보드 가로 길이
	int _boardH; // 보드 세로 길이

	vector<vector<string>> _tetrisBoard; // 테트리스 보드

	bool _checkTetroPosChange; // 테트로미노 위치가 변경되었는지 확인

	vector<vector<vector<int>>> _tetroPattern; // 보드에 출력되는 테트로미노

	bool _pressSpaceBarKey; // 스페이스바를 눌렀는지 확인
	bool _pressAKey; // A를 눌렀는지 확인
	bool _pressDKey; // D를 눌렀는지 확인

	int _tetroPosX; // 생성된 테트로미노의 중심의 X좌표
	int _tetroPosY; // 생성된 테트로미노의 중심의 Y좌표

	int _tetroRotationNum; // 생성된 테트로미노의 회전 번호

	bool _checkWall; // 테트로미노가 이동한 위치에 벽이 있는지 확인

	float _timeElapsed; // 흐른 시간
	DWORD _lastTime; // 지난 시간

	bool _contactTetroOrFloor; // 테트로미노가 다른 테트로미노 또는 바닥에 닿았는지 확인

	float _timeElapsedSinceContact; // 접촉 후 흐른 시간
	DWORD _lastTimeSinceContact; // 접촉 후 지난 시간

	bool _preventInit; // 초기화를 방지하기 위한 변수

	bool _isGameOver; // 게임오버 확인
};



// ------------------------------- 클래스밖 -------------------------------
void Run(int speed); // 게임실행

void CursorHide(); // 커서 숨기기

int TetroDescentSpeedLimit(int speed); // 테트로미노 하강 속도 제한