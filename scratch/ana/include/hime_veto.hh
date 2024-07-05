#ifndef HIME_VETO_HH
#define HIME_VETO_HH
#include <array>
#include <cmath>
#include <fstream>
#include <map>
#include <string>
#include <iostream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

constexpr int MAX_HITS_HIME = 3;

class HimeVeto {
public:
	HimeVeto(const std::string &calibFile);
	~HimeVeto() {}

	void update(const std::array<int, 32> &ltdc, const std::array<int, 32> &ttdc);
	void reset();

	unsigned int multi;
	std::array<unsigned int, MAX_HITS_HIME> bars; // barID of each hit; 0 : top, 1 : middle, 2 : bottom
	std::array<double, MAX_HITS_HIME> tofs;		  // time of flight of each hit
	std::array<double, MAX_HITS_HIME> charges;	  // geometric mean of charge on both PMTs
	std::array<double, MAX_HITS_HIME> tdiffs;	  // time-diff of each hit

private:
    void readChannelMap(const std::string &calibFile);
    unsigned int mTopLeftChannel=6, mTopRightChannel=1;
    unsigned int mMidLeftChannel=2, mMidRightChannel=3;
    unsigned int mBotLeftChannel=4, mBotRightChannel=7;
    unsigned int mTriggerChannel=31;
};
#endif