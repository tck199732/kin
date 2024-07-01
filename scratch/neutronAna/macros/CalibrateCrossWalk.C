#include "spirit.h"
const int nModules = 72;
const double barWidthInMeter = 0.04; // 40 mm
const double barGapInMeter = 0.004;	 // 4 mm
const double C_LIGHT = 0.299792458;	 // m/ns

void setChain(TChain *chain, const std::vector<int> &runIds);
std::string getFileFromRunNumber(int idx);
unsigned int getLayer(const unsigned int &hitModule);
unsigned int getCenterModule(const unsigned int &layer);
double getDistanceInMeter(const unsigned int &layer);
std::vector<double> getOffset(const std::string &filename);

void CalibrateCrossWalk(
	// clang-format off
	const std::vector<int> runIds = {1148, 1150, 1151, 1152, 1153, 1154, 1155, 1158, 1159, 1160, 1161, 1162, 1163, 1164,
									 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178,
									 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1188, 1189, 1190, 1191, 1192, 1193,
									 1194, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207,
									 1208, 1210, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1220, 1221, 1222,
									 1223, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1233, 1234, 1235, 1236,
									 1237, 1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1246, 1247, 1268, 1269, 1273,
									 1274, 1275, 1276, 1277, 1279, 1280, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289,
									 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299},

	const std::string &posOffsetFile = "database/pos_offsets_experiment.txt",
    const std::string &tofOffsetFile = "database/time_offsets_experiment_gaus.txt",
	const std::string &velocityFile = "database/velocity_marco.txt",
	const std::string &outputFilename = "tot_vs_tof.root",
    const double timeSbtLeft = -110.82,
    const std::vector<std::vector<double>> & xranges = {
        {-600, 600},
        {-600, 600},
        {-600, 600},
    },
    const std::vector<std::vector<double>> & yranges = {
        {-600, 600},
        {-600, 600},
        {-600, 600},
    },
    bool useVetoWall = true
	// clang-format on
) {
	auto chain = new TChain("spirit", "spirit");
	setChain(chain, runIds);
	auto nEntries = chain->GetEntries();
	std::cout << "Number of entries: " << nEntries << std::endl;

	auto posOffset = getOffset(posOffsetFile);
	auto tofOffset = getOffset(tofOffsetFile);
	auto velocity = getOffset(velocityFile);

	for (int i = 0; i < nModules; i++) {
		// we only have to do it when reading "our" dt calibration as we apply the shift to time,
		// Marco applies the shift to position
		posOffset[i] = -1. * posOffset[i] * velocity[i];
	}

	/****************************************************************************************************************/

	auto hTotVsTof = new TH2D("hTotVsTof", "", 2000, -100, 300, 100, 0, 75);

	// for generating random positions
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> unif(0.0, 1.0);

	auto barWidth = barWidthInMeter * 1000; // mm
	auto barGap = barGapInMeter * 1000;		// mm
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

			int centerModule = getCenterModule(layerId);
			auto deltaModule = centerModule - moduleId;

			auto tot = std::sqrt(spirit.hime_tot0[ihit] * spirit.hime_tot1[ihit]);
			auto tDiff = spirit.hime_tDiff[ihit] + posOffset[moduleId] / velocity[moduleId];

			if (moduleId <= 1 || moduleId >= nModules) {
				continue;
			}

			auto barPosition = tDiff * velocity[moduleId] * 10; // cm -> mm
			double xHit = 0, yHit = 0;
			if (layerId == 0 || layerId == 2) {
				xHit = barPosition;
				yHit = 0.5 * barGap + deltaModule * (barWidth + barGap) + barWidth * unif(gen);
			} else {
				yHit = barPosition;
				xHit = 0.5 * barGap + deltaModule * (barWidth + barGap) + barWidth * unif(gen);
			}

			if (xHit < xranges[layerId][0] || xHit > xranges[layerId][1] || yHit < yranges[layerId][0] ||
				yHit > yranges[layerId][1]) {
				continue;
			}

			auto rHit = std::sqrt(xHit * xHit + yHit * yHit) * 1e-3; // mm -> m
			auto distance = getDistanceInMeter(layerId);
			auto distanceHit = std::sqrt(distance * distance + rHit * rHit);
			auto tof = spirit.hime_tofRaw[ihit] - tofOffset[moduleId] - (distanceHit - distance) / C_LIGHT;
			tof = tof - timeSbtLeft + sbtTime;

			// ????
			tof += (11.8 + 2. * 4.8 / 2.9999 * 10);

			// zibi cross walk correction
			// if (tot < 22.13) {
			// 	tof -= (-0.1599 * tot - 6.4614);
			// } else {
			// 	tof -= (-0.1599 * 22.13 - 6.4614);
			// }

			hTotVsTof->Fill(tof, tot);
		}
	}
	auto ofile = new TFile(outputFilename.c_str(), "recreate");
	hTotVsTof->Write();
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
unsigned int getCenterModule(const unsigned int &layer) {
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
};

double getDistanceInMeter(const unsigned int &layer) {
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
};
