#pragma once
#include <filesystem> 
#include <string> 
#include <fstream>
#include "skStr.h"
#include "json.hpp"
using json = nlohmann::json;

bool WriteToJson(std::string path, std::string name, std::string value, bool userpass, std::string name2, std::string value2) 
{
	json file;
	if (!userpass) 
	{
		file[name] = value;
	}
	else
	{
		file[name] = value;
		file[name2] = value2;
	}

	std::ofstream jsonfile(path, std::ios::out);
	jsonfile << file;
	jsonfile.close();
	return true;
}
