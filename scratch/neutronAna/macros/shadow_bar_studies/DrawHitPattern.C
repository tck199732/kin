#include "../spirit.h"
const int nLayers = 3;
const int nModules = 72;
const double barWidthInMeter = 0.04; // 40 mm

void setChain(TChain *chain, const std::vector<int> &runIds);
std::string getFileFromRunNumber(int idx);
unsigned int getLayer(const unsigned int &hitModule);
std::vector<double> getOffset(const std::string &filename);
std::vector<double> getModulePositions(const std::string &filename);

void loadOffsetJson(
	const std::string &filename, std::vector<double> &offset, std::vector<std::vector<double>> &gatedModules
);

// forward angle shadow bar
const std::vector<double> runIds_before = {
	1148, 1150, 1151, 1152, 1153, 1154, 1155, 1158, 1159, 1160, 1161, 1162, 1163, 1164, 1165,
	1166, 1167, 1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178, 1179, 1180,
	1181, 1182, 1183, 1184, 1185, 1186, 1188, 1189, 1190, 1191, 1192, 1193, 1194, 1195, 1196,
	1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1210, 1211,
};

// top forward-angled bar moved to bottom backward-angle
const std::vector<double> runIds_moved = {
	1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1221, 1222, 1223, 1225, 1226, 1227, 1228, 1229,
	1230, 1231, 1232, 1234, 1235, 1236, 1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1246,
};

// only bottom backward-angled bar
const std::vector<double> runIds_final = {
	1247, 1268, 1269, 1273, 1274, 1275, 1276, 1277, 1279, 1280, 1282, 1283, 1284, 1285,
	1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299,
};

void DrawHitPattern(
	// clang-format off
    const std::string &shadow_bar_pos = "before",
    const std::string &outputFilename = "hit_pattern.root",
    bool useVetoWall = false,
    const std::string &posOffsetFile = "database/calibration/tdiff/tdiff_offset.json",
	const std::string &velocityFile = "database/velocity_marco.txt",
    const std::string &modulePositionFilename = "database/module_positions.dat",
	
    const std::vector<std::vector<double>> & xranges = {
        {-800, 800},
        {-800, 800},
        {-800, 800},
    },
    const std::vector<std::vector<double>> & yranges = {
        {-800, 800},
        {-800, 800},
        {-800, 800},
    } // clang-format on
) {

	std::vector<int> runIds;
	if (shadow_bar_pos == "before") {
		runIds.insert(runIds.end(), runIds_before.begin(), runIds_before.end());
	} else if (shadow_bar_pos == "moved") {
		runIds.insert(runIds.end(), runIds_moved.begin(), runIds_moved.end());
	} else if (shadow_bar_pos == "final") {
		runIds.insert(runIds.end(), runIds_final.begin(), runIds_final.end());
	} else {
		std::cerr << "Unknown shadow bar position: " << shadow_bar_pos << std::endl;
		return;
	}

	auto chain = new TChain("spirit", "spirit");
	setChain(chain, runIds);
	auto nEntries = chain->GetEntries();
	std::cout << "Number of entries: " << nEntries << std::endl;

	auto velocity = getOffset(velocityFile);
	auto modulePositions = getModulePositions(modulePositionFilename);
	for (auto i = 0; i < nModules; i++) {
		modulePositions[i] *= 1000; // m -> mm
	}

	std::vector<double> posOffset;
	std::vector<std::vector<double>> gatedModules;
	loadOffsetJson(posOffsetFile, posOffset, gatedModules);

	for (int i = 0; i < nModules; i++) {
		// we only have to do it when reading "our" dt calibration as we apply the shift to time,
		// Marco applies the shift to position
		posOffset[i] = -1. * posOffset[i] * velocity[i];
	}

	/****************************************************************************************************************/

	auto hHitPattern = new TH2D("hHitPattern", "", 200, -1000, 1000, 200, -1000, 1000);

	TH2D *hHitPatternLayer[3];
	for (int i = 0; i < 3; i++) {
		hHitPatternLayer[i] = new TH2D(Form("hHitPatternLayer%d", i), "", 200, -1000, 1000, 200, -1000, 1000);
	}

	// for generating random positions
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> unif(0.0, 1.0);

	auto barWidth = barWidthInMeter * 1000; // mm
	/****************************************************************************************************************/
	for (auto iEvt = 0; iEvt < nEntries; iEvt++) {
		chain->GetEntry(iEvt);
		auto vetoEvent = useVetoWall ? spirit.hime_veto_multi > 0 : false;
		if (vetoEvent) {
			continue;
		}

		double sbtTime = 0.;
		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			int moduleId = spirit.hime_moduleID[ihit];
			if (moduleId == 1) {
				sbtTime = spirit.hime_tofRaw[ihit];
			}
		}
		if (sbtTime == 0.) {
			continue;
		}

		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			int moduleId = spirit.hime_moduleID[ihit];
			int layerId = getLayer(moduleId);

			if (layerId < 0 || layerId > 2) {
				continue;
			}

			auto tot = std::sqrt(spirit.hime_tot0[ihit] * spirit.hime_tot1[ihit]);
			auto tDiff = spirit.hime_tDiff[ihit] + posOffset[moduleId] / velocity[moduleId];

			if (moduleId <= 1 || moduleId >= nModules) {
				continue;
			}

			auto barPosition = tDiff * velocity[moduleId] * 10; // cm -> mm
			double xHit = 0, yHit = 0;

			// extra position offset due to the module we used to gate the tdiff
			auto gatedMod = gatedModules[layerId][1];
			auto extra_offset = modulePositions[gatedMod];

			if (layerId == 0 || layerId == 2) {
				xHit = barPosition - extra_offset;
				yHit = modulePositions[moduleId] + barWidth * (unif(gen) - 0.5);
			} else {
				yHit = barPosition - extra_offset;
				xHit = modulePositions[moduleId] + barWidth * (unif(gen) - 0.5);
			}

			if (xHit < xranges[layerId][0] || xHit > xranges[layerId][1] || yHit < yranges[layerId][0] ||
				yHit > yranges[layerId][1]) {
				continue;
			}

			hHitPattern->Fill(xHit, yHit);
			hHitPatternLayer[layerId]->Fill(xHit, yHit);
		}
	}
	auto ofile = new TFile(outputFilename.c_str(), "recreate");
	hHitPattern->Scale(1. / nEntries);
	hHitPattern->Write();
	for (int i = 0; i < 3; i++) {
		hHitPatternLayer[i]->Scale(1. / nEntries);
		hHitPatternLayer[i]->Write();
	}

	ofile->Close();

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

void loadOffsetJson(
	const std::string &filename, std::vector<double> &offset, std::vector<std::vector<double>> &gatedModules
) {

	offset.clear();
	gatedModules.clear();
	offset.resize(nModules, 0);
	gatedModules.resize(nLayers);

	if (filename == "") {
		return;
	}

	if (!std::filesystem::exists(filename)) {
		std::cerr << "Could not open file " << filename << std::endl;
		return;
	}

	std::ifstream infile(filename.c_str());
	// load json file
	nlohmann::json j;
	infile >> j;

	for (auto i = 0; i < nModules; i++) {
		offset[i] = j[std::to_string(i)];
	}

	for (auto i = 0; i < nLayers; i++) {
		std::string key = "layer" + std::to_string(i) + "_gate";
		auto mods = j[key];
		for (auto mod : mods) {
			gatedModules[i].push_back(mod);
		}
	}

	return;
}

void setChain(TChain *chain, const std::vector<int> &runIds) {
	for (auto idx : runIds) {
		auto fname = getFileFromRunNumber(idx);
		chain->AddFile(fname.c_str());
	}
	SetBranchAddress(chain); // from spirit.hh
	return;
}

std::string getFileFromRunNumber(int idx) { return Form("spirit/data%04d.root", idx); }

unsigned int getLayer(const unsigned int &hitModule) {
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

std::vector<double> getModulePositions(const std::string &filename) {
	std::vector<double> positions(nModules, 0);
	std::ifstream infile(filename.c_str());
	if (!infile.is_open()) {
		std::cerr << "Could not open file " << filename << std::endl;
		return positions;
	}

	infile.ignore(1000, '\n');
	infile.ignore(1000, '\n');

	int id, layer;
	double pos;

	while (infile >> id >> layer >> pos) {
		positions[id] = pos;
	}
	return positions;
}