#include "CursorInfo.h"

string InputData();	// 데이터 입력
vector<int>	ConvertDataType(); // 입력받은 데이터의 타입을 int로 변경



// 데이터 입력
string InputData()
{
	string input;
	getline(cin, input);

	return input;
}



// 입력받은 데이터의 타입을 int로 변경
vector<int> ConvertDataType()
{
	stringstream stream(InputData());
	vector<int>	data; // 입력한 데이터를 담는 벡터
	int	convertType_Int = 0;

	while (stream >> convertType_Int)
	{
		data.push_back(convertType_Int);

		// 벡터 사이즈가 3이면 그 이상으로 데이터를 받지 못하게 break
		if (data.size() == 3)
		{
			break;
		}
	}

	return data;
}



// 입력받은 데이터에 오류가 있으면 수정하여 커서 이동 설정을 완료
vector<int> SetCursorData()
{
	vector<int>	cursorData = ConvertDataType(); // 오류를 수정한 커서 데이터를 담을 벡터

	// 값이 1보다 작으면 1로 변경
	for (int vectorNum = 0; vectorNum < cursorData.size(); vectorNum++)
	{
		if (cursorData[vectorNum] < 1)
		{
			cursorData[vectorNum] = 1;
		}
	}

	// 데이터를 담는 벡터의 사이즈가 3보다 작으면 사이즈가 3이 될 때까지 1를 삽입
	if (cursorData.size() < 3)
	{
		for (int vectorSize = cursorData.size(); vectorSize < 3; vectorSize++)
		{
			cursorData.push_back(1);
		}
	}

	// 속도를 나타내는 cursorData[2]의 값이 5보다 크면 5로 변경 (최대 이동 속도 = 5)
	if (cursorData[2] > 5)
	{
		cursorData[2] = 5;
	}

	return cursorData;
}