#include <BwtBuilt.h>

using namespace std;

int BwtBuilt(int argc,char *argv[]){
	long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);
	
	char bowtie_cmd[CMD_NUM] = "$(pwd)/submodules/bowtie2/bowtie2-build";
	char bwt_prename[CMD_NUM] = "Ref_Genome_Bwt";
	char PathWork[CMD_NUM];
	char ShellCommand[CMD_NUM];
	char bowtie_filechk[CMD_NUM];
	char RefGenome[CMD_NUM];
	
	for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w"){
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-ref"){
            snprintf(RefGenome, sizeof(RefGenome), "%s", argv[i + 1]);
            if (RefGenome[strlen(RefGenome) - 1] == '/') RefGenome[strlen(RefGenome) - 1] = '\0';
        }
    }
	
	sprintf(bowtie_filechk, "%s/3_refdir/Ref_Genome_Bwt.fa", PathWork);
    if (access(bowtie_filechk, F_OK) != 0) {  
		snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/3_refdir", PathWork);
		system(ShellCommand);
		
		//此处需要添加fa文件的判断
		snprintf(ShellCommand, sizeof(ShellCommand), "\\cp -rf %s %s/3_refdir/Ref_Genome_Bwt.fa", RefGenome, PathWork);
		system(ShellCommand);
		
		snprintf(ShellCommand, sizeof(ShellCommand), "%s %s/3_refdir/Ref_Genome_Bwt.fa %s", bowtie_cmd, PathWork, bwt_prename);
		system(ShellCommand);
		
		snprintf(ShellCommand, sizeof(ShellCommand), "mv ./%s.* %s/3_refdir/", bwt_prename, PathWork);
		system(ShellCommand);
	}
	
	cout << "----- Bwt Build is complated!" << endl;
	
	long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}