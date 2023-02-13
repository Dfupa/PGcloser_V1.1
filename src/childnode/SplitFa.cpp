#include <SplitFa.h>
#include <cctype>

using namespace std;

const char* itochr(int val){  
    static char result[sizeof(int)<<3+2];  
    unsigned int tempval=val;  
    if(val<0) tempval=-val;  
    int i=sizeof(int)<<3+1;  
    do{  
        result[i]="0123456789"[tempval%10];  
        tempval/=10;--i;  
    }while(tempval);  
    if(val<0) result[i--]='-';  
    return &result[i+1];  
}

//将输入的fa文件进行chromosome切分
void Fa_Chr(char *workpath,char *fa_path, char *cpu_num){
    ifstream fp;
    fp.open(fa_path,ios::in);
    string Line;
    char FileName[CMD_NUM];
	char FilePath[CMD_NUM];
    char ShellCommand[CMD_NUM];
	vector<string> falist;
    //Create the FA directory.
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/1_fa", workpath);
    system(ShellCommand);
    getline(fp,Line);
    //Extract the sequence of each chromosome
    while (!fp.eof())
    {
        strncpy(FileName, &Line[1] ,sizeof(FileName) - 4);
        for (int i = 0; i < (int)strlen(FileName); i++)
        {
            if(FileName[i] == ' ' || FileName[i] == '\t' || FileName[i] == '\n')
            {
                FileName[i] = '\0';
                break;
            }
        }
        strncat(FileName, ".fa", sizeof(FileName) - strlen(FileName));
		sprintf(FilePath, "%s/1_fa/%s", workpath, FileName);		
        snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", FilePath);
		falist.push_back(FilePath);
        system(ShellCommand);
				
        ofstream fp_tmp;
        fp_tmp.open(FilePath, ios::out);
        fp_tmp<<Line<<std::endl;
        getline(fp,Line);
        while (!fp.eof() && Line.at(0) != '>')
        {
            fp_tmp<<Line<<std::endl;
            getline(fp,Line);
        }
        fp_tmp.close();
		
    }
    fp.close();
	
	snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/1_falist", workpath);
	system(ShellCommand);
	
	
	char falistname[CMD_NUM];
	sprintf(falistname, "%s/1_falist/falist", workpath);
	snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", falistname);
    system(ShellCommand);
	
	ofstream falist_out;
	falist_out.open(falistname, ios::out);
	for(int i = 0; i < (int)falist.size(); i++){
		falist_out << falist[i] << endl;
	}
	falist_out.close();
	
	int cpu_pan = atoi(cpu_num);
	if(cpu_pan > 1){
		float falist_num = falist.size();
		float filenum = cpu_pan;
		
		int line_count = ceil(falist_num / filenum);
		snprintf(ShellCommand, sizeof(ShellCommand), "split -l %s %s/1_falist/falist %s/subfalist -d", itochr(line_count), workpath,workpath);
		
		system(ShellCommand);
		
		snprintf(ShellCommand, sizeof(ShellCommand), "mv %s/subfalist* %s/1_falist/", workpath, workpath);
		system(ShellCommand);
	}
}

int SplitFa(int argc,char *argv[])
{
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);
    char PathWork[CMD_NUM];
    char PathFA[CMD_NUM];
	char Cpu_num[CMD_NUM] = "1";

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w"){
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-fa"){
            snprintf(PathFA, sizeof(PathFA), "%s", argv[i + 1]);
            if (PathFA[strlen(PathFA) - 1] == '/') PathFA[strlen(PathFA) - 1] = '\0';
        }
		if (cmd == "-cpu"){
			snprintf(Cpu_num, sizeof(Cpu_num), "%s", argv[i + 1]);
            if (Cpu_num[strlen(Cpu_num) - 1] == '/') Cpu_num[strlen(Cpu_num) - 1] = '\0';
		}
    }
    //Cut the FA file.
    Fa_Chr(PathWork, PathFA, Cpu_num);

    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}