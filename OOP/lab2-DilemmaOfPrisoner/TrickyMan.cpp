#include "TrickyMan.h"

Choice TrickyMan::Run()
{
	if (move <=2)
	{
		move++;
		TrickyLogs[0] = Choice::DECEIVE;
		TrickyLogs[1] = Choice::DECEIVE;
		return Choice::DECEIVE;
	}
	else
	{
		int d = 0;
		if (TrickyLogs[0] == Choice::DECEIVE)
		{
			d--;
		}
		if (TrickyLogs[1] == Choice::DECEIVE)
		{
			d--;
		}
		TrickyLogs[0] = TrickyLogs[1];
		for (int i = 0; i < 6; i++)
		{
			if (GameLogs[i] == Choice::DECEIVE)
			{
				d++;
			}
		}
		if (d > 2)
		{
			TrickyLogs[1] = Choice::COOPERATE;
			return Choice::COOPERATE;
		}
		else
		{
			TrickyLogs[1] = Choice::DECEIVE;
			return Choice::DECEIVE;
		}
	}
}

void TrickyMan::SetLogs(const std::array<Choice,3> Result)
{
	GameLogs[0] = GameLogs[3];
	GameLogs[1] = GameLogs[4];
	GameLogs[2] = GameLogs[5];
	GameLogs[3] = Result[0];
	GameLogs[4] = Result[1];
	GameLogs[5] = Result[2];
}