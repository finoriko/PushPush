// PushPush2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <fstream>
#include <algorithm> //max 함수를 위한
using namespace std;

//함수 프로토 타입
void readFile(char** buffer, int* size, const char* filename);

//2차원 배열 클래스
//템플릿이 어렵다면 기초만이라도 공부해두자
//이 클래스 선언중애 T라는 클래스가 있는것 처럼
//이것을 사용할 때는 T에 int라던가 bool을 넣던가 넣어서 사용하면 된다(제네릭)
template< class T > class Array2D {
public:
	Array2D() : mArray(0) {}
	~Array2D() {
		delete[] mArray;
		mArray = 0;  //포인터에 0을 넣는 것을 버릇으로 하자
	}
	void setSize(int size0, int size1) {
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[size0 * size1];
	}
	T& operator()(int index0, int index1) {
		return mArray[index1 * mSize0 + index0];
	}
	const T& operator()(int index0, int index1) const {
		return mArray[index1 * mSize0 + index0];
	}
private:
	T* mArray;
	int mSize0;
	int mSize1;
};
//상태 클래스
class State {
public:
	State(const char* stageData, int size);
	void update(char input);
	void draw() const;
	bool hasCleared() const;
private:
	enum Object {
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	void setSize(const char* stageData, int size);

	int mWidth;
	int mHeight;
	Array2D< Object > mObjects;
	Array2D< bool > mGoalFlags;
};
int main(int argc, char** argv) 
{
	const char* filename = "stageData.txt";
	if (argc >= 2) {
		filename = argv[1];
	}
	char* stageData;
	int fileSize;
	readFile(&stageData, &fileSize, filename);
	if (!stageData) {
		cout << "stage file could not be read." << endl;
		return 1;
	}
	State* state = new State(stageData, fileSize);
	//메인 루프
	while (true) {
		//일단 스테이지 그림 그리기
		state->draw();
		//클리어 체크
		if (state->hasCleared()) {
			break; //클리어체크
		}
		//입력 받기
		cout << "a:left s:right w:up z:down. command?" << endl; //操作説明
		char input;
		cin >> input;
		//갱신
		state->update(input);
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

    std::cout << "Hello World!\n";
}
//이하 함수정의

void readFile(char** buffer, int* size, const char* filename)
{
	ifstream in(filename);
	if (!in) {
		*buffer = 0;
		*size = 0;
	}
	else {
		in.seekg(0, ifstream::end);
		*size = static_cast<int>(in.tellg());
		in.seekg(0, ifstream::beg);
		*buffer = new char[*size];
		in.read(*buffer, *size);
	}
}
State::State(const char* stageData, int size) {
	//사이즈 측정
	setSize(stageData, size);
	//배열 확보
	mObjects.setSize(mWidth, mHeight);
	mGoalFlags.setSize(mWidth, mHeight);
	//초기값 체워두기
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			mObjects(x, y) = OBJ_WALL; //남은 부분의 벽
			mGoalFlags(x, y) = false; //골이 아닐떄
		}
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		Object t;
		bool goalFlag = false;
		switch (stageData[i]) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK; goalFlag = true; break;
		case '.': t = OBJ_SPACE; goalFlag = true; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN; goalFlag = true; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //개행처리
		default: t = OBJ_UNKNOWN; break;
		}
		if (t != OBJ_UNKNOWN) { //모를때 무시하기 위한 if문
			mObjects(x, y) = t; //기입
			mGoalFlags(x, y) = goalFlag; //골 정보
			++x;
		}
	}
}
void State::setSize(const char* stageData, int size) {
	mWidth = mHeight = 0; //초기화
	//현재위치
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		switch (stageData[i]) {
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			++x;
			break;
		case '\n':
			++y;
			//최대치 갱신
			mWidth = max(mWidth, x);
			mHeight = max(mHeight, y);
			x = 0;
			break;
		}
	}
}
void State::draw() const {
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			Object o = mObjects(x, y);
			bool goalFlag = mGoalFlags(x, y);
			if (goalFlag) {
				switch (o) {
				case OBJ_SPACE: cout << '.'; break;
				case OBJ_WALL: cout << '#'; break;
				case OBJ_BLOCK: cout << 'O'; break;
				case OBJ_MAN: cout << 'P'; break;
				}
			}
			else {
				switch (o) {
				case OBJ_SPACE: cout << ' '; break;
				case OBJ_WALL: cout << '#'; break;
				case OBJ_BLOCK: cout << 'o'; break;
				case OBJ_MAN: cout << 'p'; break;
				}
			}
		}
		cout << endl;
	}
}

void State::update(char input) {
	//이동차분으로 변환
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break; //좌
	case 's': dx = 1; break; //우
	case 'w': dy = -1; break; //위。Y는 아래가 플러스다
	case 'z': dy = 1; break; //아래。
	}
	//짧은 변수명을 붙이다
	int w = mWidth;
	int h = mHeight;
	Array2D< Object >& o = mObjects;
	//사람의 좌표로 검색
	int x, y;
	x = y = -1; //위험한 값
	bool found = false;
	for (y = 0; y < mHeight; ++y) {
		for (x = 0; x < mWidth; ++x) {
			if (o(x, y) == OBJ_MAN) {
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
	}
	//이동
	//이동후 좌표
	int tx = x + dx;
	int ty = y + dy;
	//좌표의 최대 최소 체크. 분리되어 있으면 불허
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//A. 그 방향이 공백 또는 골인.사람이 이동.
	if (o(tx, ty) == OBJ_SPACE) {
		o(tx, ty) = OBJ_MAN;
		o(x, y) = OBJ_SPACE;
		//B. 그 방향이 상자. 그 방향의 다음 칸이 공백 또는 골이면 이동.
	}
	else if (o(tx, ty) == OBJ_BLOCK) {
		//2칸이 범위 내인지 체크
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //누를 수 없다
			return;
		}
		if (o(tx2, ty2) == OBJ_SPACE) {
			//차례로 교체
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_MAN;
			o(x, y) = OBJ_SPACE;
		}
	}
}


//블록부분의 goal flag가 하나라도 false라면 아직 클리어 안함
bool State::hasCleared() const {
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			if (mObjects(x, y) == OBJ_BLOCK) {
				if (mGoalFlags(x, y) == false) {
					return false;
				}
			}
		}
	}
	return true;
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
