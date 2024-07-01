#pragma once
#include <cfloat>

namespace Hime {

const int nModules = 72;
const double barWidthInMeter = 0.04; // 40 mm
const double barGapInMeter = 0.004;	 // 4 mm
const double C_LIGHT = 0.299792458;	 // m/ns

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
}; // namespace Hime