
/*  call_stack

    실제 시스템에서는 스택이 메모리에 저장되지만, 본 과제에서는 `int` 배열을 이용하여 메모리를 구현합니다.
    원래는 SFP와 Return Address에 실제 가상 메모리 주소가 들어가겠지만, 이번 과제에서는 -1로 대체합니다.

    int call_stack[]      : 실제 데이터(`int 값`) 또는 `-1` (메타데이터 구분용)을 저장하는 int 배열
    char stack_info[][]   : call_stack[]과 같은 위치(index)에 대한 설명을 저장하는 문자열 배열

    ==========================call_stack 저장 규칙==========================
    매개 변수 / 지역 변수를 push할 경우   : int 값 그대로
    Saved Frame Pointer 를 push할 경우  : call_stack에서의 index
    반환 주소값을 push할 경우       : -1
    =======================================================================


    ==========================stack_info 저장 규칙==========================
    매개 변수 / 지역 변수를 push할 경우        : 변수에 대한 설명
    Saved Frame Pointer 를 push할 경우  : 어떤 함수의 SFP인지
    반환 주소값을 push할 경우                 : "Return Address"
    ========================================================================
*/
#include <stdio.h>
#define STACK_SIZE 50 // 최대 스택 크기

int     call_stack[STACK_SIZE];         // Call Stack을 저장하는 배열
char*   stack_info[STACK_SIZE];     // Call Stack 요소에 대한 설명을 저장하는 배열

/*  SP (Stack Pointer), FP (Frame Pointer)

    SP는 현재 스택의 최상단 인덱스를 가리킵니다.
    스택이 비어있을 때 SP = -1, 하나가 쌓이면 `call_stack[0]` -> SP = 0, `call_stack[1]` -> SP = 1, ...

    FP는 현재 함수의 스택 프레임 포인터입니다.
    실행 중인 함수 스택 프레임의 sfp를 가리킵니다.
*/
int SP = -1;
int FP = -1;

void func1(int arg1, int arg2, int arg3);
void func2(int arg1, int arg2);
void func3(int arg1);

void push(int value, char* info)
{
    SP++;
    call_stack[SP] = value;
    stack_info[SP] = info;
}

void push_at(int idx_from_sp, int value, char* info)
// 함수 프롤로그에서 지역변수를 저장할 때에만 사용한다.
{
    int index = SP + idx_from_sp;
    call_stack[index] = value;
    stack_info[index] = info;
}

int pop()
{
    SP--;
    return call_stack[SP + 1];
}

/*
    현재 call_stack 전체를 출력합니다.
    해당 함수의 출력 결과들을 바탕으로 구현 완성도를 평가할 예정입니다.
*/
void print_stack()
{
    if (SP == -1)
    {
        // 참고: main 함수의 stack frame을 구현했으므로 call stack이 비는 경우는 없음.
        printf("Stack is empty.\n");
        return;
    }

    printf("====== Current Call Stack ======\n");

    for (int i = SP; i >= 0; i--)
    {
        if (call_stack[i] != -1)
            printf("%d : %s = %d", i, stack_info[i], call_stack[i]);
        else
            printf("%d : %s", i, stack_info[i]);

        if (i == SP)
            printf("    <=== [esp]\n");
        else if (i == FP)
            printf("    <=== [ebp]\n");
        else
            printf("\n");
    }
    printf("================================\n\n");
}


//func 내부는 자유롭게 추가해도 괜찮으나, 아래의 구조를 바꾸지는 마세요
void func1(int arg1, int arg2, int arg3)
{
    int var_1 = 100;

    // func1의 스택 프레임 형성 (함수 프롤로그 + push)
#define FUNC1_LOCAL_VAR_COUNT 1 // 컴파일 과정에서 계산될 지역변수의 수
    push(arg1, "Param \'arg1\' of Func \'func1\'");
    push(arg2, "Param \'arg2\' of Func \'func1\'");
    push(arg3, "Param \'arg3\' of Func \'func1\'");

    push(-1, "Return Address");
    push(FP, "SFP of Func \'main\'");
    FP = SP;
    SP += FUNC1_LOCAL_VAR_COUNT;

    push_at(0, var_1, "Local Var \'var_1\' of Func \'func1\'");

    print_stack();
    /**************************************/

    func2(11, 13);
    // func2의 스택 프레임 제거 (함수 에필로그 + pop)
#define FUNC2_PARAM_COUNT 2
    SP = FP;
    FP = pop();
    SP--; // Return Address
    SP -= FUNC2_PARAM_COUNT;
    print_stack();
    /**************************************/
}


void func2(int arg1, int arg2)
{
    int var_2 = 200;

    // func2의 스택 프레임 형성 (함수 프롤로그 + push)
#define FUNC2_LOCAL_VAR_COUNT 1
    push(arg1, "Param \'arg1\' of Func \'func2\'");
    push(arg2, "Param \'arg2\' of Func \'func2\'");

    push(-1, "Return Address");
    push(FP, "SFP of Func \'func1\'");
    FP = SP;
    SP += FUNC2_LOCAL_VAR_COUNT;

    push_at(0, var_2, "Local Var \'var_2\' of Func \'func2\'");

    print_stack();
    /**************************************/

    func3(77);
    // func3의 스택 프레임 제거 (함수 에필로그 + pop)
#define FUNC3_PARAM_COUNT 1
    SP = FP;
    FP = pop();
    SP--; // Return Address
    SP -= FUNC3_PARAM_COUNT;
    print_stack();
    /**************************************/
}


void func3(int arg1)
{
    int var_3 = 300;
    int var_4 = 400;

    // func3의 스택 프레임 형성 (함수 프롤로그 + push)
#define FUNC3_LOCAL_VAR_COUNT 2
    push(arg1, "Param \'arg1\' of Func \'func3\'");

    push(-1, "Return Address");
    push(FP, "SFP of Func \'func2\'");
    FP = SP;
    SP += FUNC3_LOCAL_VAR_COUNT;

    push_at(0, var_3, "Local Var \'var_3\' of Func \'func3\'");
    push_at(-1, var_4, "Local Var \'var_4\' of Func \'func3\'");

    print_stack();
    /**************************************/
}


//main 함수에 관련된 stack frame은 구현하지 않아도 됩니다.
int main()
{
    // FP == -1(기본값)일 때 출력이 안 되는 오류가 있어서 main 함수의 stack frame을 구현함.
    push(-1, "Stack Frame of Func \'main\'");
    FP = 0;

    func1(1, 2, 3);
    // func1의 스택 프레임 제거 (함수 에필로그 + pop)
#define FUNC1_PARAM_COUNT 3
    SP = FP;
    FP = pop();
    SP--; // Return Address
    SP -= FUNC1_PARAM_COUNT;
    print_stack();
    /**************************************/

    return 0;
}