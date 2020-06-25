// PushPushRawMemory.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
using namespace std;

/*
이 샘플에서는 아래의 m 이외에 변수를 일절 사용하지 않아.
컴파일러보다 아래 세계에서 무슨 일이 일어나고 있는지 깊이 깨달았으면 한다.
변수를 자유롭게 만들 수 있는 것이 얼마나 고마운지 실감하고
또, 그것이 어떻게 가능하게 되어 있는지를 느껴보자.

[메모리 사용법]

0 : 함수의 인수와 반환값에 사용한다.
1-17: 함수 중에서 원하는 대로 사용해도 되는 영역으로 한다.함수를 부르면 깨진다고 생각해.
18:8/폭
19:5 / 높이
20-59: 8x5 상태 배열
60-99: 문자열 형식의 스테이지 데이터

*/

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
	//글로벌 변수 초기화
	initializeGlobalVariables();

	initialize(); //스테이지 초기화
	//메인루프
	while (true) {
		//먼저 그린다
		draw();
		//클리어체크 결과는 m[0]에 들어 있다
		checkClear();
		if (m[0] == 1) {
			break; //클리어체크
		}
		//입력받기
		cout << "a:left s:right w:up z:down. command?" << endl; //조작설명
		//입력은 m[0]에 넣자
		cin >> m[0];
		//갱신
		update();
	}
	//축하메시지
	cout << "Congratulation! you win." << endl;

	//Visual studio 에서 실행할 사람을 위해 무한루프.명령어 라인에서는 Ctrl - C로 끝내주세요.
	while (true) {
		;
	}
	return 0;
}

//이하 함수 정리

//m[60]부터 시작되는 문자열을 해석하여 m[20]부터 시작되는 상태 배열을 구축하는 함수.
void initialize() {
	m[0] = 0; //0번을 읽는 쪽에 첨자를 넣는다.
	m[1] = 0; //1번이 지금이 x좌표
	m[2] = 0; //2번이 지금의 y좌표

	while (m[60 + m[0]] != '\0') { //NULL이 아닌 동안
		//3순서대로 enum을 넣는다
		switch (m[60 + m[0]]) {
		case '#': m[3] = OBJ_WALL; break;
		case ' ': m[3] = OBJ_SPACE; break;
		case 'o': m[3] = OBJ_BLOCK; break;
		case 'O': m[3] = OBJ_BLOCK_ON_GOAL; break;
		case '.': m[3] = OBJ_GOAL; break;
		case 'p': m[3] = OBJ_MAN; break;
		case 'P': m[3] = OBJ_MAN_ON_GOAL; break;
		case '\n': m[1] = 0; ++m[2]; m[3] = OBJ_UNKNOWN; break; //개행처리(++y)
		default: m[3] = OBJ_UNKNOWN; break;
		}
		++m[0];
		if (m[3] != OBJ_UNKNOWN) { //모르는 문자라면 무시하기 때문에 이 if문아 았더
			m[20 + m[2] * m[18] + m[1]] = m[3]; //기입 m[18]은 폭
			++m[1]; //++x
		}
	}
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

void update() {
	// 이동차분으로 변환
	m[1] = 0; //dx
	m[2] = 0; //dy
	switch (m[0]) { //input은m[100]이 들어가 있다
	case 'a': m[1] = -1; break; //좌
	case 's': m[1] = 1; break; //우
	case 'w': m[2] = -1; break; //상。Y은아래가 플로스
	case 'z': m[2] = 1; break; //하。
	}
	//사람좌표 검색
	for (m[0] = 0; m[0] < m[18] * m[19]; ++m[0]) {
		if (m[20 + m[0]] == OBJ_MAN || m[20 + m[0]] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	m[3] = m[0] % m[18]; //x는 폭으로 나눈 나머지
	m[4] = m[0] / m[18]; //y는 폭으로 나눈 몫

	//이동
	//이동후좌표
	m[5] = m[3] + m[1]; //tx
	m[6] = m[4] + m[2]; //ty
	//좌표의 최대 최소 체크. 분리되어 있으면 불허
	if (m[5] < 0 || m[6] < 0 || m[5] >= m[18] || m[6] >= m[19]) {
		return;
	}
	//A.그 방향이 공백 또는 골.사람이 이동.
	m[7] = 20 + m[4] * m[18] + m[3]; //사람 위치
	m[8] = 20 + m[6] * m[18] + m[5]; //이동 목표 위치
	if (m[m[8]] == OBJ_SPACE || m[m[8]] == OBJ_GOAL) { //이동할 곳에 공간이 있으면
		m[m[8]] = (m[m[8]] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //골이라면 골인 윗사람에게
		m[m[7]] = (m[m[7]] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //원래 골 위라면 골로
	//B.그 방향이 박스. 그 방향의 다음 칸이 공백 또는 골이면 이동.
	}
	else if (m[m[8]] == OBJ_BLOCK || m[m[8]] == OBJ_BLOCK_ON_GOAL) {
		//2매스가 범위 내인지 체크
		m[9] = m[5] + m[1];
		m[10] = m[6] + m[2];
		if (m[9] < 0 || m[10] < 0 || m[9] >= m[18] || m[10] >= m[19]) { //押せない
			return;
		}

		m[11] = 20 + (m[6] + m[2]) * m[18] + (m[5] + m[1]); //2매스 먼저
		if (m[m[11]] == OBJ_SPACE || m[m[11]] == OBJ_GOAL) {
			//순차적 교체
			m[m[11]] = (m[m[11]] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			m[m[8]] = (m[m[8]] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			m[m[7]] = (m[m[7]] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

//블록만 없다면 클리어 할 수 있어.
void checkClear() {
	for (m[1] = 20; m[1] < 20 + m[18] * m[19]; ++m[1]) {
		if (m[m[1]] == OBJ_BLOCK) {
			m[0] = 0; //반환값은 m[ 0 ]
			return;
		}
	}
	m[0] = 1; //반환값은m[ 0 ]
	return;
}

//#벽_공간.골o블록 p인
//########\n\
//# .. p #\n\
//# oo   #\n\
//#      #\n\
//########";
void initializeGlobalVariables() {
	//폭이18
	m[18] = 8;
	//높이19
	m[19] = 5;
	//2번째줄
	m[68] = '#';
	m[69] = ' ';
	m[70] = '.';
	m[71] = '.';
	m[72] = ' ';
	m[73] = 'p';
	m[74] = ' ';
	m[75] = '#';
	//3번째줄
	m[76] = '#';
	m[77] = ' ';
	m[78] = 'o';
	m[79] = 'o';
	m[80] = ' ';
	m[81] = ' ';
	m[82] = ' ';
	m[83] = '#';
	//4번째줄
	m[84] = '#';
	m[85] = ' ';
	m[86] = ' ';
	m[87] = ' ';
	m[88] = ' ';
	m[89] = ' ';
	m[90] = ' ';
	m[91] = '#';
	//첫줄과 마지막줄은 전부#
	for (m[0] = 0; m[0] < m[18]; ++m[0]) {
		m[60 + m[0]] = '#';
		m[92 + m[0]] = '#';
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
