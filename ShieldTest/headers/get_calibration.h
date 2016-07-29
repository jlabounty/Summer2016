#include "exist_test.h"

double get_calibration(std::string file)
{

	TTree *t = new TTree();
	TCanvas *c = new TCanvas();
	gStyle->SetOptFit();
	if(exist_test(file))
	{
		t->ReadFile(file.c_str(),"time:voltage:field");
	}
	else
	{
		cerr << "ERROR: File not found" << endl;
		c->Close();
		return 0;
	}
	t->Draw("field:voltage");
	TF1 *fit1 = new TF1("fit1","pol1",0,30);
	Graph->Fit("fit1","R");
	c->Update();

	double slope = fit1->GetParameter(1);

	return slope;
}
