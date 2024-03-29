/////////////////////////////////////////////////////////////////////////////
//Write the commit script to the node.
//Command: ./Writescript $Command $WORKPATH $PARAMETERS .... ($QUEUE $CPU $SPAN)
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CMD_NUM 2048
using namespace std;

int main(int argc, char *argv[])
{

    // Verify at least one arg.
    if (argc == 1)
    {
		std::cout << std::endl;
        std::cout << " SplitFa    - The source gene sequence FA file is split into small portions according to the included chromosomes." << std::endl;
		std::cout << " ExtrGap    - Extract gap sequence from chromosomes falist file, and split the sequence to some files." << std::endl;
		std::cout << " BwtBuilt   - Built Bowtie index file." << endl; 
		std::cout << " CompGap    - Compare paried-end gap-sequence to the reference genome by Bowtie2." << endl;
		std::cout << " ClsGap     - Close gaps by using result file Bowtie2" << endl; 
		std::cout << " MergFa     - Merge all the subfa files to GapClosed.fa" << endl; 
		std::cout << " GetCls     - Intergraton 'SplitFa', 'ExtrGap', 'CompGap', 'ClsGap', and 'MergFa' steps" << endl;
        std::cout << std::endl;
    }
    else if (argc == 2)
    {
        std::string cmd = argv[1];
        if (cmd == "SplitFa" )
        {
			std::cout << std::endl;
            std::cout << " ./PGCloser SplitFa <-w WorkPath> <-fa FAPath> [-q Queue] [-cpu CPU] [-span Span]" << std::endl;
            std::cout << std::endl;
			std::cout << " Required Parameters:" << std::endl;
            std::cout << " -w     : Working directory path for using to store the generated files." << std::endl;
            std::cout << " -fa    : The path of the original FA file."<< std::endl;
            std::cout << std::endl;
        }
		else if (cmd == "ExtrGap" )
        {
			std::cout << std::endl;
            std::cout << " ./PGCloser ExtrGap <-w WorkPath> [-falist FalistPath] [-gl GapLength]" << std::endl;
            std::cout << std::endl;
			std::cout << " Required Parameters:" << std::endl;
            std::cout << " -w       : Working directory path for using to store the generated files." << std::endl;
            std::cout << std::endl;
			std::cout << " Optional Parameters:" << std::endl;
			std::cout << " -falist  : The path of the original Falist file."<< std::endl;
			std::cout << " -gl      : The length of gap you want to extract (Default value: 10)." << std::endl;
            std::cout << std::endl;
        }
		else if (cmd == "BwtBuilt" )
        {
			std::cout << std::endl;
            std::cout << " ./PGCloser BwtBuilt  <-w WorkPath> <-ref RefFAPath>" << std::endl;
            std::cout << std::endl;
			std::cout << " Required Parameters:" << std::endl;
            std::cout << " -w       : Working directory path for using to store the generated files." << std::endl;
            std::cout << " -ref     : The path of the refrence genome FA file."<< std::endl;
            std::cout << std::endl;
        }
		else if (cmd == "CompGap"){
			std::cout << std::endl;
			std::cout << " ./PGCloser CompGap <-w WorkPath> <-falist FalistPath> <-bt BwtBuiltPath>" << std::endl;
            std::cout << std::endl;
			std::cout << " Required Parameters:" << std::endl;
            std::cout << " -w       : Working directory path for using to store the generated files." << std::endl;
			std::cout << " -falist  : The path of the original Falist file."<< std::endl;
            std::cout << " -bt      : The path of the bwt-index file generated by BwtBuilt tools."<< std::endl;
            std::cout << std::endl;
		}
		else if (cmd == "ClsGap"){
			std::cout << std::endl;
			std::cout << " ./PGCloser ClsGap <-w WorkPath> [-falist FalistPath] [-len LineLength] [-err ErrorRatio] " << std::endl;
            std::cout << std::endl;
			std::cout << " Required Parameters:" << std::endl;
            std::cout << " -w       : Working directory path for using to store the generated files." << std::endl;
			std::cout << " Optional Parameters:" << std::endl;
            std::cout << " -falist  : The path of the original Falist file."<< std::endl;
			std::cout << " -len     : The length of each line in output fasta file (default 60)."<< std::endl;
			std::cout << " -err     : The error ratio between extracted sequence length and gap length (default 2)."<< std::endl;
            std::cout << std::endl;
		}
		else if (cmd == "MergFa"){
			std::cout << std::endl;
			std::cout << " ./PGCloser MergFa <-w WorkPath> <-cfa SubfaPath> [-falist FalistPath]" << std::endl;
            std::cout << std::endl;
			std::cout << " Required Parameters:" << std::endl;
            std::cout << " -w       : Working directory path for using to store the generated files." << std::endl;
			std::cout << " -cfa     : The path of the sub Fa file."<< std::endl;
			std::cout << std::endl;
			std::cout << " Optional Parameters:" << std::endl;
            std::cout << " -falist  : The path of the original Falist file."<< std::endl;
            std::cout << std::endl;
		}
		else if (cmd == "GetCls"){
			std::cout << std::endl;
			std::cout << " ./PGCloser GetCls <-w WorkPath> <-fa FAPath> <-ref RefFaPath> [-gl GapLength] [-len LineLength] [-err ErrorRatio] [-q Queue] [-cpu CPU] [-span Span] [-cls lsf]" << std::endl;
			std::cout << std::endl;
			std::cout << " Required Parameters:" << std::endl;
            std::cout << " -w       : Working directory path for using to store the generated files." << std::endl;
			std::cout << " -fa      : The path of the original FA file."<< std::endl;
			std::cout << " -ref     : The path of the Refrence genome fa file."<< std::endl;
			std::cout << std::endl;
			std::cout << " Optional Parameters:" << std::endl;
			std::cout << " -gl      : The length of gap you want to extract (Default value: 10)." << std::endl;
			std::cout << " -len     : The length of each line in output fasta file (default 60)."<< std::endl;
			std::cout << " -err     : The error ratio between extracted sequence length and gap length (default 2)."<< std::endl;
            std::cout << " -q       : The queue you want to run the task (Default queue: normal)." << std::endl;
            std::cout << " -cpu     : The number of CPUs you want to allocate for running the task (Default value: 1)." << std::endl;
            std::cout << " -span    : The maximum number of the CPU used on each node (Default value: 20)." << std::endl;
            std::cout << " -cls     : The job system type of your computer cluster [lsf, local] (Default value: lsf)." << std::endl;
            std::cout << std::endl;
		}
        else
        {
			std::cout << std::endl;
            std::cout << " Error: Error Parameters." << std::endl;
            std::cout << " Parameters should be 'SplitFa' 'ExtrGap' 'CompGap' 'ClsGap' 'MergFa' or 'GetCls'." << std::endl;
            std::cout << std::endl;
        }
    }
    else
    {
        std::string cmd = argv[1];
        if (cmd == "SplitFa" || cmd == "ExtrGap" || cmd == "BwtBuilt" || cmd == "CompGap" || cmd == "ClsGap" || cmd =="MergFa" || cmd == "GetCls"){
            char ShellCommand[CMD_NUM];
            char Queue[CMD_NUM] = "normal";
            char Cpu[CMD_NUM] = "1";
            char Span[CMD_NUM] = "5";
            char Cluster[CMD_NUM] = "lsf";
            char PathWork[CMD_NUM];

            for (int i = 0; i < argc; i++){
                cmd = argv[i];
                if (cmd == "-w") sprintf(PathWork, "%s", argv[i + 1]);
                if (cmd == "-q") sprintf(Queue, "%s", argv[i + 1]);
                if (cmd == "-cpu") sprintf(Cpu, "%s", argv[i + 1]);
                if (cmd == "-span") sprintf(Span, "%s", argv[i + 1]);
                if (cmd == "-cls") sprintf(Cluster, "%s", argv[i + 1]);
            }
            char CurrentPath[CMD_NUM];
            //Get the current directory.
            snprintf(CurrentPath, sizeof(CurrentPath), "%s", argv[0]);
            int Count = 0;
            for (int j = int(strlen(CurrentPath)) - 1; j > 0; j--){
                if (CurrentPath[j] == '/')
                {
                    Count ++;
                    if (Count == 2) CurrentPath[j] = '\0';
                }
            }
            if (Count < 2) snprintf(CurrentPath, sizeof(CurrentPath), ".");
            FILE *fp_sh;
            sprintf(ShellCommand, "%s/run.sh", CurrentPath);
            if ((fp_sh = fopen(ShellCommand, "w")) == NULL) exit(-1);
            //Write the relevant parameters of the script.
            if(strcmp(Cluster,"lsf") == 0){
            	sprintf(ShellCommand, "#BSUB -q %s\n", Queue);
            	fputs(ShellCommand, fp_sh);
            	sprintf(ShellCommand, "#BSUB -J %s\n", argv[1]);
            	fputs(ShellCommand, fp_sh);
            	sprintf(ShellCommand, "#BSUB -o %s/%s.out\n", PathWork, argv[1]);
            	fputs(ShellCommand, fp_sh);
            	sprintf(ShellCommand, "#BSUB -e %s/%s.err\n", PathWork, argv[1]);
            	fputs(ShellCommand, fp_sh);
            	sprintf(ShellCommand, "#BSUB -n 1\n");
            	fputs(ShellCommand, fp_sh);
            	sprintf(ShellCommand, "#BSUB -R \"span[ptile=%s]\"\n", Span);
            	fputs(ShellCommand, fp_sh);
            	sprintf(ShellCommand, "%s/bin/./main", CurrentPath);
            	char tmp[CMD_NUM];
            	for (int i = 1; i < argc; i++){
            	    sprintf(tmp, " %s", argv[i]);
            	    strcat(ShellCommand, tmp);
            	}
            	fputs(ShellCommand, fp_sh);
            	fclose(fp_sh);
            	//Submit the script.
            	sprintf(ShellCommand, "bsub < %s/run.sh", CurrentPath);
            	system(ShellCommand);
            }
            else if(strcmp(Cluster,"local") == 0){
            	sprintf(ShellCommand, "%s/bin/./main", CurrentPath);
            	char tmp[CMD_NUM];
            	for (int i = 1; i < argc; i++){
            	    sprintf(tmp, " %s", argv[i]);
            	    strcat(ShellCommand, tmp);
            	}
            	fputs(ShellCommand, fp_sh);
            	fclose(fp_sh);
            	//Submit the script.
            	sprintf(ShellCommand, "sh %s/run.sh", CurrentPath);
            	system(ShellCommand);
            }
            else{
            	fclose(fp_sh);
            	std::cout << "Error: Error Parameters." << std::endl;
				exit(-1);
            }
        }
        else{
            std::cout << "Error: Error Parameters." << std::endl;
            std::cout << "Please check again." << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}