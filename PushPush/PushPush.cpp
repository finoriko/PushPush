// PushPush.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

using namespace std;
//#벽_공간.골o블록 p사람

const char gStageData[] =  "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;
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
// 함수 프로토타입
void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);
int main()
{
	//1차원 배열인 이유
	Object* state = new Object[gStageWidth * gStageHeight]; // 상태배열 확보

	initialize(state, gStageWidth, gStageHeight, gStageData); //스테이지 초기화
	//메인 루프
	while (true) {
		//일단 스테이지 그림 그리기
		
		draw(state, gStageWidth, gStageHeight);
		//클리어 체크
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break; //클리어체크
		}
		//입력 받기
		cout << "a:left s:right w:up z:down. command?" << endl; //操作説明
		char input;
		cin >> input;
		//갱신
		update(state, input, gStageWidth, gStageHeight);
	}
	//축하메시지
	cout << "Congratulation's! you won." << endl;
	//마무리
	delete[] state;
	state = 0;
	//Visual studio 에서 실행할 사람을 위해 무한루프.명령어 라인에서는 Ctrl - C로 끝내주세요.
	while (true) {
		;
	}
	return 0;
}

//이하 함수정의

//언젠가 사용할수도 있다고 생각해서 높이도 주게끔 했는데 사용하지 않아서 

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.

void initialize(Object* state, int width, int h, const char* stageData)
{
	const char* d = stageData; //읽기 포인터
	int x = 0;
	int y = 0;
	while (*d != '\0') { //NULL문자가 아닐때 까지
		Object t; // 특별히 뜻은 없지만 사용하는 횟수가 많은 변수에 나는 자주 t를 사용해.temporary의 약자.아마 좋지 않은 습관이지만, 쓸데없이 긴 이름으로 해서 읽기 어려운 것도 곤란할 것이다.
		switch (*d) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //개행처리
		default: t = OBJ_UNKNOWN; break;
		}
		++d;
		if (t != OBJ_UNKNOWN) { //모르는 문자라면 무시하기 때문에 이 if글이 있다
			state[y * width + x] = t; //기입
			++x;
		}
	}
}
void draw(const Object* state, int width, int height) {
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' }; //Object열거의 순서
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Object o = state[y * width + x];
			cout << font[o];
		}
		cout << endl;
	}
}
//첫 번째 인수는 다른 함수에서는 state라고 하지만 너무 자주 사용하기 때문에
//짧은 s로 끝냈다.w,h도 각각 width,height이다.
void update(Object* s, char input, int w, int h) {
	//이동차분으로 변환 (d는 difference건 delta건 좋아하는 분의 약자라고 생각함)
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break; //좌
	case 's': dx = 1; break; //우
	case 'w': dy = -1; break; //위。Y는 아래가 플러스다
	case 'z': dy = 1; break; //아래。
	}
	//사람좌표 검색
	int i = -1;
	for (i = 0; i < w * h; ++i) {
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	int x = i % w; //x는 폭으로 나눈 나머지
	int y = i / w; //y는 폭으로 나눈 몫

	// 이동
	// 이동후 좌표(t)에 의미가 없다.미안해요)
	int tx = x + dx;
	int ty = y + dy;
	//좌표의 최대 최소 체크. 분리되어 있으면 불허
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//A. 그 방향이 공백 또는 골인.사람이 이동.
	int p = y * w + x; //사람 위치
	int tp = ty * w + tx; //표적 위치(TargetPosition)
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL) {
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //골이라면 골인 윗사람에게
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //원래 골 위라면 골로
	//B. 그 방향이 상자. 그 방향의 다음 칸이 공백 또는 골이면 이동.
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL) {
		//2칸이 범위 내인지 체크
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //누를 수 없다
			return;
		}

		int tp2 = (ty + dy) * w + (tx + dx); //두칸 앞
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL) {
			//차례로 교체
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}
//블록만 없다면 클리어 함
bool checkClear(const Object* s, int width, int height) {
	for (int i = 0; i < width * height; ++i) {
		if (s[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;
}
