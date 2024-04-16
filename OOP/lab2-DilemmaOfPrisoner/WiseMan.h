#pragma once

#include "IStrategy.h"

class WiseMan : public IStrategy
{
	std::array<Choice,9> GameLogs={Choice::DECEIVE};
	std::array<Choice,3> WiseLogs={Choice::DECEIVE};
	int move = 1;
public:
	Choice Run() override;

	void SetLogs(const std::array<Choice,3> Result) override;
};
