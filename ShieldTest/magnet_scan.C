int magnet_scan()
{

	TTree *room = new TTree();
	TTree *no_mag = new TTree();
	TTree *nitrogen = new TTree();


	//y scans
	nitrogen->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160615_122721.txt","z:field:err_field:voltage:err_voltage");
	room->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160615_103307.txt","z:field:err_field:voltage:err_voltage");
	no_mag->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160614_102654.txt","z:field:err_field:voltage:err_voltage");


/*
	//x scans
	nitrogen->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160615_130058.txt","z:field:err_field:voltage:err_voltage");
	room->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160615_110517.txt","z:field:err_field:voltage:err_voltage");
	no_mag->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160614_102654.txt","z:field:err_field:voltage:err_voltage");
*/

	room->SetMarkerColor(kRed);
	nitrogen->SetMarkerColor(kBlue);

	room->SetFillColor(kRed);
	nitrogen->SetFillColor(kBlue);
	no_mag->SetFillColor(kBlack);

	gStyle->SetOptStat(0);

	double ymin, ymax;
	ymin = -3.0;
	ymax = 25.0;
//	ymin = -1;
//	ymax = 2.0;

	std::string title = "Field map in y-direction";

	TCanvas *c1 = new TCanvas();
	TH1F *blank = new TH1F("blank",title.c_str(),10,-650,650);
		blank->GetYaxis()->SetRangeUser(ymin,ymax);
		blank->GetXaxis()->SetTitle("position (mm)");
		blank->GetYaxis()->SetTitle("B (mT)");
		blank->GetXaxis()->SetNdivisions(505);
		blank->GetYaxis()->SetNdivisions(505);
		blank->SetLineColor(0);
	blank->Draw();
	nitrogen->Draw("field:-1*(z-600)","","SAME");
	room->Draw("field:-1*(z-600)","","SAME");
	no_mag->Draw("field:-1*(z-600)","","SAME");
	c1->SetFixedAspectRatio();

	TLine *SC_start = new TLine(-508, ymin,-508, ymax);
		SC_start->SetLineStyle(2);
		SC_start->SetLineColor(kBlue-3);
	SC_start->Draw("SAME");
	TLine *SC_end = new TLine(508, ymin, 508, ymax);
		SC_end->SetLineStyle(2);
		SC_end->SetLineColor(kBlue-3);
	SC_end->Draw("SAME");
	
	leg = new TLegend(0.2,0.9,0.5,0.8);
		leg->SetBorderSize(1);
		leg->AddEntry(nitrogen,"T_{LN2}; Magnet On","f");
		leg->AddEntry(room,"T_{Room}; Magnet On","f");
		leg->AddEntry(no_mag,"T_{Room}; Magnet Off","f");
		leg->AddEntry(SC_start,"SC begins/ends","l");
	leg->Draw();

	return 0;
}
