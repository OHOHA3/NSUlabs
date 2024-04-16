#pragma once

#include <map>
#include "IStrategy.h"
#include <memory>
#include "MimicMan.h"
#include "BadMan.h"
#include "GoodMan.h"
#include "CrazyMan.h"
#include "TrickyMan.h"
#include "WiseMan.h"
#include "Factory.h"
#include "Mode.h"

class Game
{
	std::shared_ptr<IStrategy> Tactics[3];
	Factory FactoryObj;
	std::vector<std::string> Strategies;
	int Rounds = 10;
	Mode mode=Mode::DETAILED;
	std::vector<int> Scores;
	std::map<std::string, int> ScoreMap{{ "CCC", 777 },
										{ "CCD", 339 },
										{ "CDC", 393 },
										{ "DCC", 933 },
										{ "CDD", 155 },
										{ "DCD", 505 },
										{ "DDC", 550 },
										{ "DDD", 111 }};
	void RegisterStrategies();

	void PrintHelp() const;

	void Step(const int i, const int j, const int k);

	void PrintHeadTable(const int i, const int j, const int k) const;

	void PrintResults(const int i, const int j, const int k) const;

	void Fast(const int i, const int j, const int k);

	void Detailed();

	void Tournament();
public:
	bool ParsingInput(const int Val, char** Parameters);

	void Play(const int Val, char** Parameters);
};


