#ifndef PGCLOSER_V1_1_GETCLS_H
#define PGCLOSER_V1_1_GETCLS_H

#include "main.h"

using namespace std;

void wait_bjobs(char* cmd);
void splStr(const std::string& s, std::vector<std::string>& v, const std::string& c);
int GetCls(int argc,char *argv[]);

#endif