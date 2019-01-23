

#ifndef CORE_H_
#define CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

void core_init_formula(void);
void core_init_operator(int isAdd, int isMinus, int isMultiple, int isDevide);
void core_init_maxnum(unsigned int maxNumOfAdd, unsigned int maxNumOfMinus, unsigned int maxNumOfMultiple, unsigned int maxNumOfDevide);
void core_init_misc(int _isThreeNumbers, int _isAddShift, int _isAddShiftOnly, int _isAbdicate, int _isAbdicateOnly, int _hasReminder, int _hasBracketPriority);
void genAnFormula(char *buff, int len, int blankN);
void getFileName(char *szFileName, int len);
void getTitle(char *szTitle, int len);

#ifdef __cplusplus
}
#endif
#endif