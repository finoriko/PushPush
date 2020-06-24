// PushPushRawMemory.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
using namespace std;

char m[100]; //메모리 100바이트 밖에 사용안한다.
// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
enum Object {
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};
//함수프로토타입
void initializeGlobalVariables();
void initialize();
void draw();
void update();
void checkClear();

int main()
{
    std::cout << "Hello World!\n";
}
void draw() {
	for (m[0] = 0; m[0] < m[19]; ++m[0]) {
		for (m[1] = 0; m[1] < m[18]; ++m[1]) {
			m[2] = m[20 + m[0] * m[18] + m[1]];
			switch (m[2]) {
			case OBJ_SPACE: cout << ' '; break;
			case OBJ_WALL: cout << '#'; break;
			case OBJ_GOAL: cout << '.'; break;
			case OBJ_BLOCK: cout << 'o'; break;
			case OBJ_BLOCK_ON_GOAL: cout << 'O'; break;
			case OBJ_MAN: cout << 'p'; break;
			case OBJ_MAN_ON_GOAL: cout << 'P'; break;
			}
		}
		cout << endl;
	}
}
// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
