#include "stdafx.h"
#include "StevenMath.h"


/**
*	Author: Steven Wu
*/


static bool ___GenChoiceLoop(std::vector<std::vector<int>>& ret, const std::vector<int>& prefix, int from, int max, int level)
{
	if (level <= 0)
		return false;

	if (level == 1)
	{
		for (int a = from; a < max; a++)
		{
			std::vector<int> vt = prefix;
			vt.push_back(a);
			ret.push_back(vt);
		}
		return true;
	}

	for (int a = from; a < max; a++)
	{
		int sub_choice = level - 1;
		std::vector<int> vt = prefix;
		vt.push_back(a);
		int sub_from = a + 1;
		bool rst = ___GenChoiceLoop(ret, vt, sub_from, max, sub_choice);
		if (!rst)
			break;
	}
}

std::vector<std::vector<int>> GenChoice(int all, int choice)
{
	std::vector<int> prefix;
	std::vector<std::vector<int>> ret;
	___GenChoiceLoop(ret, prefix, 0, all, choice);
	return ret;
}