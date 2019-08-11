#ifndef _TOOLS_H
#define _TOOLS_H

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a > _b ? _a : _b; })


 #define min(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a < _b ? _a : _b; })



double * copyFloatArray(double p[], unsigned int size);
void initializeFloatArray(double p[], unsigned int size, double value);
void initializeIntArray(int p[], unsigned int size, int value);
void initializeList(void * p[], unsigned int n);

unsigned int getNumberOfDigit(int num);


double * getCumulativeProba(double * proba, unsigned int size);
void updateCumulativeProba(double * cumulProba,
						double * proba, unsigned int size, unsigned int index);
int getIndexCumulativeProba(double * cumulProba, unsigned int size, double valueToFind);


unsigned int gcd(unsigned int x, unsigned int y);
int gcd_array(unsigned int * input, unsigned int size);
int lcm(unsigned int x, unsigned int y);
int lcm_array(unsigned int * input, unsigned int size);

#endif 
