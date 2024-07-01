#ifndef SAMURAI_HH
#define SAMURAI_HH

#include <array>

#include "TChain.h"

constexpr int SAMURAI_MAXHITS = 256;
struct samuraiData {
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

samuraiData samurai;
void SetSamuraiBranchAddr(TChain *&chain) {
	chain->SetBranchAddress("run", &samurai.run);
	chain->SetBranchAddress("event", &samurai.event);
	chain->SetBranchAddress("lupots", &samurai.lupots);

	chain->SetBranchAddress("kyoto_multi", &samurai.kyoto_multi);
	chain->SetBranchAddress("kyoto_bar", &samurai.kyoto_bar[0]);
	chain->SetBranchAddress("hime_veto_multi", &samurai.hime_veto_multi);
	chain->SetBranchAddress("hime_veto_bar", &samurai.hime_veto_bar[0]);
	chain->SetBranchAddress("hime_veto_tof", &samurai.hime_veto_tof[0]);
	chain->SetBranchAddress("hime_veto_charge", &samurai.hime_veto_charge[0]);
	chain->SetBranchAddress("hime_veto_tdiff", &samurai.hime_veto_tdiff[0]);
	chain->SetBranchAddress("hime_veto_x", &samurai.hime_veto_x[0]);

	return;
}

void samuraiData::reset() {
	lupots = 0;
	kyoto_multi = 0;
	hime_veto_multi = 0;
	kyoto_bar.fill(0);
	hime_veto_bar.fill(0);
	hime_veto_tof.fill(0);
	hime_veto_charge.fill(0);
	hime_veto_tdiff.fill(0);
	hime_veto_x.fill(0);
	return;
}

#endif