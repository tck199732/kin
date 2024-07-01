void CleanHist1D(TH1D *hist, const double &x1, const double &x2);
void CleanHist(TH2D *hist, const double &x1, const double &x2, const double &y1, const double &y2);
void Calibrate(
	// clang-format off
    const int& start_bin = 5,
    const int& end_bin = 29,
    const std::string& fileName = "results/tot_vs_tof.root",
    const std::string& histName = "hTotVsTof",
    const std::vector<double>& xrange = {-12, -5},
    const std::vector<double>& yrange = {1, 23},
    const std::vector<std::vector<double>> & ranges = {
        {-9, -5},
        {-9, -6.5},
        {-9, -6.5},
        {-9, -6.5},
        {-9, -6.5},
        {-9, -7},
        {-10, -7.5},
        {-10, -7.5},
        {-10, -7.5},
        {-10.5, -7.5},
        {-11, -8},
        {-11, -8},
        {-11, -8},
        {-11, -8},
        {-11, -8.5},
        {-11, -8.5},
        {-11, -8.5},
        {-11, -8.5},
        {-11, -8.5},
        {-11, -8.5},
        {-11, -9.5},
        {-11, -9.5},
        {-11, -9.5},
        {-11, -9.5},
        {-11, -9.5},
    } // clang-format on
) {

	auto size = ranges.size();
	if (size != end_bin - start_bin + 1) {
		std::cerr << "Size of ranges does not match the number of bins" << std::endl;
		return;
	}

	auto tFile = new TFile(fileName.c_str(), "READ");
	auto hTotVsTof = (TH2D *)tFile->Get(histName.c_str());
	CleanHist(hTotVsTof, xrange[0], xrange[1], yrange[0], yrange[1]);

	std::vector<double> x;
	std::vector<double> y;

	for (int ibin = start_bin; ibin <= end_bin; ibin++) {
		std::string pName = Form("p_%03d", ibin);
		auto hTof = hTotVsTof->ProjectionX(pName.c_str(), ibin, ibin);
		hTof->Fit("gaus", "Q", "", ranges[ibin - start_bin][0], ranges[ibin - start_bin][1]);
		auto mean = hTof->GetFunction("gaus")->GetParameter(1);
		auto tot = hTotVsTof->GetYaxis()->GetBinCenter(ibin);
		// std::cout << "tot: " << tot << " Mean: " << mean << std::endl;

		x.push_back(tot);
		y.push_back(mean);

		// auto canvas = new TCanvas("canvas", "canvas", 800, 600);
		// hTof->Draw();
		// hTof->GetXaxis()->SetRangeUser(xrange[0], xrange[1]);
		// canvas->SaveAs(Form("hTof_%03d.png", ibin));
		// delete canvas;
	}

	auto gr = new TGraph(x.size(), x.data(), y.data());
	gr->SetName("gr");
	gr->Fit("pol1", "Q");

	auto slope = gr->GetFunction("pol1")->GetParameter(1);
	auto intercept = gr->GetFunction("pol1")->GetParameter(0);
	std::cout << "Slope: " << slope << " Intercept: " << intercept << std::endl;

	// auto fit = gr->GetFunction("pol1");
	// fit->SetLineColor(kRed);
	// fit->SetLineWidth(2);
	// auto canvas = new TCanvas("canvas", "canvas", 800, 600);
	// gr->Draw("AP");
	// canvas->SaveAs("calibration.png");

	return;
}

void CleanHist(TH2D *hist, const double &x1, const double &x2, const double &y1, const double &y2) {

	for (int i = 1; i <= hist->GetNbinsX(); i++) {
		for (int j = 1; j <= hist->GetNbinsY(); j++) {
			if (hist->GetXaxis()->GetBinCenter(i) < x1 || hist->GetXaxis()->GetBinCenter(i) > x2) {
				hist->SetBinContent(i, j, 0);
			}
			if (hist->GetYaxis()->GetBinCenter(j) < y1 || hist->GetYaxis()->GetBinCenter(j) > y2) {
				hist->SetBinContent(i, j, 0);
			}
		}
	}
	return;
}

void CleanHist1D(TH1D *hist, const double &x1, const double &x2) {
	for (int i = 1; i <= hist->GetNbinsX(); i++) {
		if (hist->GetBinCenter(i) < x1 || hist->GetBinCenter(i) > x2) {
			hist->SetBinContent(i, 0);
		}
	}
	return;
}