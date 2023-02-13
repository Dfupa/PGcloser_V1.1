#include <ExtrGap.h>

using namespace std;

//for .fa file and _gap.fa output
void Subfasta_Out(char* headline, char* context_char, int line_len, char* filename){
	string context(context_char);
	ofstream ost_tmp;
	ost_tmp.open(filename, ios::out);
	ost_tmp << headline << endl;
	while(!context.empty()){
		string subseq = "";
		if((int)context.size() < line_len){
			ost_tmp << context << endl;
			break;
		}
		else{
			subseq = context.substr(0, line_len);
			context = context.substr(line_len, (int)context.size() - line_len);
			ost_tmp << subseq << endl;
		}
	}
	ost_tmp.close();
} 

void Chr_Gap(char *filename, string& pRead){
	char ShellCommand[CMD_NUM];
	char tmpfile_name[CMD_NUM];
	
	snprintf(ShellCommand, sizeof(ShellCommand), "cp %s %s_tmp", filename, filename); 
	system(ShellCommand);
	snprintf(ShellCommand, sizeof(ShellCommand), "sed -i '1d' %s_tmp", filename); 
	system(ShellCommand);

	snprintf(ShellCommand, sizeof(ShellCommand), "touch %s_tmp2", filename, filename);
	system(ShellCommand);
	snprintf(ShellCommand, sizeof(ShellCommand), "cat %s_tmp | tr -d '\n' > %s_tmp2", filename, filename); 
	system(ShellCommand);
	
	sprintf(tmpfile_name, "%s_tmp2", filename);
	ifstream is_tmp;
	is_tmp.open(tmpfile_name, ios::in);
	getline(is_tmp, pRead);
	is_tmp.close();
	
	snprintf(ShellCommand, sizeof(ShellCommand), "rm -rf %s_tmp", filename); 
	system(ShellCommand);
	snprintf(ShellCommand, sizeof(ShellCommand), "rm -rf %s_tmp2", filename); 
	system(ShellCommand);
}

//对输入的长string进行NNN提取切分，再根据两端截取长度和gap长度限制进行合并，并将结果输出到文件
void Gap_Split(char* workpath, char* falist, char* sub_chrpath, string& scaff_seq, int gap_len){
	int gap_pair_len = 200;
	char ShellCommand[CMD_NUM];
	int line_len = 60;
	vector<string> vec_substr;
	vector<string> vec_nstr;
	int firstN = 0;
	int firstnotN = 0;
	
	string substring = "";
	string nstring = "";
	
	while(!scaff_seq.empty()){
		firstN = scaff_seq.find_first_of("N");
		if( -1 == firstN){
			vec_substr.push_back(scaff_seq);
			break;
		}
		if(-1 != (int)scaff_seq.find_first_not_of("N", firstN)){
			firstnotN = scaff_seq.find_first_not_of("N", firstN);
		}
		else{
			firstnotN = (int)scaff_seq.size();
		}
		
		substring = scaff_seq.substr(0, firstN);
		nstring = scaff_seq.substr(firstN, firstnotN - firstN);
		scaff_seq = scaff_seq.substr(firstnotN, (int)scaff_seq.size() - firstN);
		
		if(0 == firstN){
			substring = nstring + substring;
			vec_substr.push_back(substring);
			continue;
		}
		
		if(0 != firstN){
			vec_substr.push_back(substring);
		}
		vec_nstr.push_back(nstring);
		
	}
	if(vec_substr[0].find("N") < vec_substr[0].size()){
		vec_substr[1] = vec_substr[0] + vec_substr[1];
		vec_substr.erase(vec_substr.begin());
	}
	if(vec_substr.size() == vec_nstr.size()){
		vec_substr[vec_substr.size()-1] = vec_substr[vec_substr.size()-1] + vec_nstr[vec_nstr.size()-1];
		vec_nstr.erase(vec_nstr.end());
	}
	for(int i =0 ; i <(int)vec_substr.size(); ++i){
		if(((int)vec_substr[i].length() < gap_pair_len)  && (i != (int)vec_substr.size()-1)){
				vec_substr[i+1] = vec_substr[i] + vec_nstr[i] + vec_substr[i+1];
				vec_substr.erase(vec_substr.begin() + i);
				vec_nstr.erase(vec_nstr.begin() + i);
				i=i-1;
			}
			else if(((int)vec_substr[i].length() < gap_pair_len)  && (i == (int)vec_substr.size()-1)){
				vec_substr[i-1] = vec_substr[i-1] + vec_nstr[i-1] + vec_substr[i];
				vec_substr.erase(vec_substr.begin() + i);
				vec_nstr.erase(vec_nstr.begin() + i - 1);
				i=i-1;
			}
			else{
				continue;
			}
	}
	for(int j = 0; j < (int)vec_nstr.size(); ++j){
		if((int)vec_nstr[j].size() < gap_len){
			vec_substr[j+1] = vec_substr[j] + vec_nstr[j] + vec_substr[j+1];
			vec_substr.erase(vec_substr.begin() + j);
			vec_nstr.erase(vec_nstr.begin() + j);
			j=j-1;
		}
		else{
			continue;
		}
	}
	if((int)vec_nstr.size() == 0){
		char nogap_file[CMD_NUM];
		sprintf(nogap_file, "%s_nogap",falist);
		ofstream nogap_out;
		nogap_out.open(nogap_file, ios::app);
		nogap_out << sub_chrpath << endl;
		nogap_out.close();
	}
	else{
		char gap_file[CMD_NUM];
		sprintf(gap_file, "%s_gap", falist);
		ofstream gap_out;
		gap_out.open(gap_file, ios::app);
		gap_out << sub_chrpath << endl;
		gap_out.close();
		
		
		ifstream fp;
		fp.open(sub_chrpath,ios::in);
		string chr_seq;
		getline(fp, chr_seq);
		fp.close();
		
		char filepath[CMD_NUM];
		strncpy(filepath, &chr_seq[1], sizeof(filepath) - 4);
		for(int i=0; i<(int)strlen(filepath); ++i){
			if(filepath[i] == ' ' || filepath[i] == '\t' || filepath[i] == '\n'){
				filepath[i] = '\0';
				break;
			}
		}
		
		snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/2_chr_gaps/%s", workpath, filepath);
		system(ShellCommand);
		snprintf(ShellCommand, sizeof(ShellCommand), "touch %s/2_chr_gaps/%s/%s_gap_1.fa", workpath, filepath, filepath);
		system(ShellCommand);
		snprintf(ShellCommand, sizeof(ShellCommand), "touch %s/2_chr_gaps/%s/%s_gap_2.fa", workpath, filepath, filepath);
		system(ShellCommand);
		snprintf(ShellCommand, sizeof(ShellCommand), "touch %s/2_chr_gaps/%s/%s_dist.txt", workpath, filepath, filepath);
		system(ShellCommand);
		
		char gap_1_file[CMD_NUM];
		sprintf(gap_1_file, "%s/2_chr_gaps/%s/%s_gap_1.fa", workpath, filepath, filepath);
		snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", gap_1_file);
		system(ShellCommand);
		char gap_2_file[CMD_NUM];
		sprintf(gap_2_file, "%s/2_chr_gaps/%s/%s_gap_2.fa", workpath, filepath, filepath);
		snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", gap_2_file);
		system(ShellCommand);
		char dist_file[CMD_NUM];
		sprintf(dist_file, "%s/2_chr_gaps/%s/%s_dist.txt", workpath, filepath, filepath);
		snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", dist_file);
		system(ShellCommand);
		
		vector<string> gap_headline;
		vector<string> gap_left;
		vector<string> gap_right;
		
		for(int j = 0 ; j < (int)vec_substr.size(); ++j){
			char subfilename[CMD_NUM];
			char filehead[CMD_NUM];
			char gap_filehead[CMD_NUM];
			string gapseq_left = "";
			string gapseq_right = "";
			
			sprintf(subfilename, "%s/2_chr_gaps/%s/%s_%d.fa", workpath, filepath, filepath, j+1);
			sprintf(filehead, ">%s_%d", filepath, j+1);
			if(j!=(int)vec_substr.size()-1){
				sprintf(gap_filehead, ">%s_%d_to_%s_%d", filepath, j+1, filepath,j+2);
				gap_headline.push_back(gap_filehead);
			}
			snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", subfilename);
			system(ShellCommand);
			
			//--------------------提取gap两端序列各200bp，分别存储left和right vector中--------------
			if(j==0){
				gapseq_left = vec_substr[j].substr(vec_substr[j].size()-gap_pair_len, vec_substr[j].size());
				gap_left.push_back(gapseq_left);
				
			}
			else if(j == (int)vec_substr.size()-1){
				gapseq_right = vec_substr[j].substr(0, gap_pair_len);
				gap_right.push_back(gapseq_right);
			}
			else{
				gapseq_right = vec_substr[j].substr(0, gap_pair_len);
				gap_right.push_back(gapseq_right);
				gapseq_left = vec_substr[j].substr(vec_substr[j].size()-gap_pair_len, vec_substr[j].size());
				gap_left.push_back(gapseq_left);
			}
			
			//-------------------将切割之后的fa序列按照line_len长度写入文件--------------------------
			
			ofstream ost_tmp;
			ost_tmp.open(subfilename, ios::out);
			ost_tmp << filehead << endl;
			ost_tmp << vec_substr[j] << endl;
			ost_tmp.close();
			
		}
		
		//----------------------写入_gap.fa和_dist.txt文件内容---------------------
		ofstream gap_1_out;
		gap_1_out.open(gap_1_file, ios::out);
		ofstream gap_2_out;
		gap_2_out.open(gap_2_file, ios::out);
		ofstream dist_out;
		dist_out.open(dist_file, ios::out);
		for(int j = 0 ; j < (int)vec_nstr.size(); ++j){
			string headline_left = "";
			headline_left = gap_headline[j] + "/1";
			gap_1_out << headline_left << endl;
			//Format_out(gap_out, gap_left[j], line_len);
			while(!gap_left[j].empty()){
				string subseq = "";
				if((int)gap_left[j].length() < line_len){
					gap_1_out << gap_left[j] << endl;
					break;
				}
				else{
					subseq = gap_left[j].substr(0, line_len);
					gap_left[j] = gap_left[j].substr(line_len, (int)gap_left[j].size() - line_len);
					gap_1_out << subseq << endl;
				}
			}
			
			string headline_right = "";
			headline_right = gap_headline[j] + "/2";
			gap_2_out << headline_right << endl;
			while(!gap_right[j].empty()){
				string subseq = "";
				if((int)gap_right[j].length() < line_len){
					gap_2_out << gap_right[j] << endl;
					break;
				}
				else{
					subseq = gap_right[j].substr(0, line_len);
					gap_right[j] = gap_right[j].substr(line_len, (int)gap_right[j].size() - line_len);
					gap_2_out << subseq << endl;
				}
			}
			
			dist_out << gap_headline[j] << endl << vec_nstr[j] << endl;
		}
		gap_1_out.close();
		gap_2_out.close();
		dist_out.close();	
		//vector<string>().swap(vec_nstr);

	}
}

int ExtrGap(int argc,char *argv[]){
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);

    char ShellCommand[CMD_NUM];
    char PathWork[CMD_NUM];
    char PathFalist[CMD_NUM];
	char gap_len[CMD_NUM] = "10";

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w"){
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
			snprintf(PathFalist, sizeof(PathFalist), "%s/1_falist/falist", PathWork);
        }
        if (cmd == "-falist")
        {
            snprintf(PathFalist, sizeof(PathFalist), "%s", argv[i + 1]);
            if (PathFalist[strlen(PathFalist) - 1] == '/') PathFalist[strlen(PathFalist) - 1] = '\0';
        }
		if (cmd == "-gl")sprintf(gap_len, "%s", argv[i+1]);
    }
	
	snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/2_chr_gaps", PathWork);
    system(ShellCommand);
	
	vector<string> vec_falist;
	ifstream if_falist;
	if_falist.open(PathFalist, ios::in);
	string falist_line;
	while(!if_falist.eof()){
		getline(if_falist, falist_line);
		if((int)falist_line.length() < 2){
			continue;
		}
		else{
			vec_falist.push_back(falist_line);
		}
	}
	if_falist.close();
	
	//创建包含gap和不包含gap的falist文件
	snprintf(ShellCommand, sizeof(ShellCommand), "rm -rf %s_gap", PathFalist);
    system(ShellCommand);
	snprintf(ShellCommand, sizeof(ShellCommand), "rm -rf %s_nogap", PathFalist);
	system(ShellCommand);
	snprintf(ShellCommand, sizeof(ShellCommand), "touch %s_gap", PathFalist);
    system(ShellCommand);
	snprintf(ShellCommand, sizeof(ShellCommand), "touch %s_nogap", PathFalist);
	system(ShellCommand);
	
	//判断是否包含提交长度一半的NNN空白序列
	int gap_len_int = atoi(gap_len);
	string chk_n(ceil(gap_len_int/2), 'N');
	
	for(int i=0; i<(int)vec_falist.size(); i++){
		
		char chk_nseq[CMD_NUM];
		string filecont;
		snprintf(ShellCommand, sizeof(ShellCommand), "cat %s | grep '%s' | wc -l", vec_falist[i].c_str(), chk_n.c_str()); 
		FILE *pfn = popen(ShellCommand, "r");
		fread(chk_nseq, sizeof(chk_nseq), 1, pfn);
		pclose(pfn);
		
		char *sub_chrpath = const_cast<char*>(vec_falist[i].c_str());
		
		//如果没有包含，则输出该falist到_nogap文件，然后继续循环
		if(atoi(chk_nseq) == 0){
			char nogap_file[CMD_NUM];
			//cout << "no_gap: " << sub_chrpath << endl;
			sprintf(nogap_file, "%s_nogap",PathFalist);
			ofstream nogap_out;
			nogap_out.open(nogap_file, ios::app);
			nogap_out << sub_chrpath << endl;
			nogap_out.close();
			continue;
		}
		else{
			Chr_Gap(sub_chrpath, filecont);
			Gap_Split(PathWork, PathFalist, sub_chrpath, filecont, gap_len_int);
		}
	}
	
    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}

