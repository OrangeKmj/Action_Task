#include <iostream>
#include "Tetris.h"
using namespace std;

int main()
{
	srand((unsigned int)time(nullptr));
	
	int speed = 0;
	cin >> speed;

	Run(speed);

	return 0;
}