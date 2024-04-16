#include "Mix.h"

std::array<unsigned short, 44100> Mix::Convert(std::vector<std::array<unsigned short, 44100>> Samples, Config Regime, int Time)
{
	if (Time >= Regime.Begin)
	{
		for (int k = 0; k < FileBits; k++)
		{
			Samples[0][k] = Samples[0][k] / 2 + Samples[Regime.File - 1][k] / 2;
		}
	}
	return Samples[0];
}

void Mix::SetBits(int FileGcnt)
{
	FileBits = FileGcnt;
}