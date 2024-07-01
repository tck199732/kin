#include "spirit.h"
#include <random>
#include <vector>

std::vector<double> getOffset(const std::string &filename);
void addFilesToChain(const std::vector<std::string> &files, TChain *&chain);
int getLayer(int hitModule);
int getCenterModule(int layer);
double getDistanceInMeter(int layer);

const int nModules = 72;

void PlotHimeCharge(
	// clang-format off
    const std::vector<std::string> &files = {
        // "spirit/data1036.root",
        // "spirit/data1037.root",
        // "spirit/data1038.root",
        // "spirit/data1039.root",
        // "spirit/data1040.root",
        // "spirit/data1041.root",
        // "spirit/data1042.root",
        // "spirit/data1043.root",
        // "spirit/data1044.root",
        // "spirit/data1045.root",
        // "spirit/data1046.root",
        // "spirit/data1047.root",
        // "spirit/data1048.root",
        
        // 6.27 
        // "spirit/data1148.root",
        // "spirit/data1149.root",
        // "spirit/data1150.root",
        // "spirit/data1151.root",
        // "spirit/data1152.root",
        // "spirit/data1153.root",
        // "spirit/data1154.root",
        "spirit/data1160.root",
        "spirit/data1161.root",
        "spirit/data1162.root",
        "spirit/data1163.root",
        "spirit/data1164.root",
        "spirit/data1165.root",
        "spirit/data1166.root",
        "spirit/data1167.root",
        "spirit/data1168.root",
        "spirit/data1169.root",
        "spirit/data1170.root",
        "spirit/data1171.root",
        "spirit/data1172.root",
        "spirit/data1173.root",
        "spirit/data1174.root",
        "spirit/data1175.root",
        "spirit/data1176.root",
        "spirit/data1177.root",
        "spirit/data1178.root",
        "spirit/data1179.root",
        "spirit/data1180.root",
        // "spirit/data1181.root",
        // "spirit/data1182.root",
        "spirit/data1183.root",
        "spirit/data1184.root",
        "spirit/data1185.root",
        "spirit/data1186.root",
        "spirit/data1187.root",
        // "spirit/data1183.root",
    }
	// const std::string& filenameOut = "results-kin/hime-hit-pattern.root",
	// clang-format on
) {
	auto chain = new TChain("spirit", "spirit");
	addFilesToChain(files, chain);
	SetBranchAddress(chain);

	TH2D *hTotVsTof[3];
	TH2D *hTotVsTofVetoed[3];

	TH1D *hTot[3];
	TH1D *hTotVetoed[3];

	for (int i = 0; i < 3; i++) {
		hTot[i] = new TH1D(Form("hChargeLayer%d", i), "", 400, 0, 40);
		hTotVetoed[i] = new TH1D(Form("hChargeVetoedLayer%d", i), "", 400, 0, 40);

		hTotVsTof[i] = new TH2D(Form("hChargeVsTofLayer%d", i), "", 400, -300, 100, 400, 0, 40);
		hTotVsTofVetoed[i] = new TH2D(Form("hChargeVsTofVetoedLayer%d", i), "", 400, -300, 100, 400, 0, 40);
	}

	auto nEvents = chain->GetEntries();
	std::cout << "Number of events: " << nEvents << std::endl;
	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);

		auto isVetoHit = spirit.hime_veto_multi > 0;

		for (auto h = 0; h < spirit.hime_nHits; h++) {

			auto hitModule = spirit.hime_moduleID[h];
			if (hitModule < 2) {
				continue;
			}

			auto layer = getLayer(hitModule);
			if (layer == -1) {
				std::cerr << "Invalid module ID: " << hitModule << std::endl;
				continue;
			}
			auto tot = std::sqrt(spirit.hime_tot0[h] * spirit.hime_tot1[h]);

			if (!isVetoHit) {
				// only hime hit
				hTotVetoed[layer]->Fill(tot);
			} else {
				// both hits
				hTot[layer]->Fill(tot);
			}
		}
	}

	auto canvas = new TCanvas("canvas", "canvas", 2000, 1000);
	canvas->Divide(3, 1);
	for (int i = 0; i < 3; i++) {
		canvas->cd(i + 1);
		hTot[i]->SetLineColor(kRed);
		hTot[i]->Draw();
		hTotVetoed[i]->SetLineColor(kBlue);
		hTotVetoed[i]->Draw("SAME");
	}
	canvas->SaveAs("hime-charge.png");

	return;
}

std::vector<double> getOffset(const std::string &filename) {
	std::vector<double> offset(nModules, 0);
	std::ifstream infile(filename.c_str());
	if (!infile.is_open()) {
		std::cerr << "Could not open file " << filename << std::endl;
		return offset;
	}

	std::string line;
	while (std::getline(infile, line)) {
		if (line[0] == '#') {
			continue;
		}
		std::stringstream ss(line);
		int detid;
		ss >> detid >> offset[detid];
	}
	return offset;
}

void addFilesToChain(const std::vector<std::string> &files, TChain *&chain) {
	for (auto fname : files) {
		chain->AddFile(fname.c_str());
	}
	return;
}

int getLayer(int hitModule) {
	if (hitModule >= 0 && hitModule <= 23) {
		return 0;
	}
	if (hitModule >= 24 && hitModule <= 47) {
		return 1;
	}
	if (hitModule >= 48 && hitModule <= 71) {
		return 2;
	}
	return -1;
}

int getCenterModule(int layer) {
	switch (layer) {
	case 0:
		return 11;
	case 1:
		return 35;
	case 2:
		return 59;
	default:
		return -1;
	}
	return -1;
}

double getDistanceInMeter(int layer) {
	// all in meter
	if (layer == 0) {
		return 4.8;
	}
	if (layer == 1) {
		return 4.8 + 0.06;
	}
	if (layer == 2) {
		return 4.8 + 0.06 + 0.06;
	}
	return -DBL_MAX;
}