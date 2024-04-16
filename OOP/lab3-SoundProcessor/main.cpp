#include <iostream>
#include "SoundProcessor.h"
#include "Pars.h"

int main(int argc, char** argv)
{
	Pars WorkFile;
	SoundProcessor Conv;
	if (!WorkFile.CheckArguments(argc, argv))
	{
		return 0;
	}
	if (!WorkFile.CheckConfig(argc, argv))
	{
		return 0;
	}
	if (!WorkFile.CheckMusic(argc, argv))
	{
		return 0;
	}
	Conv.Preparing(argc, argv);
	Conv.Convert();
	std::cout<<"Successful"<<std::endl;
	return 0;
}
