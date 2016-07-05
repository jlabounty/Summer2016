#include "HeliumPlots.h"

int HeliumPlots()
{
	int color = 1;		//Integer corresponding to the color of the plot which is created. Each time a plot is created this integer should be iterated.
	gStyle->SetOptStat(0);	//Prevents fit information from bring printed on the canvas.
	std::string title = "";	//Title of the canvas which will host all of the plots
	bool plot_reference_line = true;	//If true will plot a 1:1 reference line on top of the plots.
	bool plot_rapheal = false; 		//If true will plot rapheals measurement on top of new measurement
	bool draw_legend = true;		//If true will draw legend on plot
	bool print_jpg = false;			//Prints canvas automatically to Plot.jpg
	bool fixed_aspect_ratio = true;		//If true fixes output canvas aspect ratio to 1:1

	double ymin, ymax, xmax, xmin;
	ymin = 0;	//Defines the range of the canvas which will be printed in x and y
	ymax = 125;
	xmin = 0;
	xmax = ymax;

	//Create canvas to draw all of the plots on. This will be passed by reference into each function below.
	TCanvas *c00 = new TCanvas();
	c00->cd();
	//Use blank histogram to set the parameters of the canvas
        TH1F *blank = new TH1F("blank",title.c_str(),10,xmin,xmax);
                blank->GetYaxis()->SetRangeUser(ymin,ymax);
                blank->GetXaxis()->SetTitle("B_{0} (mT)");
                blank->GetYaxis()->SetTitle("B_{i} (mT)");
                blank->GetXaxis()->SetNdivisions(505);
                blank->GetYaxis()->SetNdivisions(505);
                blank->SetLineColor(0);
        blank->Draw();

	//Further canvas settings (axis grid, aspect ratio, etc.)
	c00->SetGridx(1);
	c00->SetGridy(1);
	if(fixed_aspect_ratio) c00->SetFixedAspectRatio();

	//Legend for the plots. This is passed by reference into each function called below which will add its own entry to it.
        leg = new TLegend(0.2,0.9,0.7,0.75);
                leg->SetBorderSize(1);

	//If bool above is set to true this will plot a 1:1 shielding reference line on top of the measurements
	if(plot_reference_line)
	{
		TF1 *onetoone = new TF1("onetoone","pol1",0,1500);
			onetoone->SetParameter(0,0);
			onetoone->SetParameter(1,1);
			onetoone->SetLineColor(kBlack);
			onetoone->SetLineStyle(7);
		onetoone->Draw("SAME");
		leg->AddEntry(onetoone,"1:1 Reference","l");
	}


	//If the bool above is true this will plot rapheals measurements on top of the current plot
	if(plot_rapheal)
	{
		TTree *t = new TTree();
		t->ReadFile("./DataFiles/1layer_wide_sc_fit_results.txt","Bo:w:Bi:r:t:y:u:i:o:p");
		TCanvas *ctemp = new TCanvas();
		t->Draw("Bi:Bo:r","","pl");
		c00->cd();
		TGraphErrors *r = new TGraphErrors(t->GetEntries(),t->GetV2(),t->GetV1());
		for ( int i = 0; i < t->GetEntries(); i++ )
			r->SetPointError(i,0, t->GetV3()[i]);
		r->Draw("Psame");
		leg->AddEntry(r,"Rapheal's 1-Layer YBCO (lN2)","pl");
		ctemp->Close();
	}

/*
	There are 3 functions located in the header file for this macro. Each one takes the following parameters as input:
		Calibration File 	---	Room temperature calibration measuremnt used to convert voltage -> applied field. The only function which doesnt require this is Helium_calib_single, as we have a fixed current -> field relationship for Xu Du's magnet.
		Measurement File	---	A file containing the data for the measuremnt. For the Helium measurements this should be in the form "current field" and for all others in the form "time voltage field"
		Title			---	A string which will be the entries label in the legend
		TCanvas			---	The TCanvas (defailt *c00) which is passed by reference for the function to draw on
		TLegend			---	The TLegend (defailt *leg) which is passed by reference for the function to draw on
		color			---	An integer which specifies the color of the markers for that measurement

*/

/*
	Solenoid_calib_single_old(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_132435.txt",	//Room temperature calibration 
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_134030.txt",	//Before baking
		"22 mm YBCO 1/2 Sheets Nitrogen",
		*c00,
		*leg,
		color);	//Title of plot
*/
	color++;
	if(color == 5) color++;



	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_114940.txt",	//Measurement File
		"1 Layer YBCO (lN2), Inner Layer",
		*c00,
		*leg,
		color);	//Title of plot

	color++;
	if(color == 5) color++;
/*
	Helium_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/HeliumTest_2016_06_29/HeliumScan1_2016_06_29.txt.txt",
//		"22 mm YBCO 1/2 Sheets Helium (Trial 1)",
//		"He Trial 1",
		"YBCO: 1 Layer (lHe)",
//		"YBCO: 22mm 1/2 Sheets (lHe)",
		*c00,
		*leg,
		color);
*/
	color++;
	if(color == 5) color++;

/*
	Helium_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/HeliumTest_2016_06_29/HeliumScan2_2016_06_29.txt",
//		"1 Layer YBCO (lHe) - Trial 2",
//		"YBCO: 22mm 1/2 Sheets #2 (lHe)",
		"22 mm YBCO 1/2 Sheets Helium (Trial 2)",
		*c00,
		*leg,
		color);
*/
	color++;
	if(color == 5) color++;

/*
	Helium_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/HeliumTest_2016_06_29/HeliumScan4_2016_06_29.txt",
//		"22 mm YBCO 1/2 Sheets Helium, 2 layers",
		"YBCO: 2 Layers (lHe)",
		*c00,
		*leg,
		color);
*/
	color++;
	if(color == 5) color++;



/*
	Helium_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/HeliumTest_2016_06_29/HeliumScan3_2016_06_29.txt",
//		"NbTi Helium 1/2 Sheets",
		"NbTi: 22mm 1/2 Sheets",
//		"NbTi: 1 Layer",
		*c00,
		*leg,
		color);
*/
	color++;
	if(color == 5) color++;
/*
	Helium_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/HeliumTest_2016_06_29/HeliumScan5_2016_06_29.txt",
		"NbTi: No Overlap",
		*c00,
		*leg,
		2);
*/
	color++;
	if(color == 5) color++;
/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_134725.txt",	//Measurement File
		"1 Layer YBCO (lN2), Outer Layer",
		*c00,
		*leg,
		color);	//Title of plot
*/
	color++;
	color++;
	if(color == 5) color++;
/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_183353.txt",	//Measurement File
		"2 Layer Solder Test",
		*c00,
		*leg,
		color);	//Title of plot
*/
	color++;
	if(color == 5) color++;

/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_203307.txt",	//Measurement File
//		"22mm Sheet #1 with Zip Ties",
		"1-Layer Test 1 (Inner Layer, with zip ties)",
		*c00,
		*leg,
		color);	//Title of plot
*/
	color++;
	if(color == 5) color++;
	color = 3;
/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_220717.txt",	//Measurement File
//		"22mm Sheet #1 with Zip Ties, Immediate Retest",
		"1-Layer Test 2",
		*c00,
		*leg,
		color);	//Title of plot
*/
	color++;
	if(color == 5) color++;

/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_222417.txt",	//Measurement File
//		"22mm Sheet #1 with Zip Ties, Immediate Retest 2",
//		"1-Layer Test 3",
		"YBCO: 1 Layer (lN2)",
//		"Small Cylinder",
		*c00,
		*leg,
		2);	//Title of plot
*/
	color++;
	if(color == 5) color++;
/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_230157.txt",	//Measurement File
		"22mm Sheet #1 with Zip Ties, 32 Degree Test",
		*c00,
		*leg,
		color);	//Title of plot
*/
	color++;
	if(color == 5) color++;

/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_155007.txt",	//Measurement File
//		"2 Layers with Kapton",
		"2-Layer Test",
		*c00,
		*leg,
		color);	//Title of plot
*/
	color++;
	if(color == 5) color++;
/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_233036.txt",	//Measurement File
		"65^{o} SC, 65^{o} Hall Probe",
		*c00,
		*leg,
		color);	//Title of plot
*/
	color++;
	if(color == 5) color++;
/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160702_000059.txt",	//Measurement File
		"65^{o} SC, 0^{o} Hall Probe",
		*c00,
		*leg,
		3);	//Title of plot
*/
	color++;
	color++;
	if(color == 5) color++;

/*
	Solenoid_calib_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt",	//Shunt resistor calibration file
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160702_001725.txt",	//Measurement File
		"0^{o} SC, 65^{o} Hall Probe",
		*c00,
		*leg,
		color);	//Title of plot
*/
	color++;
	if(color == 5) color++;
/*
	Solenoid_calib_single_old(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_151411.txt",	//Room temperature calibration 
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_153114.txt",	//Before baking
		"Large Cylinder",
		*c00,
		*leg,
		1);	//Title of plot
*/

	color++;

	Solenoid_calib_single_python(
		"./SummaryOf_DataFile_160701_114940.txt",	//Measurement File
		"Python Measurement File",
		*c00,
		*leg,
		color);	//Title of plot


	if(draw_legend) leg->Draw();
	if(print_jpg) c00->Print("Plot.jpg");

	return 0;
}
