#ifndef PGCLOSER_V1_1_EXTRGAP_H
#define PGCLOSER_V1_1_EXTRGAP_H

#include "main.h"
#include <string.h>
#define CMD_NUM 2048

void Subfasta_Out(char* headline, char* context_char, int line_len, char* filename);
void Chr_Gap(char *filename, std::string& pRead);
void Gap_Split(char* workpath, char* falist, char* sub_chrpath, std::string& scaff_seq, int gap_len);
int ExtrGap(int argc,char *argv[]);

#endif 