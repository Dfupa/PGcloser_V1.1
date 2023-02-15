#include <ClsGap.h>
#include <string>

using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx
{
    template<> struct hash<string>
    {
        size_t operator()(const string& s) const
        { return hash<const char*>()( s.c_str() ); }
    };
}

void spString(const std::string& s, std::vector<std::string>& v, const std::string& c){
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

void sam_assort_tobed(char* samfile, char* distfile, char* bedfile_name, char* bedglc_name, char* Err_rate){
	vector<string> vect;
	vector<string> vec_two;
	vector<string> vec_one;
	vector<string> vec_none;
	int length_vec;
	int length_dist;
	int length_vectwo;
	int err = atoi(Err_rate);
	ifstream if_sam;
	if_sam.open(samfile, ios::in);
	if(!if_sam){
		cout << "Unable to open samfile!" << endl;
		if_sam.close();
	}
	else{
		
		string oneline;		
		while(!if_sam.eof()){
			getline(if_sam, oneline);
			if(oneline[0] != '@' && (int)oneline.length() > 4){
				vect.push_back(oneline);
			}
		}
		if_sam.close();
		
		if((int)vect.size()%2==0){
			length_vec = (int)vect.size();
		}
		else{
			length_vec = (int)vect.size()-1;
		}
		
		for(int i=0; i<length_vec-1; i=i+2){
			vector<string> vect_split;
			vector<string> vect_split2;
			spString(vect[i], vect_split, "\t");
			spString(vect[i+1], vect_split2, "\t");
			if(vect_split[1]=="113" || vect_split[1]=="177"){
				vec_two.push_back(vect[i]);
				vec_two.push_back(vect[i+1]);
			}
			else if(vect_split[1]=="65" || vect_split[1]=="129"){
				vec_two.push_back(vect[i]);
				vec_two.push_back(vect[i+1]);
			}
			else if(vect_split[1]=="97" || vect_split[1]=="145"){
				vec_two.push_back(vect[i]);
				vec_two.push_back(vect[i+1]);
			}
			else if(vect_split[1]=="81" || vect_split[1]=="161"){
				vec_two.push_back(vect[i]);
				vec_two.push_back(vect[i+1]);
			}
			else if(vect_split[1]=="83" || vect_split[1]=="163"){
				vec_two.push_back(vect[i]);
				vec_two.push_back(vect[i+1]);
			}	
			else if(vect_split[1]=="99" || vect_split[1]=="147"){
				vec_two.push_back(vect[i]);
				vec_two.push_back(vect[i+1]);
			}
			else if((vect_split[1]=="69" && vect_split2[1]=="153") || (vect_split2[1]=="69" && vect_split[1]=="153")){
				vec_one.push_back(vect[i]);
				vec_one.push_back(vect[i+1]);
			}
			else if((vect_split[1]=="69" && vect_split2[1]=="137") || (vect_split2[1]=="69" && vect_split[1]=="137")){
				vec_one.push_back(vect[i]);
				vec_one.push_back(vect[i+1]);
			}
			else if((vect_split[1]=="89" && vect_split2[1]=="133") || (vect_split2[1]=="89" && vect_split[1]=="133")){
				vec_one.push_back(vect[i]);
				vec_one.push_back(vect[i+1]);
			}
			else if((vect_split[1]=="73" && vect_split2[1]=="133") || (vect_split2[1]=="73" && vect_split[1]=="133")){
				vec_one.push_back(vect[i]);
				vec_one.push_back(vect[i+1]);
			}
			else if(vect_split[1]=="77" || vect_split[1]=="141"){
				vec_none.push_back(vect[i]);
				vec_none.push_back(vect[i+1]);
			}
			else{
				vec_none.push_back(vect[i]);
				vec_none.push_back(vect[i+1]);
			}
		}
		
		vector<string> vec_dist;
		ifstream if_dist;
		if_dist.open(distfile, ios::in);
		string oneline2;		
		while(!if_dist.eof()){
			getline(if_dist, oneline2);
			if((int)oneline2.length() < 2){
				continue;
			}
			vec_dist.push_back(oneline2);
		}
		if_dist.close();
		
		if((int)vec_dist.size()%2==0){
			length_dist = (int)vec_dist.size();
		}
		else{
			length_dist = (int)vec_dist.size()-1;
		}
		
		hash_map<string, int> hmap_dist;
		for(int i=0; i< length_dist-1; i=i+2){
			hmap_dist[vec_dist[i].substr(1 ,(int)vec_dist[i].length()-1)] = (int)vec_dist[i+1].length();
		}
		
		ofstream of_bed;
		of_bed.open(bedfile_name, ios::out);
		ofstream of_bedglc;
		of_bedglc.open(bedglc_name, ios::out);
		int pairgap_length = 200;
		if((int)vec_two.size()%2==0){
			length_vectwo = (int)vec_two.size();
		}
		else{
			length_vectwo = (int)vec_two.size()-1;
		}
		
		for(int i=0; i<length_vectwo-1;i=i+2){
			vector<string> vec_fstline;
			vector<string> vec_secline;
			spString(vec_two[i], vec_fstline, "\t");
			spString(vec_two[i+1], vec_secline, "\t");
			
			if(vec_fstline[2] == vec_secline[2]){
				int distsam = abs(atoi(vec_fstline[3].c_str()) - atoi(vec_fstline[7].c_str())); 
				int distchk = abs(distsam - hmap_dist[vec_fstline[0]]);
				if((distchk / hmap_dist[vec_fstline[0]]) <= err){
					if(vec_fstline[1] == "65" || vec_fstline[1] == "97" || vec_fstline[1] == "99"){
						if(atoi(vec_fstline[7].c_str()) - atoi(vec_fstline[3].c_str()) >= pairgap_length){
							of_bedglc << vec_fstline[0] << "\t" << "2s" << "\t" << "+" << "\t" << hmap_dist[vec_fstline[0]] << endl;	
							of_bed << vec_fstline[2] << "\t" << atoi(vec_fstline[3].c_str())+pairgap_length-1 << "\t" << atoi(vec_fstline[7].c_str())-1 << "\t" << vec_fstline[0] << endl;
						}
						else if(atoi(vec_fstline[7].c_str()) - atoi(vec_fstline[3].c_str()) <=0){
							vec_none.push_back(vec_two[i]);
							vec_none.push_back(vec_two[i+1]);
						}
						else{
							vec_one.push_back(vec_two[i]);
							vec_one.push_back(vec_two[i+1]);
						}
					}
					else if(vec_fstline[1] == "113" || vec_fstline[1] == "81" || vec_fstline[1] == "83"){
						if(atoi(vec_fstline[3].c_str()) - atoi(vec_fstline[7].c_str()) >= pairgap_length){
							of_bedglc << vec_fstline[0] << "\t" << "2s" << "\t" << "-" << "\t" << hmap_dist[vec_fstline[0]] << endl;
							of_bed << vec_fstline[2] << "\t" << atoi(vec_fstline[7].c_str())+pairgap_length-1 << "\t" << atoi(vec_fstline[3].c_str())-1 << "\t" << vec_fstline[0] << endl;
						}
						else if(atoi(vec_fstline[3].c_str()) - atoi(vec_fstline[7].c_str()) <= 0){
							vec_none.push_back(vec_two[i]);
							vec_none.push_back(vec_two[i+1]);
						}
						else{
							vec_one.push_back(vec_two[i]);
							vec_one.push_back(vec_two[i+1]);
						}
					}
					else{
						vec_one.push_back(vec_two[i]);
						vec_one.push_back(vec_two[i+1]);
					}
				}
				else{
					vec_one.push_back(vec_two[i]);
					vec_one.push_back(vec_two[i+1]);
				}
			}
			else{
				vec_one.push_back(vec_two[i]);
				vec_one.push_back(vec_two[i+1]);
			}
		}
		
		int length_vecone;
		if((int)vec_one.size()%2==0){
			length_vecone = (int)vec_one.size();
		}
		else{
			length_vecone = (int)vec_one.size()-1;
		}
		
		for(int i=0; i<length_vecone-1;i=i+2){
			vector<string> vec_fstline2;
			vector<string> vec_secline2;
			spString(vec_one[i], vec_fstline2, "\t");
			spString(vec_one[i+1], vec_secline2, "\t");
			
			if((vec_fstline2[1] == "65" && vec_secline2[1] == "129") || (vec_fstline2[1] == "97" && vec_secline2[1] == "145") || (vec_fstline2[1] == "99" && vec_secline2[1] == "147") || (vec_fstline2[1] == "73" && vec_secline2[1] == "133")){
				of_bedglc << vec_fstline2[0] << "\t" << "1s" << "\t" << "+" << "\t" << hmap_dist[vec_fstline2[0]] << endl;
				of_bed << vec_fstline2[2] << "\t" << atoi(vec_fstline2[3].c_str())+pairgap_length-1 << "\t" << atoi(vec_fstline2[3].c_str())+pairgap_length+hmap_dist[vec_fstline2[0]]-1 << "\t" << vec_fstline2[0] << endl;
			}
			else if((vec_fstline2[1] == "113" && vec_secline2[1] == "177") || (vec_fstline2[1] == "81" && vec_secline2[1] == "161") || (vec_fstline2[1] == "83" && vec_secline2[1] == "163") || (vec_fstline2[1] == "89" && vec_secline2[1] == "133")){
				of_bedglc << vec_fstline2[0] << "\t" << "1s" << "\t" << "-" << "\t" << hmap_dist[vec_fstline2[0]] << endl;
				if(atoi(vec_fstline2[3].c_str())-hmap_dist[vec_fstline2[0]] + 1 < 0){
					of_bed << vec_fstline2[2] << "\t0\t" << atoi(vec_fstline2[3].c_str())+1 << "\t" << vec_fstline2[0] << endl;
				}
				else{
					of_bed << vec_fstline2[2] << "\t" << atoi(vec_fstline2[3].c_str())-hmap_dist[vec_fstline2[0]] + 1 << "\t" << atoi(vec_fstline2[3].c_str())+1 << "\t" << vec_fstline2[0] << endl;
				}
			}
			else if((vec_fstline2[1] == "69" && vec_secline2[1] == "137") || (vec_fstline2[1] == "137" && vec_secline2[1] == "69")){
				of_bedglc << vec_secline2[0] << "\t" << "1sh" << "\t" << "+" << "\t" << hmap_dist[vec_secline2[0]] << endl;
				if(atoi(vec_secline2[3].c_str())-hmap_dist[vec_secline2[0]] - 1 < 0){
					of_bed << vec_secline2[2] << "\t0\t" << atoi(vec_fstline2[3].c_str()) - 1 << "\t" << vec_secline2[0] << endl;
				}
				else{
					of_bed << vec_secline2[2] << "\t" << atoi(vec_secline2[3].c_str())-hmap_dist[vec_secline2[0]] - 1 << "\t" << atoi(vec_fstline2[3].c_str()) - 1 << "\t" << vec_secline2[0] << endl;
				}
			}
			else if((vec_fstline2[1] == "69" && vec_secline2[1] == "153") || (vec_fstline2[1] == "153" && vec_secline2[1] == "69")){
				of_bedglc << vec_secline2[0] << "\t" << "1sh" << "\t" << "-" << "\t" << hmap_dist[vec_secline2[0]] << endl;
				if(atoi(vec_secline2[3].c_str())-hmap_dist[vec_secline2[0]] + 1 < 0){
					of_bed << vec_secline2[2] << "\t0\t" << atoi(vec_fstline2[3].c_str()) +1 << "\t" << vec_secline2[0] << endl;
				}
				else{
					of_bed << vec_secline2[2] << "\t" << atoi(vec_secline2[3].c_str())-hmap_dist[vec_secline2[0]] + 1 << "\t" << atoi(vec_fstline2[3].c_str()) + 1 << "\t" << vec_secline2[0] << endl;
				}
			}
			else{
				vec_none.push_back(vec_one[i]);
				vec_none.push_back(vec_one[i+1]);
			}

		}
		hmap_dist.clear();
	}
}

void Bed_Run(char* workpath, char* fapath, char* bedpath, char* sub_chrname){
	char ShellCommand[CMD_NUM];
	char* Bedtools_path = (char*)"$(pwd)/bedtools2/bin/bedtools";
	
	snprintf(ShellCommand, sizeof(ShellCommand), "%s getfasta -fi %s -bed %s -name -fo %s.bedout", Bedtools_path, fapath, bedpath, sub_chrname);
	system(ShellCommand);
	
	snprintf(ShellCommand, sizeof(ShellCommand), "mv %s.bedout %s/4_bedfile/", sub_chrname, workpath);
	system(ShellCommand);
}

std::string Add_Str(std::string& targseq, int dist){
	string resseq;
	int num_n = ceil(dist / 2);
	if(num_n > 1000){
		num_n = 1000;
	}
	string nnn(num_n, 'N');
	resseq = targseq + nnn;
	return resseq;
}

std::string Tran_Str(std::string& targseq){
	string resseq = targseq;
	for(int i=0; i<(int)targseq.length();i++){
		if(targseq[i] == 'A'){
			resseq[i] = 'T';
		}
		else if(targseq[i] == 'T'){
			resseq[i] = 'A';
		}
		else if(targseq[i] == 'G'){
			resseq[i] = 'C';
		}
		else if(targseq[i] == 'C'){
			resseq[i] = 'G';
		}
	}
	reverse(resseq.begin(), resseq.end());
	return resseq;
}

void Extr_Seq(char *filename, string& pRead){
	char ShellCommand[CMD_NUM];
	char tmpfile_name[CMD_NUM];
	
	snprintf(ShellCommand, sizeof(ShellCommand), "cp %s %s_tmp", filename, filename); 
	system(ShellCommand);
	snprintf(ShellCommand, sizeof(ShellCommand), "sed -i '1d' %s_tmp", filename); 
	system(ShellCommand);
	
	sprintf(tmpfile_name, "%s_tmp", filename);
	ifstream is_tmp;
	is_tmp.open(tmpfile_name, ios::in);
	getline(is_tmp, pRead);
	is_tmp.close();
	
	snprintf(ShellCommand, sizeof(ShellCommand), "rm -rf %s_tmp", filename); 
	system(ShellCommand);
}

void Fill_Gap(char* workpath, char* sub_chrname, char* outfile, char* bedglc_name, char* Line_len){
	char ShellCommand[CMD_NUM];
	vector<string> vec_distfile;
	char distfile_name[CMD_NUM];
	sprintf(distfile_name, "%s/2_chr_gaps/%s/%s_dist.txt", workpath, sub_chrname, sub_chrname);
	ifstream if_distfile;
	if_distfile.open(distfile_name, ios::in);
	string line_tmp;
	while(!if_distfile.eof()){
		getline(if_distfile, line_tmp);
		if((int)line_tmp.length() < 2){
			continue;
		}
		else{
			vec_distfile.push_back(line_tmp);
		}
	}
	if_distfile.close();
	if((int)vec_distfile.size()%2 !=0){
		cout << "Dist file: " << sub_chrname << " is not formated!" << endl;
	}
	
	vector<string> vec_outfile;
	ifstream if_outfile;
	if_outfile.open(outfile, ios::in);
	string oneline;
	while(!if_outfile.eof()){
		getline(if_outfile, oneline);
		if(oneline[0] == '\t' || oneline[0] == '\n' || oneline[0] == ' ' || oneline == ""){
			continue;
		}
		else{
			vec_outfile.push_back(oneline);
		}
	}
	if_outfile.close();
	if((int)vec_outfile.size()%2 !=0){
		cout << "Bedout file: " << sub_chrname << " is not formated!" << endl;
	}
	
	vector<string> vec_bedglc;
	ifstream if_bedglc;
	if_bedglc.open(bedglc_name, ios::in);
	string oneline2;
	while(!if_bedglc.eof()){
		getline(if_bedglc, oneline2);
		if((int)oneline2.length() < 2){
			continue;
		}
		else{
			vec_bedglc.push_back(oneline2);
		}
	}
	if_bedglc.close();

	char tmpfa_name[CMD_NUM];
	sprintf(tmpfa_name, "%s/4_cfa/%s.tmpfa", workpath, sub_chrname);
	snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", tmpfa_name);
	system(ShellCommand); 
	
	ofstream ost_tmp;
	ost_tmp.open(tmpfa_name, ios::out);
	
	int length_df;
	if((int)vec_distfile.size()%2==0){
		length_df = (int)vec_distfile.size();
	}
	else{
		length_df = (int)vec_distfile.size()-1;
	}
	int length_of;
	if((int)vec_outfile.size()%2 == 0){
		length_of = (int)vec_outfile.size();
	}
	else{
		length_of = (int)vec_outfile.size()-1 ;
	}
	
	for(int x=0; x<length_df-1; x=x+2){
		for(int y=0; y<length_of-1;y=y+2){
			string str_name = vec_distfile[x].substr(1, (int)vec_distfile[x].length()-1);
			if(vec_outfile[y] == vec_distfile[x]){
				for(int z=0; z<(int)vec_bedglc.size(); z++){
					if(vec_bedglc[z].find(str_name) < vec_bedglc[z].length()){
						vector<string> vec_tmp;
						vector<string> vec_tmpsub;
						spString(vec_bedglc[z], vec_tmp, "\t");
						spString(vec_tmp[0], vec_tmpsub, "_to_");
						char file_left[CMD_NUM];
						sprintf(file_left, "%s/2_chr_gaps/%s/%s.fa", workpath, sub_chrname, vec_tmpsub[0].c_str());
						char file_right[CMD_NUM];
						sprintf(file_right, "%s/2_chr_gaps/%s/%s.fa", workpath, sub_chrname, vec_tmpsub[1].c_str());
						
						if(vec_tmp[2] == "+" && vec_tmp[1] == "2s"){
							string seq_right;
							if(x==0){
								string seq_left;
								Extr_Seq(file_left, seq_left);
								ost_tmp << seq_left << endl;
							}
							Extr_Seq(file_right, seq_right);
							ost_tmp << vec_outfile[y+1] << endl;
							ost_tmp << seq_right << endl;
						}
						else if(vec_tmp[2] == "-" && vec_tmp[1] == "2s"){
							string seq_gap;
							string seq_right;
							if(x==0){
								string seq_left;
								Extr_Seq(file_left, seq_left);
								ost_tmp << seq_left << endl;
							}
							seq_gap = Tran_Str(vec_outfile[y+1]);
							Extr_Seq(file_right, seq_right);
							ost_tmp << seq_gap << endl;
							ost_tmp << seq_right << endl;
						}
						else if(vec_tmp[2] == "+" && vec_tmp[1] == "1s"){
							string seq_gap;
							string seq_right;
							if(x==0){
								string seq_left;
								Extr_Seq(file_left, seq_left);
								ost_tmp << seq_left << endl;
							}
							seq_gap = Add_Str(vec_outfile[y+1], atoi(vec_tmp[3].c_str()));
							Extr_Seq(file_right, seq_right);
							ost_tmp << seq_gap << endl;
							ost_tmp << seq_right << endl;
						}
						else if(vec_tmp[2] == "-" && vec_tmp[1] == "1s"){
							string trans = Tran_Str(vec_outfile[y+1]);
							string seq_gap = Add_Str(trans, atoi(vec_tmp[3].c_str()));
							string seq_right;
							if(x==0){
								string seq_left;
								Extr_Seq(file_left, seq_left);
								ost_tmp << seq_left << endl;
							}
							Extr_Seq(file_right, seq_right);
							ost_tmp << seq_gap << endl;
							ost_tmp << seq_right << endl;
						}
						else if(vec_tmp[2] == "+" && vec_tmp[1] == "1sh"){
							int num_n = ceil(atoi(vec_tmp[3].c_str()) / 2);
							if(num_n > 1000){
								num_n = 1000;
							}
							string nnn(num_n, 'N');
							string seq_gap =  nnn + vec_outfile[y+1];
							if(x==0){
								string seq_left;
								Extr_Seq(file_left, seq_left);
								ost_tmp << seq_left << endl;
							}
							string seq_right;
							Extr_Seq(file_right, seq_right);
							ost_tmp << seq_gap << endl;
							ost_tmp << seq_right << endl;
						}
						else if(vec_tmp[2] == "-" && vec_tmp[1] == "1sh"){
							string seq_gap = Tran_Str(vec_outfile[y+1]);
							int num_n = ceil(atoi(vec_tmp[3].c_str()) / 2);
							if(num_n > 1000){
								num_n = 1000;
							}
							string nnn(num_n, 'N');
							seq_gap =  nnn + seq_gap;
							if(x==0){
								string seq_left;
								Extr_Seq(file_left, seq_left);
								ost_tmp << seq_left << endl;
							}
							string seq_right;
							Extr_Seq(file_right, seq_right);
							ost_tmp << seq_gap << endl;
							ost_tmp << seq_right << endl;
						}
						
						break;
					}
					else{
						continue;
					}
				}
				break;
			}
			else if(y == (int)vec_outfile.size()-2 && vec_outfile[y].find(vec_distfile[x]) == vec_outfile[y].npos){
				vector<string> vec_tmplast;
				spString(str_name, vec_tmplast, "_to_");
				char file_left[CMD_NUM];
				sprintf(file_left, "%s/2_chr_gaps/%s/%s.fa", workpath, sub_chrname, vec_tmplast[0].c_str());
				char file_right[CMD_NUM];
				sprintf(file_right, "%s/2_chr_gaps/%s/%s.fa", workpath, sub_chrname, vec_tmplast[1].c_str());
				
				string seq_right;
				if(x==0){
					string seq_left;
					Extr_Seq(file_left, seq_left);
					ost_tmp << seq_left << endl;
				}
				Extr_Seq(file_right, seq_right);
				ost_tmp << vec_distfile[x+1] << endl;
				ost_tmp << seq_right << endl;
			}
			else{
				continue;
			}
		}
	}
	ost_tmp.close();
	
	snprintf(ShellCommand, sizeof(ShellCommand), "touch %s_2", tmpfa_name);
	system(ShellCommand);
	snprintf(ShellCommand, sizeof(ShellCommand), "cat %s | tr -d '\n' > %s_2", tmpfa_name, tmpfa_name); 
	system(ShellCommand);
	
	string sub_fullseq;
	char tmpfile_name[CMD_NUM];
	sprintf(tmpfile_name, "%s_2", tmpfa_name);
	ifstream is_tmp;
	is_tmp.open(tmpfile_name, ios::in);
	getline(is_tmp, sub_fullseq);
	is_tmp.close();

	
	char cfa_subfaname[CMD_NUM];
	sprintf(cfa_subfaname, "%s/4_cfa/%s.fa", workpath, sub_chrname);

	snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", cfa_subfaname);
	system(ShellCommand);
	char headline[CMD_NUM];
	sprintf(headline, ">%s\tlength=%d", sub_chrname, (int)sub_fullseq.length());

	ofstream ost_fa;
	ost_fa.open(cfa_subfaname, ios::out);
	ost_fa << headline << endl;
	int llen = atoi(Line_len);
	for(int a=0; a<(int)sub_fullseq.length();a++){
		if(a !=0 && a%llen == 0){
			ost_fa << endl;
		}
		ost_fa << sub_fullseq[a];
	}
	
	snprintf(ShellCommand, sizeof(ShellCommand), "rm -rf %s", tmpfa_name); 
	system(ShellCommand);
	snprintf(ShellCommand, sizeof(ShellCommand), "rm -rf %s_2", tmpfa_name); 
	system(ShellCommand);
}

int ClsGap(int argc, char *argv[]){
	long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);

    char ShellCommand[CMD_NUM];
    char PathWork[CMD_NUM];
    char PathFalist[CMD_NUM];	
	char Line_len[CMD_NUM] = "60";
	char Err_rate[CMD_NUM] = "2";
	
    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
			snprintf(PathFalist, sizeof(PathFalist), "%s/1_falist/falist", PathWork);
        }
        if (cmd == "-falist")
        {
            snprintf(PathFalist, sizeof(PathFalist), "%s", argv[i + 1]);
            if (PathFalist[strlen(PathFalist) - 1] == '/') PathFalist[strlen(PathFalist) - 1] = '\0';
        }
		if (cmd == "-len")sprintf(Line_len, "%s", argv[i + 1]);
		if (cmd == "-err")sprintf(Err_rate, "%s", argv[i + 1]);
    }
	
	snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/4_bedfile", PathWork);
	system(ShellCommand);
	
	snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/4_cfa", PathWork);
	system(ShellCommand); 
	
	vector<string> vec_falist;
	ifstream if_falist;
	if_falist.open(PathFalist, ios::in);
	string falist_line;
	while(!if_falist.eof()){
		getline(if_falist, falist_line);
		if((int)falist_line.length() <2){
				continue;
		}
		vec_falist.push_back(falist_line);
	}
	if_falist.close();
	
	for(int i=0; i<(int)vec_falist.size(); i++){
		vector<string> vec_tmp;
		string name_tmp;
		spString(vec_falist[i], vec_tmp, "/");
		name_tmp = vec_tmp[vec_tmp.size()-1];
		char* sub_chrname;
		const int chrlen = name_tmp.length();
		sub_chrname = new char[chrlen+1];
		strcpy(sub_chrname, name_tmp.substr(0, name_tmp.length()-3).c_str());
		char samfile[CMD_NUM];
		sprintf(samfile, "%s/3_refdir/%s.sam", PathWork, sub_chrname);
		char distfile[CMD_NUM];
		sprintf(distfile, "%s/2_chr_gaps/%s/%s_dist.txt", PathWork, sub_chrname, sub_chrname);
		
		char bedfile_name[CMD_NUM];
		sprintf(bedfile_name, "%s/4_bedfile/%s.bed", PathWork, sub_chrname);
		snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", bedfile_name);
		system(ShellCommand);
		
		char bedglc_name[CMD_NUM];
		sprintf(bedglc_name, "%s/4_bedfile/%s.bedglc", PathWork, sub_chrname);
		snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", bedglc_name);
		system(ShellCommand);
		
		sam_assort_tobed(samfile, distfile, bedfile_name, bedglc_name, Err_rate);
		
		char fapath[CMD_NUM];
		sprintf(fapath, "%s/3_refdir/Ref_Genome_Bwt.fa", PathWork);
		Bed_Run(PathWork, fapath, bedfile_name, sub_chrname);
		
		char outfile[CMD_NUM];
		sprintf(outfile, "%s/4_bedfile/%s.bedout", PathWork, sub_chrname);
		char outfile_size[20];
		snprintf(ShellCommand, sizeof(ShellCommand), "wc -c %s | awk '{print $1}'", outfile); 
		FILE *pf1 = popen(ShellCommand, "r");
		fread(outfile_size, sizeof(outfile_size), 1, pf1);
		pclose(pf1);
		if(atoi(outfile_size)==0){
			continue;
		}
		Fill_Gap(PathWork, sub_chrname, outfile, bedglc_name, Line_len);
	}
	
	long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}