#include "stdafx.h"
#include "ManagerTemplate.h"

namespace UPasta
{
	std::vector<StopCast*>Args;
	std::vector<NewPath>path;

	std::map<int, NewPath> PredAllDashData;
	std::map <int, int> PredAllNewPathTicks;
	Vector3 PredLastYasuoWallCastPos;
}