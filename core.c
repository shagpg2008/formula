
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "core.h"

#define MAX_OPERATOR 4
#define OP_ADD 0
#define OP_MINUS 1
#define OP_MULTI 2
#define OP_DEVIDE 3

#define GET_0_N(n) (n)==0?0:(rand()%(n))

static char isThreeNumbers = 0, isAddShift = 0, isAbdicate = 0, hasReminder = 0;
static char numOfOperators = 0;
static char operators[MAX_OPERATOR] = {0};
static unsigned int maxNums[MAX_OPERATOR+1] = {0};


void core_init_operator(int isAdd, int isMinus, int isMultiple, int isDevide)
{
	srand((int)time(0));
	numOfOperators = 0;

	if(isAdd) {
		operators[numOfOperators++] = OP_ADD;
	}

	if(isMinus) {
		operators[numOfOperators++] = OP_MINUS;
	}

	if(isMultiple) {
		operators[numOfOperators++] = OP_MULTI;
	}

	if(isDevide) {
		operators[numOfOperators++] = OP_DEVIDE;
	}
}

void core_init_maxnum(unsigned int maxNumOfAdd, unsigned int maxNumOfMinus, unsigned int maxNumOfMultiple, unsigned int maxNumOfDevide)
{
	maxNums[OP_ADD] = maxNumOfAdd;
	maxNums[OP_MINUS] = maxNumOfMinus;
	maxNums[OP_MULTI] = maxNumOfMultiple;
	maxNums[OP_DEVIDE] = maxNumOfDevide;
}

void core_init_misc(int _isThreeNumbers, int _isAddShift, int _isAbdicate, int _hasReminder)
{
	isThreeNumbers = (char)_isThreeNumbers;
	isAddShift = (char)_isAddShift;
	isAbdicate = (char)_isAbdicate;
	hasReminder = (char)_hasReminder;
}

static unsigned int getNum(unsigned int max)
{
	return GET_0_N(max);
}

static unsigned int getOpNum(unsigned int max, unsigned int *num, unsigned int op)
{
	unsigned int theOtherNum = 0;
	switch(operators[op]) {
	case OP_ADD:
		theOtherNum = GET_0_N(max - *num);
		break;
	case OP_MINUS:
		if(*num == 0) {
			theOtherNum = 0;
		} else {
			theOtherNum = GET_0_N(*num)+1;
		}
		break;
	case OP_MULTI:
		theOtherNum = GET_0_N(max)+1;
		break;
	case OP_DEVIDE: {
		unsigned int tmp = *num;
		theOtherNum = GET_0_N(max)+1;
		if(*num != 0) {
			*num *= theOtherNum;
			theOtherNum = tmp;
		}
	}break;
	default:
		break;
	}

	return theOtherNum;
}

static unsigned int getOp()
{
	//return GET_0_N(numOfOperators);
	return rand()%numOfOperators;
}

static const char* getOpStr(char op)
{
	static const char* opStr[] = {"+", "-", "x", "��"};
	return opStr[operators[op]];
}

static unsigned int getResult(unsigned int num1, unsigned int num2, char op)
{
	unsigned int result = 0;
	switch(operators[op]) {
	case OP_ADD:
		result = num1 + num2;
		break;
	case OP_MINUS:
		result = num1 - num2;
		break;
	case OP_MULTI:
		result = num1 * num2;
		break;
	case OP_DEVIDE:
		result = num1 / num2;
		break;
	default:
		break;
	}

	return result;
}

void getFileName(char *szFileName, int len)
{
	time_t time_now   = time(NULL); 
	struct tm* tm_now = localtime(&time_now);
	_snprintf(szFileName, len, "%04d%02d%02d%02d%02d%02d", 
		tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, 
		tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec); 
}

void getTitle(char *szTitle, int len)
{
	int res = 0, index = 0;
	char *lpszTitle = szTitle;

	for(index = 0; index < numOfOperators; index++) {
		switch (operators[index]) {
		case OP_ADD:
			res = _snprintf(lpszTitle, len, "%d���ڼӷ�", maxNums[OP_ADD]);
			break;
		case OP_MINUS:
			res = _snprintf(lpszTitle, len, "%d���ڼ���", maxNums[OP_MINUS]);
			break;
		case OP_MULTI:
			res = _snprintf(lpszTitle, len, "%d���ڳ˷�", maxNums[OP_MULTI]);
			break;
		case OP_DEVIDE:
			res = _snprintf(lpszTitle, len, "%d���ڳ���", maxNums[OP_DEVIDE]);
			break;
		default: break;
		}

		if(res > 0) {
			lpszTitle += res;
			len -= res;
		}
	}
}

//-������û��
void genAnFormula(char *buff, int len, int blankN)
{
	const char *lpRack = "(   )";
	char op = getOp(), op2;
	unsigned int num1 = getNum(maxNums[operators[op]]);
	unsigned int num2 = getOpNum(maxNums[operators[op]], &num1, op);
	unsigned int num3 = 0, result = getResult(num1, num2, op);
	char num1Str[8] = {0};
	char num2Str[8] = {0};
	char num3Str[8] = {0};
	char resStr[8]  = {0};

	if(isThreeNumbers) {
		unsigned int max = 0;
		op2 = getOp();
		max = maxNums[operators[op2]];
		switch(operators[op2]) {
			case OP_ADD: {
				if(max <  result) {
					result = 0;
				}
				num3 = max==0?0:getOpNum(max, &result, op2);
				result = getResult(result, num3, op2);
				break;
			}
			case OP_MINUS:{
				max = min(result, max);
				num3 = getOpNum(max, &max, op2);
				result = getResult(result, num3, op2);
				break;
			}
			case OP_MULTI: {
				num3 = getOpNum(maxNums[OP_MULTI], &num2, op2);
				if(operators[op] == OP_ADD || operators[op] == OP_MINUS ) {
					result = getResult(num2, num3, op2);
					if(operators[op] == OP_MINUS && num1 < result) {
						num3 = num1/num2;
						result = getResult(num2, num3, op2);
					}
					result = getResult(num1, result, op);
				}else{
					result = getResult(result, num3, op2);
				}
				break;
			}
			case OP_DEVIDE:
				if(operators[op] == OP_ADD || operators[op] == OP_MINUS ) {
					num3 = getOpNum(maxNums[OP_DEVIDE], &num2, op2);
					result = getResult(num2, num3, op2);
					result = getResult(num1, result, op);
				}else{
					num3 = getOpNum(maxNums[OP_DEVIDE], &result, op2);
					result = getResult(result, num3, op2);
				}
				break;
			default:break;
		}
	}
	
	if(blankN == 4) {
		blankN = rand()%(3+isThreeNumbers);
	}

	if(isThreeNumbers) {
		_snprintf(buff, len, "%s %s %s %s %s = %s", 
			blankN==1?lpRack:itoa(num1, num1Str, 10),
			getOpStr(op),
			blankN==2?lpRack:itoa(num2, num2Str, 10),
			getOpStr(op2),
			blankN==3?lpRack:itoa(num3, num3Str, 10),
			blankN==0?lpRack:itoa(result, resStr, 10)
		);
	} else {
		_snprintf(buff, len, "%s %s %s = %s", 
			blankN==1?lpRack:itoa(num1, num1Str, 10),
			getOpStr(op),
			blankN==2?lpRack:itoa(num2, num2Str, 10),
			blankN==0?lpRack:itoa(result, resStr, 10)
		);
	}
}
