#pragma once

#include <vector>
#include <string>

namespace DayZ {
	const std::vector<std::string> InventoryItemWhitelist {"inventoryItem", "item", "clothing", "ProxyMagazines", "Weapon", "itemoptics"};
	const std::vector<std::string> itemBlacklist {"AreaDamageTriggerBase", "ClutterCutter6x6", "GardenPlotPolytunnel", "GardenPlot", "GardenPlotGreenhouse", "ContaminatedTrigger_Dynamic", "ContaminatedArea_Dynamic", "TrapTrigger", "AreaDamageTriggerBase"};
}