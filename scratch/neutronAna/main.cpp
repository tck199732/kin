#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "TChain.h"
#include "TFile.h"
#include "TH2D.h"

#include "hime.hh"
#include "spirit.hh"

#include "argparse/argparse.hpp"

struct argParser {
	std::string posOffsetFile;
	std::string timeOffsetFile;
	std::string velocityFile;
	std::string outputFilename;

	bool isApplyTimeOffset;
	bool isApplyPosOffset;
	bool isApplyVelocity;
	bool isApplyCrossWalk;

	double crosswalkThreshold;
	double crosswalkSlope;
	double crosswalkIntercept;

	double timeSbtLeft;
	bool disableVeto;
	bool debug;
	bool verbose;
};

void addArguments(int argc, char **argv);
std::vector<double> getOffset(const std::string &filename);
argParser args;
argparse::ArgumentParser program("Neutron Analysis", "1.0");

// const int Hime::nModules = 72;
const std::vector<int> runIds = {1148, 1150, 1151, 1152, 1153, 1154, 1155, 1158, 1159, 1160, 1161, 1162, 1163, 1164,
								 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178,
								 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1188, 1189, 1190, 1191, 1192, 1193,
								 1194, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207,
								 1208, 1210, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1220, 1221, 1222,
								 1223, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1233, 1234, 1235, 1236,
								 1237, 1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1246, 1247, 1268, 1269, 1273,
								 1274, 1275, 1276, 1277, 1279, 1280, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289,
								 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299};

void setChain(TChain *chain, const std::vector<int> &runIds);
std::string getFileFromRunNumber(int idx);

int main(int argc, char *argv[]) {
	addArguments(argc, argv);

	auto chain = new TChain("spirit", "spirit");
	setChain(chain, runIds);
	auto nEntries = chain->GetEntries();

	std::cout << "Number of entries: " << nEntries << std::endl;

	auto posOffset = args.isApplyPosOffset ? getOffset(args.posOffsetFile) : std::vector<double>(Hime::nModules, 0);
	auto timeOffset = args.isApplyTimeOffset ? getOffset(args.timeOffsetFile) : std::vector<double>(Hime::nModules, 0);
	auto velocity = args.isApplyVelocity ? getOffset(args.velocityFile) : std::vector<double>(Hime::nModules, 0);

	for (int i = 0; i < Hime::nModules; i++) {
		// we only have to do it when reading "our" dt calibration as we apply the shift to time,
		// Marco applies the shift to position
		posOffset[i] = -1. * posOffset[i] * velocity[i];
	}

	for (int i = 0; i < Hime::nModules; i++) {
		std::cout << "Module: " << i << " PosOffset: " << posOffset[i] << " TimeOffset: " << timeOffset[i]
				  << " Velocity: " << velocity[i] << std::endl;
	}
	/****************************************************************************************************************/

	auto hSbtTimeOfTrigger = new TH1D("hSbtTimeOfTrigger", "", 100000, -300, 500);
	auto hTofVsModuleID = new TH2D("hTofVsModuleID", "", 72, 0, 72, 5000, -250, 250);

	auto hTotVsTof = new TH2D("hTotVsTof", "", 2000, -100, 300, 100, 0, 75);
	TH2D *hTotVsTofLayer[3];
	for (auto i = 0; i < 3; i++) {
		hTotVsTofLayer[i] = new TH2D(Form("hTotVsTofLayer%d", i), "", 2000, -100, 300, 100, 0, 75);
	}

	auto hTotVsTof_L2_VL1 = new TH2D("hTotVsTof_L2_VL1", "", 2000, -100, 300, 100, 0, 75);
	auto hTotVsTof_L2_L1 = new TH2D("hTotVsTof_L2_L1", "", 2000, -100, 300, 100, 0, 75);

	TH2D *hTotVsTofDet[Hime::nModules];
	for (auto i = 0; i < Hime::nModules; i++) {
		hTotVsTofDet[i] = new TH2D(Form("hTotVsTofDet%d", i), "", 2000, -100, 300, 100, 0, 75);
	}

	auto hxPosVsyPos = new TH2D("hxPosVsyPosAll", "", 500, -1000, 1000, 500, -1000, 1000);
	TH2D *hxPosVsyPosLayer[3];
	for (auto i = 0; i < 3; i++) {
		hxPosVsyPosLayer[i] = new TH2D(Form("hxPosVsyPosLayer%d", i), "", 500, -1000, 1000, 500, -1000, 1000);
	}

	auto hModuleIDVsTDiff = new TH2D("hModuleIDVsTDiff", "", 500, -25, 25, 72, 0, 72);
	TH2D *hModuleIDVsTDiffLayer[3];
	for (auto i = 0; i < 3; i++) {
		hModuleIDVsTDiffLayer[i] = new TH2D(Form("hModuleIDVsTDiffLayer%d", i), "", 500, -25, 25, 72, 0, 72);
	}

	// for generating random positions
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> unif(0.0, 1.0);

	auto barWidth = Hime::barWidthInMeter * 1000; // mm
	auto barGap = Hime::barGapInMeter * 1000;	  // mm

	/****************************************************************************************************************/
	for (auto iEvt = 0; iEvt < nEntries; iEvt++) {
		chain->GetEntry(iEvt);
		double sbtTime = 0.;
		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			int moduleId = spirit.hime_moduleID[ihit];
			if (moduleId == 1) {
				sbtTime = spirit.hime_tofRaw[ihit];
				hSbtTimeOfTrigger->Fill(sbtTime);
			}
		}
		if (sbtTime == 0.) {
			continue;
		}
		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			int moduleId = spirit.hime_moduleID[ihit];
			int layerId = Hime::getLayer(moduleId);

			if (layerId < 0 || layerId > 2) {
				continue;
			}

			int centerModule = Hime::getCenterModule(layerId);
			auto deltaModule = centerModule - moduleId;

			auto tot = std::sqrt(spirit.hime_tot0[ihit] * spirit.hime_tot1[ihit]);
			auto tDiff = spirit.hime_tDiff[ihit] + posOffset[moduleId] / velocity[moduleId];

			if (moduleId <= 1 || moduleId >= Hime::nModules) {
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

			auto rHit = std::sqrt(xHit * xHit + yHit * yHit) * 1e-3; // mm -> m
			auto distance = Hime::getDistanceInMeter(layerId);
			auto distanceHit = std::sqrt(distance * distance + rHit * rHit);
			auto tof = spirit.hime_tofRaw[ihit] - timeOffset[moduleId] - (distanceHit - distance) / Hime::C_LIGHT;

			tof = tof - args.timeSbtLeft + sbtTime;

			// ????
			tof += (11.8 + 2. * 4.8 / 2.9999 * 10);

			if (args.isApplyCrossWalk) {
				if (tot > args.crosswalkThreshold) {
					tof -= (args.crosswalkThreshold * args.crosswalkSlope + args.crosswalkIntercept);
				} else {
					tof -= (tot * args.crosswalkSlope + args.crosswalkIntercept);
				}
			}

			auto vetoCondition = args.disableVeto ? true : spirit.hime_veto_multi == 0;

			if (vetoCondition) {
				hTofVsModuleID->Fill(moduleId, tof);
				hTotVsTof->Fill(tof, tot);

				hTotVsTofLayer[layerId]->Fill(tof, tot);
				hTotVsTofDet[moduleId]->Fill(tof, tot);

				hxPosVsyPos->Fill(xHit, yHit);
				hxPosVsyPosLayer[layerId]->Fill(xHit, yHit);

				hModuleIDVsTDiff->Fill(tDiff, moduleId);
				hModuleIDVsTDiffLayer[layerId]->Fill(tDiff, moduleId);
				// auto hTotVsTof_L2_VL1 = new TH2D("hTotVsTof_L2_VL1", "", 2000, -100, 300, 100, 0, 75);
				// auto hTotVsTof_L2_L1 = new TH2D("hTotVsTof_L2_L1", "", 2000, -100, 300, 100, 0, 75);
			}
		}
	}

	auto ofile = new TFile(args.outputFilename.c_str(), "recreate");
	hSbtTimeOfTrigger->Write();
	hTofVsModuleID->Write();
	hTotVsTof->Write();
	for (auto i = 0; i < 3; i++) {
		hTotVsTofLayer[i]->Write();
	}
	hTotVsTof_L2_VL1->Write();
	hTotVsTof_L2_L1->Write();

	for (auto i = 0; i < Hime::nModules; i++) {
		hTotVsTofDet[i]->Write();
	}

	hxPosVsyPos->Write();
	for (auto i = 0; i < 3; i++) {
		hxPosVsyPosLayer[i]->Write();
	}

	hModuleIDVsTDiff->Write();
	for (auto i = 0; i < 3; i++) {
		hModuleIDVsTDiffLayer[i]->Write();
	}

	ofile->Close();

	return 0;
}

void addArguments(int argc, char **argv) {

	program.add_argument("--posOffsetFile")
		.default_value(std::string("database/pos_offsets_experiment.txt"))
		.help("path to the position offset file.")
		.required();

	program.add_argument("--timeOffsetFile")
		.default_value(std::string("database/time_offsets_experiment_gaus.txt"))
		.help("path to the time offset file.")
		.required();

	program.add_argument("--velocityFile")
		.default_value(std::string("database/velocity_marco.txt"))
		.help("path to the velocity file.")
		.required();

	program.add_argument("-o", "--outputFilename")
		.default_value(std::string("output.root"))
		.help("path to the output file.")
		.required();

	program.add_argument("--isApplyTimeOffset").default_value(true).help("Apply time offset to the data").required();
	program.add_argument("--isApplyPosOffset").default_value(true).help("Apply position offset to the data").required();
	program.add_argument("--isApplyVelocity").default_value(true).help("Apply velocity to the data").required();

	program.add_argument("--disableVeto")
		.default_value(false)
		.help("if true, events with veto-multi > 0 are rejected")
		.flag();

	program.add_argument("--isApplyCrossWalk")
		.default_value(false)
		.help("Apply crosswalk correction to the data")
		.flag();

	program.add_argument("--crosswalkThreshold")
		.scan<'g', double>()
		.default_value(23.0)
		.help("Threshold for the crosswalk correction");

	program.add_argument("--crosswalkSlope")
		.scan<'g', double>()
		.default_value(-0.197618)
		.help("Slope of the crosswalk");

	program.add_argument("--crosswalkIntercept")
		.scan<'g', double>()
		.default_value(-6.45002)
		.help("Intercept of the crosswalk");

	program.add_argument("--timeSbtLeft").scan<'g', double>().default_value(-110.82).help("Time subtraction left");

	program.add_argument("-d", "--debug").help("debug mode").flag();
	program.add_argument("--verbose").help("verbose mode").flag();

	try {
		program.parse_args(argc, argv);
	} catch (const std::runtime_error &err) {
		std::cout << err.what() << std::endl;
		std::cout << program;
		exit(0);
	}

	args.posOffsetFile = program.get<std::string>("--posOffsetFile");
	args.timeOffsetFile = program.get<std::string>("--timeOffsetFile");
	args.velocityFile = program.get<std::string>("--velocityFile");
	args.outputFilename = program.get<std::string>("--outputFilename");

	args.isApplyTimeOffset = program.get<bool>("--isApplyTimeOffset");
	args.isApplyPosOffset = program.get<bool>("--isApplyPosOffset");
	args.isApplyVelocity = program.get<bool>("--isApplyVelocity");
	args.isApplyCrossWalk = program.get<bool>("--isApplyCrossWalk");

	args.timeSbtLeft = program.get<double>("--timeSbtLeft");
	args.disableVeto = program.get<bool>("--disableVeto");

	args.crosswalkThreshold = program.get<double>("--crosswalkThreshold");
	args.crosswalkSlope = program.get<double>("--crosswalkSlope");
	args.crosswalkIntercept = program.get<double>("--crosswalkIntercept");

	args.debug = program.get<bool>("--debug");
	args.verbose = program.get<bool>("--verbose");

	return;
}

std::vector<double> getOffset(const std::string &filename) {
	std::vector<double> offset(Hime::nModules, 0);
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