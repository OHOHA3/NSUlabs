#pragma once

#include <fstream>
#include <iostream>

class Pars
{
	std::ifstream File;
	std::string Line;
public:
	bool CheckConfig(int Val, char** Arguments);
	bool ReadFile();
	std::string GetLine();
	void PrintHelp();
	void ErrorArguments();
	void ErrorConfig();
	void ErrorMusic(int Number);
	bool CheckArguments(int Val, char** Arguments);
	bool CheckMusic(int Val, char** Arguments);
};