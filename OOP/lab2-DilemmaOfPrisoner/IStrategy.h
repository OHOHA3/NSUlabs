#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Choice.h"
#include <array>

class IStrategy
{
public:
	virtual ~IStrategy() = default;

	virtual Choice Run() = 0;

	virtual void SetLogs(const std::array<Choice,3> Result) = 0;
};