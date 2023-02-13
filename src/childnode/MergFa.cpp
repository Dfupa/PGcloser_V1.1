#include <MergFa.h>

using namespace std;

void spStr(const std::string& s, std::vector<std::string>& v, const std::string& c){
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

int MergFa(int argc, char *argv[]){
	long StartTime = time((time_t*)NULL);
	printf("start time = %ld\n", StartTime);
	char ShellCommand[CMD_NUM];
	char PathWork[CMD_NUM];
	char FaList[CMD_NUM];
	char CfaPath[CMD_NUM];
	
	for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
			snprintf(FaList, sizeof(FaList), "%s/1_falist/falist", PathWork);
			snprintf(CfaPath, sizeof(CfaPath), "%s/4_cfa", PathWork);
        }
        if (cmd == "-falist")
        {
            snprintf(FaList, sizeof(FaList), "%s", argv[i + 1]);
            if (FaList[strlen(FaList) - 1] == '/') FaList[strlen(FaList) - 1] = '\0';
        }
		if (cmd == "-cfa")
        {
            snprintf(CfaPath, sizeof(CfaPath), "%s", argv[i + 1]);
            if (CfaPath[strlen(CfaPath) - 1] == '/') CfaPath[strlen(CfaPath) - 1] = '\0';
        }
    }
	
	snprintf(ShellCommand, sizeof(ShellCommand), "touch %s/GapClosed.fa", PathWork);
	system(ShellCommand);
	
	
	
	char gopfile_name[CMD_NUM*20];
	snprintf(ShellCommand, sizeof(ShellCommand), "ls %s/1_falist/*_gap | tr '\n' '\t'", PathWork); 
	FILE *pfn = popen(ShellCommand, "r");
	fread(gopfile_name, sizeof(gopfile_name), 1, pfn);
	pclose(pfn);
	vector<string> gapfafile;
	string gapfalist_line = gopfile_name;
	spStr(gapfalist_line, gapfafile, "\t");

	//如果不存在_gap文件，则按照falist的顺序进行整合
	if((int)gapfafile.size() == 0){
		ifstream if_falist;
		if_falist.open(FaList, ios::in);
		vector<string> falist_name;
		string onefaname;
		while(!if_falist.eof()){
			vector<string> name_tmp;
			getline(if_falist, onefaname);
			if((int)onefaname.length() < 2){
				continue;
			}
			spStr(onefaname, name_tmp, "/");
			falist_name.push_back(name_tmp[(int)name_tmp.size()-1]);
		}
		if_falist.close();
		
		for(int i=0; i< (int)falist_name.size(); i++){
			snprintf(ShellCommand, sizeof(ShellCommand), "cat %s/%s >> %s/GapClosed.fa", CfaPath, falist_name[i].c_str(), PathWork);
			system(ShellCommand);
		}
	}
	//如果存在_gap文件，则需要进行判断，没有包含gap的fa文件需要从1_fa目录读取，其他从Cpath目录读取
	else{
		vector<string> vec_gaplist;
		for(int j=0; j< (int)gapfafile.size(); j++){
			if((int)gapfafile[j].length() < 2){
				continue;
			}
			
			char gapfafile_size[20];
			snprintf(ShellCommand, sizeof(ShellCommand), "wc -c %s | awk '{print $1}'", gapfafile[j].c_str()); 
			FILE *pf1 = popen(ShellCommand, "r");
			fread(gapfafile_size, sizeof(gapfafile_size), 1, pf1);
			pclose(pf1);
			if(atoi(gapfafile_size)==0){
				continue;
			}
			
			ifstream if_gapfile;
			if_gapfile.open(gapfafile[j].c_str(), ios::in);
			string tmp_line;
			while(!if_gapfile.eof()){
				getline(if_gapfile,tmp_line);
				if((int)tmp_line.length() < 2){
					continue;
				}
				vec_gaplist.push_back(tmp_line);
			}
			if_gapfile.close();
		}
		
		char falist_name[CMD_NUM];
		snprintf(falist_name, sizeof(falist_name), "%s/1_falist/falist", PathWork);
		ifstream if_falist;
		if_falist.open(falist_name, ios::in);
		vector<string> vec_falist;
		string onefaname;
		while(!if_falist.eof()){
			getline(if_falist, onefaname);
			if((int)onefaname.length() < 2){
				continue;
			}
			vec_falist.push_back(onefaname);
		}
		if_falist.close();
		
		int gapfa_num =0;
		int nogapfa_num = 0;
		for(int k=0; k<(int)vec_falist.size(); k++){
			vector<string> name_tmp;
			spStr(vec_falist[k], name_tmp, "/");
			string fa_name = name_tmp[(int)name_tmp.size()-1];
			
			for(int m=0; m<(int)vec_gaplist.size(); m++){
				if((int)vec_gaplist[m].length() < 2){
					continue;
				}
				else{
					if(vec_gaplist[m] == vec_falist[k]){
						char cfaname[CMD_NUM];
						sprintf(cfaname, "%s/%s", CfaPath, fa_name.c_str());
						ifstream fp;
						fp.open(cfaname,ios::in);
						if(!fp){
							snprintf(ShellCommand, sizeof(ShellCommand), "cat %s/1_fa/%s >> %s/GapClosed.fa", PathWork, fa_name.c_str(), PathWork);
							system(ShellCommand);
							nogapfa_num++;
							fp.close();
							break;
						}
						else{
							snprintf(ShellCommand, sizeof(ShellCommand), "cat %s/%s >> %s/GapClosed.fa", CfaPath, fa_name.c_str(), PathWork);
							system(ShellCommand);
							snprintf(ShellCommand, sizeof(ShellCommand), "echo -e '\r' >> %s/GapClosed.fa", PathWork);
							system(ShellCommand);
							gapfa_num++;
							fp.close();
							break;
						}
					}
					else if(m==(int)vec_gaplist.size()-1 && vec_gaplist[m] != vec_falist[k]){
						snprintf(ShellCommand, sizeof(ShellCommand), "cat %s/1_fa/%s >> %s/GapClosed.fa", PathWork, fa_name.c_str(), PathWork);
						system(ShellCommand);
						nogapfa_num++;
					}
					else{
						continue;
					}
				}
			}
		}
	}
	
	
    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
	
}