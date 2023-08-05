#include "Livonia.h"

ImVec2 DayZ::Livonia::translatePoint(const DMARender::Vector3& gamePoint)
{
    float mult = 15360.0f / 12800.0f;
    return ImVec2(gamePoint.x * mult, (12800.0f - gamePoint.z) * mult);
}
