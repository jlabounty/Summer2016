#include "exist_test.h"
#include "vector_math.h"

void unpack_file(std::string file, vector<double> &time, vector<double> &current, vector<double> &InternalField)
{

	TTree *t = new TTree();
	if(exist_test(file))
	{
		t->ReadFile(file.c_str(),"time:current:field");
	}
	else
	{
		return -1;
	}

	t->Draw("time:current:field","","goff");

        for(int i = 0; i < t->GetEntries(); i++)
        {
		time.push_back((t->GetV1())[i]);			//Time
		current.push_back((t->GetV2())[i]);		//Current in mA 
		InternalField.push_back((t->GetV3())[i]);	//Internal Magnetic Field (mT)
	}

}

void unpack_file(std::string file, vector<double> &time, vector<double> &current, vector<double> &InternalField, int stop_line)
{

	TTree *t = new TTree();
	if(exist_test(file))
	{
		t->ReadFile(file.c_str(),"time:current:field");
	}
	else
	{
		return -1;
	}

	t->Draw("time:current:field","","goff");

        for(int i = 0; i < t->GetEntries(); i++)
        {
		if (i == stop_line) break;
		time.push_back((t->GetV1())[i]);			//Time
		current.push_back((t->GetV2())[i]);		//Current in mA 
		InternalField.push_back((t->GetV3())[i]);	//Internal Magnetic Field (mT)
	}

}

double current_conversion(std::string setup, vector<double> &current, vector<double> &AppliedField)
{

	double calibration = 0.00;

	if(setup == "Helmholtz")
		calibration = 0.839957;
	if(setup == "Solenoid")
		calibration =  24.5;
	if(setup == "Dipole")
		calibration = 19.6124;
	if(setup == "DipoleNew")
		calibration = 20.3989;
	if(setup == "SmallBlack")
		calibration =  19.5361;

	for(int i = 0; i < current.size(); i++)
	{
		AppliedField.push_back(calibration*current[i]);
	}

	return calibration;

}


double current_conversion(double calibration, vector<double> &current, vector<double> &AppliedField)
{
	for(int i = 0; i < current.size(); i++)
	{
		AppliedField.push_back(calibration*current[i]);
	}
	return calibration;
}

void remove_zero_offset(int start, int finish, vector<double> &InternalField)
{
	double offset_average = 0.00;
	for (int i = start; i < finish; i++)
	{
		offset_average = offset_average + InternalField[i];
	}
	offset_average = offset_average / (finish - start);

	for(int i = 0; i < InternalField.size(); i++)
	{
		InternalField[i] = InternalField[i] - offset_average;
	}
}

void gradient_vector(const vector<double> &current, const vector<double> &time, vector<double> &current_grad)
{
	int size = current.size();
	for(int i = 0; i < size - 1 ; i++)
	{
		current_grad.push_back( (current[i+1] - current[i]) / (time[i+1] - time[i]) );
	}

}




void find_stable_regions(const vector<double> &current_grad, vector<double> &region_start, vector<double> &region_end, double threshold, double length)
{
	int j;

	for(int i = 0; i < current_grad.size(); i++)
	{
		if(TMath::Abs(current_grad[i]) >= threshold)
		{
			for(j = i+1; j < current_grad.size(); j++)
			{
				if(TMath::Abs(current_grad[j]) >= threshold)
				{
					if(j - i - 1 > length)
					{
						region_start.push_back(i);
						region_end.push_back(j);
						i = j + 1;
						continue;
					}
				}
			}

			if(j - i - 1 > length)
			{
				region_start.push_back(i);
				region_end.push_back(j);
				i = j + 1;
				continue;
			}
		}
	}
}

void extrapolate_field(const vector<double> &InternalField, const vector<double> &time, double start, double end, vector<double> &InternalField_stable, vector<double> &InternalField_err)
{
	vector<double> InternalField_partial;
	for (int i = start; i < end; i++)
	{
		InternalField_partial.push_back(InternalField[i]);
	}
	double mean_lin = mean_vector(InternalField_partial);
	double stdev_lin = standard_deviation_vector(InternalField_partial);

	TCanvas *c1 = new TCanvas();
	TGraph *gr1 = new TGraph(end-start, &time[start], &InternalField[start]);
	gr1->Draw("ap");
	if(InternalField[end] < 5000)
	{
		// average and stdev
		InternalField_stable.push_back(mean_lin);
		InternalField_err.push_back(stdev_lin);

		TLine *mean_line_lin = new TLine(0, mean_lin, gr1->GetXaxis()->GetXmax(), mean_lin);
			mean_line_lin->SetLineColor(kBlue-1);
			mean_line_lin->Draw("SAME");
		TLine *stdev_line_lin = new TLine(0, mean_lin - stdev_lin, gr1->GetXaxis()->GetXmax(), mean_lin - stdev_lin);
			stdev_line_lin->SetLineColor(kBlue-2);
			stdev_line_lin->SetLineStyle(7);
			stdev_line_lin->Draw("SAME");
		TLine *stdev_line2_lin = new TLine(0, mean_lin + stdev_lin, gr1->GetXaxis()->GetXmax(), mean_lin + stdev_lin);
			stdev_line2_lin->SetLineColor(kBlue-2);
			stdev_line2_lin->SetLineStyle(7);
			stdev_line2_lin->Draw("SAME");

		c1->Close();
	}
	else
	{
	//log fit to infinity ->   y = C ( 1 - e-kt ), k > 0
		TF1 *expfit = new TF1("expfit","[0]*(1 - TMath::Exp(-1*[1]*x)) + [2]", gr1->GetXaxis()->GetXmin(), gr1->GetXaxis()->GetXmax());
			expfit->SetParameter(0,mean_lin);
			expfit->SetParameter(1,0.0001);
			expfit->SetParameter(2, gr1->GetXaxis()->GetXmin());
		TF1 *logfit = new TF1("logfit","[0]*TMath::Log(x) + [1]", gr1->GetXaxis()->GetXmin(), gr1->GetXaxis()->GetXmax());
			logfit->SetParameter(0,1);
			logfit->SetParameter(1,gr1->GetXaxis()->GetXmin());
		gr1->Fit("logfit");
		c1->Update();

		InternalField_stable.push_back(logfit->Eval(gr1->GetXaxis()->GetXmin() + 1000 ));
		InternalField_err.push_back(stdev_lin);
	}

}

int DataCut( std::string data_file_path, std::string summary_file_path, std::string data_file_name, std::string calibration_string, std::string negation, int line_stop)
{

	cout << "       Executing DataCut function for file: " << data_file_name << endl;
//	std::string data_file_path = "/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/";
//	std::string data_file_name = "DataFile_160720_172022.txt";
	std::string data_file = data_file_path+data_file_name;
	vector<double> time, current, InternalField;

	if(line_stop == 0)
	{
		unpack_file(data_file, time, current, InternalField);
	}
	else
	{
		unpack_file(data_file, time, current, InternalField, line_stop);
	}	

	vector<double> AppliedField;
	double conversion_factor = current_conversion("Dipole", current, AppliedField);

	remove_zero_offset(0, 10, InternalField);

	if(negation == "True")
	{
		for(int i = 0; i < InternalField.size(); i++)
		{
			InternalField[i] = InternalField[i]*-1.00;
		}
	}

	vector<double> current_grad;
	gradient_vector(current, time, current_grad);
/*
	TCanvas *c = new TCanvas();
	TGraph *g = new TGraph(time.size(), &time[0], &current[0]);
	g->Draw("ap");	

	TCanvas *c1 = new TCanvas();
	TGraph *g1 = new TGraph(current_grad.size(), &time[0], &current_grad[0]);
	g1->SetMarkerColor(kBlue);
	g1->Draw("ap");	
*/
	vector<double> region_start, region_end;
	double threshold = 0.005;
	double length = 30;
	find_stable_regions(current_grad, region_start, region_end, threshold, length);

	vector<double> InternalField_stable, InternalField_err;
	vector<double> ExternalField_stable, ExternalField_err;

	for (int i = 0; i < region_start.size(); i++)
	{
		extrapolate_field(InternalField, time, region_start[i], region_end[i], InternalField_stable, InternalField_err);
		ExternalField_stable.push_back( mean_vector(AppliedField, region_start[i], region_end[i]) );
		ExternalField_err.push_back( standard_deviation_vector(AppliedField, region_start[i], region_end[i]) );
	}

	
	std::string data_file_root = (data_file_name.erase(data_file_name.size()-3,3)).append("root");
	TFile f((summary_file_path+"SummaryOf_"+data_file_root).c_str(),"RECREATE");
	TTree *t = new TTree("t","Shielding Measurement with Long Time Extrapolation");
	double InternalField_i, InternalField_err_i;
	double ExternalField_i, ExternalField_err_i;
	double current_i, current_err_i;

	TBranch *b_InternalField = t->Branch("Bi",&InternalField_i,"Field inside SC/D");
	TBranch *b_InternalField_err = t->Branch("BiErr",&InternalField_err_i,"Error in Field inside SC/D");
	TBranch *b_ExternalField = t->Branch("Bo",&ExternalField_i,"Field outside SC/D");
	TBranch *b_ExternalField_err = t->Branch("BoErr",&ExternalField_err_i,"Error in Field outside SC/D");
	TBranch *b_calibration = t->Branch("Calibration",&conversion_factor,"Calibration Factor A<->mT/D");
	TBranch *b_current = t->Branch("I",&current_i,"Current through shunt/D");
	TBranch *b_current_err = t->Branch("IErr",&current_err_i,"Error in Current through shunt/D");

	for(int i = 0; i < InternalField_stable.size(); i++)
	{
		InternalField_i = InternalField_stable[i];
		InternalField_err_i = InternalField_err[i];
		ExternalField_i = ExternalField_stable[i];
//		ExternalField_err_i = ExternalField_err[i];
		ExternalField_err_i = 0.00;
		current_i = ExternalField_i / conversion_factor;
		current_err_i = ExternalField_err_i / conversion_factor;
		t->Fill();
	}
	f.Write();
	f.Close();

	return 0;
}



























