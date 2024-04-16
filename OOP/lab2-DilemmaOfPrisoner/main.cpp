#include "Game.h"

int main(int argc, char* argv[])
{
	Game play;
	if (!play.ParsingInput(argc,argv))
	{
		return 0;
	}
	play.Play(argc, argv);
	return 0;
}