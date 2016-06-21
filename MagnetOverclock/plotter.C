#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>
#include "TGraph.h"

int plotter()
{

	//Measurement 1: 4/11/16
	std::ifstream file("./Round1/DataFile_160411_120720.txt"); //First run no cooling
	std::ifstream file1("./Round1/DataFile_160411_122618.txt"); //Second run no cooling
	std::ifstream file2("./Round1/DataFile_160411_124012.txt"); //First run cryo

	//Measurement 2: 4/15/16
	std::ifstream file3("./Round2/DataFile_160415_152018.txt"); //Room temperature
	std::ifstream file4("./Round2/DataFile_160415_154319.txt"); //Cryo 

	std::string str;
	
	//------------------------------------------------------------------------

	vector<double> v_t, v_x, v_y;
	double ti, xi, yi;
        while (std::getline(file, str))
        {
		if(file >> ti >> xi >> yi)
		{
			v_t.push_back(ti); //Time
			v_x.push_back(xi); //Current in mA -> Voltage in mV
			v_y.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}

	}
	
	vector<double> v_x_NORM, v_y_NORM;
	double x_max, y_max;
	x_max = *std::max_element(v_x.begin(), v_x.end());
	y_max = *std::max_element(v_y.begin(), v_y.end());
	cout << x_max << " | " << y_max << endl;
	for(int i = 0; i < v_x.size(); i++)
	{
		v_x_NORM.push_back(v_x[i] / x_max);
		v_y_NORM.push_back(v_y[i] / y_max);
	}

	TCanvas *c1 = new TCanvas();
	gr1 = new TGraph(v_x.size(),&(v_x[0]),&(v_y[0]));
		gr1->Draw("AP");
		gr1->GetXaxis()->SetTitle("Voltage (mV)");
		gr1->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr1->SetTitle("Voltage from Power Supply vs. Magentic Field");
		gr1->SetMarkerColor(kRed);
	c1->Update();

	TCanvas *c5 = new TCanvas();
//	gr5 = new TGraph(v_t.size(),&(v_t[0]),&(v_y_NORM[0]));
	gr5 = new TGraph(230,&(v_t[0]),&(v_y[0]));
		gr5->Draw("AP");
		gr5->GetXaxis()->SetTitle("Time (Seconds)");
		gr5->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr5->SetTitle("Magentic Field vs. Time");
		gr5->SetMarkerColor(kRed);
	c5->Update();

	TCanvas *c8 = new TCanvas();
//	gr8 = new TGraph(v_t.size(),&(v_t[0]),&(v_x_NORM[0]));
	gr8 = new TGraph(230,&(v_t[0]),&(v_x[0]));
		gr8->Draw("AP");
		gr8->GetXaxis()->SetTitle("Time (Seconds)");
		gr8->GetYaxis()->SetTitle("Current from Power Supply (A)");
		gr8->SetTitle("Current vs. Time");
		gr8->SetMarkerColor(kRed);
	c8->Update();
/*
	TCanvas *c9 = new TCanvas();
		gr5->Draw("AP");
		gr8->Draw("P SAME");
		c9->SetTitle("Normalized Current and Field");
	c9->Update();
*/
	//------------------------------------------------------------------------

	vector<double> v_t1, v_x1, v_y1;
	double ti1, xi1, yi1;
        while (std::getline(file1, str))
        {
		if(file1 >> ti1 >> xi1 >> yi1)
		{
			v_t1.push_back(ti1); //Time
			v_x1.push_back(xi1); //Current in mA -> Voltage in mV
			v_y1.push_back(TMath::Abs(yi1)); //Magnetic Field (mT)
		}

	}

	double x1_max, y1_max;
	x1_max = *std::max_element(v_x1.begin(), v_x1.end());
	y1_max = *std::max_element(v_y1.begin(), v_y1.end());
	cout << x1_max << " | " << y1_max << endl;

	TCanvas *c4 = new TCanvas();
//	gr3 = new TGraph(v_x1.size(),&(v_x1[0]),&(v_y1[0]));
	gr3 = new TGraph(250,&(v_x1[0]),&(v_y1[0]));
		gr3->Draw("AP");
		gr3->GetXaxis()->SetTitle("Voltage (mV)");
		gr3->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr3->SetTitle("Voltage from Power Supply vs. Magentic Field");
		gr3->SetMarkerColor(kGreen);
		gr3->SetLineColor(kGreen);
	c4->Update();

	TCanvas *c6 = new TCanvas();
//	gr6 = new TGraph(v_t1.size(),&(v_t1[0]),&(v_y1[0]));
	gr6 = new TGraph(250,&(v_t1[0]),&(v_y1[0]));
		gr6->Draw("AP");
		gr6->GetXaxis()->SetTitle("Time (Seconds)");
		gr6->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr6->SetTitle("Magentic Field vs. Time");
		gr6->SetMarkerColor(kGreen);
	c6->Update();

	//------------------------------------------------------------------------

	vector<double> v_t2, v_x2, v_y2;
	double ti2, xi2, yi2;
        while (std::getline(file2, str))
        {
		if(file2 >> ti2 >> xi2 >> yi2)
		{
			v_t2.push_back(ti2); //Time
			v_x2.push_back(xi2); //Current in mA -> Voltage in mV
			v_y2.push_back(TMath::Abs(yi2)); //Magnetic Field (mT)
		}

	}

	double x2_max, y2_max;
	x2_max = *std::max_element(v_x2.begin(), v_x2.end());
	y2_max = *std::max_element(v_y2.begin(), v_y2.end());
	cout << x2_max << " | " << y2_max << endl;
	TCanvas *c2 = new TCanvas();
//	gr2 = new TGraph(v_x2.size(),&(v_x2[0]),&(v_y2[0]));
	gr2 = new TGraph(200,&(v_x2[0]),&(v_y2[0]));
		gr2->Draw("AP");
		gr2->GetXaxis()->SetTitle("Voltage (mV) [= Current (A)]");
		gr2->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr2->SetTitle("Voltage from Power Supply vs. Magentic Field");
		gr2->SetMarkerColor(kBlue);
		gr2->SetLineColor(kBlue);
	c2->Update();

	TCanvas *c7 = new TCanvas();
//	gr7 = new TGraph(v_t2.size(),&(v_t2[0]),&(v_y2[0]));
	gr7 = new TGraph(200,&(v_t2[0]),&(v_y2[0]));
		gr7->Draw("AP");
		gr7->GetXaxis()->SetTitle("Time (Seconds)");
		gr7->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr7->SetTitle("Magentic Field vs. Time");
		gr7->SetMarkerColor(kBlue);
	c7->Update();
	//------------------------------------------------------------------------
/*
	TCanvas *c3 = new TCanvas();
	gr2->Draw("AP");
	TF1 *expdecay = new TF1("expdecay","[0]*(1-TMath::Exp(-1*[1]*x))",0,20);
		expdecay->SetParameter(0,100);
		expdecay->SetParameter(1,2);
		gr2->Fit("expdecay");
		TF1 *f3 = gr2->GetFunction("expdecay");
		f3->SetName("f3");
		f3->SetLineColor(kBlue-3);
		f3->SetRange(0,35);
	f3->Draw("SAME");
	gr1->Draw("P SAME");
	TF1 *fit1 = new TF1("fit1","pol1",0,10);
	gr1->Fit("pol1");
	TF1 *f4 = gr1->GetFunction("pol1");
		f4->SetName("f4");
		f4->SetLineColor(kRed);
		f4->SetRange(0,12);
	gr3->Draw("P SAME");
	leg = new TLegend(0.2,0.8,0.48,0.9);
		leg->AddEntry(gr1,"Room Temperature (Run 1)","p");
		leg->AddEntry(f4,"Fit: Room Temperature (Run 1)","l");
		leg->AddEntry(gr3,"Room Temperature (Run 2)","p");
		leg->AddEntry(gr2,"Cryo","p");
		leg->AddEntry(f3,"Fit: Cryo -> a*(1-e^(-b*x))","l");
	leg->Draw();
	c3->Update();
*/
	//------------------------------------------------------------------------

	vector<double> v_t3, v_x3, v_y3;
	double ti3, xi3, yi3;
        while (std::getline(file3, str))
        {
		if(file3 >> ti3 >> xi3 >> yi3)
		{
			v_t3.push_back(ti3); //Time
			v_x3.push_back(xi3); //Current in mA -> Voltage in mV
			v_y3.push_back(TMath::Abs(yi3)); //Magnetic Field (mT)
		}

	}

	double x3_max, y3_max;
	x3_max = *std::max_element(v_x3.begin(), v_x3.end());
	y3_max = *std::max_element(v_y3.begin(), v_y3.end());
	cout << x3_max << " | " << y3_max << endl;

	TCanvas *c9 = new TCanvas();
	gr9 = new TGraph(v_x3.size(),&(v_x3[0]),&(v_y3[0]));
//	gr3 = new TGraph(250,&(v_x1[0]),&(v_y1[0]));
		gr9->Draw("AP");
		gr9->GetXaxis()->SetTitle("Voltage (mV)");
		gr9->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr9->SetTitle("Voltage from Power Supply vs. Magentic Field");
		gr9->SetMarkerColor(kGreen+4);
		gr9->SetLineColor(kGreen+4);
	c9->Update();

	TCanvas *c8 = new TCanvas();
	gr8 = new TGraph(v_t3.size(),&(v_t3[0]),&(v_y3[0]));
//	gr6 = new TGraph(250,&(v_t1[0]),&(v_y1[0]));
		gr8->Draw("AP");
		gr8->GetXaxis()->SetTitle("Time (Seconds)");
		gr8->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr8->SetTitle("Magentic Field vs. Time");
		gr8->SetMarkerColor(kGreen+4);
	c8->Update();


	//------------------------------------------------------------------------

	vector<double> v_t4, v_x4, v_y4;
	double ti4, xi4, yi4;
        while (std::getline(file4, str))
        {
		if(file4 >> ti4 >> xi4 >> yi4)
		{
			v_t4.push_back(ti4); //Time
			v_x4.push_back(xi4); //Current in mA -> Voltage in mV
			v_y4.push_back(TMath::Abs(yi4)); //Magnetic Field (mT)
		}

	}

	double x4_max, y4_max;
	x4_max = *std::max_element(v_x4.begin(), v_x4.end());
	y4_max = *std::max_element(v_y4.begin(), v_y4.end());
	cout << x4_max << " | " << y4_max << endl;
	TCanvas *c12 = new TCanvas();
//	gr12 = new TGraph(v_x4.size(),&(v_x4[0]),&(v_y4[0]));
	gr12 = new TGraph(500,&(v_x4[0]),&(v_y4[0]));
		gr12->Draw("AP");
		gr12->GetXaxis()->SetTitle("Voltage (mV) [= Current (A)]");
		gr12->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr12->SetTitle("Voltage from Power Supply vs. Magentic Field");
		gr12->SetMarkerColor(kBlue);
		gr12->SetLineColor(kBlue);
	c12->Update();

	TCanvas *c13 = new TCanvas();
//	gr13 = new TGraph(v_t4.size(),&(v_t4[0]),&(v_y4[0]));
	gr13 = new TGraph(500,&(v_t4[0]),&(v_y4[0]));
		gr13->Draw("AP");
		gr13->GetXaxis()->SetTitle("Time (Seconds)");
		gr13->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr13->SetTitle("Magentic Field vs. Time");
		gr13->SetMarkerColor(kBlue);
	c13->Update();

	//------------------------------------------------------------------------

	TCanvas *c14 = new TCanvas();
	gr1->Draw("AP");
	gr1->Fit("pol1");
	TF1 *f4 = gr1->GetFunction("pol1");
		f4->SetName("f4");
		f4->SetLineColor(kRed);
		f4->SetRange(0,12);
	f4->Draw("SAME");
	gr3->Draw("P SAME");
	gr9->Draw("P SAME");
	gr9->Fit("pol1");
	TF1 *f4 = gr1->GetFunction("pol1");
	TF1 *f9 = gr9->GetFunction("pol1");
		f9->SetName("f9");
		f9->SetLineColor(kGreen+4);
		f9->SetRange(0,12);
	f9->Draw("SAME");
	leg2 = new TLegend(0.2,0.8,0.48,0.9);
		leg2->AddEntry(gr1,"Room Temperature (Day 1, Run 1)","p");
		leg2->AddEntry(f4,"Fit: Room Temperature (Day 1, Run 1)","l");
		leg2->AddEntry(gr3,"Room Temperature (Day 1, Run 2)","p");
		leg2->AddEntry(gr9,"Room Temperature (Day 2, Run 1)","p");
		leg2->AddEntry(f9,"Fit: Room Temperature (Day 2, Run 1)","l");
	leg2->Draw();
	c14->Update();

	TCanvas *c15 = new TCanvas();
	gr12->Draw("AP");
		gr12->SetLineColor(kBlue-3);
/*	TF1 *expdecay2 = new TF1("expdecay2","[0]*(1-TMath::Exp(-1*[1]*x))",0,40);
		expdecay2->SetParameter(0,100);
		expdecay2->SetParameter(1,2);
		gr12->Fit("expdecay2");
		TF1 *f12 = gr12->GetFunction("expdecay2");
		f12->SetName("f12");
		f12->SetLineColor(kBlue-3);
		f12->SetRange(0,40);
	f12->Draw("SAME");
*/	gr2->Draw("P SAME");
		gr2->SetMarkerColor(kRed-3);
//	f3->Draw("SAME");
//		f3->SetLineColor(kWhite);
	leg3 = new TLegend(0.2,0.8,0.48,0.9);
		leg3->AddEntry(gr2,"Cryo, Trial 1","p");
//		leg3->AddEntry(f3,"Fit: Cryo, Trial 1 -> a*(1-e^(-b*x))","l");
		leg3->AddEntry(gr12,"Cryo, Trial 2","p");
//		leg3->AddEntry(f12,"Fit: Cryo, Trial 2 -> a*(1-e^(-b*x))","l");
	leg3->Draw();
	c15->Update();


	//------------------------------------------------------------------------

	vector<double> v_Field_Cryo, v_Current_Cryo, v_Voltage_Cryo;
	double FCi, CCi, VCi;

//	std::ifstream fileCryo("./Table_Cryo.txt"); //Cryo 
	std::ifstream fileCryo("./Table_RoomTemp.txt"); //Cryo 
        while (std::getline(fileCryo, str))
        {
		if(fileCryo >> FCi >> CCi >> VCi)
		{
			v_Field_Cryo.push_back(TMath::Abs(FCi)); //Time
			v_Current_Cryo.push_back(TMath::Abs(CCi)); //Current in mA -> Voltage in mV
			v_Voltage_Cryo.push_back(TMath::Abs(VCi)); //Magnetic Field (mT)
		}

	}

	TCanvas *c16 = new TCanvas();
	gr16 = new TGraph(v_Field_Cryo.size(),&(v_Current_Cryo[0]),&(v_Field_Cryo[0]));
		gr16->Draw("AP");
		gr16->GetXaxis()->SetTitle("Current in Shunt(A)");
		gr16->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr16->SetTitle("Magentic Field vs. Shunt Resistor Current");
		gr16->SetMarkerColor(kBlue);
	c16->Update();

	TCanvas *c17 = new TCanvas();
	gr17 = new TGraph(v_Voltage_Cryo.size(),&(v_Voltage_Cryo[0]),&(v_Current_Cryo[0]));
		gr17->Draw("AP");
		gr17->GetYaxis()->SetTitle("Current in Shunt (A)");
		gr17->GetXaxis()->SetTitle("Voltage across magnet (V)");
		gr17->SetTitle("Magnet Voltage vs. Shunt Resistor Current");
		gr17->SetMarkerColor(kBlue);
	c17->Update();

	TCanvas *c18 = new TCanvas();
	gr18 = new TGraph(v_Voltage_Cryo.size(),&(v_Voltage_Cryo[0]),&(v_Field_Cryo[0]));
		gr18->Draw("AP");
		gr18->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr18->GetXaxis()->SetTitle("Voltage across magnet (V)");
		gr18->SetTitle("Magnet Voltage vs. Magnetic Field");
		gr18->SetMarkerColor(kBlue);
	c18->Update();


	return 0;
}
