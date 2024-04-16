#pragma once

#include "IStrategy.h"

class CrazyMan : public IStrategy
{
public:
	Choice Run() override;

	void SetLogs(const std::array<Choice,3> Result) override;
};
