#pragma once
#include "WozFile.h"

class WozDisk
{
public:
	WozDisk();

	void AddCycles(uint32_t cycles);
	void PhaseOn(uint8_t iPhase);
	void PhaseOff(uint8_t iPhase);
	int8_t MoveTrackPosition(int iDiff);
	bool GetWriteProtect();
	uint8_t GetBits(uint8_t &bits);
	bool GetNextBit(uint8_t &bit);
	void SetSpinning(bool spinning);
	bool InsertDisk(const char* filename);
	void RemoveDisk();
	bool IsDiskPresent();
	void UpdateMagneticField();
	void DoRotation();
	uint8_t ToothBefore();
	uint8_t ToothAfter();
	int8_t UpdateWheel();

private:
	WozFile		_WozFile;
	int16_t		_trackPosition;
	bool		_phase[4] = {false};
	uint8_t     _magneticField[8];
	uint8_t     _toothPosition = 0;
	uint64_t    _cycles = 0;
	uint64_t    _lastShiftCycle = 0;
	bool		_spinning = false;
	int32_t     _delayMomentum = 0;
};