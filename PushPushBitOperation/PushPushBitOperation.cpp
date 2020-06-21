// PushPushBitOperation.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <fstream>

using namespace std;

//함수 프로토 타입
void readFile(char** buffer, int* size, const char* filename);

//2차원 배열 클래스
//템플릿이 어렵다면 기초만이라도 공부해두자
//이 클래스 선언중애 T라는 클래스가 있는것 처럼
//이것을 사용할 때는 T에 int라던가 bool을 넣던가 넣어서 사용하면 된다(제네릭)
template<class T> class Array2D
{
public:
    Array2D() : mArray(0) {}
    ~Array2D()
    {
        delete[] mArray;
        mArray = 0;  //포인터에 0을 넣는 것을 버릇으로 하자
    }
    void setSize(int size0, int size1)
    {
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
        OBJ_GOAL_FLAG = (1 << 7), //골 플래그

    };
    void setSize(const char* stageData, int size);

    int mWidth;
    int mHeight;
    Array2D<unsigned char> mObjects; //비트 연산 할 거라 unsgined char 여기까지 아껴도 된다
};
int main()
{
    std::cout << "Hello World!\n";
}

//이하 함수 정의

void readFile(char** buffer, int* size, const char* filename) {
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

void State::draw() const {
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) {
            switch (mObjects(x, y)) {
            case (OBJ_SPACE | OBJ_GOAL_FLAG): cout << '.'; break;
            case (OBJ_WALL | OBJ_GOAL_FLAG): cout << '#'; break;
            case (OBJ_BLOCK | OBJ_GOAL_FLAG): cout << 'O'; break;
            case (OBJ_MAN | OBJ_GOAL_FLAG): cout << 'P'; break;
            case OBJ_SPACE: cout << ' '; break;
            case OBJ_WALL: cout << '#'; break;
            case OBJ_BLOCK: cout << 'o'; break;
            case OBJ_MAN: cout << 'p'; break;
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
