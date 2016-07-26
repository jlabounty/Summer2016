int stability_hall_probe()
{

        std::string file_path = 
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160726_160548.txt";
        TTree *t = new TTree();
        TCanvas *c = new TCanvas();
        gStyle->SetOptFit();
        t->ReadFile(file_path.c_str(),"seconds:voltage:field");
        t->Draw("field:seconds");
        TF1 *fit1 = new TF1("fit1","pol1");
        Graph->Fit("fit1");

        double slope = fit1->GetParameter(1);
        double slope_err = fit1->GetParError(1);

	TCanvas *c2 = new TCanvas();
	t->Draw("field:seconds");
        c2->Update();

	cout << endl << "Assuming Linear Drift:" << endl;
	cout << "    Slope (mT/second): " << slope  << " +/- " << slope_err << endl;
	cout << "    Slope (mT/minute): " << slope*60.0   << " +/- " << slope_err*60.0 << endl;
	cout << "    Slope (mT/hour):   " << slope*60.0*60.0   << " +/- " << slope_err*60.0*60.0 << endl << endl;

	cout << "In a 1.5 hour dipole measurement, the probe will drift by " << slope*60.0*60.0*1.5 << " mT" << endl;
	cout << "In a 100 point ferromagnet B vs. z measurement (5 sec/point), the probe will drift by " << slope*5*100 << " mT" << endl;

        return 0;
}
