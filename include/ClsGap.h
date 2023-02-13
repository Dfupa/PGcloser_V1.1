#ifndef PGCLOSER_V1_1_CLSGAP_H
#define PGCLOSER_V1_1_CLSGAP_H

#include "main.h"
#define CMD_NUM 2048

void spString(const std::string& s, std::vector<std::string>& v, const std::string& c);
void sam_assort_tobed(char* samfile, char* distfile, char* bedfile_name, char* bedglc_name, char* Err_rate);
void Bed_Run(char* workpath, char* fapath, char* bedpath, char* sub_chrname);
std::vector<std::string> getFiles(std::string& cate_dir);
std::string Add_Str(std::string& targseq, int dist);
std::string Tran_Str(std::string& targseq);
void Extr_Seq(char *filename, std::string& pRead);
void Fill_Gap(char* workpath, char* sub_chrname, char* outfile, char* bedglc_name, char* Line_len);
int ClsGap(int argc, char *argv[]);

#endif 