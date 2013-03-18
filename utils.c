#include "utils.h"
#include <stdio.h>


int indexof(char *aword, char *arrayofwords[]){
  int i; unsigned int ct=10;
//unsigned int ct=COUNT_OF(arrayofwords); //DOES NOT COUNT!!! TODO!
  for (i=0 ; i<ct ;i++){
    if (strcmp(aword,arrayofwords[i])==0){return i;}}
  return -1;//not found
}
 
