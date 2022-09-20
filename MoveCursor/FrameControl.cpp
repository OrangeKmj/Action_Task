#include "FrameControl.h"

// 커서가 한 번 움직이는데 걸리는 시간 제어
void SpeedControl(int cursorSpeed)
{
	float timeElapsed = 0.0f; // 흐른 시간
	DWORD lastTime = timeGetTime();	// 마지막 시간

	while (true)
	{
		DWORD curTime = timeGetTime(); // 현재 시간
		float timeDelta	= (curTime - lastTime) * 0.001f; // timeDelta(1번 생성 후 흐른 시간) 1초 단위로 바꿈

		timeElapsed += timeDelta;

		// 1.0f / _cursorSpeed 만큼 시간이 흐르면 frameControl을 빠져나감
		if (timeElapsed >= 1.0f / cursorSpeed)
		{
			break;
		}
		lastTime = curTime;
	}
}