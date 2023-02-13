#include <iostream>
#include "SplitFa.h"
#include "ExtrGap.h"
#include "BwtBuilt.h"
#include "CompGap.h"
#include "ClsGap.h"
#include "MergFa.h"
#include "GetCls.h"

int main(int argc, char *argv[])
{
    std::string cmd = argv[1];
    if (cmd == "SplitFa" ) SplitFa(argc, argv);
	else if (cmd == "ExtrGap") ExtrGap(argc, argv);
	else if (cmd == "BwtBuilt") BwtBuilt(argc, argv);
	else if(cmd == "CompGap") CompGap(argc, argv);
	else if(cmd == "ClsGap") ClsGap(argc, argv);
	else if(cmd == "MergFa") MergFa(argc, argv);
	else if(cmd == "GetCls") GetCls(argc, argv);
    else
    {
        std::cout << "Error: Error Parameters." << std::endl;
        std::cout << "Parameters should be 'SplitFa' 'ExtrGap' 'BwtBuilt' 'CompGap' 'ClsGap' 'MergFa'or 'GetCls'." << std::endl;
        std::cout << std::endl;
    }
    return 0;
}