int get_calibration_factor()
{

	std::string file_path = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160726_133846.txt";
	TTree *t = new TTree();
	TCanvas *c = new TCanvas();
	gStyle->SetOptFit();
	t->ReadFile(file_path.c_str(),"time:voltage:field");
	t->Draw("field:voltage");
	TF1 *fit1 = new TF1("fit1","pol1",0,30);
	Graph->Fit("fit1","R");
	c->Update();

	double slope = fit1->GetParameter(1);

	cout << endl << "The Calibration Factor for this setup is: " << TMath::Abs(slope) << " mT/mV" << endl;
	cout << "Consult the shunt resistor you are using for the mV<->A conversion factor (most commonly 1:1)" << endl;
	c->Print("./plots/CalibrationPlot.png");

	return 0;
}
