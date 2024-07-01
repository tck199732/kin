#include "hime_veto.hh"
#include <iostream>
HimeVeto::HimeVeto(const std::string &calibFile) {
	reset();
	tdiff_offset.fill(0.0);
	tdiff_slope.fill(1.0);

	std::ifstream ifs(calibFile.c_str());
	if (!ifs.is_open()) {
		throw std::runtime_error(("Cannot open the file: " + calibFile).c_str());
	}

	int barID;
	while (ifs.peek() == '#') {
		ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	while (ifs >> barID) {
		if (barID < 0 || barID > 2) {
			throw std::runtime_error("Invalid barID");
			break;
		}
		ifs >> tdiff_offset[barID] >> tdiff_slope[barID];
	}

	ifs.close();
}

void HimeVeto::update(const std::array<int, 32> &ltdc, const std::array<int, 32> &ttdc) {
	std::array<int, 32> tdc_window;
	for (auto i = 0; i < 32; i++) {
		tdc_window[i] = ttdc[i] - ltdc[i];
	}

	// top
	if (tdc_window[6] > 0 && tdc_window[1] > 0) {
		bars[multi] = 0;
		tofs[multi] = 0.5 * (ltdc[1] + ltdc[6]) - ltdc[31];
		charges[multi] = std::sqrt(tdc_window[1] * tdc_window[6]);
		tdiffs[multi] = (ltdc[6] - ltdc[1]) - tdiff_offset[0];
		xpos[multi] = tdiffs[multi] * tdiff_slope[0];
		multi++;
	}
	// mid
	if (tdc_window[2] > 0 && tdc_window[3] > 0) {
		bars[multi] = 1;
		tofs[multi] = 0.5 * (ltdc[3] + ltdc[2]) - ltdc[31];
		charges[multi] = std::sqrt(tdc_window[3] * tdc_window[2]);
		tdiffs[multi] = (ltdc[2] - ltdc[3]) - tdiff_offset[1];
		xpos[multi] = tdiffs[multi] * tdiff_slope[1];
		multi++;
	}
	// bottom
	if (tdc_window[4] > 0 && tdc_window[7] > 0) {
		bars[multi] = 2;
		tofs[multi] = 0.5 * (ltdc[4] + ltdc[7]) - ltdc[31];
		charges[multi] = std::sqrt(tdc_window[4] * tdc_window[7]);
		tdiffs[multi] = ltdc[4] - ltdc[7] - tdiff_offset[2];
		xpos[multi] = tdiffs[multi] * tdiff_slope[2];
		multi++;
	}
}

void HimeVeto::reset() {
	multi = 0;
	bars.fill(-1);
	tofs.fill(-1);
	charges.fill(-1);
	tdiffs.fill(-1);
	xpos.fill(-1);
	return;
}