#include "sbt.hh"

void SbtDetectors::update(
	const std::array<int, 32> &qdc, const std::array<int, 32> &tdc, const std::array<int, 2> &trigger
) {
	// qdc ch 8,9 for 25 mu; 10,11 for 100 mu
	// tdc ch 6, 7 for 25 mu; 8, 9 for 100 mu
	sbt1.update(0, {qdc[8], qdc[9]}, {tdc[6], tdc[7]}, trigger[0]);
	sbt2.update(1, {qdc[10], qdc[11]}, {tdc[8], tdc[9]}, trigger[0]);
}

void SbtDetectors::reset() {
	sbt1.reset();
	sbt2.reset();
}

void SbtDetector::update(int detId, const std::array<int, 2> &qdc, const std::array<int, 2> &tdc, int trigger) {
	charge = std::sqrt(qdc[0] * qdc[1]);
	// time = 0.5 * (tdc[0] + tdc[1]);
	time = 0.5 * (tdc[0] + tdc[1]) - trigger;
	trigger = trigger;

	detID = detId;
	thickness = (detId == 0) ? SBT1THICKNESS : SBT2THICKNESS;
}

void SbtDetector::reset() {
	charge = -1;
	time = -1;
	trigger = -1;
	detID = -1;
	thickness = -1;
}