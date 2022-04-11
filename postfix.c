/* postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10		//스택의 최대 사이즈
#define MAX_EXPRESSION_SIZE 20	//연산식의 최대 사이즈

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);	//문자를 postfixstack에 푸쉬하기위한 함수 프로토타입
char postfixPop();			//postfixstack에서 탑에 위치한 문자를 팝하기 위한 함수 프로토타입
void evalPush(int x);		//계산된 하나의 항을 스택에 저장하기 위한 함수 프로토타입
int evalPop();				//계산된 항을 POP하기 위한 함수 프로토타입
void getInfix();			//중위표기법으로 표기된 연산을 받아오는 함수 프로토타입
precedence getToken(char symbol);	//문자를 받아서 토큰으로 리턴하는 함수 프로토타입
precedence getPriority(char x);		//getToken함수를 호출하는 함수 프로토타입
void charCat(char* c);		//문자하나를 전달받아, postfixExp에 추가하는 함수 프로토타입
void toPostfix();			//중위표기를 후위표기로 변환하는 함수
void debug();				//중위표기법과 후위표기법, 계산된 결과 및 postfixstack에 남아있는 것을 출력하는 함수
void reset();				//현재 상태를 리셋하는 함수
void evaluation();			//변환한 연산식을 계산하는 함수

int main()
{
	char command;	//입력받을 커맨드변수

	do{	//메뉴얼
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");	//커맨드 입력받음
		scanf(" %c", &command);

		switch(command) {		//입력한 커맨드에 따른 기능 수행
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)	//postfix를 postfixstack에 push하기 위한 함수
{
    postfixStack[++postfixStackTop] = x;	//x를 Top을 1증가시킨후 스택에 저장
}

char postfixPop()		//postfixstack에 있는 문자를 POP하기 위한 함수
{
    char x;				//POP할 문자를 저장할 변수
    if(postfixStackTop == -1)	//스택에 아무것도 없으면
        return '\0';			//널캐릭터 리턴
    else {				//스택에 문자가 있다면
    	x = postfixStack[postfixStackTop--];	//x에 스택의 탑에 위치한 문자를 팝하여 꺼낸 후 저장
    }
    return x;		//팝한 문자를 리턴
}

void evalPush(int x)	//계산된 하나의 항을 계산을 위한 스택에 저장하기 위한 함수
{
    evalStack[++evalStackTop] = x;	//계산된 x를 계산을 위한 스택의 탑에 저장
}

int evalPop()		//계산된 항을 POP하기 위한 함수
{
    if(evalStackTop == -1)	//스택에 아무것도 들어있지 않다면
        return -1;			//-1리턴
    else
        return evalStack[evalStackTop--];	//스택에 들어있다면 스택의 탑에 저장된 수를 리턴
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()	//중위표기법으로 표기된 연산을 받아오는 함수
{
    printf("Type the expression >>> ");	//사용자로부터 연산을 입력받음
    scanf("%s",infixExp);	//infixExp에 저장
}

precedence getToken(char symbol)	//문자를 받아서 토큰으로 리턴
{
	switch(symbol) {				//isp이기에 좌측괄호의 우선순위가 가장 낮음
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)		//들어온 문자를 토큰으로 리턴해주는 함수
{
	return getToken(x);				//getToken함수 호출
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')		//후위표기로 변경된 문자열을 저장하는 배열에 아무것도 없다면
		strncpy(postfixExp, c, 1);	//c를 복사하여 postfixExp에 저장
	else
		strncat(postfixExp, c, 1);	//다른 문자가 들어와있으면 c를 이어붙임
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()	//중위표기를 후위표기로 변환하는 함수
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x,c; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')		//문자열 마지막 널캐릭터 만나면 종료
	{
		x = getToken(*exp);	//문자를 x에 토큰으로 바꿔서 저장
		if(x == operand){	//x가 피연산자라면
			charCat(exp);	//연산식배열에 붙임
		}
		else if(x == lparen){	//x가 '('라면
			postfixPush(*exp);	//스택의 탑에 푸쉬
		}
		else if(x == rparen){	//x가 ')'라면
			while((x = postfixPop()) != '(')	//스택의 탑에서부터 좌측괄호가 나올때까지
				charCat(&x);					//팝해서 연산식배열에 이어붙임
		}		
		else{				//x가 연산자라면
			if(postfixStackTop == -1)	//스택이 비어있을 때
				postfixPush(*exp);		//해당연산자를 스택에 푸쉬
			else{			//스택이 비어있지 않다면
				if(x > getToken(postfixStack[postfixStackTop]))	//x의 연산자 우선순위가 스택탑의 연산자보다 높을 때
					postfixPush(*exp);	//스택에 푸쉬
				else{		//x의 연산자 우선순위가 스택탑의 연산자보다 낮거나 같을 때
					while(getToken(postfixStack[postfixStackTop]) >= x){
						c = postfixPop();	//팝해서 c에 저장 후
						charCat(&c);		//c 연산식배열에 이어붙임
					}
					postfixPush(*exp);		//그리고 우선순위 높은 연산자는 스택에 올림
				}
			}
		}
		exp++;
	}

	while(postfixStackTop != -1)	//스택탑이 -1이 될때까지
    {
    	x = postfixPop();		//스택에 남아있는 문자들 팝하여
    	charCat(&x);			//연산식배열에 이어붙임
    }
}
void debug()	//중위표기법과 후위표기법, 계산된 결과 및 postfixstack에 남아있는 것을 출력하는 함수
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);		//중위표기법출력
	printf("postExp =  %s\n", postfixExp);		//후위표기법출력
	printf("eval result = %d\n", evalResult);	//결과값 출력

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)	//postfixStack에 남아있는 문자 출력
		printf("%c  ", postfixStack[i]);

	printf("\n");
}

void reset()	//리셋하는 함수
{
	infixExp[0] = '\0';		//중위연산식 초기화
	postfixExp[0] = '\0';	//후위연산식 초기화

	for(int i = 0; i < MAX_STACK_SIZE; i++)	//스택최대 사이즈만큼 돌면서 postfixstack 초기화
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;	//각 스택의 탑도 초기상태로 세팅
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()	//변환한 연산식을 계산하는 함수
{
	char *exp = postfixExp;	//postfixExp의 문자 하나씩을 읽기위한 포인터
	char x;					//문자하나를 임시로 저장하기 위한 변수
	int num_1, num_2;		//연산 전 항들을 꺼내놓기 위한 변수
	/* postfixExp, evalStack을 이용한 계산 */
	while(*exp != '\0'){	//널캐릭터가 나오기전까지 문자를 하나씩 읽음 
		x=getToken(*exp);	//읽어들인 문자를 토큰으로 변환하여 저장
		if(x==operand){		//x가 피연산자라면 
			evalPush(*exp - '0');	//문자에 48을 빼서 스택에 저장
		}
		else{	//x가 연산자라면
			num_2 = evalPop();	//계산에 필요한 항을 스택에서 팝하여 저장
			num_1 = evalPop();

			switch (x)	//x의 연산자에 따른 각 연산 수행
			{
			case plus:
				evalPush(num_1 + num_2);
				break;
			case minus:
				evalPush(num_1 - num_2);
				break;
			case times:
				evalPush(num_1 * num_2);
				break;
			case divide:
				evalPush(num_1 / num_2);
				break;
			default:
				break;
			}
		}
		exp++;
	}
	evalResult = evalPop();	//evalResult에 계산된 값을 저장
}

