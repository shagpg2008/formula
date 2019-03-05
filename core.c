
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

typedef unsigned char EquationType[2];

#define GET_0_N(n) (n)==0?0:(rand()%(n))
#define NUM(array) (sizeof(array)/sizeof(array[0]))
#define MAX_NUM 100
#define MAX_FORMULA(n) ((n)*((n)+1)/2)

static unsigned int maxNumEquationOfAddShift = 0;
static unsigned int equationOfAddShiftBase = 0;
static unsigned int maxNumEquationOfAddNormal = 0;
static unsigned int equationOfAddNormalBase = 0;
static unsigned int maxNumEquationOfMinusAdicate = 0;
static unsigned int equationOfMinusAdicateBase = 0;
static unsigned int maxNumEquationOfMinusNormal = 0;
static unsigned int equationOfMinusNormalBase = 0;

unsigned char eqAddMinusNormalBelow[/*MAX_FORMULA*/ 1425][2] = {{0,0}
};

unsigned short eqAddMinusNormalBelowMap[MAX_NUM+1] = {/*no use -->*/0};

unsigned char eqAddMinusShiftBelow[/*MAX_FORMULA*/1075][2] = {{0,0}
};

unsigned short eqAddMinusShiftBelowMap[MAX_NUM+1] = {/*no use -->*/0};

enum MAXNUM{MAXNUM5, MAXNUM10, MAXNUM20, MAXNUM100};

/*unsigned char eqMultiDevide[5050 ][2] = {{0,0}
1x1: 0~45
11x1: 46~855
11x11:856-4950
};
*/

unsigned char eqMultiDevideBelow100[4952][2] = {{0,0}};
unsigned short eqMultiDevideBelow100Map[4][2] = {{1,46}, {47,810}, {857,4095}};

static char isThreeNumbers = 0, isAddShift = 0, isAddShiftOnly = 0, isAbdicate = 0, isAbdicateOnly = 0,hasReminder = 0, hasBracketPriority = 0;
static char numOfOperators = 0;
static char operators[MAX_OPERATOR] = {0};
static unsigned int maxNums[MAX_OPERATOR+1] = {0};

static int getMaxNumIndex(unsigned int max) 
{
	switch (max){
		case 5:return MAXNUM5;
		case 10:return MAXNUM10;
		case 20:return MAXNUM20;
		case 100:return MAXNUM100;
		default:break;
	}

	return MAXNUM20;
}

static unsigned int getNumBetween(unsigned int min, unsigned max)
{
	unsigned int num = 0;
	if(max <= min) {
		return 0;
	}
	num = rand();
	max -= min;
	return min + (num%max);
}

static void genMultiDevideEq(int min1, int max1, int min2, int max2)
{
	static int index = 1;
	int i = 0, j = 0;
	for(i = min1; i < max1; i++)
		for(j = min2; j <= i && j < max2; j++) {
			eqMultiDevideBelow100[index][0] = i;
			eqMultiDevideBelow100[index++][1] = j;
		}
}

void core_init_formula(void)
{
	int i = 0, j = 0, indexNormal = 0, indexNormalMap = 1, indexShift = 0, indexShiftMap = 1;

	for(i = 1; i <= MAX_NUM; i++) {
		eqAddMinusNormalBelowMap[indexNormalMap++] = indexNormal;
		eqAddMinusShiftBelowMap[indexShiftMap++] = indexShift;
		
		for(j = 1; i-j >= j; j++) {	
			if(((i%10) < (j%10)) || ((i%100) < (j%100))) {
				eqAddMinusShiftBelow[indexShift][0] = i - j; 
				eqAddMinusShiftBelow[indexShift][1] = j;
				indexShift++;
			} else {
				eqAddMinusNormalBelow[indexNormal][0] = i - j; 
				eqAddMinusNormalBelow[indexNormal][1] = j;
				indexNormal++;
			}
		}
	}

	genMultiDevideEq(1, 10, 1, 10);
	genMultiDevideEq(10, 100, 1, 10);
	genMultiDevideEq(10, 100, 10, 100);
}


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

static void initNumOfEquation(unsigned int maxNum, unsigned int *lpShiftAdicate, unsigned int *lpShiftAdicateBase, unsigned int *lpNormal, unsigned int *lpNormalBase);
void core_init_maxnum(unsigned int maxNumOfAdd, unsigned int maxNumOfMinus, unsigned int maxNumOfMultiple, unsigned int maxNumOfDevide)
{
	maxNums[OP_ADD] = maxNumOfAdd;
	maxNums[OP_MINUS] = maxNumOfMinus;
	maxNums[OP_MULTI] = maxNumOfMultiple;
	maxNums[OP_DEVIDE] = maxNumOfDevide;

	initNumOfEquation(maxNumOfAdd, &maxNumEquationOfAddShift, &equationOfAddShiftBase, &maxNumEquationOfAddNormal, &equationOfAddNormalBase);
	initNumOfEquation(maxNumOfMinus, &maxNumEquationOfMinusAdicate, &equationOfMinusAdicateBase, &maxNumEquationOfMinusNormal, &equationOfMinusNormalBase);
}

void core_init_misc(int _isThreeNumbers, int _isAddShift, int _isAddShiftOnly, int _isAbdicate, int _isAbdicateOnly, int _hasReminder, int _hasBracketPriority)
{
	isThreeNumbers = (char)_isThreeNumbers;
	isAddShift = (char)_isAddShift;
	isAddShiftOnly = (char)_isAddShiftOnly;
	isAbdicate = (char)_isAbdicate;
	isAbdicateOnly = (char)_isAbdicateOnly;
	hasReminder = (char)_hasReminder;
	hasBracketPriority = (char)_hasBracketPriority;
}

static void initNumOfEquation(unsigned int maxNum, unsigned int *lpShiftAdicate, unsigned int *lpShiftAdicateBase, unsigned int *lpNormal, unsigned int *lpNormalBase)
{
	unsigned short maxNumOfShift[][2] = {
		{0, eqAddMinusShiftBelowMap[5]-0},
		{eqAddMinusShiftBelowMap[5], eqAddMinusShiftBelowMap[10]-eqAddMinusShiftBelowMap[5]},
		{eqAddMinusShiftBelowMap[10], eqAddMinusShiftBelowMap[20]-eqAddMinusShiftBelowMap[10]},
		{eqAddMinusShiftBelowMap[20], eqAddMinusShiftBelowMap[100]-eqAddMinusShiftBelowMap[20]}
	};
	unsigned short maxNumOfNormal[][2] = {
		{0, eqAddMinusShiftBelowMap[5]-0},
		{eqAddMinusNormalBelowMap[5], eqAddMinusNormalBelowMap[10]-eqAddMinusNormalBelowMap[5]},
		{eqAddMinusNormalBelowMap[10], eqAddMinusNormalBelowMap[20]-eqAddMinusNormalBelowMap[10]},
		{eqAddMinusNormalBelowMap[20], eqAddMinusNormalBelowMap[100]-eqAddMinusNormalBelowMap[20]}
	};

	int maxNumIndex = getMaxNumIndex(maxNum);

	*lpNormal = maxNumOfNormal[maxNumIndex][1];
	*lpNormalBase = maxNumOfNormal[maxNumIndex][0];
	*lpShiftAdicate = maxNumOfShift[maxNumIndex][1];
	*lpShiftAdicateBase = maxNumOfShift[maxNumIndex][0];
}

static EquationType *getAddMinusEquationFrom(int num, unsigned int op)
{
	if(OP_ADD == op && isAddShift && isAddShiftOnly) {

	}
}

static EquationType *getAddMinusEquation(unsigned int op)
{
	unsigned int num = 0, index = 0, max = 0, isShiftAdicateThisTime = 0, randx = rand();

	if(op == OP_ADD) {
		max = maxNums[OP_ADD];
		if(isAddShift) {
			if(isAddShiftOnly) {
				num = maxNumEquationOfAddShift;
				index = equationOfAddShiftBase;
				isShiftAdicateThisTime = 1;
			} else {
				if(randx % 2) {
					num = maxNumEquationOfAddShift;
					index = equationOfAddShiftBase;
					isShiftAdicateThisTime = 1;
				} else {
					num = maxNumEquationOfAddNormal;
					index = equationOfAddNormalBase;
				}
			}
		} else {
			num = maxNumEquationOfAddNormal;
			index = equationOfAddNormalBase;
		}
	}
	else if(op == OP_MINUS) {
		max = maxNums[OP_MINUS];
		if(isAbdicate) {
			if(isAbdicateOnly) {
				num = maxNumEquationOfMinusAdicate;
				index = equationOfMinusAdicateBase;
				isShiftAdicateThisTime = 1;
			} else {
				if(randx % 2) {
					num = maxNumEquationOfMinusAdicate;
					index = equationOfMinusAdicateBase;
					isShiftAdicateThisTime = 1;
				} else {
					num = maxNumEquationOfMinusNormal;
					index = equationOfMinusNormalBase;
				}
			}
		} else {
			num = maxNumEquationOfMinusNormal;
			index = equationOfMinusNormalBase;
		}
	}
	else {
		return NULL;
	}

	index += (randx % num);
	if(isShiftAdicateThisTime) {
		return &eqAddMinusShiftBelow[index];
	} else {
		return &eqAddMinusNormalBelow[index];
	}

	return NULL;
}

static EquationType *getMultiDevideEquation(unsigned int op)
{
	static EquationType eq = {0,0};
	unsigned int index = 0, max = 0, randx = rand(), mapIndex = maxNums[op];

	index = eqMultiDevideBelow100Map[mapIndex][0];
	max   = eqMultiDevideBelow100Map[mapIndex][1];
	index += randx%max;
	return &eqMultiDevideBelow100[index];
}

static unsigned int getNum(unsigned int max)
{
	return GET_0_N(max);
}

static unsigned int getOpNum(unsigned int max, unsigned int *num, unsigned int op)
{
	unsigned int theOtherNum = 0;
	switch(operators[op]) {
	case OP_ADD:{
		unsigned char low1 = (*num)%10, low2 = 0, tmp = 0;

		theOtherNum = GET_0_N(max - *num);
		low2 = (theOtherNum % 10);
		tmp = low1+low2;

		if(!isAddShift) {	
			if(tmp >= 10) {
				tmp -= 10-1;
				theOtherNum -= tmp;
			}
		} else {
			unsigned int result = *num + theOtherNum;
			if(tmp < 10 &&  result + 10 < max) {
				theOtherNum += 10 - tmp + 1;
			}
		}}
		break;
	case OP_MINUS:
		if(*num == 0) {
			theOtherNum = 0;
		} else {
			unsigned char low1 = (*num)%10, low2 = 0, tmp = 0;
			theOtherNum = GET_0_N(*num)+1;
			low2 = (theOtherNum % 10);

			if(!isAbdicate) {
				if(low1 < low2) {
					tmp = low2 - low1;
					if(low1 > 0) {
						tmp ++;
					}

					theOtherNum -= tmp;
				}
			} else {
				if(low1 > low2) {
					unsigned char ten1 = (*num)/10, ten2 = theOtherNum / 10;
					if(ten1 > ten2) {
						theOtherNum += low1 - low2;
						if(low2 < 8) {
							theOtherNum++;
						}
					}
				}
			}
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
	static const char* opStr[] = {"+", "-", "x", "÷"};
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
			res = _snprintf(lpszTitle, len, "%d以内加法", maxNums[OP_ADD]);
			break;
		case OP_MINUS:
			res = _snprintf(lpszTitle, len, "%d以内减法", maxNums[OP_MINUS]);
			break;
		case OP_MULTI:
			res = _snprintf(lpszTitle, len, "%d以内乘法", maxNums[OP_MULTI]);
			break;
		case OP_DEVIDE:
			res = _snprintf(lpszTitle, len, "%d以内除法", maxNums[OP_DEVIDE]);
			break;
		default: break;
		}

		if(res > 0) {
			lpszTitle += res;
			len -= res;
		}
	}
}

void genAnFormula(char *buff, int len, int blankN)
{
	const char *lpRack = "(　　)";
	const char *lpRes  = blankN==0?"":lpRack;
	char op = getOp(), op2 = isThreeNumbers?getOp():-1;
	unsigned int num1 = 0, num2 = 0, num3 = 0, result = 0;
	unsigned int reminder = 0, hasReminderInThisFormula = 0, hasBracketPriorityInThisFormula = 0, priorityN = 0;
	EquationType *lpEqType = NULL;
	char num1Str[8] = {0};
	char num2Str[8] = {0};
	char num3Str[8] = {0};
	char resStr[8]  = {0};

	if(operators[op] == OP_MULTI || operators[op] == OP_DEVIDE ) {
		lpEqType = getMultiDevideEquation(operators[op]);
	} else {
		lpEqType = getAddMinusEquation(operators[op]);
	}
	
	if(operators[op] == OP_ADD) {
		if(rand()%2) {
			num1 = (*lpEqType)[0];
			num2 = (*lpEqType)[1];
		} else {
			num1 = (*lpEqType)[1];
			num2 = (*lpEqType)[0];
		}
	}else if(operators[op] == OP_MINUS){
		num1 = (*lpEqType)[0] + (*lpEqType)[1];
		num2 = rand()%2?(*lpEqType)[1]:(*lpEqType)[0];
	} else if(operators[op] == OP_MULTI){
		if(rand()%2) {
			num1 = (*lpEqType)[0];
			num2 = (*lpEqType)[1];
		} else {
			num1 = (*lpEqType)[1];
			num2 = (*lpEqType)[0];
		}
	} else if(operators[op] == OP_DEVIDE){
		int randx = rand();
		num1 = (*lpEqType)[0] * (*lpEqType)[1];
		num2 = randx%2?(*lpEqType)[1]:(*lpEqType)[0];
	} else {
		num1 = getNum(maxNums[operators[op]]);
		num2 = getOpNum(maxNums[operators[op]], &num1, op);
	}

	result = getResult(num1, num2, op);

	if(hasReminder && operators[op] == OP_DEVIDE) {
		reminder = GET_0_N(num2);
		hasReminderInThisFormula = (reminder % 2);
		if(hasReminderInThisFormula) {
			num1 += reminder;
		}
	} 
	
	if(!hasReminderInThisFormula && isThreeNumbers) {
		unsigned int max = maxNums[operators[op2]];
		//op2 = getOp();

		if(hasBracketPriority){
			priorityN = rand();
			hasBracketPriorityInThisFormula = (priorityN % 2);
			priorityN %= 101;
			priorityN %= 2;
		}
	
		if(hasBracketPriorityInThisFormula) {
			unsigned int tmp = result;
		//	num3 = getOpNum(max, &tmp, op2);

			if(priorityN == 0) {
				if(operators[op2] == OP_DEVIDE && tmp > result) {
					if(operators[op] == OP_ADD || operators[op] == OP_MINUS) {
						num1 += tmp - result;
					}else{
						num1 *= tmp - result;
					}
				}

				result = getResult(tmp, num3, op2);
			} else {
				if(operators[op2] == OP_DEVIDE) {
					num2 = tmp;
				}

				result = getResult(num2, num3, op2);

				if(result == 0 && operators[op] == OP_DEVIDE) {
					num2 = 1;
					if(operators[op2] == OP_ADD || operators[op2] == OP_MINUS) {
						num3 = 0;
					} else {
						num3 = 1;
					}

					result = getResult(num2, num3, op2);
				}

				if(operators[op] == OP_MINUS && num1 < result) {
					num1 += result;
				}

				result = getResult(num1, result, op);
			}
		}else

		switch(operators[op2]) {
			case OP_ADD:
			case OP_MINUS:{
				num3 = getOpNum(max, &result, op2);
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
					unsigned int tmp =  result;
					num3 = getOpNum(maxNums[OP_DEVIDE], &tmp, op2);
					num1 *= num3;
					result = getResult(num1, num2, op);
					result = getResult(result, num3, op2);
				}
				break;
			default:break;
		}
	}else {
		
	}
	
	if(blankN == 4) {
		blankN = rand()%(3+isThreeNumbers);
	}

	if(!hasReminderInThisFormula) {
		if(result == 0 
			&& ((operators[op] == OP_MULTI || operators[op] == OP_DEVIDE) 
				&& (!isThreeNumbers || (operators[op2] == OP_MULTI || operators[op2] == OP_DEVIDE)))) {
			blankN = 0;
		}
		
		if(isThreeNumbers) {
			if(hasBracketPriorityInThisFormula) {
				if(priorityN == 0) {
					_snprintf(buff, len, "(%s %s %s) %s %s = %s", 
						blankN==1?lpRack:itoa(num1, num1Str, 10),
						getOpStr(op),
						blankN==2?lpRack:itoa(num2, num2Str, 10),
						getOpStr(op2),
						blankN==3?lpRack:itoa(num3, num3Str, 10),
						blankN==0?lpRes:itoa(result, resStr, 10)
					);
				} else {
					_snprintf(buff, len, "%s %s (%s %s %s) = %s", 
						blankN==1?lpRack:itoa(num1, num1Str, 10),
						getOpStr(op),
						blankN==2?lpRack:itoa(num2, num2Str, 10),
						getOpStr(op2),
						blankN==3?lpRack:itoa(num3, num3Str, 10),
						blankN==0?lpRes:itoa(result, resStr, 10)
					);
				}
			} else {
				_snprintf(buff, len, "%s %s %s %s %s = %s", 
					blankN==1?lpRack:itoa(num1, num1Str, 10),
					getOpStr(op),
					blankN==2?lpRack:itoa(num2, num2Str, 10),
					getOpStr(op2),
					blankN==3?lpRack:itoa(num3, num3Str, 10),
					blankN==0?lpRes:itoa(result, resStr, 10)
				);
			}
		} else {
			_snprintf(buff, len, "%s %s %s = %s", 
				blankN==1?lpRack:itoa(num1, num1Str, 10),
				getOpStr(op),
				blankN==2?lpRack:itoa(num2, num2Str, 10),
				blankN==0?lpRes:itoa(result, resStr, 10)
			);
		}
	} else {
		_snprintf(buff, len, "%s %s %s = %s余%s", 
				blankN==1?lpRack:itoa(num1, num1Str, 10),
				getOpStr(op),
				blankN==2?lpRack:itoa(num2, num2Str, 10),
				blankN==0?lpRack:itoa(result, resStr, 10),
				blankN==0?lpRack:itoa(reminder, num3Str, 10)
		);
	}
}

//-带括号没有
void genAnFormula1(char *buff, int len, int blankN)
{
	const char *lpRack = "(　　)";
	char op = getOp(), op2;
	unsigned int num1 = getNum(maxNums[operators[op]]);
	unsigned int num2 = getOpNum(maxNums[operators[op]], &num1, op);
	unsigned int num3 = 0, result = getResult(num1, num2, op);
	unsigned int reminder = 0, hasReminderInThisFormula = 0, hasBracketPriorityInThisFormula = 0, priorityN = 0;
	char num1Str[8] = {0};
	char num2Str[8] = {0};
	char num3Str[8] = {0};
	char resStr[8]  = {0};

	if(hasReminder && operators[op] == OP_DEVIDE) {
		reminder = GET_0_N(num2);
		hasReminderInThisFormula = (reminder % 2);
		if(hasReminderInThisFormula) {
			num1 += reminder;
		}
	} 
	
	if(!hasReminderInThisFormula && isThreeNumbers) {
		unsigned int max = 0;
		op2 = getOp();

		if(hasBracketPriority){
			priorityN = rand();
			hasBracketPriorityInThisFormula = (priorityN % 2);
			priorityN %= 101;
			priorityN %= 2;
		}
	
		max = maxNums[operators[op2]];
		if(hasBracketPriorityInThisFormula) {
			unsigned int tmp = result;
			num3 = getOpNum(max, &tmp, op2);

			if(priorityN == 0) {
				if(operators[op2] == OP_DEVIDE && tmp > result) {
					if(operators[op] == OP_ADD || operators[op] == OP_MINUS) {
						num1 += tmp - result;
					}else{
						num1 *= tmp - result;
					}
				}

				result = getResult(tmp, num3, op2);
			} else {
				if(operators[op2] == OP_DEVIDE) {
					num2 = tmp;
				}

				result = getResult(num2, num3, op2);

				if(result == 0 && operators[op] == OP_DEVIDE) {
					num2 = 1;
					if(operators[op2] == OP_ADD || operators[op2] == OP_MINUS) {
						num3 = 0;
					} else {
						num3 = 1;
					}

					result = getResult(num2, num3, op2);
				}

				if(operators[op] == OP_MINUS && num1 < result) {
					num1 += result;
				}

				result = getResult(num1, result, op);
			}
		}else

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
					unsigned int tmp =  result;
					num3 = getOpNum(maxNums[OP_DEVIDE], &tmp, op2);
					num1 *= num3;
					result = getResult(num1, num2, op);
					result = getResult(result, num3, op2);
				}
				break;
			default:break;
		}
	}
	
	if(blankN == 4) {
		blankN = rand()%(3+isThreeNumbers);
	}

	if(!hasReminderInThisFormula) {
		if(result == 0 
			&& ((operators[op] == OP_MULTI || operators[op] == OP_DEVIDE) 
				&& (!isThreeNumbers || (operators[op2] == OP_MULTI || operators[op2] == OP_DEVIDE)))) {
			blankN = 0;
		}
		
		if(isThreeNumbers) {
			if(hasBracketPriorityInThisFormula) {
				if(priorityN == 0) {
					_snprintf(buff, len, "(%s %s %s) %s %s = %s", 
						blankN==1?lpRack:itoa(num1, num1Str, 10),
						getOpStr(op),
						blankN==2?lpRack:itoa(num2, num2Str, 10),
						getOpStr(op2),
						blankN==3?lpRack:itoa(num3, num3Str, 10),
						blankN==0?lpRack:itoa(result, resStr, 10)
					);
				} else {
					_snprintf(buff, len, "%s %s (%s %s %s) = %s", 
						blankN==1?lpRack:itoa(num1, num1Str, 10),
						getOpStr(op),
						blankN==2?lpRack:itoa(num2, num2Str, 10),
						getOpStr(op2),
						blankN==3?lpRack:itoa(num3, num3Str, 10),
						blankN==0?lpRack:itoa(result, resStr, 10)
					);
				}
			} else {
				_snprintf(buff, len, "%s %s %s %s %s = %s", 
					blankN==1?lpRack:itoa(num1, num1Str, 10),
					getOpStr(op),
					blankN==2?lpRack:itoa(num2, num2Str, 10),
					getOpStr(op2),
					blankN==3?lpRack:itoa(num3, num3Str, 10),
					blankN==0?lpRack:itoa(result, resStr, 10)
				);
			}
		} else {
			_snprintf(buff, len, "%s %s %s = %s", 
				blankN==1?lpRack:itoa(num1, num1Str, 10),
				getOpStr(op),
				blankN==2?lpRack:itoa(num2, num2Str, 10),
				blankN==0?lpRack:itoa(result, resStr, 10)
			);
		}
	} else {
		_snprintf(buff, len, "%s %s %s = %s余%s", 
				blankN==1?lpRack:itoa(num1, num1Str, 10),
				getOpStr(op),
				blankN==2?lpRack:itoa(num2, num2Str, 10),
				blankN==0?lpRack:itoa(result, resStr, 10),
				blankN==0?lpRack:itoa(reminder, num3Str, 10)
		);
	}
}
