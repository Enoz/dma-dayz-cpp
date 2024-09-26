#pragma once

#include <vector>
#include <string>


namespace DayZ {
	const std::vector<std::string> InventoryItemWhitelist{ "ProxyMagazines", "Weapon" };
	const std::vector<std::string> itemBlacklist {"AreaDamageTriggerBase", "clothing", "inventoryItem", "item", "TrapTrigger", "itemoptics", "TripWireTrigger", "ClutterCutter6x6", "GardenPlotPolytunnel", "GardenPlot", "GardenPlotGreenhouse", "ContaminatedTrigger_Dynamic", "ContaminatedArea_Dynamic", "AreaDamageTriggerBase"};
}