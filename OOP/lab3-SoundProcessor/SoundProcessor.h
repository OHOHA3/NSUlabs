#pragma once

#include <fstream>
#include "WAVContainer.h"
#include "Config.h"
#include <vector>

class SoundProcessor
{
	std::vector<std::ifstream> InFile;
	std::ofstream OutFile;
	WAVHEADER Header;
	std::vector<Config> Regime;
	std::vector<std::array<unsigned short,44100>> Samples;
public:
	void Convert();
	void Preparing(int Val, char** FileNames);
};