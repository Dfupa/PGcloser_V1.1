#ifndef PGCLOSER_V1_1_SPLITFA_H
#define PGCLOSER_V1_1_SPLITFA_H

#include "main.h"
#define CMD_NUM 2048

const char* itochr(int val);
void Fa_Chr(char *workpath,char *fa_path, char *cpu_num);
int SplitFa(int argc,char *argv[]);

#endif 