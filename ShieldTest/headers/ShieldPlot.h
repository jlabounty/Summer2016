/*
        There are 3 functions located in this header file. Each one takes the following parameters as input:
                Calibration File        ---     Room temperature calibration measuremnt used to convert voltage -> applied field. The only function which doesnt require this is Helium_calib_single, as we have a fixed current -> field relationship for Xu Du's magnet.
                Measurement File        ---     A file containing the data for the measuremnt. For the Helium measurements this should be in the form "current field" and for all others in the form "time voltage field"
                Title                   ---     A string which will be the entries label in the legend
                TCanvas                 ---     The TCanvas (defailt *c00) which is passed by reference for the function to draw on
                TLegend                 ---     The TLegend (defailt *leg) which is passed by reference for the function to draw on
                color                   ---     An integer which specifies the color of the markers for that measurement

*/

//Various variables which need to be shared between these functions.
int n=2;
bool onetoone_leg = true;
bool lin_extrap = false;


//This version of the function uses the old calibration files read from the back of the black power supply, which feature an exponential increase at low current values. This is taken into account by fitting an exponential function if 9.0 < voltage < 13.0 and a linear fit elsewhere. This function should not be used except for older measurements, as we now have a linear calibration with the shunt resistor.
void Solenoid_calib_single_old(std::string ten_file_txt, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color)
{

	cout << "*******************************************************" << endl << "Beginning: " << title << endl;
	//Read the measurement files into vectors
	std::ifstream ten_file(ten_file_txt.c_str()); 		//Room temperature calibration
	std::ifstream ten_file1(ten_file1_txt.c_str()); 	//Measurement file

        std::string str;
        double ti, xi, yi;
        vector<double> ten_time, ten_field, ten_voltage;
        vector<double> ten_time_1, ten_field_1, ten_voltage_1;

        while (std::getline(ten_file, str))
        {
                if(ten_file >> ti >> xi >> yi)
                {
                        ten_time.push_back(ti); 			//Time
                        ten_voltage.push_back(TMath::Abs(xi)); 		//Current in mA -> Voltage in mV
                        ten_field.push_back(TMath::Abs(yi)); 		//Magnetic Field (mT)
                }
        }

        while (std::getline(ten_file1, str))
        {
                if(ten_file1 >> ti >> xi >> yi)
                {
                        ten_time_1.push_back(ti); 			//Time
                        ten_voltage_1.push_back(TMath::Abs(xi)); 	//Current in mA -> Voltage in mV
                        ten_field_1.push_back(TMath::Abs(yi)); 		//Magnetic Field (mT)
                }
        }

	//Graph the room temperature calibration file: Voltage vs. Field
	TCanvas *c1 = new TCanvas();
        gr1 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr1->Draw("AP");
                gr1->GetXaxis()->SetTitle("Voltage (mV)");
                gr1->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr1->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr1->SetMarkerColor(kRed);
        TF1 *fit1 = new TF1("fit1","pol1",13,300);
        gr1->Fit("fit1","R");
        TF1 *f1 = gr1->GetFunction("fit1");
                f1->SetName("f1");
                f1->SetLineColor(kRed);
                f1->SetRange(0,300);
                f1->Draw("SAME");
        c1->Update();

	//Graph the cryo measurement file: Voltage vs. Field
        gr2 = new TGraph(ten_time_1.size(),&(ten_voltage_1[0]),&(ten_field_1[0]));
                gr2->Draw("p SAME");
                gr2->GetXaxis()->SetTitle("Voltage (mV)");
                gr2->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr2->SetTitle("Voltage from Power Supply vs. Magnetic Field --- 10 minutes");
                gr2->SetMarkerColor(kBlue);
        TF1 *fit2 = new TF1("fit2","pol1",60,300);
        gr2->Fit("fit2","R");
        TF1 *f2 = gr2->GetFunction("fit2");
                f2->SetName("f2");
                f2->SetLineColor(kBlue-3);
                f2->SetRange(0,300);
                f2->Draw("SAME");
        c1->Update();

	//Legend for c1 (containing gr1 and gr2)
        leg3 = new TLegend(0.2,0.8,0.48,0.9);
                leg3->AddEntry(gr1,"Room Temperature","p");
                leg3->AddEntry(gr2,"Cryo","p");
        leg3->Draw();

	//Get linear and exponential fits to the room temperature calibration
	TCanvas *c2 = new TCanvas();
	gr4 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr4->Draw("AP");
                gr4->SetMarkerColor(kRed);
	gr5 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr5->Draw("P");
                gr5->SetMarkerColor(kRed);
	TF1 *fit4 = new TF1("fit4","pol1",11,300);
        gr5->Fit("fit4","R");
        TF1 *f4 = gr5->GetFunction("fit4");
                f4->SetName("f4");
                f4->SetLineColor(kBlue-3);
                f4->SetRange(11,300);
                f4->Draw("SAME");
        TF1 *fit5 = new TF1("fit5","pol2",9,11);
        gr4->Fit("fit5","R");
        TF1 *f5 = gr4->GetFunction("fit5");
                f5->SetName("f5");
                f5->SetLineColor(kGreen-3);
                f5->SetRange(9,11);
                f5->Draw("SAME");
	c2->Update();
	
	//Use the above fits (f4 and f5) to convert voltage measured during the measurement into applied field.
	vector<double> ten_voltageTOfield_1;
	for(int i = 0; i < ten_voltage_1.size(); i++)
	{
		if(ten_voltage_1[i] < 2)
		{
			ten_voltageTOfield_1.push_back(0.00);
		}
		else if(ten_voltage_1[i] < 11)
		{
			ten_voltageTOfield_1.push_back(f5->Eval(ten_voltage_1[i]));
		}
		else
		{
			ten_voltageTOfield_1.push_back(f4->Eval(ten_voltage_1[i]));
		}
	}
	
	//Create a measure of the shielding factor, which can also be plotted
	vector<double>  shielded_overlap, shielded_non;
	for (int i = 0; i < ten_voltage_1.size(); i++)
	{
		shielded_overlap.push_back(ten_voltageTOfield_1[i] - ten_field_1[i]);
	}


	//Finally, plot the applied field vs. internal field on the canvas which was passed in by reference
	c00.cd();
        gr12 = new TGraph(ten_time_1.size(),&(ten_voltageTOfield_1[0]),&(ten_field_1[0]));
        //gr12 = new TGraph(ten_time_1.size(),&(shielded_overlap[0]),&(ten_field_1[0]));		//Shielding factor plot
                gr12->Draw("P SAME");
                gr12->GetXaxis()->SetTitle("Applied Field (mT)");
                gr12->GetXaxis()->SetRangeUser(0,125);
                //gr12->GetYaxis()->SetTitle("Magnetic Field within Shield (mT)");
                gr12->GetYaxis()->SetTitle("B_{o} - B_{i} (mT)");
                gr12->GetYaxis()->SetRangeUser(0,125);
                gr12->SetTitle(title.c_str());
                gr12->SetMarkerColor(color);

	//Add this plot to the legend passed in by reference
	leg.AddEntry(gr12,title.c_str(),"p");

	//Clean up, close all canvases but the main one.
	cout << "All done with: " << title << endl;
	c1->Close();
	c2->Close();

}




//*************************************************************************************************************************




//New solenoid calibration using shunt resistor. This one only uses a linear fit of the calibration data.
void Solenoid_calib_single(std::string ten_file_txt, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color)
{
	cout << "*******************************************************" << endl << "Beginning: " << title << endl;
//Measurement Files

	//Read the measurement files into vectors
	std::ifstream ten_file(ten_file_txt.c_str()); 		//Room temperature calibration
	std::ifstream ten_file1(ten_file1_txt.c_str()); 	//Measurement File

        std::string str;
        double ti, xi, yi;
        vector<double> ten_time, ten_field, ten_voltage;
        vector<double> ten_time_1, ten_field_1, ten_voltage_1;

        while (std::getline(ten_file, str))
        {
                if(ten_file >> ti >> xi >> yi)
                {
                        ten_time.push_back(ti); 			//Time
                        ten_voltage.push_back(TMath::Abs(xi)); 		//Current in mA -> Voltage in mV
                        ten_field.push_back(TMath::Abs(yi)); 		//Magnetic Field (mT)
                }
        }

        while (std::getline(ten_file1, str))
        {
                if(ten_file1 >> ti >> xi >> yi)
                {
                        ten_time_1.push_back(ti); 			//Time
                        ten_voltage_1.push_back(TMath::Abs(xi)); 	//Current in mA -> Voltage in mV
                        ten_field_1.push_back(TMath::Abs(yi)); 		//Magnetic Field (mT)
                }
        }

	//Graph the room temperature calibration file: Voltage vs. Field
	TCanvas *c1 = new TCanvas();
        gr1 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr1->Draw("AP");
                gr1->GetXaxis()->SetTitle("Voltage (mV)");
                gr1->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr1->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr1->SetMarkerColor(kRed);
        c1->Update();

	//Graph the cryo measurement file: Voltage vs. Field
        gr2 = new TGraph(ten_time_1.size(),&(ten_voltage_1[0]),&(ten_field_1[0]));
                gr2->Draw("p SAME");
                gr2->GetXaxis()->SetTitle("Voltage (mV)");
                gr2->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr2->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr2->SetMarkerColor(kBlue);
        c1->Update();

	//Legend for c1 (containing gr1 and gr2)
        leg3 = new TLegend(0.2,0.8,0.48,0.9);
                leg3->AddEntry(gr1,"Room Temperature","p");
                leg3->AddEntry(gr2,"Cryo","p");
        leg3->Draw();

	//Get linear fit to the room temperature calibration
	TCanvas *c2 = new TCanvas();
	gr4 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr4->Draw("AP");
                gr4->SetMarkerColor(kRed);
	TF1 *fit4 = new TF1("fit4","pol1",0,2000);
        gr4->Fit("fit4","R");
        TF1 *f4 = gr4->GetFunction("fit4");
                f4->SetName("f4");
                f4->SetLineColor(kBlue-3);
                f4->SetRange(0,2000);
                f4->Draw("SAME");
	c2->Update();

	//Use the above fit to convert voltage measured during the measurement into applied field.
        vector<double> ten_voltageTOfield_1;
        for(int i = 0; i < ten_voltage_1.size(); i++)
        {
		ten_voltageTOfield_1.push_back(f4->Eval(ten_voltage_1[i]));
	}

	//Create a measure of the shielding factor, which can also be plotted
	vector<double>  shielded_overlap;
	for (int i = 0; i < ten_voltage_1.size(); i++)
	{
		shielded_overlap.push_back(ten_voltageTOfield_1[i] - ten_field_1[i]);
	}


	//Finally, plot the applied field vs. internal field on the canvas which was passed in by reference
	c00.cd();
        gr12 = new TGraph(ten_time_1.size(),&(ten_voltageTOfield_1[0]),&(ten_field_1[0]));
                if (n<2) 
		{
			gr12->Draw("AP");
		}
		else
		{
			gr12->Draw("P SAME");
		}
                gr12->GetXaxis()->SetTitle("Applied Field (mT)");
                gr12->GetXaxis()->SetRangeUser(0,125);
                gr12->GetYaxis()->SetTitle("Magnetic Field within Shield (mT)");
                gr12->GetYaxis()->SetRangeUser(0,125);
                gr12->SetTitle(title.c_str());
                gr12->SetMarkerColor(color);

	//Some optional linear extrapolation to higher fields
	if(lin_extrap)
	{
		TF1 *fit6 = new TF1("fit6","pol1",25,2000);
		gr12->Fit("fit6","R");
		TF1 *f6 = gr12->GetFunction("fit6");
			f6->SetName("f6");
			f6->SetLineColor(color);
			f6->SetRange(0,2000);
			double y_int = f6->GetParameter(0);
			f6->Delete();
		TF1 *f7 = new TF1("f7","pol1",0,2000);
			f7->SetParameter(0,y_int);
			f7->SetParameter(1,1);
			f7->SetLineStyle(3);
			f7->Draw("SAME");
	}

	c00.Update();
	
	//Add the above plot(s) to the legend passed in by reference
	leg.AddEntry(gr12,title.c_str(),"p");
	if(lin_extrap)
	{
		std::string title_ext = title + " Linearly Extrapolated";
		leg.AddEntry(f7,title_ext.c_str(),"l");
	}


	//Clean up open canvases.
	c1->Close();
	c2->Close();

	cout << "All done with: " << title << endl;
}


//*************************************************************************************************************************


//New solenoid calibration using shunt resistor. This one only uses a linear fit of the calibration data.
//This takes an output file generated by plot_output.py (found here: https://github.com/jlabounty/Summer2016/blob/master/ShieldTest/plot_output.py) and no calibration file.
//This will automatically run the python script (needs to be in the same directory as the root script)
void shield_plot_python(std::string file_path, std::string summary_file_path, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color, std::string helm_or_sol, std::string negate, int file_end)
{
	cout << "*******************************************************" << endl << "Beginning: " << title << endl;
//Measurement Files

	std::string file_end_str = Form("%d",file_end);

	std::string python_input = "python plot_output.py \""+file_path+"\" \""+ten_file1_txt+"\" "+negate+" "+helm_or_sol+" "+file_end_str;

	cout << "       Python Command Executed for: " << ten_file1_txt << endl;
	cout << "       [If this plot looks odd, you may have to adjust the parameters in 'plot_output.py' directly]" << endl;
	gSystem->Exec(python_input.c_str());


	if(exists_test((summary_file_path+"SummaryOf_"+ten_file1_txt).c_str()))
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

        while (std::getline(ten_file1, str))
        {
                if(ten_file1 >> AppliedField_i >> AppliedField_err_i >> InternalField_i >> InternalField_err_i)
                {
                        AppliedField.push_back(AppliedField_i); 			//Calculated Applied Field
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
//	gr12 = new TGraphErrors(AppliedField.size(),&(AppliedField[0]),&(InternalField[0]),&(AppliedField_err[0]),&(InternalField_err[0]));
	gr12 = new TGraphErrors(AppliedField.size(),&(AppliedField[0]),&(InternalField[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr12->Draw("PL SAME");
                gr12->SetMarkerColor(color);
                gr12->SetLineColor(color);

	//Some optional linear extrapolation to higher fields
	if(lin_extrap)
	{
		TF1 *fit6 = new TF1("fit6","pol1",25,2000);
		gr12->Fit("fit6","R");
		TF1 *f6 = gr12->GetFunction("fit6");
			f6->SetName("f6");
			f6->SetLineColor(color);
			f6->SetRange(0,2000);
			double y_int = f6->GetParameter(0);
			f6->Delete();
		TF1 *f7 = new TF1("f7","pol1",0,2000);
			f7->SetParameter(0,y_int);
			f7->SetParameter(1,1);
			f7->SetLineStyle(3);
			f7->Draw("SAME");
	}

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


//*************************************************************************************************************************

//New solenoid calibration using shunt resistor. This one only uses a linear fit of the calibration data.
//This takes an output file generated by plot_output.py (found here: https://github.com/jlabounty/Summer2016/blob/master/ShieldTest/plot_output.py) and no calibration file.
//This will NOT automatically run the python script, but will instead look for an existing output file (needs to be in the same directory as the file would normally be created)
void shield_plot_python_existing(std::string file_path, std::string summary_file_path, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color, std::string helm_or_sol, std::string negate)
{
	cout << "*******************************************************" << endl << "Beginning: " << title << endl;
//Measurement Files

	cout << "Python Command NOT Executed for: " << ten_file1_txt << endl;
	cout << "Reading Existing File." << endl;

	//Read the measurement summary file into vectors if it exists
	if(exists_test((summary_file_path+"SummaryOf_"+ten_file1_txt).c_str()))
	{
		cout << "       Summary File Found. Proceeding with Plot" << endl;
	} 
	else 
	{
		cerr << "ERROR: Summary File for " << ten_file1_txt << " not found. Aborting." << endl;
		return -1;
	}   
	std::ifstream ten_file1((summary_file_path+"SummaryOf_"+ten_file1_txt).c_str()); 	//Measurement File

        std::string str;
        double AppliedField_i, AppliedField_err_i, InternalField_i, InternalField_err_i;
        vector<double> AppliedField, AppliedField_err, AppliedField_Noerr, InternalField, InternalField_err;

        while (std::getline(ten_file1, str))
        {
                if(ten_file1 >> AppliedField_i >> AppliedField_err_i >> InternalField_i >> InternalField_err_i)
                {
                        AppliedField.push_back(AppliedField_i); 			//Calculated Applied Field
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
//	gr12 = new TGraphErrors(AppliedField.size(),&(AppliedField[0]),&(InternalField[0]),&(AppliedField_err[0]),&(InternalField_err[0]));
	gr12 = new TGraphErrors(AppliedField.size(),&(AppliedField[0]),&(InternalField[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr12->Draw("P SAME");
                gr12->SetMarkerColor(color);

	//Some optional linear extrapolation to higher fields
	if(lin_extrap)
	{
		TF1 *fit6 = new TF1("fit6","pol1",25,2000);
		gr12->Fit("fit6","R");
		TF1 *f6 = gr12->GetFunction("fit6");
			f6->SetName("f6");
			f6->SetLineColor(color);
			f6->SetRange(0,2000);
			double y_int = f6->GetParameter(0);
			f6->Delete();
		TF1 *f7 = new TF1("f7","pol1",0,2000);
			f7->SetParameter(0,y_int);
			f7->SetParameter(1,1);
			f7->SetLineStyle(3);
			f7->Draw("SAME");
	}

	c00.Update();
	
	//Add the above plot(s) to the legend passed in by reference
	leg.AddEntry(gr12,title.c_str(),"p");
	if(lin_extrap)
	{
		std::string title_ext = title + " Linearly Extrapolated";
		leg.AddEntry(f7,title_ext.c_str(),"l");
	}


	cout << "All done with: " << title << endl;
}


//*************************************************************************************************************************

//Plots shielding factor rather than simply internal vs. external field
//New solenoid calibration using shunt resistor. This one only uses a linear fit of the calibration data.
//This takes an output file generated by plot_output.py (found here: https://github.com/jlabounty/Summer2016/blob/master/ShieldTest/plot_output.py) and no calibration file.
//This will automatically run the python script (needs to be in the same directory as the root script)
void shield_factor_plot_python(std::string file_path, std::string summary_file_path, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color, std::string helm_or_sol, std::string negate, int file_end)
{
	cout << "*******************************************************" << endl << "Beginning (Shielding Factor): " << title << endl;
//Measurement Files

	std::string file_end_str = Form("%d",file_end);

	std::string python_input = "python plot_output.py \""+file_path+"\" \""+ten_file1_txt+"\" "+negate+" "+helm_or_sol+" "+file_end_str;

	cout << "       Python Command Executed for: " << ten_file1_txt << endl;
	cout << "       [If this plot looks odd, you may have to adjust the parameters in 'plot_output.py' directly]" << endl;
	gSystem->Exec(python_input.c_str());

	//Read the measurement summary file into vectors if it exists
	if(exists_test((summary_file_path+"SummaryOf_"+ten_file1_txt).c_str()))
	{
		cout << "       Summary File Found. Proceeding with Plot" << endl;
	} 
	else 
	{
		cerr << "ERROR: Summary File for " << ten_file1_txt << " not found. Aborting." << endl;
		return -1;
	}   
	std::ifstream ten_file1((summary_file_path+"SummaryOf_"+ten_file1_txt).c_str()); 	//Measurement File

        std::string str;
        double AppliedField_i, AppliedField_err_i, InternalField_i, InternalField_err_i;
        vector<double> AppliedField, AppliedField_err, AppliedField_Noerr, InternalField, InternalField_err;

        while (std::getline(ten_file1, str))
        {
                if(ten_file1 >> AppliedField_i >> AppliedField_err_i >> InternalField_i >> InternalField_err_i)
                {
                        AppliedField.push_back(AppliedField_i); 			//Calculated Applied Field
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

	vector<double> ShieldingFactor;
	for(int i = 0; i < AppliedField.size(); i++)
	{
		ShieldingFactor.push_back( AppliedField[i] - InternalField[i] );
	}

	//Plot the applied field vs. internal field on the canvas which was passed in by reference
	c00.cd();
	gr12 = new TGraphErrors(AppliedField.size(),&(AppliedField[0]),&(ShieldingFactor[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr12->Draw("PL SAME");
                gr12->SetMarkerColor(color);
                gr12->SetLineColor(color);

	//Some optional linear extrapolation to higher fields
	if(lin_extrap)
	{
		TF1 *fit6 = new TF1("fit6","pol1",25,2000);
		gr12->Fit("fit6","R");
		TF1 *f6 = gr12->GetFunction("fit6");
			f6->SetName("f6");
			f6->SetLineColor(color);
			f6->SetRange(0,2000);
			double y_int = f6->GetParameter(0);
			f6->Delete();
		TF1 *f7 = new TF1("f7","pol1",0,2000);
			f7->SetParameter(0,y_int);
			f7->SetParameter(1,1);
			f7->SetLineStyle(3);
			f7->Draw("SAME");
	}

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


//*************************************************************************************************************************


//Calibration for the measurements done in Xu Du's magnet with a fixed calibration factor of 58.8 mT/Amp. These files are also slightly different, in that they do not contain a timestamp. Thankfully, we know that each measurement was taken once per second so that can be simply created. This function was also written slightly later, so the file is read in as a TTree rather than using iostream.
int Helium_calib_single(std::string helium_file, std::string title, TCanvas &c00, TLegend &leg, int color)
{
	cout << "*******************************************************" << endl << "Beginning: " << title << endl;
	//Open the measurement file. Read the two columns of data into two vectors.
	TTree *t1 = new TTree();
	if(exists_test((helium_file).c_str()))
	{
		cout << "       Summary File Found. Proceeding with Plot" << endl;
	} 
	else 
	{
		cerr << "ERROR: Data File for " << helium_file << " not found. Aborting." << endl;
		return -1;
	}   
	t1->ReadFile(helium_file.c_str(),"amperage:field");

	//The vectors have to be drawn before they can be read in. Temporary canvas created for this purpose and then closed immedietly after.
	TCanvas *ctemp = new TCanvas();	
	t1->Draw("amperage:field");
	ctemp->Close();

	//Number of seconds created from size of TTree (1 measurement per second fixed rate)
	vector<double> seconds;
	for(int i = 0; i < t1->GetEntries(); i++)
	{
		seconds.push_back(i);
	}

	vector<double> fields;
	for ( int i = 0; i < t1->GetEntries(); i++ )
	{
		fields.push_back( -1*t1->GetV2()[i] );	//Hall probe was oriented in the opposite direction to the fiels, this simply flips the field from negative to positive.
	}


	vector<double> amps;
	for ( int i = 0; i < t1->GetEntries(); i++ )
	{
		amps.push_back( t1->GetV1()[i] );
	}


	//Each measurement file contains a ramp up and ramp down. This finds the turning point and makes sure we only plot the data before that point.
	int seconds_min;
	for (int i = 1; i< seconds.size(); i++)
	{
		if(TMath::Abs(amps[i]) > TMath::Abs(amps[i-1])) seconds_min = seconds[i];
	}

	//Converts the amperage into applied field using the conversion given by Xu Du
	vector<double> applied_fields;
	for ( int i = 0; i < t1->GetEntries(); i++ )
	{
		applied_fields.push_back( t1->GetV1()[i]*58.8 ); 	//58.8 mT per A conversion given by Xu Du
	}



	//Plots Internal field vs. external field for ramp up
        TCanvas *c1 = new TCanvas();
        gr1 = new TGraph(seconds_min,&(applied_fields[0]),&(fields[0]));
                gr1->Draw("AP");
                gr1->GetXaxis()->SetTitle("External Field (mT)");
                gr1->GetYaxis()->SetTitle("Internal Field (mT)");
                gr1->SetTitle("Internal vs. External Magnetic Field");
                gr1->SetMarkerColor(kGreen);

	//Plots internal field vs. time for whole measurement
	TCanvas *c2 = new TCanvas();
        gr2 = new TGraph(seconds.size(),&(seconds[0]),&(fields[0]));
                gr2->Draw("AP");
                gr2->GetXaxis()->SetTitle("Time (seconds)");
                gr2->GetYaxis()->SetTitle("Internal Field (mT)");
                gr2->SetTitle("Internal Field vs. Time");
                gr2->SetMarkerColor(kBlue);

	//Plots internal field vs. external field on the canvas which was passed in by reference
	c00.cd();
        gr3 = new TGraph(seconds_min,&(applied_fields[0]),&(fields[0]));
	gr3->Draw("p SAME");
                gr3->GetXaxis()->SetTitle("External Field (mT)");
                gr3->GetYaxis()->SetTitle("Internal Field (mT)");
                gr3->SetTitle("Internal vs. External Magnetic Field");
                gr3->SetMarkerColor(color);

	//Add this plot to the legend passed in by reference
	leg.AddEntry(gr3,title.c_str(),"p");

	//Clean up, close unwanted canvases
	c1->Close();
	c2->Close();
	cout << "All done with: " << title << endl;

	return 0;
}




//*************************************************************************************************************************




//Deprecated version of the function kept here for some backwards compatability. To use in place of the newer version simply change the name to Solenoid_calib_single_old(...) and change the new one to something different.
void Solenoid_calib_single_old_2(std::string ten_file_txt, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color)
{
	cout << "*******************************************************" << endl << "Beginning: " << title << endl;
//Measurement Files

	std::ifstream ten_file(ten_file_txt.c_str()); //first run no cooling
	std::ifstream ten_file1(ten_file1_txt.c_str()); //second run with cooling

        std::string str;
        double ti, xi, yi;
        vector<double> ten_time, ten_field, ten_voltage;
        vector<double> ten_time_1, ten_field_1, ten_voltage_1;

        while (std::getline(ten_file, str))
        {
                if(ten_file >> ti >> xi >> yi)
                {
                        ten_time.push_back(ti); //Time
                        ten_voltage.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
                        ten_field.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
                }
        }

        while (std::getline(ten_file1, str))
        {
                if(ten_file1 >> ti >> xi >> yi)
                {
                        ten_time_1.push_back(ti); //Time
                        ten_voltage_1.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
                        ten_field_1.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
                }
        }



	TCanvas *c1 = new TCanvas();
        gr1 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr1->Draw("AP");
                gr1->GetXaxis()->SetTitle("Voltage (mV)");
                gr1->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr1->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr1->SetMarkerColor(kRed);
        TF1 *fit1 = new TF1("fit1","pol1",13,300);
        gr1->Fit("fit1","R");
        TF1 *f1 = gr1->GetFunction("fit1");
                f1->SetName("f1");
                f1->SetLineColor(kRed);
                f1->SetRange(0,300);
                f1->Draw("SAME");
        c1->Update();

        gr2 = new TGraph(ten_time_1.size(),&(ten_voltage_1[0]),&(ten_field_1[0]));
                gr2->Draw("p SAME");
                gr2->GetXaxis()->SetTitle("Voltage (mV)");
                gr2->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr2->SetTitle("Voltage from Power Supply vs. Magnetic Field --- 10 minutes");
                gr2->SetMarkerColor(kBlue);
        TF1 *fit2 = new TF1("fit2","pol1",60,300);
        gr2->Fit("fit2","R");
        TF1 *f2 = gr2->GetFunction("fit2");
                f2->SetName("f2");
                f2->SetLineColor(kBlue-3);
                f2->SetRange(0,300);
                f2->Draw("SAME");
        c1->Update();
/*
        gr3 = new TGraph(ten_time_2.size(),&(ten_voltage_2[0]),&(ten_field_2[0]));
                gr3->Draw("p SAME");
                gr3->GetXaxis()->SetTitle("Voltage (mV)");
                gr3->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr3->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr3->SetMarkerColor(kGreen);
        TF1 *fit3 = new TF1("fit3","pol1",60,300);
        gr3->Fit("fit3","R");
        TF1 *f3 = gr3->GetFunction("fit3");
                f3->SetName("f2");
                f3->SetLineColor(kGreen-3);
                f3->SetRange(0,300);
                f3->Draw("SAME");
*/        c1->Update();

        leg3 = new TLegend(0.2,0.8,0.48,0.9);
                leg3->AddEntry(gr1,"Room Temperature","p");
                leg3->AddEntry(gr2,"Cryo","p");
//                leg3->AddEntry(gr3,"Cryo After Baking","p");
        leg3->Draw();

	TCanvas *c2 = new TCanvas();
	gr4 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr4->Draw("AP");
                gr4->SetMarkerColor(kRed);
	gr5 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr5->Draw("P");
                gr5->SetMarkerColor(kRed);
	TF1 *fit4 = new TF1("fit4","pol1",11,300);
        gr5->Fit("fit4","R");
        TF1 *f4 = gr5->GetFunction("fit4");
                f4->SetName("f4");
                f4->SetLineColor(kBlue-3);
                f4->SetRange(11,300);
                f4->Draw("SAME");
        TF1 *fit5 = new TF1("fit5","pol2",9,11);
        gr4->Fit("fit5","R");
        TF1 *f5 = gr4->GetFunction("fit5");
                f5->SetName("f5");
                f5->SetLineColor(kGreen-3);
                f5->SetRange(9,11);
                f5->Draw("SAME");
	c2->Update();

        vector<double> ten_voltageTOfield_1;

        for(int i = 0; i < ten_voltage_1.size(); i++)
        {
//                      ten_voltageTOfield_1.push_back(f4->Eval(ten_voltage_1[i]));

                if(ten_voltage_1[i] < 2)
                {
                        ten_voltageTOfield_1.push_back(0.00);
                }
                if(ten_voltage_1[i] < 11)
                {
                        ten_voltageTOfield_1.push_back(f5->Eval(ten_voltage_1[i]));
                }
                else
                {
                        ten_voltageTOfield_1.push_back(f4->Eval(ten_voltage_1[i]));
                }
	}


	c00.cd();
        gr12 = new TGraph(ten_time_1.size(),&(ten_voltageTOfield_1[0]),&(ten_field_1[0]));
                if (n<2) 
		{
			gr12->Draw("AP");
		}
		else
		{
			gr12->Draw("P SAME");
		}
                gr12->GetXaxis()->SetTitle("Applied Field (mT)");
                gr12->GetXaxis()->SetRangeUser(0,125);
                gr12->GetYaxis()->SetTitle("Magnetic Field within Shield (mT)");
                gr12->GetYaxis()->SetRangeUser(0,125);
                gr12->SetTitle(title.c_str());
		if(n == 5) n++; //I dont like yellow on graphs
                gr12->SetMarkerColor(color);
/*
        TF1 *onetoone = new TF1("onetoone","pol1",0,1500);
		onetoone->SetParameter(0,0);
		onetoone->SetParameter(1,1);
		onetoone->SetLineColor(kBlack);
		onetoone->SetLineStyle(7);
	onetoone->Draw("SAME");
*/
	if(lin_extrap)
	{
		TF1 *fit6 = new TF1("fit6","pol1",25,2000);
		gr12->Fit("fit6","R");
		TF1 *f6 = gr12->GetFunction("fit6");
			f6->SetName("f6");
			f6->SetLineColor(color);
			f6->SetRange(0,2000);
			double y_int = f6->GetParameter(0);
			f6->Delete();
		TF1 *f7 = new TF1("f7","pol1",0,2000);
			f7->SetParameter(0,y_int);
			f7->SetParameter(1,1);
			f7->SetLineStyle(3);
			f7->Draw("SAME");
	}
//	c00.SetFixedAspectRatio();
	c00.Update();
	
	n++;
//	leg.AddEntry(gr12,(title+": Old Calibration").c_str(),"p");
	leg.AddEntry(gr12,(title).c_str(),"p");
	if(lin_extrap)
	{
		std::string title_ext = title + " Linearly Extrapolated";
		leg.AddEntry(f7,title_ext.c_str(),"l");
	}


//	c1->Close();
//	c2->Close();
//	c3->Close();

//	gr2->SetMarkerColor(kRed);
//	gr2->Draw("p SAME");
//	onetoone->Draw("l SAME");

//	gr1->SetMarkerColor(kBlue);
//	gr1->Draw("p SAME");

	cout << "All done with: " << title << endl;

}

