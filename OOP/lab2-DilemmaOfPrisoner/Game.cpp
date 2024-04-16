#include "Game.h"

void Game::RegisterStrategies()
{
	FactoryObj.RegisterCreator("goodman", std::make_shared<Creator<GoodMan>>());
	FactoryObj.RegisterCreator("badman", std::make_shared<Creator<BadMan>>());
	FactoryObj.RegisterCreator("crazyman", std::make_shared<Creator<CrazyMan>>());
	FactoryObj.RegisterCreator("mimicman", std::make_shared<Creator<MimicMan>>());
	FactoryObj.RegisterCreator("trickyman", std::make_shared<Creator<TrickyMan>>());
	FactoryObj.RegisterCreator("wiseman", std::make_shared<Creator<WiseMan>>());
}

void Game::PrintHelp() const
{
	std::cout << "========================================================" << std::endl;
	std::cout << "invalid arguments" << std::endl;
	std::cout << "========================================================" << std::endl;
	std::cout << "factory strategy strategy...(min 3) --mode=mode --steps=n" << std::endl;
	std::cout << "========================================================" << std::endl;
	std::cout << "Strategies:" << std::endl << "goodman, badman, crazyman, mimicman, trickyman, wiseman" << std::endl;
	std::cout << "========================================================" << std::endl;
	std::cout << "modes(optional):" << std::endl << "detailed, fast, tournament" << std::endl;
	std::cout << "========================================================" << std::endl;
	std::cout << "n(optional)=N" << std::endl;
	std::cout << "========================================================" << std::endl;
}

bool Game::ParsingInput(const int Val, char** Parameters)
{
	RegisterStrategies();
	if (Val < 4)
	{
		PrintHelp();
		return false;
	}
	bool StepsFlag = false;
	bool ModeFlag = false;
	std::string Strategy = "";
	std::string ModeStr = "";
	for (int i = 1; i < Val; i++)
	{
		if (FactoryObj.CanCreate(Parameters[i]) && !StepsFlag && !ModeFlag)
		{
			Strategy = "";
			for (int j = 0; Parameters[i][j]; j++)
			{
				Strategy.push_back(Parameters[i][j]);
			}
			Strategies.push_back(Strategy);
		}
		else
		{
			if (Strategies.size() < 3)
			{
				PrintHelp();
				return false;
			}
			std::string CheckStr;
			for (int j = 0; Parameters[i][j]; j++)
			{
				CheckStr.push_back(Parameters[i][j]);
			}
			CheckStr.resize(8);
			if (CheckStr == "--steps=" && !StepsFlag)
			{
				StepsFlag = true;
				Rounds = 0;
				for (int j = 8; Parameters[i][j]; j++)
				{
					if (j==9 && Parameters[i][8]=='0')
					{
						PrintHelp();
						return false;
					}
					if (Parameters[i][j] >= '0' && Parameters[i][j] <= '9')
					{
						Rounds *= 10;
						Rounds += Parameters[i][j] - '0';
					}
					else
					{
						PrintHelp();
						return false;
					}
				}
				if (Rounds < 1)
				{
					PrintHelp();
					return false;
				}
			}
			else
			{
				if (StepsFlag && !ModeFlag)
				{
					PrintHelp();
					return false;
				}
				CheckStr.resize(7);
				if (CheckStr == "--mode=" && !ModeFlag)
				{
					ModeFlag = true;
					for (int j = 7; Parameters[i][j]; j++)
					{
						ModeStr.push_back(Parameters[i][j]);
					}
					if ((ModeStr == "detailed" || ModeStr == "fast") && Strategies.size() > 3)
					{
						PrintHelp();
						return false;
					}
					if (ModeStr != "detailed" && ModeStr != "fast" && ModeStr != "tournament")
					{
						PrintHelp();
						return false;
					}
				}
				else
				{
					PrintHelp();
					return false;
				}
			}
		}
	}
	Scores.resize(Strategies.size());
	if (ModeStr == "")
	{
		if (Strategies.size() == 3)
		{
			mode =Mode::DETAILED;
		}
		else
		{
			mode = Mode::TOURNAMENT;
		}
	}
	else
	{
		if (ModeStr == "detailed")
		{
			mode =Mode::DETAILED;
		}
		if (ModeStr == "fast")
		{
			mode = Mode::FAST;
		}
		if (ModeStr == "tournament")
		{
			mode = Mode::TOURNAMENT;
		}
	}
	return true;
}

void Game::Play(const int Val, char** Parameters)
{
	srand(time(NULL));
	switch (mode)
	{
	case Mode::DETAILED:
		Detailed();
		break;
	case Mode::FAST:
		Fast(0,1,2);
		break;
	case Mode::TOURNAMENT:
		Tournament();
		break;
	}
}

void Game::Step(const int i, const int j, const int k)
{
	std::string ResultStr = "";
	std::array<Choice,3> ResultEn={Choice::DECEIVE};
	int TmpScores[3] = { 0 };

	ResultEn[0] = Tactics[0]->Run();
	ResultEn[1] = Tactics[1]->Run();
	ResultEn[2] = Tactics[2]->Run();

	Tactics[0]->SetLogs(ResultEn);
	Tactics[1]->SetLogs(ResultEn);
	Tactics[2]->SetLogs(ResultEn);

	for (int l = 0; l < 3; l++)
	{
		if (ResultEn[l] == Choice::DECEIVE)
		{
			ResultStr.push_back('D');
		}
		else
		{
			ResultStr.push_back('C');
		}
	}
	TmpScores[0] = ScoreMap[ResultStr] / 100;
	TmpScores[1] = ScoreMap[ResultStr] / 10 % 10;
	TmpScores[2] = ScoreMap[ResultStr] % 10;

	Scores[i] += TmpScores[0];
	Scores[j] += TmpScores[1];
	Scores[k] += TmpScores[2];

	std::cout << ResultStr[0] << " ";
	std::cout << ResultStr[1] << " ";
	std::cout << ResultStr[2] << " -> ";
	std::cout << TmpScores[0] << " ";
	std::cout << TmpScores[1] << " ";
	std::cout << TmpScores[2] << std::endl;
}

void Game::PrintHeadTable(const int i, const int j, const int k) const
{
	std::cout << Strategies[i][0] << " " << Strategies[j][0] << " " << Strategies[k][0] << "    ";
	std::cout << Strategies[i][0] << " " << Strategies[j][0] << " " << Strategies[k][0] << std::endl;
}

void Game::PrintResults(const int i, const int j, const int k) const
{
	std::cout << Strategies[i] << " = " << Scores[i] << std::endl;
	std::cout << Strategies[j] << " = " << Scores[j] << std::endl;
	std::cout << Strategies[k] << " = " << Scores[k] << std::endl;
	std::cout << std::endl;
}

void Game::Fast(const int i, const int j, const int k)
{
	Tactics[0] = FactoryObj.Create(Strategies[i]);
	Tactics[1] = FactoryObj.Create(Strategies[j]);
	Tactics[2] = FactoryObj.Create(Strategies[k]);
	PrintHeadTable(i, j, k);
	for (int l = 0; l < Rounds; l++)
	{
		Step(i, j, k);
	};
	PrintResults(i, j, k);
}

void Game::Detailed()
{
	Tactics[0] = FactoryObj.Create(Strategies[0]);
	Tactics[1] = FactoryObj.Create(Strategies[1]);
	Tactics[2] = FactoryObj.Create(Strategies[2]);
	std::string exit = "";
	while (exit != "quit")
	{
		PrintHeadTable(0, 1, 2);
		Step(0, 1, 2);
		PrintResults(0, 1, 2);
		std::cin >> exit;
	};
}

void Game::Tournament()
{
	int ScoreFirst = 0;
	int ScoreSecond = 0;
	int ScoreThird = 0;
	int ValStrategies = Strategies.size();
	for (int i = 0; i < ValStrategies - 2; i++)
	{
		for (int j = i + 1; j < ValStrategies - 1; j++)
		{
			for (int k = j + 1; k < ValStrategies; k++)
			{
				ScoreFirst = Scores[i];
				ScoreSecond = Scores[j];
				ScoreThird = Scores[k];
				Scores[i] = 0;
				Scores[j] = 0;
				Scores[k] = 0;
				Fast(i,j,k);
				Scores[i] += ScoreFirst;
				Scores[j] += ScoreSecond;
				Scores[k] += ScoreThird;
			}
		}
	}
	std::vector<int> Place(ValStrategies,0);
	int Index = 0;
	int Max = 0;
	for (int i = 0; i < ValStrategies; i++)
	{
		Max = 0;
		for (int j = 0;j<ValStrategies;j++)
		{
			if (Scores[j]>Max && Place[j]!=1)
			{
				Max = Scores[j];
				Index = j;
			}
		}
		Place[Index]=1;
		std::cout <<"Place number "<<i+1<<" is "<< Strategies[Index] << " - " << Scores[Index] << std::endl;
	}
}

