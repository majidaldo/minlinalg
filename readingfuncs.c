#include "readingfuncs.h"
#include <string.h>

int read000(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u %f",i1,v1);}
int read001(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u %lf",i1,v1);}
int read002(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u %f %f",i1,v1,v2);}
int read003(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u %d",i1,v1);}
int read004(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u ",i1);}
int read010(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%f",v1);}
int read011(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%lf",v1);}
int read012(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%f %f",v1,v2);}
int read013(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%d",v1);}
int read100(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u %u %f",i1,i2,v1);}
int read101(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u %u %lf",i1,i2,v1);}
int read102(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u %u %f %f",i1,i2,v1,v2);}
int read103(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u %u %d",i1,i2,v1);}
int read104(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%u %u ",i1,i2);}
int read110(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%f",v1);}
int read111(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%lf",v1);}
int read112(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%f %f",v1,v2);}
int read113(char *aline,void *i1,void *i2,void *v1,void *v2){
return sscanf(aline,"%d",v1);}
