/*circular.c*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //MAX_QUEUEX_SIZE를 4로 고정 

typedef char element; //element (배열의 요소)를 문자형으로 선언
typedef struct {
	element queue[MAX_QUEUE_SIZE]; //element를 queue[MAX_QUEUE_SIZE]로 선언 (element를 크기가 MAX_QUEUE_SIZE인 큐라고 선언)
	int front, rear; //int 형 변수 front , rear 선언
}QueueType; //구조체를 호출 할 때 사용할 이름


QueueType *createQueue(); //반환값이 QueueType에 저장될 Queue생성 함수 선언
int freeQueue(QueueType *cQ); //할당된 메모리를 해제하는 함수 선언
int isEmpty(QueueType *cQ); //큐가 비었는지 확인하는 함수 선언
int isFull(QueueType *cQ); //큐가 꽉 찼는지 확인하는 함수 선언 
void enQueue(QueueType *cQ, element item); //큐에 데이터를 입력받는 함수 선언
void deQueue(QueueType *cQ, element* item); //큐에 저장된 데이터를 삭제하는 함수 선언
void printQ(QueueType *cQ); //큐를 출력하는 함수 선언
void debugQ(QueueType *cQ); //현재 큐의 상태를 보여주는 함수 선언
element getElement(); //배열의 요소를 입력받는 함수 선언


int main(void)
{
    printf("[-----[Gahyun] [2020045070] -----\n");
	QueueType *cQ = createQueue(); //구조체 QueueType 의 *cQ에 creaateQueue함수의 반환값을 대입
	element data; //data는 문자형 배열의 요소 element라고 선언

	char command; //메뉴 선택 받을 입력받을 문자형 변수 command 선언

	do{
        //메뉴출력
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //command 값 입력받기

		switch(command) { //command 값에 따른 switch 조건문
		case 'i': case 'I': //i를 입력받았을 경우 (대소문자 모두 가능)
			data = getElement(); //data에 getElement() 함수의 반환값을 저장
			enQueue(cQ, data); //enQueue함수 호출
			break;
		case 'd': case 'D':  //d를 입력받았을 경우 (대소문자 모두 가능)
			deQueue(cQ,&data); //deQueue함수 호출
			break;
		case 'p': case 'P': //p를 입력받았을 경우 (대소문자 모두 가능)
			printQ(cQ); //printQ함수 호출
			break;
		case 'b': case 'B': //b를 입력받았을 경우 (대소문자 모두 가능)
			debugQ(cQ); //debugQ함수 호출
			break;
		case 'q': case 'Q':  //q를 입력받았을 경우 (대소문자 모두 가능)
			break; //종료
		default: //조건에 없는 문자를 입력받았을 경우
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); //q나 Q를 입력받지 않는 동안 반복


	return 1;
}

QueueType *createQueue()  //*createQueue함수 정의
{
	QueueType *cQ; //*cQ를 QueueType으로 선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); //cQ에 QueueType크기의 동적 메모리 할당 
	cQ->front = 0; //cQ의 front를 0으로 초기화
	cQ->rear = 0; //cQ의 rear 0으로 초기화
	return cQ; //cQ 반환
}

int freeQueue(QueueType *cQ) //freeQueue 함수 정의
{
    if(cQ == NULL) return 1; //cQ가 비어있을 경우 1 return
    free(cQ); //cQ에 할당된 메모리 해제 
    return 1; //1 return
}

element getElement() //getElement 함수 정의
{
	element item; //item을 element로 선언
	printf("Input element = ");
	scanf(" %c", &item); //item 값 입력 받기
	return item; //item 반환
}


int isEmpty(QueueType *cQ) //Queue가 비었는지 확인하는 함수
{
	if (cQ->front == cQ->rear){ //만약 front와 rear가 같다면
		printf("Circular Queue is empty!"); 
		return 1; //1 return
	}
	else return 0; //아니면 0 return
}

int isFull(QueueType *cQ) //Queue가 꽉찼는지 확인하는 함수
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { //rear를 증가시켰더니 front와 같다면
		printf(" Circular Queue is full!");
		return 1; //1 return
	}
	else return 0; //아니면 0 return
}

void enQueue(QueueType *cQ, element item) //cQ에 데이터 입력받는 함수
{
	if(isFull(cQ)) return; //isFull함수에서 반환된 값이 1이면, return
	else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; //rear++
		cQ->queue[cQ->rear] = item; //증가된 rear 위치에 item 대입
	}
}

void deQueue(QueueType *cQ, element *item) //cQ에 저장된 데이터 삭제하는 함수
{
	if(isEmpty(cQ)) return; //isEmpty함수에서 반환된 값이 1이면 return
	else {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; //front ++
		*item = cQ->queue[cQ->front]; //item의 주소에 증가된 front위치의 값 대입
	}
}


void printQ(QueueType *cQ)  //cQ 출력하는 함수
{
	int i, first, last; //정수형 변수 i first last선언

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //first 증가시키기
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; //last증가시키기

	printf("Circular Queue : [");

	i = first; 
	while(i != last){ //first인 i가 last가 아닐 동안
		printf("%3c", cQ->queue[i]); // queue[i]의 값 출력
		i = (i+1)%MAX_QUEUE_SIZE; //i++

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) //cQ의 현재 상태를 출력하는 함수
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) //큐의 개수만큼 반복
	{
		if(i == cQ->front) { //i가 front와 같다면
			printf("  [%d] = front\n", i); //i 값 출력
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //현재 front의 위치, rear의 위치 출력
}
