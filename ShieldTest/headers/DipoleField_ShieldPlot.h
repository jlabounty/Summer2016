#include "exist_test.h"

double GetFieldMapFactor()
{

	cout << "       Finding Field Map Factor" << endl;
	double FieldMapFactor;
	
        double z_offset = 1.000;
        double z_scale = 1.000;
        double field_offset = -300.3000;
        double field_scale = -1.000;

        TTree *t = new TTree();
        std::string file = "/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/DataFile_160720_162008.txt";

        //Check to see if file exists. If so read in file into TTree.
        if(exists_test(file))
        {
                t->ReadFile(file.c_str(),"z:field:non:important:numbers");
        }
        else
        {
                cerr << "ERROR: File not found" << endl;
                return -1;
        }

        //Create new branches for offsets and multiplicative factors
        TBranch *b_field_offset = t->Branch("field_offset",&field_offset,"pt/D");
        TBranch *b_field_scale = t->Branch("field_scale",&field_scale,"pt/D");
        TBranch *b_z_offset = t->Branch("z_offset",&z_offset,"pt/D");
        TBranch *b_z_scale = t->Branch("z_scale",&z_scale,"pt/D");

        //Plot the data with appropriate offsets
	TCanvas *ctemp = new TCanvas();
        t->Draw("(field_scale*field)-field_offset:(z_scale*z)-z_offset");

        TF1 *fit1 = new TF1("fit1","gaus");
        Graph->Fit("fit1");

        cout << "       The Field Map Factor for a 1\" Cylinder is: " << fit1->Eval(fit1->GetParameter(1) - 25.4/2.0) / fit1->Eval(fit1->GetParameter(1)) << endl;

	FieldMapFactor = fit1->Eval(fit1->GetParameter(1) - 25.4/2.0) / fit1->Eval(fit1->GetParameter(1));
	ctemp->Close();
	return FieldMapFactor;

}

void DipoleFieldShieldPlot(std::string file_path, std::string summary_file_path, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color, std::string helm_or_sol, std::string negate, int file_end)
{
	cout << "*******************************************************" << endl << "Beginning: " << title  << " WITH correction for dipole field" << endl;
//Measurement Files

	std::string file_end_str = Form("%d",file_end);

	std::string python_input = "python plot_output.py \""+file_path+"\" \""+ten_file1_txt+"\" "+negate+" "+helm_or_sol+" "+file_end_str;

	cout << "       Python Command Executed for: " << ten_file1_txt << endl;
	cout << "       [If this plot looks odd, you may have to adjust the parameters in 'plot_output.py' directly]" << endl;
	gSystem->Exec(python_input.c_str());


	if(exist_test((summary_file_path+"SummaryOf_"+ten_file1_txt).c_str()))
	{
		cout << "       Summary File Found. Proceeding with Plot" << endl;
	} 
	else 
	{
		cerr << "ERROR: Summary File for " << ten_file1_txt << " not found. Aborting." << endl;
		return -1;
	}   
	
	//Read the measurement summary file into vectors
	std::ifstream ten_file1((summary_file_path+"SummaryOf_"+ten_file1_txt).c_str()); 	//Measurement File

        std::string str;
        double AppliedField_i, AppliedField_err_i, InternalField_i, InternalField_err_i;
        vector<double> AppliedField, AppliedField_err, AppliedField_Noerr, InternalField, InternalField_err;
        vector<double> AppliedField_DipoleCorrection;

	double fieldAtEdge_vs_fieldAtCenter = GetFieldMapFactor();

        while (std::getline(ten_file1, str))
        {
                if(ten_file1 >> AppliedField_i >> AppliedField_err_i >> InternalField_i >> InternalField_err_i)
                {
                        AppliedField.push_back(AppliedField_i); 			//Calculated Applied Field
                        AppliedField_DipoleCorrection.push_back(AppliedField_i / fieldAtEdge_vs_fieldAtCenter); 			//Calculated Applied Field
			if(AppliedField_err_i > 0.01)
			{
                        	AppliedField_err.push_back(AppliedField_err_i); 		//Error in applied field
			}
			else
			{
				AppliedField_err.push_back(0.01);
			}
                        AppliedField_Noerr.push_back(0.00); 				//If error in applied field is to be neglected, use this instead of true error
                        InternalField.push_back(InternalField_i); 			//Measured Internal Field
                        InternalField_err.push_back(InternalField_err_i); 		//Error in measurement of internal field
                }
        }

	//Plot the applied field vs. internal field on the canvas which was passed in by reference
	c00.cd();
	//This is the minimum shielding, using the field at the center of the superconductor
	gr12 = new TGraphErrors(AppliedField.size(),&(AppliedField[0]),&(InternalField[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr12->Draw("PL SAME");
                gr12->SetMarkerColor(color);
                gr12->SetLineColor(color);

	//This is the maximum shielding, using the field at the edge of the superconductor (extrapolated from field map)
	gr13 = new TGraphErrors(AppliedField_DipoleCorrection.size(),&(AppliedField_DipoleCorrection[0]),&(InternalField[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr13->Draw("PL SAME");
                gr13->SetMarkerColor(color);
                gr13->SetLineColor(color);

	//Draw a crosshatching between the two curves
	int n = AppliedField_DipoleCorrection.size();
	TGraph *grshade = new TGraph(2*AppliedField_DipoleCorrection.size());
	for (int i=0;i<n;i++) 
	{
		grshade->SetPoint(i,AppliedField[i],InternalField[i]);
		grshade->SetPoint(n+i,AppliedField_DipoleCorrection[n-i-1],InternalField[n-i-1]);
	}
	grshade->SetFillStyle(3013);
	grshade->SetFillColor(color);
	grshade->Draw("f SAME");

	c00.Update();
	
	//Add the above plot(s) to the legend passed in by reference
	leg.AddEntry(gr12,title.c_str(),"pl");
	if(lin_extrap)
	{
		std::string title_ext = title + " Linearly Extrapolated";
		leg.AddEntry(f7,title_ext.c_str(),"l");
	}


	cout << "All done with: " << title << endl;
}

