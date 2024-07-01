#ifndef ANA_HH
#define ANA_HH

#include <array>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "hime_veto.hh"
#include "kyoto.hh"
#include "sbt.hh"
#include "scaler.hh"

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

struct VmeEasiroc {
	std::array<int, 64> hadc; // high gain
	std::array<int, 64> ladc; // low gain
	std::array<int, 64> ltdc; // leading edge
	std::array<int, 64> ttdc; // trailing edge

	void reset() {
		hadc.fill(-1);
		ladc.fill(-1);
		ltdc.fill(-1);
		ttdc.fill(-1);
	}
};

struct V1290 {
	std::array<int, 32> ltdc;
	std::array<int, 32> ttdc;

	void reset() {
		ltdc.fill(-1);
		ttdc.fill(-1);
	}
};

struct SIS3820 {
	// http://ribfdaq.rarfadv.riken.go.jp/~samurai/babiscr.php
	std::array<int, 32> scaler;
	void reset() { scaler.fill(0); }
};

struct AMTTDC {
	std::array<int, 64> tdc_x1; // geo 2
	std::array<int, 64> tdc_y1; // geo 3
	std::array<int, 64> tdc_x2; // geo 4
	std::array<int, 64> tdc_y2; // geo 5

	void reset() {
		tdc_x1.fill(-1);
		tdc_y1.fill(-1);
		tdc_x2.fill(-1);
		tdc_y2.fill(-1);
	}
};

struct MQDC32 {
	std::array<int, 32> qdc;
	void reset() { qdc.fill(-1); }
};

struct MTDC32 {
	std::array<int, 32> tdc;
	std::array<int, 2> trigger;
	void reset() {
		tdc.fill(-1);
		trigger.fill(-1);
	}
};

struct samuraiData {
	unsigned int run, event;
	// std::uint64_t lupots;
	ULong64_t lupots;

	VmeEasiroc vme_easiroc;
	V1290 v1290;
	SIS3820 sis3820;
	AMTTDC amttdc;
	MQDC32 mqdc32;
	MTDC32 mtdc32;

	void reset() {
		run = 0;
		event = 0;
		lupots = 0;
		vme_easiroc.reset();
		v1290.reset();
		sis3820.reset();
		amttdc.reset();
		mqdc32.reset();
		mtdc32.reset();
	}
};

samuraiData samurai;
void SetSpiritBranchAddress(TChain *&chain) {
	chain->SetBranchAddress("run", &samurai.run);
	chain->SetBranchAddress("event", &samurai.event);
	chain->SetBranchAddress("lupots", &samurai.lupots);
	chain->SetBranchAddress("easiroc_hadc", &samurai.vme_easiroc.hadc[0]);
	chain->SetBranchAddress("easiroc_ladc", &samurai.vme_easiroc.ladc[0]);
	chain->SetBranchAddress("easiroc_ltdc", &samurai.vme_easiroc.ltdc[0]);
	chain->SetBranchAddress("easiroc_ttdc", &samurai.vme_easiroc.ttdc[0]);
	chain->SetBranchAddress("v1290_ltdc", &samurai.v1290.ltdc[0]);
	chain->SetBranchAddress("v1290_ttdc", &samurai.v1290.ttdc[0]);

	chain->SetBranchAddress("sis3820_scaler", &samurai.sis3820.scaler[0]);
	chain->SetBranchAddress("amttdc_x1", &samurai.amttdc.tdc_x1[0]);
	chain->SetBranchAddress("amttdc_y1", &samurai.amttdc.tdc_y1[0]);
	chain->SetBranchAddress("amttdc_x2", &samurai.amttdc.tdc_x2[0]);
	chain->SetBranchAddress("amttdc_y2", &samurai.amttdc.tdc_y2[0]);

	chain->SetBranchAddress("mqdc32_qdc", &samurai.mqdc32.qdc[0]);
	chain->SetBranchAddress("mtdc32_tdc", &samurai.mtdc32.tdc[0]);
	chain->SetBranchAddress("mtdc32_trigger", &samurai.mtdc32.trigger[0]);
	return;
}

#endif