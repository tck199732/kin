#ifndef HIME_HH
#define HIME_HH

#include "TChain.h"

constexpr int HIME_MAXHITS = 256;
struct himeData {
	unsigned int nHits;
	std::array<double, HIME_MAXHITS> tofRaw;
	std::array<double, HIME_MAXHITS> tDiff;
	std::array<double, HIME_MAXHITS> tSum;
	std::array<double, HIME_MAXHITS> tot0;
	std::array<double, HIME_MAXHITS> tot1;
	std::array<unsigned int, HIME_MAXHITS> moduleID;

	// std::array<double, HIME_MAXHITS> HIME_X;
	// std::array<double, HIME_MAXHITS> HIME_Y;
	// std::array<double, HIME_MAXHITS> HIME_X_layer1;
	// std::array<double, HIME_MAXHITS> HIME_Y_layer1;
	// std::array<double, HIME_MAXHITS> HIME_X_layer2;
	// std::array<double, HIME_MAXHITS> HIME_Y_layer2;
	// std::array<double, HIME_MAXHITS> HIME_X_layer3;
	// std::array<double, HIME_MAXHITS> HIME_Y_layer3;
	// std::array<double, HIME_MAXHITS> HIME_layer;

	ULong64_t slowScaler;
	ULong64_t fastScaler;
	ULong_t eventNumber;

	void reset();
};

himeData hime;
void SetHimeBranchAddr(TChain *chain) {
	chain->SetBranchAddress("nHits", &hime.nHits);
	// chain->SetBranchAddress("HIME_X", &hime.HIME_X);
	// chain->SetBranchAddress("HIME_Y", &hime.HIME_Y);
	// chain->SetBranchAddress("HIME_X_layer1", &hime.HIME_X_layer1);
	// chain->SetBranchAddress("HIME_Y_layer1", &hime.HIME_Y_layer1);
	// chain->SetBranchAddress("HIME_X_layer2", &hime.HIME_X_layer2);
	// chain->SetBranchAddress("HIME_Y_layer2", &hime.HIME_Y_layer2);
	// chain->SetBranchAddress("HIME_X_layer3", &hime.HIME_X_layer3);
	// chain->SetBranchAddress("HIME_Y_layer3", &hime.HIME_Y_layer3);
	// chain->SetBranchAddress("HIME_layer", &hime.HIME_layer);

	chain->SetBranchAddress("tofRaw", &hime.tofRaw[0]);
	chain->SetBranchAddress("tDiff", &hime.tDiff[0]);
	chain->SetBranchAddress("tSum", &hime.tSum[0]);
	chain->SetBranchAddress("tot0", &hime.tot0[0]);
	chain->SetBranchAddress("tot1", &hime.tot1[0]);
	chain->SetBranchAddress("moduleID", &hime.moduleID[0]);

	chain->SetBranchAddress("slowScaler", &hime.slowScaler);
	chain->SetBranchAddress("fastScaler", &hime.fastScaler);
	chain->SetBranchAddress("eventNumber", &hime.eventNumber);

	return;
}

void himeData::reset() {
	slowScaler = 0;
	fastScaler = 0;
	eventNumber = 0;
	nHits = 0;
	// HIME_X.fill(0);
	// HIME_Y.fill(0);
	// HIME_X_layer1.fill(0);
	// HIME_Y_layer1.fill(0);
	// HIME_X_layer2.fill(0);
	// HIME_Y_layer2.fill(0);
	// HIME_X_layer3.fill(0);
	// HIME_Y_layer3.fill(0);
	// HIME_layer.fill(0);
	tofRaw.fill(0);
	tDiff.fill(0);
	tSum.fill(0);
	tot0.fill(0);
	tot1.fill(0);
	moduleID.fill(0);
	return;
}

#endif