#include "spirit.h"
#include <random>
#include <vector>

std::vector<double> getOffset(const std::string &filename);
void addFilesToChain(const std::vector<std::string> &files, TChain *&chain);
int getLayer(int hitModule);
int getCenterModule(int layer);
double getDistanceInMeter(int layer);

const int nModules = 72;
const double barWidthInMeter = 0.04; // 40 mm
const double barGapInMeter = 0.004;	 // 4 mm
const double C_LIGHT = 0.299792458;	 // m/ns

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<> unif(0.0, 1.0);

void PlotTotTof(
	// clang-format off
    const std::vector<std::string> &files = {
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
        // "spirit/data1187.root",
        // "spirit/data1183.root",

    },
	const std::string& filenameOut = "results-kin/hime-hit-pattern.root",
	const std::string& posOffsetFile = "database/pos_offsets_marco.txt", 
    const std::string& timeOffsetFile = "database/time_offsets_6.24.txt", 
    const std::string& velocityFile = "database/velocity_marco.txt",

    bool isApplyingVeto = true,
    bool isApplyingTimeOffset = true,
    bool isApplyingPosOffset = true
	// clang-format on
) {
	auto chain = new TChain("spirit", "spirit");
	addFilesToChain(files, chain);
	SetBranchAddress(chain);

	auto posOffset = isApplyingPosOffset ? getOffset(posOffsetFile) : std::vector<double>(nModules, 0); // marco = cm
	auto timeOffset = isApplyingTimeOffset ? getOffset(timeOffsetFile) : std::vector<double>(nModules, 0);
	auto calibVelocity = getOffset(velocityFile); // marco = cm/ns

	auto hTotVsTof = new TH2D("hTotVsTof", "", 500, -200, 500, 300, 0, 75);
	std::array<TH2D *, 3> hTotVsTofLayer;
	for (auto l = 0; l < 3; l++) {
		hTotVsTofLayer[l] = new TH2D(Form("hTotVsTofLayer%d", l), "", 500, -200, 500, 300, 0, 75);
	}

	auto barWidth = barWidthInMeter * 1000; // mm
	auto barGap = barGapInMeter * 1000;		// mm

	auto nEvents = chain->GetEntries();
	std::cout << "Number of events: " << nEvents << std::endl;
	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);

		auto isVetoed = isApplyingVeto && !(spirit.hime_veto_multi > 0);
		for (auto h = 0; h < spirit.hime_nHits; h++) {

			auto hitModule = spirit.hime_moduleID[h];
			if (hitModule < 2) {
				continue;
			}

			auto tDiff = spirit.hime_tDiff[h] + posOffset[hitModule] / calibVelocity[hitModule];
			auto barPos = calibVelocity[hitModule] * tDiff; // position along the bar
			barPos *= 10;									// cm -> mm

			auto layer = getLayer(hitModule);
			if (layer == -1) {
				std::cerr << "Invalid module ID: " << hitModule << std::endl;
				continue;
			}
			auto distance = getDistanceInMeter(layer); // m
			auto centerModule = getCenterModule(layer);
			double deltaModule = centerModule - static_cast<int>(hitModule); // hitModule is unsigned int
			double xHit, yHit;
			if (layer == 0 || layer == 2) {
				xHit = barPos;
				yHit = 0.5 * barGap + deltaModule * (barWidth + barGap) + barWidth * unif(gen);
			} else if (layer == 1) {
				// std::swap(xHit, yHit);
				yHit = barPos;
				xHit = 0.5 * barGap + deltaModule * (barWidth + barGap) + barWidth * unif(gen);
			} else {
				std::cerr << "Should never happen, layer = " << layer << std::endl;
				std::exit(1);
			}

			auto rHit = std::sqrt(xHit * xHit + yHit * yHit) * 1e-3; // mm -> m
			auto distanceHit = std::sqrt(distance * distance + rHit * rHit);
			auto tof = spirit.hime_tofRaw[h] - timeOffset[hitModule] - (distanceHit - distance) / C_LIGHT;
			auto tot = std::sqrt(spirit.hime_tot0[h] * spirit.hime_tot1[h]);

			if (isVetoed) {
				hTotVsTof->Fill(tof, tot);
				hTotVsTofLayer[layer]->Fill(tof, tot);
			}
		}
	}

	// normalization
	hTotVsTof->Scale(1.0 / nEvents);
	for (auto l = 0; l < 3; l++) {
		hTotVsTofLayer[l]->Scale(1.0 / nEvents);
	}

	// auto canvas = new TCanvas("canvas", "canvas", 800, 600);
	// hTotVsTof->GetYaxis()->SetRangeUser(0, 40);
	// hTotVsTof->Draw("colz");
	// canvas->SaveAs("hTotVsTof.png");

	// auto canvas = new TCanvas("canvas", "canvas", 800, 600);
	// canvas->Divide(3, 1);
	// for (int i = 0; i < 3; i++) {
	// 	canvas->cd(i + 1);
	// 	hTotVsTofLayer[i]->Draw("COLZ");
	// }
	// canvas->SaveAs("hTotVsTofLayer.png");

	// delete canvas;
	// canvas = new TCanvas("canvas", "canvas", 800, 600);
	// canvas->Divide(3, 1);
	// for (int i = 0; i < 3; i++) {
	// 	canvas->cd(i + 1);
	// 	hTotVsTofLayer[i]->ProjectionY()->Draw("COLZ");
	// }
	// canvas->SaveAs("hTotLayer.png");

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