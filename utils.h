#ifndef UTILS_H
#define UTILS_H

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

int indexof(char *aword, char *arrayofwords[]);


#endif
