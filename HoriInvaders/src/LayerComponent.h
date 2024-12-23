#pragma once

#include <unordered_set>
#include <string>

struct LayerComponent
{
	// Check if layers set contains all specified layers
	bool Contains(std::vector<std::string>&& checkLayers)
	{
		for (auto& checkLayer : checkLayers)
		{
			if (!layers.contains(checkLayer))
				return false;
		}

		return true;
	}

	std::unordered_set<std::string> layers;
};