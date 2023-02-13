#ifndef PGCLOSER_V1_1_COMGAP_H
#define PGCLOSER_V1_1_COMGAP_H

#include "main.h"
#define CMD_NUM 2048

void splitStr(const std::string& s, std::vector<std::string>& v, const std::string& c);
//void Bwt_Build(char* workpath, char* ref_genome);
void Bwt_Run(char* workpath, char* falist, char* btpath);
int CompGap(int argc,char *argv[]);

#endif 