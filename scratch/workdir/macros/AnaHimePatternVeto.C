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

void AnaHimePatternVeto(
	// clang-format off
    const std::vector<std::string> &files = {
        "spirit/data1036.root",
        "spirit/data1037.root",
        "spirit/data1038.root",
        "spirit/data1039.root",
        "spirit/data1040.root",
        "spirit/data1041.root",
        "spirit/data1042.root",
        "spirit/data1043.root",
        "spirit/data1044.root",
        "spirit/data1045.root",
        "spirit/data1046.root",
        "spirit/data1047.root",
        "spirit/data1048.root"
    },
	const std::string& filenameOut = "results-kin/hime-hit-pattern-veto.root",
	const std::string& posOffsetFile = "database/pos_offsets_marco.txt", 
    const std::string& timeOffsetFile = "database/time_offsets_6.24.txt", 
    const std::string& velocityFile = "database/velocity_marco.txt",
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

	TH2D *hHitPatternVeto[3][3];
	for (auto ilayer = 0; ilayer < 3; ilayer++) {
		for (auto ibar = 0; ibar < 3; ibar++) {
			hHitPatternVeto[ilayer][ibar] =
				new TH2D(Form("hHitPatternVeto_l%d_b%d", ilayer, ibar), "", 1000, -1000, 1000, 1000, -1000, 1000);
		}
	}
	auto barWidth = barWidthInMeter * 1000; // mm
	auto barGap = barGapInMeter * 1000;		// mm

	auto nEvents = chain->GetEntries();
	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);

		std::array<bool, 3> isVetoHit = {false, false, false};
		for (auto i = 0; i < spirit.hime_veto_multi; i++) {
			int bar = spirit.hime_veto_bar[i];
			isVetoHit[bar] = true;
		}

		for (auto h = 0; h < spirit.hime_nHits; h++) {

			auto hitModule = spirit.hime_moduleID[h];
			auto tDiff = spirit.hime_tDiff[h] + posOffset[hitModule] / calibVelocity[hitModule];
			auto barPos = calibVelocity[hitModule] * tDiff; // position along the bar
			barPos *= 10;									// cm -> mm

			auto layer = getLayer(hitModule);
			if (layer == -1) {
				std::cerr << "Invalid module ID: " << hitModule << std::endl;
				continue;
			}
			auto centerModule = getCenterModule(layer);
			double deltaModule = centerModule - static_cast<int>(hitModule); // hitModule is unsigned int
			double xHit, yHit;
			if (layer == 0 || layer == 2) {
				xHit = barPos;
				yHit = 0.5 * barGap + deltaModule * (barWidth + barGap) + barWidth * unif(gen);
			} else if (layer == 1) {
				yHit = barPos;
				xHit = 0.5 * barGap + deltaModule * (barWidth + barGap) + barWidth * unif(gen);
			} else {
				std::cerr << "Should never happen, layer = " << layer << std::endl;
				std::exit(1);
			}

			for (auto ibar = 0; ibar < 3; ibar++) {
				if (!isVetoHit[ibar]) {
					hHitPatternVeto[layer][ibar]->Fill(xHit, yHit);
				}
			}
		}
	}

	// normalization
	for (auto l = 0; l < 3; l++) {
		for (auto b = 0; b < 3; b++) {
			hHitPatternVeto[l][b]->Scale(1.0 / nEvents);
		}
	}

	auto fileOut = new TFile(filenameOut.c_str(), "RECREATE");
	for (auto l = 0; l < 3; l++) {
		for (auto b = 0; b < 3; b++) {
			hHitPatternVeto[l][b]->Write();
		}
	}
	fileOut->Close();
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