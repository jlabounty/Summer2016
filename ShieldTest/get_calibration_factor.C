int get_calibration_factor()
{

	std::string file_path = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160722_123707.txt";
	TTree *t = new TTree();
	TCanvas *c = new TCanvas();
	gStyle->SetOptFit();
	t->ReadFile(file_path.c_str(),"time:voltage:field");
	t->Draw("field:voltage");
	TF1 *fit1 = new TF1("fit1","pol1",0,25);
	Graph->Fit("fit1");
	c->Update();

	double slope = fit1->GetParameter(1);

	cout << endl << "The Calibration Factor for this setup is: " << slope << " mT/mV" << endl;
	cout << "Consult the shunt resistor you are using for the mV<->A conversion factor (most commonly 1:1)" << endl;

	return 0;
}
