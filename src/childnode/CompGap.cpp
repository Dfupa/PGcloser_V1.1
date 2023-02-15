#include <CompGap.h>

using namespace std;

void splitStr(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
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

void Bwt_Run(char* workpath, char* falist, char* btpath){
	if(access(falist, F_OK) !=0){
		cout << "The falist file is not found!" << endl;
	}
	else{
		ifstream if_falist;
		vector<string> vec_falist;
		string str_line;
		if_falist.open(falist, ios::in);
		while(!if_falist.eof()){
			getline(if_falist, str_line);
			if((int)str_line.length() <2){
				continue;
			}
			vec_falist.push_back(str_line);
		}
		if_falist.close();
		vector<string> vec_faname;
		
		for(int j =0; j < (int)vec_falist.size(); j++){
			vector<string> vec_tmp;
			string name_tmp;
			splitStr(vec_falist[j], vec_tmp, "/");
			name_tmp = vec_tmp[vec_tmp.size()-1];
			vec_faname.push_back(name_tmp.substr(0, name_tmp.length()-3));
		}
		
		for(int i = 0; i < (int)vec_faname.size(); i++){
			char gapfile_chk[CMD_NUM];
			sprintf(gapfile_chk, "%s/2_chr_gaps/%s/%s_gap_1.fa", workpath, vec_faname[i].c_str(), vec_faname[i].c_str());
			if(access(gapfile_chk, F_OK) !=0){
				cout << "The falist file "<<  vec_faname[i] <<" is not found!" << endl;
			}
			else{
				char bowtie_cmd[CMD_NUM] = "$(pwd)/submodules/bowtie2/bowtie2";
				if(access(bowtie_cmd, F_OK) != 0){
					cout << "bowtie2 is not found" << endl;
					break;
				}
				char gappath[CMD_NUM];
				char bowtie_run[CMD_NUM];
				sprintf(gappath, "%s/2_chr_gaps/%s/%s", workpath, vec_faname[i].c_str(), vec_faname[i].c_str());
				sprintf(bowtie_run, "%s -p 2 -f -x %s/Ref_Genome_Bwt -1 %s_gap_1.fa -2 %s_gap_2.fa -S %s/3_refdir/%s.sam", bowtie_cmd, btpath, gappath, gappath, workpath, vec_faname[i].c_str());
				system(bowtie_run);
				cout << "//Bowtie_run command No." << i << "-----" <<endl;
			}		
		}
	}
}

int CompGap(int argc,char *argv[]){
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);
	char ShellCommand[CMD_NUM];
    char PathWork[CMD_NUM];
	char PathFalist[CMD_NUM];
	char BtPath[CMD_NUM];
	
    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
			snprintf(PathFalist, sizeof(PathFalist), "%s/1_falist/falist", PathWork);
			snprintf(BtPath, sizeof(BtPath), "%s/3_refdir", PathWork);
		}
		if (cmd == "-falist")
        {
            snprintf(PathFalist, sizeof(PathFalist), "%s", argv[i + 1]);
            if (PathFalist[strlen(PathFalist) - 1] == '/') PathFalist[strlen(PathFalist) - 1] = '\0';
        }
		if (cmd == "-bt")
        {
            snprintf(BtPath, sizeof(BtPath), "%s", argv[i + 1]);
            if (BtPath[strlen(BtPath) - 1] == '/') BtPath[strlen(BtPath) - 1] = '\0';
        }
    }

	snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/3_refdir", PathWork);
	system(ShellCommand);
	
	Bwt_Run(PathWork, PathFalist, BtPath);

    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}