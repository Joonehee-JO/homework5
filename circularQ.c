/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4	//큐의 최대사이즈

typedef char element;		//char형 element 선언
typedef struct {			//QueueType 구조체 선언
	element queue[MAX_QUEUE_SIZE];	//멤버변수들
	int front, rear;
}QueueType;

QueueType *createQueue();			//큐를 생성하는 함수 프로토타입
int freeQueue(QueueType *cQ);		//큐를 메모리해제하는 함수 프로토타입
int isEmpty(QueueType *cQ);			//큐가 비어있는지 확인하는 함수 프로토타입
int isFull(QueueType *cQ);			//큐가 가득찼는지 확인하는 함수 프로토타입
void enQueue(QueueType *cQ, element item);		//큐에 데이터를 add하는 함수 프로토타입
void deQueue(QueueType *cQ, element* item);		//큐에 데이터를 Pop하는 함수 프로토타입
void printQ(QueueType *cQ);						//큐에 저장된값을 출력하는 함수 프로토타입
void debugQ(QueueType *cQ);			//현재 큐의 구조를 보여주는 함수 
element getElement();				//사용자로부터 입력할 데이터를 받는 함수 프로토타입

int main(void)
{
	QueueType *cQ = createQueue();	//큐타입 구조체 cQ선언 및 큐 생성
	element data;					//입력받을 데이터
	char command;					//커맨드 입력받는 변수

	printf("[----- [Cho Joon Hee]  [2017038076] -----]\n");

	do{	//메뉴얼
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);		//커맨드 입력

		switch(command) {		//입력값에 따른 기능 수행
		case 'i': case 'I':
			data = getElement();	//사용자로부터 큐에 저장할 data를 입력받음
			enQueue(cQ, data);		//데이터를 큐에 푸쉬함
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);		//큐의 첫번째 원소 팝
			break;
		case 'p': case 'P':
			printQ(cQ);				//큐에 저장된 값들을 출력
			break;
		case 'b': case 'B':
			debugQ(cQ);				//현재 큐의 구조 출력
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);			//큐 메모리반납
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue()	//큐를 생성하는 함수
{
	QueueType *cQ;			//큐타입 cQ선언
	cQ = (QueueType *)malloc(sizeof(QueueType));	//cQ에 구조체 사이즈만큼 동적할당 받음
	cQ->front = 0;			//멤버변수 front 0으로 초기화
	cQ->rear = 0;			//rear 0으로 초기화
	return cQ;				//할당받은 메모리주소 리턴
}

int freeQueue(QueueType *cQ)	//큐에 할당받은 메모리 반납하는 함수
{
    if(cQ == NULL) return 0;	//전처리, cQ에 할당된게 없으면 0리턴
    free(cQ);					//cQ에 할당받은 메모리 반납
    return 1;					//1리턴 후 함수종료
}

element getElement()			//사용자로부터 데이터 입력받는 함수
{
	element item;				//elemetn item 선언
	printf("Input element = ");	//사용자로부터 값 입력받음
	scanf(" %c", &item);		
	return item;				//입력받은 값 리턴
}


/* complete the function */
int isEmpty(QueueType *cQ)	//큐가 비어있는지 확인하는 함수
{
	if(cQ->front == cQ->rear)	//큐가 현재 비어있는 조건
		return 1;				//비어있다면 1리턴

    return 0;					//큐가 비어있지 않을 경우 0리턴
}

/* complete the function */
int isFull(QueueType *cQ)	//큐가 꽉차있는지 확인하는 함수
{
	if((cQ->rear+1)%MAX_QUEUE_SIZE == cQ->front)	//모듈러 연산을 통한 큐가 꽉차있을 조건
		return 1; 			//꽉찼을 경우 1리턴

   	return 0;				//그렇지 않을 경우 0리턴
}


/* complete the function */
void enQueue(QueueType *cQ, element item)	//큐에 데이터를 푸쉬하는 함수
{
	if(isFull(cQ)){		//전처리조건, 큐가 꽉찼는지 체크 후 꽉찼다면 함수종료
		printf("Que is Full!\n");
		return;
	}
	else {			//큐가 비어있다면
		cQ->rear = (cQ->rear+1)%MAX_QUEUE_SIZE;	//모듈러연산을 통하여 rear값 증가, 큐가 원형큐형태를 띔
		cQ->queue[cQ->rear] = item;				//큐에 데이터 푸쉬
	}
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)	//큐에 데이터를 팝하는 함수
{
	if(isEmpty(cQ)){	//전처리조건, 큐가 비어있는지 확인 후 비어있다면 함수종료
		printf("Que is Empty!\n");
		return;
	}
	else{
		cQ->front = (cQ->front+1)%MAX_QUEUE_SIZE;	//모듈러연산을 통하여 front값 증가
		*item = cQ->queue[cQ->front];		//큐의 프론트 다음 값, 큐의 첫번째 원소 팝하여 item에 저장
		printf("Que's first element %c is delete\n", *item);	//삭제된 원소 출력 후 함수종료
	}
    return;
}


void printQ(QueueType *cQ)	//큐에 저장된 데이터 출력하는 함수
{
	int i, first, last;		//지역변수 선언

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;	//first에 원형큐의 front할당
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;	//rear에 원형큐의 rear할당

	printf("Circular Queue : [");	

	i = first;
	while(i != last){	//큐가 비어있지않다면 반복문을 통하여 큐에 저장된 데이터출력
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)	//현재 큐의 구조(상태)를 보여주는 함수
{

	printf("\n---DEBUG\n");	
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)	//큐의 최대사이즈만큼 돌면서
	{
		if(i == cQ->front) {				//i가 큐의 front와 같다면 해당 i값, front의 위치 출력
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);	//큐에 저장된 값 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);	//front의 위치와 rear의 위치 출력
}


