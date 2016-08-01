#include "headers/DataCut.h"

int DataCut_test()
{

	DataCut( 
			"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/", 
			"./SummaryFiles/", 
			"DataFile_150222_041457.txt",	//24 hour 2015 measurement 
			"Dipole", 
			"True", 
			0);

	return 0;
}
