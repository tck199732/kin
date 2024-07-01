#include "spirit.h"
#include <random>
#include <vector>

std::vector<double> getOffset(const std::string &filename);
void addFilesToChain(const std::vector<std::string> &files, TChain *&chain);
int getLayer(int hitModule);
template <typename T> bool matchHit(const std::vector<T> &vec, T val);

// choose the calibration module for each layer
const int nModules = 72;
const std::vector<std::vector<unsigned int>> calibModule = {
	{28, 35, 42},
	{4, 11, 18},
	{28, 35, 42},
};

void CalibrateHimeTdiff(
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
	const std::string& filenameOut = "results-kin/hime-calib-tdiff.root",
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

	auto hModuleVsTdiffFull = new TH2D("hModuleVsTdiffFull", "", 200, -20, 20, nModules, 0, nModules);
	std::array<TH2D *, 3> hModuleVsTdiffLayer;
	for (auto l = 0; l < 3; l++) {
		hModuleVsTdiffLayer[l] = new TH2D(Form("hModuleVsTdiffLayer%d", l), "", 200, -20, 20, nModules, 0, nModules);
	}

	auto nEvents = chain->GetEntries();
	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);

		std::array<bool, 3> isCalibrate = {false, false, false};
		for (auto h = 0; h < spirit.hime_nHits; h++) {
			auto hitModule = spirit.hime_moduleID[h];
			for (auto layer = 0; layer < 3; layer++) {
				if (matchHit(calibModule[layer], hitModule)) {
					isCalibrate[layer] = true;
				}
			}
		}

		for (auto h = 0; h < spirit.hime_nHits; h++) {

			auto hitModule = spirit.hime_moduleID[h];
			auto tDiff = spirit.hime_tDiff[h] + posOffset[hitModule] / calibVelocity[hitModule];

			auto layer = getLayer(hitModule); // 0, 1, 2
			if (layer == -1) {
				std::cerr << "Invalid module ID: " << hitModule << std::endl;
				continue;
			}

			hModuleVsTdiffFull->Fill(tDiff, hitModule);
			if (isCalibrate[layer]) {
				hModuleVsTdiffLayer[layer]->Fill(tDiff, hitModule);
			}
		}
	}

	// normalization
	hModuleVsTdiffFull->Scale(1.0 / nEvents);
	for (auto l = 0; l < 3; l++) {
		hModuleVsTdiffLayer[l]->Scale(1.0 / nEvents);
	}

	auto fileOut = new TFile(filenameOut.c_str(), "RECREATE");
	hModuleVsTdiffFull->Write();
	for (auto l = 0; l < 3; l++) {
		hModuleVsTdiffLayer[l]->Write();
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

template <typename T> bool matchHit(const std::vector<T> &vec, T val) {
	return std::find(vec.begin(), vec.end(), val) != vec.end();
}