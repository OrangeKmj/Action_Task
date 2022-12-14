#pragma once
#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <algorithm>
#pragma comment(lib, "winmm.lib")
using namespace std;

// ---------------------- 테트리스 경계선  보드 크기 ----------------------
enum BorderLineBoardSize
{
	WIDTH_BORDER = 12,
	HEIGHT_BORDER = 22
};



// -------------------- 테트리스 화면,  로직 보드 크기 --------------------
enum ScreenAndLogicBoardSize
{
	WIDTH = 10,
	HEIGHT = 20
};



// ------------------------------- 입력  키 -------------------------------
enum GetKey
{
	GETKEY_SPACE = 32,
	GETKEY_A = 65,
	GETKEY_D = 68,
	GETKEY_a = 97,
	GETKEY_d = 100,

	GETKEY_C = 67,
	GETKEY_c = 99,
	GETKEY_P = 80,
	GETKEY_p = 112
};



// ------------------------------ 테트로미노 ------------------------------
const vector<vector<vector<int>>> pattern_I = {
	{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
	{{0, -2}, {0, -1}, {0, 0}, {0, 1}},
	{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
	{{0, -2}, {0, -1}, {0, 0}, {0, 1}}
};

const vector<vector<vector<int>>> pattern_O = {
	{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
	{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
	{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
	{{0, 0}, {1, 0}, {0, 1}, {1, 1}}
};

const vector<vector<vector<int>>> pattern_T = {
	{{-1, 0}, {0, 0}, {1, 0}, {0, 1}},
	{{0, -1}, {-1, 0}, {0, 0}, {0, 1}},
	{{0, -1}, {-1, 0}, {0, 0}, {1, 0}},
	{{0, -1}, {0, 0}, {1, 0}, {0, 1}}
};

const vector<vector<vector<int>>> pattern_J = {
	{{-1, 0}, {0, 0}, {1, 0}, {1, 1}},
	{{0, -1}, {0, 0}, {-1, 1}, {0, 1}},
	{{-1, -1}, {-1, 0}, {0, 0}, {1, 0}},
	{{0, -1}, {1, -1}, {0, 0}, {0, 1}}
};

const vector<vector<vector<int>>> pattern_L = {
	{{-1, 0}, {0, 0}, {1, 0}, {-1, 1}},
	{{-1, -1}, {0, -1}, {0, 0}, {0, 1}},
	{{1, -1}, {-1, 0}, {0, 0}, {1, 0}},
	{{0, -1}, {0, 0}, {0, 1}, {1, 1}}
};

const vector<vector<vector<int>>> pattern_S = {
	{{0, 0}, {1, 0}, {-1, 1}, {0, 1}},
	{{-1, -1}, {-1, 0}, {0, 0}, {0, 1}},
	{{0, 0}, {1, 0}, {-1, 1}, {0, 1}},
	{{-1, -1}, {-1, 0}, {0, 0}, {0, 1}}
};

const vector<vector<vector<int>>> pattern_Z = {
	{{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
	{{0, -1}, {-1, 0}, {0, 0}, {-1, 1}},
	{{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
	{{0, -1}, {-1, 0}, {0, 0}, {-1, 1}}
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



	// ----------------------- 테트리스 보드 초기화 -----------------------
	void BoardInit();



	// ------------------------ 테트리스 보드 출력 ------------------------
	void DrawBorderLineBoard();

	void DrawScreenBoard();



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



	// ---------- 테트로미노를 테트리스 로직 및  화면 보드에 저장 ----------
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

	//------------------------------ 추가함수 -----------------------------
	void PressCKey();
	void PressPKey();

	void controlAi();	//Ai작동

	void moveAi();	//목표좌표로 이동
	void rotateAi();	//목표 로테이션으로 회전

	void setMapWeight();	//가중치 설정

	void maxWeight();	//최대 가중치

	void showKeySet();

private:
	// ---------------------------- 멤버  변수 ----------------------------
	int _tetroDescentSpeed; // 테트로미노 하강 속도

	int _borderLineBoardW; // 테트리스 경계선 보드 가로 길이
	int _borderLineBoardH; // 테트리스 경계선 보드 세로 길이

	vector<vector<string>> _tetrisBorderLineBoard; // 테트리스 경계선 보드

	int _boardW; // 테트리스 화면 및 로직 보드 가로 길이
	int _boardH; // 테트리스 화면 및 로직 보드 세로 길이

	vector<vector<string>> _tetrisScreenBoard; // 테트리스 화면 보드
	vector<vector<string>> _tetrisLogicBoard; // 테트리스 로직 보드

	// [만약 테트리스 경계선 보드 크기를 키워서 테트리스 화면 보드를 옮기고 싶으면 _screenBoardStartPos를 변경]
	int _screenBoardStartPosX; // 테트리스 화면 보드가 출력되는 시작 X 좌표 
	int _screenBoardStartPosY; // 테트리스 화면 보드가 출력되는 시작 Y 좌표

	bool _checkTetroPosChange; // 테트로미노 위치가 변경되었는지 확인

	vector<vector<vector<int>>> _tetroPattern; // 화면에 출력되는 테트로미노

	bool _pressSpaceBarKey; // 스페이스바를 눌렀는지 확인
	bool _pressAKey; // A를 눌렀는지 확인
	bool _pressDKey; // D를 눌렀는지 확인

	int _tetroPosX; // 생성된 테트로미노의 중심 칸의 X좌표
	int _tetroPosY; // 생성된 테트로미노의 중심 칸의 Y좌표

	int _tetroRotationNum; // 생성된 테트로미노의 회전 번호

	bool _checkWall; // 테트로미노가 이동한 위치에 벽이 있는지 확인

	float _timeElapsed; // 흐른 시간
	DWORD _lastTime; // 지난 시간

	bool _contactTetroOrFloor; // 테트로미노가 다른 테트로미노 또는 바닥에 닿았는지 확인

	float _timeElapsedSinceContact; // 접촉 후 흐른 시간
	DWORD _lastTimeSinceContact; // 접촉 후 지난 시간

	bool _preventInit; // 초기화 방지를 위한 변수

	bool _isGameOver; // 게임오버 확인


	//------------------------------- 추가변수 -------------------------------

	bool _pressCKey; //C를 눌렀는지 확인

	int rotstop;

	int max_weight_idx;

	vector<int> tetro_move_weight;
	vector<int> tetro_movex;
	vector<int> tetro_rotation_weight; //로테이션

	int map[HEIGHT][WIDTH];

};



// ------------------------------- 클래스밖 -------------------------------
void Run(int speed); // 게임실행

void CursorHide(); // 커서 숨기기

int TetroDescentSpeedLimit(int speed); // 테트로미노 시작 하강 속도 제한