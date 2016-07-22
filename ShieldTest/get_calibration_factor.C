int get_calibration_factor()
{

	std::string file_path = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160722_122115.txt";
	TTree *t = new TTree();
	TCanvas *c = new TCanvas();
	gStyle->SetOptFit();
	t->ReadFile(file_path.c_str(),"time:voltage:field");
	t->Draw("field:voltage");
	TF1 *fit1 = new TF1("fit1","pol1",0,25);
	Graph->Fit("fit1");
	c->Update();

	return 0;
}
