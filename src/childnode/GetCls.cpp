#include <GetCls.h>

#define CMD_NUM 2048

using namespace std;

void wait_bjobs(char* cmd){
	char ShellCommand[CMD_NUM];  
	int lastTime = 0;  
	while (1) {  
		int now = clock()/CLOCKS_PER_SEC; 
		
		if (now - lastTime > 0 ) {
			
			char resultm[CMD_NUM] = "";
			snprintf(ShellCommand, sizeof(ShellCommand), "%s", cmd);
			FILE *pf = popen(ShellCommand, "r");
			fread(resultm, sizeof(resultm), 1, pf);
			pclose(pf);	
			if(*resultm == '\0' || resultm == '\0'){
				break;
			}
			lastTime = now;  
		}
	}
}

void splStr(const std::string& s, std::vector<std::string>& v, const std::string& c){
	  std::string::size_type pos1, pos2;
	    pos2 = s.find(c);
		  pos1 = 0;
		    while(std::string::npos != pos2)
			    {
					    v.push_back(s.substr(pos1, pos2-pos1));
						 
						    pos1 = pos2 + c.size();
							    pos2 = s.find(c, pos1);
								  }
			  if(pos1 != s.length())
				    v.push_back(s.substr(pos1));
}

std::vector<std::string> getFs(std::string& cate_dir){
	vector<string> filelist;
    DIR *dir;  
    struct dirent *ptr;  
    if ((dir=opendir(cate_dir.c_str())) == NULL){  
        perror("Open dir error...");  
        exit(1);  
    }  
    while ((ptr=readdir(dir)) != NULL){  
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0){
			continue;  
		}
        else if(ptr->d_type == 8){     
            filelist.push_back(ptr->d_name); 
		}
        else if(ptr->d_type == 10){  
            continue;  
		}
        else if(ptr->d_type == 4){  
            filelist.push_back(ptr->d_name);  
        }  
    }  
    closedir(dir);  
    sort(filelist.begin(), filelist.end()); 	
	return filelist;
}

int GetCls(int argc,char *argv[]){
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);

    char PathWork[CMD_NUM];
    //char PathFalist[CMD_NUM];
	char PathFa[CMD_NUM];
	char CpuNum[CMD_NUM] = "1";
	char GapLen[CMD_NUM] = "10";
	char LineLen[CMD_NUM] = "60";
	char Err_rate[CMD_NUM] = "2";
	char Queue[CMD_NUM] = "normal";
	char Span[CMD_NUM] = "20";
	char Cluster[CMD_NUM] = "lsf";
	char Command[CMD_NUM];
	char RefGenome[CMD_NUM];
	
    for (int i = 0; i < argc; i++){
        std::string cmd = argv[i];
        if (cmd == "-w"){
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
		if (cmd == "-fa"){
            snprintf(PathFa, sizeof(PathFa), "%s", argv[i + 1]);
            if (PathFa[strlen(PathFa) - 1] == '/') PathFa[strlen(PathFa) - 1] = '\0';
        }
		if (cmd == "-cpu"){
            snprintf(CpuNum, sizeof(CpuNum), "%s", argv[i + 1]);
            if (CpuNum[strlen(CpuNum) - 1] == '/') CpuNum[strlen(CpuNum) - 1] = '\0';
        }
		if (cmd == "-gl")sprintf(GapLen, "%s", argv[i+1]);
		if (cmd == "-len")sprintf(LineLen, "%s", argv[i+1]);
		if (cmd == "-err")sprintf(Err_rate, "%s", argv[i + 1]);
		if (cmd == "-ref"){
            snprintf(RefGenome, sizeof(RefGenome), "%s", argv[i + 1]);
            if (RefGenome[strlen(RefGenome) - 1] == '/') RefGenome[strlen(RefGenome) - 1] = '\0';
        }	
		if (cmd == "-q") sprintf(Queue, "%s", argv[i + 1]);
        if (cmd == "-span") sprintf(Span, "%s", argv[i + 1]);
        if (cmd == "-cls") sprintf(Cluster, "%s", argv[i+1]);
    }
	
	
	snprintf(Command, sizeof(Command), "mkdir -p ./sub_script");
    system(Command);
	snprintf(Command, sizeof(Command), "mkdir -p ./log");
    system(Command);
	
	//--------------------------SplitFa---------------------------------------------------
	FILE *fp_sh;
    snprintf(Command, sizeof(Command), "./sub_script/SplitFa.sh");
    if ((fp_sh = fopen(Command, "w")) == NULL)
        exit(-1);
    if(strcmp(Cluster,"lsf") == 0){
    	snprintf(Command, sizeof(Command), "#BSUB -q %s\n", Queue);
    	fputs(Command, fp_sh);
    	snprintf(Command, sizeof(Command), "#BSUB -J SplitFa\n");
    	fputs(Command, fp_sh);
    	snprintf(Command, sizeof(Command), "#BSUB -o ./log/SplitFa.out\n");
    	fputs(Command, fp_sh);
    	snprintf(Command, sizeof(Command), "#BSUB -e ./log/SplitFa.err\n");
    	fputs(Command, fp_sh);
    	snprintf(Command, sizeof(Command), "#BSUB -n 1\n");
    	fputs(Command, fp_sh);
		snprintf(Command, sizeof(Command), "#BSUB -R \"span[ptile=%s]\"\n", Span);
		fputs(Command, fp_sh);
		snprintf(Command, sizeof(Command), "./bin/./main SplitFa -w %s -fa %s -cpu %s\n", PathWork, PathFa, CpuNum);
    	fputs(Command, fp_sh);
		fclose(fp_sh);
		snprintf(Command, sizeof(Command), "bsub < ./sub_script/SplitFa.sh");
		system(Command);
	}
	else if(strcmp(Cluster,"local") == 0){
		snprintf(Command, sizeof(Command), "./bin/./main SplitFa -w %s -fa %s -cpu 1\n", PathWork, PathFa);
    	fputs(Command, fp_sh);
		fclose(fp_sh);
		snprintf(Command, sizeof(Command), "sh ./sub_script/SplitFa.sh");
		system(Command);
	}
	else{
		fclose(fp_sh);
		std::cout << "Error: Error Parameters." << std::endl;
		exit(-1);
	}
	
	char cmdchk[40] = "bjobs | grep SplitFa";
	wait_bjobs(cmdchk);
	
	
	//-------------------------------ExtrGap-----------------------------------------------
	
	vector<string> falist_name;
	
	char subfalist_name[CMD_NUM*20];
	snprintf(Command, sizeof(Command), "ls %s/1_falist/subfalist* | tr '\n' '\t'", PathWork); 
	FILE *pfn = popen(Command, "r");
	fread(subfalist_name, sizeof(subfalist_name), 1, pfn);
	pclose(pfn);

	string falist_line = subfalist_name;
	if((int)falist_line.length() < 2 || falist_line.find("No such file or directory") < falist_line.length()){
		string tmp_name = "falist";
		falist_name.push_back(tmp_name);
	}
	else{
		splStr(falist_line, falist_name, "\t");
		for(int j=0; j<(int)falist_name.size(); j++){
			vector<string> name_tmp;
			splStr(falist_name[j], name_tmp, "/");
			falist_name[j] = name_tmp[(int)name_tmp.size()-1];
		}
	}
	
	for(int i = 0; i < (int)falist_name.size(); i++){
		FILE *fp_sh1;
		snprintf(Command, sizeof(Command), "./sub_script/ExtrGap_%s.sh", falist_name[i].c_str());
		if ((fp_sh1 = fopen(Command, "w")) == NULL)
			exit(-1);
		if(strcmp(Cluster,"lsf") == 0){
			snprintf(Command, sizeof(Command), "#BSUB -q %s\n", Queue);
			fputs(Command, fp_sh1);
			snprintf(Command, sizeof(Command), "#BSUB -J ExtrGap%d\n", i);
			fputs(Command, fp_sh1);
			snprintf(Command, sizeof(Command), "#BSUB -o ./log/ExtrGap_%s.out\n", falist_name[i].c_str());
			fputs(Command, fp_sh1);
			snprintf(Command, sizeof(Command), "#BSUB -e ./log/ExtrGap_%s.err\n", falist_name[i].c_str());
			fputs(Command, fp_sh1);
			snprintf(Command, sizeof(Command), "#BSUB -n 1\n");
			fputs(Command, fp_sh1);
			snprintf(Command, sizeof(Command), "#BSUB -R \"span[ptile=%s]\"\n", Span);
			fputs(Command, fp_sh1);
			snprintf(Command, sizeof(Command), "./bin/./main ExtrGap -w %s -falist %s/1_falist/%s -gl %s\n", PathWork, PathWork, falist_name[i].c_str(), GapLen);
			fputs(Command, fp_sh1);
			fclose(fp_sh1);
			snprintf(Command, sizeof(Command), "bsub < ./sub_script/ExtrGap_%s.sh", falist_name[i].c_str());
			system(Command);
		}
		else if(strcmp(Cluster,"local") == 0){
			snprintf(Command, sizeof(Command), "./bin/./main ExtrGap -w %s -falist %s/1_falist/%s -gl %s\n", PathWork, PathWork, falist_name[i].c_str(), GapLen);
			fputs(Command, fp_sh1);
			fclose(fp_sh1);
			snprintf(Command, sizeof(Command), "sh ./sub_script/ExtrGap_%s.sh", falist_name[i].c_str());
			system(Command);
		}
		else{
			fclose(fp_sh1);
			std::cout << "Error: Error Parameters." << std::endl;
			exit(-1);
		}
	}
	
	char cmdchk2[40] = "bjobs | grep ExtrGap";
	wait_bjobs(cmdchk2);
	
	
	//-----------------------------------BwtBuilt-----------------------------------------
	
	FILE *fp_sh2;
    snprintf(Command, sizeof(Command), "./sub_script/bwtbuilt.sh");
    if ((fp_sh2 = fopen(Command, "w")) == NULL)
        exit(-1);
    if(strcmp(Cluster,"lsf") == 0){
    	snprintf(Command, sizeof(Command), "#BSUB -q %s\n", Queue);
    	fputs(Command, fp_sh2);
    	snprintf(Command, sizeof(Command), "#BSUB -J BwtBuilt\n");
    	fputs(Command, fp_sh2);
    	snprintf(Command, sizeof(Command), "#BSUB -o ./log/BwtBuilt.out\n");
    	fputs(Command, fp_sh2);
    	snprintf(Command, sizeof(Command), "#BSUB -e ./log/BwtBuilt.err\n");
    	fputs(Command, fp_sh2);
    	snprintf(Command, sizeof(Command), "#BSUB -n 1\n");
    	fputs(Command, fp_sh2);
		snprintf(Command, sizeof(Command), "#BSUB -R \"span[ptile=%s]\"\n", Span);
		fputs(Command, fp_sh2);
		snprintf(Command, sizeof(Command), "./bin/./main BwtBuilt -w %s -ref %s \n", PathWork, RefGenome);
    	fputs(Command, fp_sh2);
		fclose(fp_sh2);
		snprintf(Command, sizeof(Command), "bsub < ./sub_script/bwtbuilt.sh");
		system(Command);
	}
	else if(strcmp(Cluster,"local") == 0){
		snprintf(Command, sizeof(Command), "./bin/./main BwtBuilt -w %s -ref %s \n", PathWork, RefGenome);
    	fputs(Command, fp_sh2);
		fclose(fp_sh2);
		snprintf(Command, sizeof(Command), "sh ./sub_script/bwtbuilt.sh");
		system(Command);
	}
	else{
		fclose(fp_sh2);
		std::cout << "Error: Error Parameters." << std::endl;
		exit(-1);
	}
	char cmdchk3[40] = "bjobs | grep BwtBuilt";
	wait_bjobs(cmdchk3);
	
	//-------------------------------------CompGap----------------------------------------
	char gopfile_name[CMD_NUM*20];
	snprintf(Command, sizeof(Command), "ls %s/1_falist/*_gap | tr '\n' '\t'", PathWork); 
	FILE *pfn21 = popen(Command, "r");
	fread(gopfile_name, sizeof(gopfile_name), 1, pfn21);
	pclose(pfn21);
	vector<string> gapfalist;
	string gapfalist_line = gopfile_name;
	splStr(gapfalist_line, gapfalist, "\t");
	for(int j=0; j<(int)gapfalist.size(); j++){
		vector<string> name_tmp;
		splStr(gapfalist[j], name_tmp, "/");
		gapfalist[j] = name_tmp[(int)name_tmp.size()-1];
	}
	for(int i = 0; i < (int)gapfalist.size(); i++){
		if((int)gapfalist[i].length() < 4){
			continue;
		}
		FILE *fp_sh3;
		snprintf(Command, sizeof(Command), "./sub_script/CompGap_%s.sh", gapfalist[i].c_str());
		if ((fp_sh3 = fopen(Command, "w")) == NULL)
			exit(-1);
		if(strcmp(Cluster,"lsf") == 0){
			snprintf(Command, sizeof(Command), "#BSUB -q %s\n", Queue);
			fputs(Command, fp_sh3);
			snprintf(Command, sizeof(Command), "#BSUB -J CompGap%d\n", i);
			fputs(Command, fp_sh3);
			snprintf(Command, sizeof(Command), "#BSUB -o ./log/CompGap_%s.out\n", gapfalist[i].c_str());
			fputs(Command, fp_sh3);
			snprintf(Command, sizeof(Command), "#BSUB -e ./log/CompGap_%s.err\n", gapfalist[i].c_str());
			fputs(Command, fp_sh3);
			snprintf(Command, sizeof(Command), "#BSUB -n 1\n");
			fputs(Command, fp_sh3);
			snprintf(Command, sizeof(Command), "#BSUB -R \"span[ptile=%s]\"\n", Span);
			fputs(Command, fp_sh3);
			snprintf(Command, sizeof(Command), "./bin/./main CompGap -w %s -falist %s/1_falist/%s\n", PathWork, PathWork, gapfalist[i].c_str());
			fputs(Command, fp_sh3);
			fclose(fp_sh3);
			snprintf(Command, sizeof(Command), "bsub < ./sub_script/CompGap_%s.sh", gapfalist[i].c_str());
			system(Command);
		}
		else if(strcmp(Cluster,"local") == 0){
			snprintf(Command, sizeof(Command), "./bin/./main CompGap -w %s -falist %s/1_falist/%s\n", PathWork, PathWork, gapfalist[i].c_str());
			fputs(Command, fp_sh3);
			fclose(fp_sh3);
			snprintf(Command, sizeof(Command), "sh ./sub_script/CompGap_%s.sh", gapfalist[i].c_str());
			system(Command);
		}
		else{
			fclose(fp_sh3);
			std::cout << "Error: Error Parameters." << std::endl;
			exit(-1);
		}
	}
	char cmdchk4[40] = "bjobs | grep CompGap";
	wait_bjobs(cmdchk4);
	
	//---------------------------------------ClsGap-----------------------------------------
	for(int i = 0; i < (int)gapfalist.size(); i++){
		if((int)gapfalist[i].length() < 4){
			continue;
		}
		FILE *fp_sh4;
		snprintf(Command, sizeof(Command), "./sub_script/ClsGap_%s.sh", gapfalist[i].c_str());
		if ((fp_sh4 = fopen(Command, "w")) == NULL)
			exit(-1);
		if(strcmp(Cluster,"lsf") == 0){
			snprintf(Command, sizeof(Command), "#BSUB -q %s\n", Queue);
			fputs(Command, fp_sh4);
			snprintf(Command, sizeof(Command), "#BSUB -J ClsGap%d\n", i);
			fputs(Command, fp_sh4);
			snprintf(Command, sizeof(Command), "#BSUB -o ./log/ClsGap_%s.out\n", gapfalist[i].c_str());
			fputs(Command, fp_sh4);
			snprintf(Command, sizeof(Command), "#BSUB -e ./log/ClsGap_%s.err\n", gapfalist[i].c_str());
			fputs(Command, fp_sh4);
			snprintf(Command, sizeof(Command), "#BSUB -n 1\n");
			fputs(Command, fp_sh4);
			snprintf(Command, sizeof(Command), "#BSUB -R \"span[ptile=%s]\"\n", Span);
			fputs(Command, fp_sh4);
			snprintf(Command, sizeof(Command), "./bin/./main ClsGap -w %s -falist %s/1_falist/%s -len %s -err %s\n", PathWork, PathWork, gapfalist[i].c_str(), LineLen, Err_rate);
			fputs(Command, fp_sh4);
			fclose(fp_sh4);
			snprintf(Command, sizeof(Command), "bsub < ./sub_script/ClsGap_%s.sh", gapfalist[i].c_str());
			system(Command);
		}
		else if(strcmp(Cluster,"local") == 0){
			snprintf(Command, sizeof(Command), "./bin/./main ClsGap -w %s -falist %s/1_falist/%s -len %s -err %s\n", PathWork, PathWork, gapfalist[i].c_str(), LineLen, Err_rate);
			fputs(Command, fp_sh4);
			fclose(fp_sh4);
			snprintf(Command, sizeof(Command), "sh ./sub_script/ClsGap_%s.sh", gapfalist[i].c_str());
			system(Command);
		}
		else{
			fclose(fp_sh4);
			std::cout << "Error: Error Parameters." << std::endl;
			exit(-1);
		}
	}
	char cmdchk5[40] = "bjobs | grep ClsGap";
	wait_bjobs(cmdchk5);
	
	//-------------------------------------MergFa--------------------------------------------
	FILE *fp_sh5;
    snprintf(Command, sizeof(Command), "./sub_script/MergFa.sh");
    if ((fp_sh5 = fopen(Command, "w")) == NULL)
        exit(-1);
    if(strcmp(Cluster,"lsf") == 0){
    	snprintf(Command, sizeof(Command), "#BSUB -q %s\n", Queue);
    	fputs(Command, fp_sh5);
    	snprintf(Command, sizeof(Command), "#BSUB -J MergFa\n");
    	fputs(Command, fp_sh5);
    	snprintf(Command, sizeof(Command), "#BSUB -o ./log/MergFa.out\n");
    	fputs(Command, fp_sh5);
    	snprintf(Command, sizeof(Command), "#BSUB -e ./log/MergFa.err\n");
    	fputs(Command, fp_sh5);
    	snprintf(Command, sizeof(Command), "#BSUB -n 1\n");
    	fputs(Command, fp_sh5);
		snprintf(Command, sizeof(Command), "#BSUB -R \"span[ptile=%s]\"\n", Span);
		fputs(Command, fp_sh5);
		snprintf(Command, sizeof(Command), "./bin/./main MergFa -w %s -cfa %s/4_cfa -falist %s/1_falist/falist\n", PathWork, PathWork, PathWork);
    	fputs(Command, fp_sh5);
		fclose(fp_sh5);
		snprintf(Command, sizeof(Command), "bsub < ./sub_script/MergFa.sh");
		system(Command);
	}
	else if(strcmp(Cluster,"local") == 0){
		snprintf(Command, sizeof(Command), "./bin/./main MergFa -w %s -cfa %s/4_cfa -falist %s/1_falist/falist\n", PathWork, PathWork, PathWork);
    	fputs(Command, fp_sh5);
		fclose(fp_sh5);
		snprintf(Command, sizeof(Command), "sh ./sub_script/MergFa.sh");
		system(Command);
	}
	else{
		fclose(fp_sh5);
		std::cout << "Error: Error Parameters." << std::endl;
		exit(-1);
	}

    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}