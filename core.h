

#ifndef CORE_H_
#define CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

void core_init_operator(int isAdd, int isMinus, int isMultiple, int isDevide);
void core_init_maxnum(unsigned int maxNumOfAdd, unsigned int maxNumOfMinus, unsigned int maxNumOfMultiple, unsigned int maxNumOfDevide);
void core_init_misc(int numOfNumber, int _isAddShift, int _isAbdicate, int _hasReminder);
void genAnFormula(char *buff, int len, int blankN);
void getFileName(char *szFileName, int len);
void getTitle(char *szTitle, int len);

#ifdef __cplusplus
}
#endif
#endif