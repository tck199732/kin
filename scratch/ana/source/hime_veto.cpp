#include "hime_veto.hh"

HimeVeto::HimeVeto(
    const std::string &calibFile
) {

	reset();
    readChannelMap(calibFile);
}

void HimeVeto::readChannelMap(const std::string &calibFile) {
    std::ifstream ifs(calibFile.c_str());
    if (!ifs.is_open()) {
        throw std::runtime_error(("Cannot open the file: " + calibFile).c_str());
    }
    std::map<std::string, unsigned int> channelJson = json::parse(ifs);
    mTopLeftChannel = channelJson["top_left"];
    mTopRightChannel = channelJson["top_right"];
    mMidLeftChannel = channelJson["mid_left"];
    mMidRightChannel = channelJson["mid_right"];
    mBotLeftChannel = channelJson["bot_left"];
    mBotRightChannel = channelJson["bot_right"];
    mTriggerChannel = channelJson["trigger"];
    ifs.close();
}

void HimeVeto::update(const std::array<int, 32> &ltdc, const std::array<int, 32> &ttdc) {
	std::array<int, 32> tdc_window;
	for (auto i = 0; i < 32; i++) {
		tdc_window[i] = ttdc[i] - ltdc[i];
	}

    // alias 
    auto chTL = mTopLeftChannel, chTR = mTopRightChannel;
    auto chML = mMidLeftChannel, chMR = mMidRightChannel;
    auto chBL = mBotLeftChannel, chBR = mBotRightChannel;
    auto chTrig = mTriggerChannel;

	// top
	if (tdc_window[chTL] > 0 && tdc_window[chTR] > 0) {
		bars[multi] = 0;
		tofs[multi] = 0.5 * (ltdc[chTL] + ltdc[chTR]) - ltdc[chTrig];
		charges[multi] = std::sqrt(tdc_window[chTL] * tdc_window[chTR]);
		tdiffs[multi] = ltdc[chTL] - ltdc[chTR];
		multi++;
	}
	// mid
	if (tdc_window[chML] > 0 && tdc_window[chMR] > 0) {
		bars[multi] = 1;
		tofs[multi] = 0.5 * (ltdc[chML] + ltdc[chMR]) - ltdc[chTrig];
		charges[multi] = std::sqrt(tdc_window[chML] * tdc_window[chMR]);
		tdiffs[multi] = ltdc[chML] - ltdc[chMR];
		multi++;
	}
	// bottom
	if (tdc_window[chBL] > 0 && tdc_window[chBR] > 0) {
		bars[multi] = 2;
		tofs[multi] = 0.5 * (ltdc[chBL] + ltdc[chBR]) - ltdc[chTrig];
		charges[multi] = std::sqrt(tdc_window[chBL] * tdc_window[chBR]);
		tdiffs[multi] = ltdc[chBL] - ltdc[chBR];
		multi++;
	}
}

void HimeVeto::reset() {
	multi = 0;
	bars.fill(-1);
	tofs.fill(-1);
	charges.fill(-1);
	tdiffs.fill(-1);
	return;
}