#include "HeliumPlots.h"

int HeliumPlots()
{
	int color = 2;				//Integer corresponding to the color of the plot which is created.
	gStyle->SetOptStat(0);			//Prevents fit information from bring printed on the canvas.
	std::string title = "YBCO: 45 Layer 4.5\" Solder Test Comparison";			//Title of the canvas which will host all of the plots
	bool plot_reference_line = true;	//If true will plot a 1:1 reference line on top of the plots.
	bool plot_rapheal = true; 		//If true will plot rapheals measurement on top of new measurement
	bool draw_legend = true;		//If true will draw legend on plot
	bool print_jpg = true;			//Prints canvas automatically to Plot_[title].jpg
	bool fixed_aspect_ratio = false;		//If true fixes output canvas aspect ratio to 1:1

	double ymin, ymax, xmax, xmin;
	ymin = -0.75;	//Defines the range of the canvas which will be printed in x and y
	ymax = 700;
	xmin = 0;
	xmax = ymax;

	//Create canvas to draw all of the plots on. This will be passed by reference into each function below.
	TCanvas *c00 = new TCanvas("c00",title.c_str(),750,750);	//Makes canvas large enough for png printing.
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
		std::string title_rapheal = "2015 1-Layer (Helmholtz)";
		cout << "*******************************************************" << endl << "Beginning: " << title_rapheal << endl;
		//Plots rapheals 1-layer measurement from his thesis
		TTree *t = new TTree();
		t->ReadFile("./DataFiles/1layer_wide_sc_fit_results.txt","Bo:w:Bi:r:t:y:u:i:o:p");
		TCanvas *ctemp = new TCanvas();
		t->Draw("Bi:Bo:r","","pl");
		c00->cd();
		TGraphErrors *r = new TGraphErrors(t->GetEntries(),t->GetV2(),t->GetV1());
		for ( int i = 0; i < t->GetEntries(); i++ )
			r->SetPointError(i,0, t->GetV3()[i]);
		r->Draw("Pl same");
		leg->AddEntry(r,title_rapheal.c_str(),"pl");
		ctemp->Close();
		cout << "All done with: " << title_rapheal.c_str() << endl;
	}

/*
	There are a number of functions located in the header file for this macro. Each one takes the following parameters as input:
		Calibration File 	---	Room temperature calibration measuremnt used to convert voltage -> applied field. This is only required for old measurements before we implemented the datacut algorithm.
		Measurement File	---	A file containing the data for the measuremnt. For the Helium measurements this should be in the form "current field" and for all other (non-summary) files in the form "time voltage field". For the summary file the format is "Applied_Field Error_Applied_Field Internal_Field Error_Internal_Field"
		Title			---	A string which will be the entries label in the legend
		TCanvas			---	The TCanvas (defailt *c00) which is passed by reference for the function to draw on
		TLegend			---	The TLegend (defailt *leg) which is passed by reference for the function to draw on
		color			---	An integer which specifies the color of the markers for that measurement

	For the python calibration, the path of the directory where the python macros are stored is also necessary, as well as which setup the measurement was taken in (for the calibration factor) and whether or not to invert the measurement.

*/

/*
	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160701_183353.txt",								//File name for measurement file
		"YBCO: 2 Layer Solder Test (lN2)",							//File name for legend
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Solenoid",										//1 of solenoid calibration, 2 if helmholtz calibration
		"False");										//Negate measurements of the field?
	color++;
*/
/*
	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160701_203307.txt",								//File name for measurement file
		"YBCO: 1 Layer (lN2)",									//File name for legend
//		"YBCO: 22mm 1/2 Sheets (lN2)",
//		"22mm Sheet #1 with Zip Ties --- Trial 1/3",
//		"Python Measurement File",
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Solenoid",										//1 of solenoid calibration, 2 if helmholtz calibration
		"False");										//Negate measurements of the field?
	color++;
*/
/*
	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160701_155007.txt",								//File name for measurement file
		"YBCO: 2 Layers (Kapton)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Solenoid",										//Solenoid or helmholtz calibration?
		"False");										//Negate measurements of the field?
*/

/*
        Solenoid_calib_single_old(
                "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_115402.txt",   //Room temperature calibration 
                "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_151902.txt",   //Before baking
                "YBCO Sheets: Baking Test 3 Minutes --- Before",
                *c00,
                *leg,
                color);     //Title of plot


	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160706_132740.txt",								//File name for measurement file
		"YBCO Sheets: Baking Test 3 Minutes --- After",						//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Helmholtz",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True");										//Negate measurements of the field?
	color++;


        Solenoid_calib_single_old(
                "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_122150.txt",   //Room temperature calibration 
                "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_131013.txt",   //Before baking
                "YBCO Sheets: Baking Test 5 Minutes --- Before",
                *c00,
                *leg,
                color);     //Title of plot

	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160713_111005.txt",								//File name for measurement file
		"YBCO Sheets: Baking Test 5 Minutes --- After",						//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Helmholtz",										//1 of solenoid calibration, 2 if helmholtz calibration
		"False");										//Negate measurements of the field?
	color++;


        Solenoid_calib_single_old(
                "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_134420.txt",   //Room temperature calibration 
                "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_141147.txt",   //Before baking
                "YBCO Sheets: Baking Test 7 Minutes --- Before",
                *c00,
                *leg,
                color);     //Title of plot


	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160713_140922.txt",								//File name for measurement file
		"YBCO Sheets: Baking Test 7 Minutes --- After",						//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Helmholtz",										//1 of solenoid calibration, 2 if helmholtz calibration
		"False");										//Negate measurements of the field?
	color++;
*/
/*
	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160713_165100.txt",								//File name for measurement file
		"5 Layer YBCO Solder Test (Halves Ziptied Together)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Solenoid",										//1 of solenoid calibration, 2 if helmholtz calibration
		"False");										//Negate measurements of the field?
	color++;


	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160714_155647.txt",								//File name for measurement file
		"5 Layer YBCO Solder Test (Halves Soldered Together)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Solenoid",										//1 of solenoid calibration, 2 if helmholtz calibration
		"False");										//Negate measurements of the field?
	color++;


	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160714_183120.txt",								//File name for measurement file
		"5 Layer YBCO Solder Test (Halves Soldered Together) Run 2",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Solenoid",										//1 of solenoid calibration, 2 if helmholtz calibration
		"False");										//Negate measurements of the field?
	color++;
	color++;


	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160715_112513.txt",								//File name for measurement file
		"5 Layer YBCO Solder Test (Halves Soldered Together) Run 3",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Solenoid",										//1 of solenoid calibration, 2 if helmholtz calibration
		"False");										//Negate measurements of the field?
*/

	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160720_172022.txt",								//File name for measurement file
		"2016 10-Layer Solder (Dipole)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Dipole",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		1962);											//Line of file to stop python. 0 if full file.

	color++;

	//Low fields

/*
	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160722_130339.txt",								//File name for measurement file
		"2016 1-Layer (Dipole)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"SmallBlack",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0);											//Line of file to stop python. 0 if full file.
*/
/*
	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160722_110958.txt",								//File name for measurement file
		"1-Layer With Overlap (6/22)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"DipoleNew",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0);											//Line of file to stop python. 0 if full file.
*/

/*
	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160722_173048.txt",								//File name for measurement file
		"2016 45-Layer Solder Trial 2 (Dipole)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"DipoleNew",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0);											//Line of file to stop python. 0 if full file.
*/
	color++;

	Solenoid_calib_single_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160722_184220.txt",								//File name for measurement file
		"2016 45-Layer Solder (Dipole)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"DipoleNew",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0);											//Line of file to stop python. 0 if full file.


	if(draw_legend) leg->Draw();
	if(print_jpg) 
	{
		c00->Print(("Plot_"+title+".png").c_str());
		c00->Print(("Plot_"+title+".eps").c_str());
		cout << "Image Created with title: Plot_" << title << ".png" << endl;
	}

	return 0;
}
