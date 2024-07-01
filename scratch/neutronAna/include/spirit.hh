#ifndef SPIRIT_HH
#define SPIRIT_HH

#include "TChain.h"

constexpr int HIME_MAXHITS = 256;
constexpr int SAMURAI_MAXHITS = 256;

struct spiritData {
	unsigned int hime_nHits;
	std::array<double, HIME_MAXHITS> hime_tofRaw;
	std::array<double, HIME_MAXHITS> hime_tDiff;
	std::array<double, HIME_MAXHITS> hime_tSum;
	std::array<double, HIME_MAXHITS> hime_tot0;
	std::array<double, HIME_MAXHITS> hime_tot1;
	std::array<unsigned int, HIME_MAXHITS> hime_moduleID;

	ULong64_t hime_slowScaler;
	ULong64_t hime_fastScaler;
	ULong64_t hime_eventNumber;

	unsigned int run, event;
	ULong64_t lupots;
	unsigned int kyoto_multi;
	std::array<unsigned int, SAMURAI_MAXHITS> kyoto_bar;
	unsigned int hime_veto_multi;
	std::array<unsigned int, SAMURAI_MAXHITS> hime_veto_bar;
	std::array<double, SAMURAI_MAXHITS> hime_veto_tof;
	std::array<double, SAMURAI_MAXHITS> hime_veto_charge;
	std::array<double, SAMURAI_MAXHITS> hime_veto_tdiff;
	std::array<double, SAMURAI_MAXHITS> hime_veto_x;

	void reset();
};

spiritData spirit;

void spiritData::reset() {
	hime_nHits = 0;
	hime_tofRaw.fill(0);
	hime_tDiff.fill(0);
	hime_tSum.fill(0);
	hime_tot0.fill(0);
	hime_tot1.fill(0);
	hime_moduleID.fill(0);

	hime_slowScaler = 0;
	hime_fastScaler = 0;
	hime_eventNumber = 0;

	run = 0;
	event = 0;
	lupots = 0;
	kyoto_multi = 0;
	kyoto_bar.fill(0);
	hime_veto_multi = 0;
	hime_veto_bar.fill(0);
	hime_veto_tof.fill(0);
	hime_veto_charge.fill(0);
	hime_veto_tdiff.fill(0);
	hime_veto_x.fill(0);
	return;
}

void SetBranchAddress(TChain *&chain) {
	chain->SetBranchAddress("hime_nHits", &spirit.hime_nHits);
	chain->SetBranchAddress("hime_tofRaw", &spirit.hime_tofRaw[0]);
	chain->SetBranchAddress("hime_tDiff", &spirit.hime_tDiff[0]);
	chain->SetBranchAddress("hime_tSum", &spirit.hime_tSum[0]);
	chain->SetBranchAddress("hime_tot0", &spirit.hime_tot0[0]);
	chain->SetBranchAddress("hime_tot1", &spirit.hime_tot1[0]);
	chain->SetBranchAddress("hime_moduleID", &spirit.hime_moduleID[0]);

	chain->SetBranchAddress("hime_slowScaler", &spirit.hime_slowScaler);
	chain->SetBranchAddress("hime_fastScaler", &spirit.hime_fastScaler);
	chain->SetBranchAddress("hime_eventNumber", &spirit.hime_eventNumber);

	chain->SetBranchAddress("run", &spirit.run);
	chain->SetBranchAddress("event", &spirit.event);
	chain->SetBranchAddress("lupots", &spirit.lupots);
	chain->SetBranchAddress("kyoto_multi", &spirit.kyoto_multi);
	chain->SetBranchAddress("kyoto_bar", &spirit.kyoto_bar[0]);
	chain->SetBranchAddress("hime_veto_multi", &spirit.hime_veto_multi);
	chain->SetBranchAddress("hime_veto_bar", &spirit.hime_veto_bar[0]);
	chain->SetBranchAddress("hime_veto_tof", &spirit.hime_veto_tof[0]);
	chain->SetBranchAddress("hime_veto_charge", &spirit.hime_veto_charge[0]);
	chain->SetBranchAddress("hime_veto_tdiff", &spirit.hime_veto_tdiff[0]);
	chain->SetBranchAddress("hime_veto_x", &spirit.hime_veto_x[0]);
	return;
}

#endif
