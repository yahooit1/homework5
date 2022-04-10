/* postfix.c
 *
 *  Created on: Apr 2, 2019
 *      Author: rsyoung
 */

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10 //MAX_STACK_SIZE의 크기 10으로 고정
#define MAX_EXPRESSION_SIZE 20 //MAX_EXPRESSION_SIZE 20으로 고정

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum{ //연산자의 우선순위
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence; //구조체를 호출할 이름

char infixExp[MAX_EXPRESSION_SIZE]; //크기가 MAX_EXPRESSION_SIZE (20)이고 문자형인 배열 infixExp 선언
char postfixExp[MAX_EXPRESSION_SIZE]; //크기가 MAX_EXPRESSINO_SIZE (20)이고 문자형인 배열 postfixExp 선언
char postfixStack[MAX_STACK_SIZE]; //크기가 MAX_STACK_SIZE(10)이고 문자형인 배열 postfixStack 선언
int evalStack[MAX_STACK_SIZE]; //크기가 MAX_STACK_SIZE(10)이고 정수형인 배열 evalStack 선언

int postfixStackTop = -1;  //정수형 변수 postfixStackTop 선언 후 -1로 초기화
int evalStackTop = -1; //정수형 변수 evalStackTop 선언 후 -1로 초기화

int evalResult = 0; //정수형 변수 evalResult 선언 후 0으로 초기화 

void postfixPush(char x); //postfixPush 함수 선언
char postfixPop(); //postfixPop함수 선언
void evalPush(int x); //evalPush 함수 선언
int evalPop(); //evalPop 함수 선언
void getInfix(); //getInfix 함수 선언 
precedence getToken(char symbol); //getToken 함수 선언
precedence getPriority(char x); //getPriority 함수 선언
void charCat(char* c); // charCat 함수 선언
void toPostfix(); //toPostfix함수 선언
void debug(); //debug 함수 선언
void reset(); //reset함수 선언
void evaluation(); //evaluation 함수 선언 

int main()
{
	char command; //메뉴 입력받을 문자형변수 command 선언
         printf("[----- [Gahyun]  [2020045070] -----]\n"); 

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //comman 입력받기

		switch(command) { //comand값에 따른 switch 조건문
		case 'i': case 'I': //i 일 경우(대소문자 모두 가능하게 함)
			getInfix();//getInfix함수 호출
			break;
		case 'p': case 'P': //i 일 경우(대소문자 모두 가능하게 함)
			toPostfix(); //toPostfix 함수 호출
			break;
		case 'e': case 'E': //e 일 경우(대소문자 모두 가능하게 함)
			evaluation(); //evaluation 함수 호출
			break;
		case 'd': case 'D'://d 일 경우(대소문자 모두 가능하게 함)
			debug(); //debug 함수 호출
			break;
		case 'r': case 'R': //r 일 경우(대소문자 모두 가능하게 함)
			reset(); //reset 함수 호출
			break;
		case 'q': case 'Q': //q 일 경우(대소문자 모두 가능하게 함)
			break; //종료
		default: //조건에 없는 알파벳을 입력했을 경우
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); //q ,Q 가 입력되지 않는 동안 반복

	return 1;


}

void postfixPush(char x) //후위표기식 연산을 위해 스택에 Push하는 함수
{
    postfixStack[++postfixStackTop] = x; //x 값을 postFixStack배열 중 증가시킨 postfixStackTop 위치에 저장
                                            //즉, x를 '현재 Top위치'보다 하나 더 올린 위치에 저장
}

char postfixPop() //후위표기식 연산을 위해 스택에서 Pop하는 함수 
{
	char x;
    if(postfixStackTop == -1) //postfixStackTop 이 -1 일 경우 즉, postfixStackTop이 공백스택일 때
        return '\0'; //return NULL
    else {
    	x = postfixStack[postfixStackTop--]; // x값을 postFixStack배열에서 감소시킨 postfixStackTop 위치에 저장
                                                //즉, 'x를 현재 Top위치'보다 하나 더 내린 위치에 저장
    }
    return x; //return x
}

void evalPush(int x) //evalStack Push하는 함수
{
    evalStack[++evalStackTop] = x; //evalStack 배열에서 증가시킨 evalStackTop 위치에 x를 저장
                                    //즉, 'x를 현재 evalStack의 Top위치'보다 하나 올린 위치에 저장
}

int evalPop() //evalStack Pop하는 함수
{
    if(evalStackTop == -1) //evalStackTop이 -1이면 즉, evalStackTop이 공백스택일때
        return -1; //return -1
    else
        return evalStack[evalStackTop--]; //evalStack[evalStackTop--]에 저장된 값을 반환
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */

void getInfix() //중위 표기식 입력받는 함수
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); //infixExp 값 입력받기 
}

precedence getToken(char symbol) //우선순위반환함수
{
	switch(symbol) { //symbol 값에 따른 switch 조건문
	case '(' : return lparen; // ( 이면 lparen 반환
	case ')' : return rparen; // ) 이면 rparen 반환
	case '+' : return plus; // + 이면 plus 반환
	case '-' : return minus; // -이면 minus 반환
	case '/' : return divide; // /이면 divide 반환
	case '*' : return times; // *이면 times 반환 
	default : return operand; // 조건에 없는 문자일 경우 operand 반환
	}
}

precedence getPriority(char x) //입력받은 x의 우선순위를 반환하는 함수
{
	return getToken(x); //getToken(x)의 반환값을 반환
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0') //postfixExp 값이 NULL일 경우
		strncpy(postfixExp, c, 1); // postfixExp를 c에 문자열 1개 복사
	else
		strncat(postfixExp, c, 1); //postfixExp를 c에 문자열 1개 붙여넣기
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix() //postfix로 변경하는 함수
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp; // 포인터 exp에 infixExp(입력받은 중위표기식의 값) 저장
	char x; /* postfix로 변경할 때 임시로 저장하기 위한 변수 */ 

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') //포인터 exp가 NULL이 아닐동안 반복
	{
		if(getPriority(*exp) == operand) // 만약 getPriority 함수의 반환값이 operand 일 경우
		{
			x = *exp; // x에 *exp를 저장
        	charCat(&x); //charCat 함수 호출
		}
        else if(getPriority(*exp) == lparen) { //반환값이 lparen일 경우

        	postfixPush(*exp); //postfixPush 함수 호출 (*exp에 저장된 값을 postfixStack에 push)
        }
        else if(getPriority(*exp) == rparen) //반환값이 rparen일 경우
        {
        	while((x = postfixPop()) != '(') { //x에 postfixPop의 반환값을 저장한 후 x가 (가 아닐동안 반복
        		charCat(&x); //charCat 함수 호출 &x에 postfixExp를 복사/붙여넣기
        	}
        }
        else
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) 
            //postfixStack[postfixStackTop]의 우선순위가 *exp의 우선순위보다 높을동안

            {
            	x = postfixPop(); //x에 postfixPop함수값을 저장 즉, Pop하기
            	charCat(&x); //charCat 함수 호출. &x에 postfixExp를 복사/붙여넣기
            }
            postfixPush(*exp); //*exp를 postfixStack에 Push
        }
        exp++; //exp증가
	}

    while(postfixStackTop != -1) //postfixStackTop 가 -1이 아닐 동안 반복 
                                //postfixStackTop이 공백스택이 아닌동안
    {
    	x = postfixPop(); //x에 postfixPop 저장 즉, postfixStack에서 Pop하기
    	charCat(&x); //charCat 함수 호출 &x에 postfixExp를 복사/붙여넣기
    }

}

void debug() 
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++) //MAX_STACK_SIZE (스택의 크기)만큼 반복
		printf("%c  ", postfixStack[i]); //postfixStack값 출력 (Stack에 저장된 값 출력)
	printf("\n");

}

void reset() //스택에 저장된 모든 값을 초기화시키는 함수
{
	infixExp[0] = '\0'; //infixExp의 시작 주소에 NULL 저장
	postfixExp[0] = '\0'; //postfixExp의 시작 주소에 NULL 저장

	for(int i = 0; i < MAX_STACK_SIZE; i++) //MAX_STACK_SIZE만큼반복  
		postfixStack[i] = '\0'; //postfixStack의 모든 자리에 NULL 저장
         
	postfixStackTop = -1; //postfixStackTop에 -1 저장 (초기값)
	evalStackTop = -1; // evalStackTop에 -1 저장 (초기값)
	evalResult = 0; //evalResult에 0 저장(초기값)
}
void evaluation() // 연산하는 함수
{
	int opr1, opr2, i; //정수형변수선언 

	int length = strlen(postfixExp); //정수형 변수 length 에 postfixExp의 길이 저장
	char symbol; //문자형변수선언
	evalStackTop = -1; //evalStackTop을 -1로 초기화

	for(i = 0; i < length; i++) //postfixExp의 길이만큼 반복
	{
		symbol = postfixExp[i]; //symbol에 postfixExp배열에 저장된 값을 저장
		if(getToken(symbol) == operand) { //지정된 연산자 외에 다른 값이 입력됐을 때
			evalPush(symbol - '0'); //연산자 - 0을 evalStack에 Push
		}
		else {
			opr2 = evalPop(); //opr2=evalPop() evalStack Pop하여 opr2에 저장 (제일 Top에 있던 값(가장 최근에입력받은 값) opr2에 저장)
			opr1 = evalPop(); //opr1=evalPop() evalStack Pop하여 opr1에 저장 (그 다음으로 Top에 있던 값 opr1에 저장)
			switch(getToken(symbol)) { //getToken의 반환값에 따른 switch 조건문
			case plus: evalPush(opr1 + opr2); break; //plus일 경우 opr1+opr2의 값을 evalStack에 Push
			case minus: evalPush(opr1 - opr2); break; //minus일 경우 opr+opr2의 값을 evalStack에 Push
			case times: evalPush(opr1 * opr2); break; //times일 경우 opr1*opr2의 값을 evalStack에 Push
			case divide: evalPush(opr1 / opr2); break;//divide일 경우 opr1/opr2의 값을 evalStack에 Push
			default: break;
			}
		}
	}
	evalResult = evalPop(); //evalResult에 evalStack[evalStackTop]에 저장된 값 저장
}