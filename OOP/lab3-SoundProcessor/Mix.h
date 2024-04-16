#pragma once

#include "IConverter.h"

class Mix : public IConverter
{
	int FileBits;
public:
	std::array<unsigned short, 44100> Convert(std::vector<std::array<unsigned short, 44100>> Samples, Config Regime, int Time) override;
	void SetBits(int FileGcnt) override;
};
