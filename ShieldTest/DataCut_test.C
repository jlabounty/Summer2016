#include "headers/DataCut.h"

int DataCut_test()
{

	vector<std::string> DataFile_list;
		DataFile_list.push_back("DataFile_160808_132712.txt");
		DataFile_list.push_back("DataFile_160808_143227.txt");
		DataFile_list.push_back("DataFile_160808_153301.txt");

	DataCut_SepFiles( 
			"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/", 
			"./SummaryFiles/", 
			DataFile_list, 
			"Dipole", 
			"True", 
			0);

	return 0;
}
