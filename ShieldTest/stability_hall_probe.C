#include <unistd.h>

int stability_hall_probe()
{

	//File to be read in should be 3 numbers in the following format: seconds_since_start voltage magnetic_field
        std::string file_path = 
			"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160727_104726.txt";
        TTree *t = new TTree();
        TCanvas *c = new TCanvas();
        gStyle->SetOptFit();

	//Checks if file exists
	struct stat buffer;   
	if(stat (file_path.c_str(), &buffer) == 0)
	{
	        t->ReadFile(file_path.c_str(),"seconds:voltage:field");
	}
	else
	{
		cerr << endl;
		cerr << "**************** ERROR: File not found ****************" << endl;
		cerr << "Please check the file path and/or allow file syncronization services (Dropbox, etc.) to finish." << endl << endl;
		c->Close();
		return -1;
	}

	//Draw graph and fit a line to it. A line may not always describe the drift perfectly, but it is usually a good approximation.
        t->Draw("field:seconds");
        TF1 *fit1 = new TF1("fit1","pol1");
        Graph->Fit("fit1");

	//Get the slope and error of the line
        double slope = fit1->GetParameter(1);
        double slope_err = fit1->GetParError(1);

	//Draw a version of the above plot without the line on a different canvas
	TCanvas *c2 = new TCanvas();
	t->Draw("field:seconds");
        c2->Update();

	//Output the mT/[time] slopes for different times
	cout << endl << "Assuming Linear Drift:" << endl;
	cout << "    Slope (mT/second): " << slope  << " +/- " << slope_err << endl;
	cout << "    Slope (mT/minute): " << slope*60.0   << " +/- " << slope_err*60.0 << endl;
	cout << "    Slope (mT/hour):   " << slope*60.0*60.0   << " +/- " 
		<< slope_err*60.0*60.0 << endl << endl;

	//Output some drift values for common measurements
	cout << "In a 1.5 hour dipole measurement, the probe will drift by " << slope*60.0*60.0*1.5 
		<< " mT" << endl;
	cout << "In a 50 point ferromagnet B vs. z measurement (5 sec/point), the probe will drift by "
		<< slope*5*50 << " mT" << endl;

        return 0;
}
