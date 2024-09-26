#include "Deerisle.h"

ImVec2 DayZ::Deerisle::translatePoint(const DMARender::Vector3& gamePoint)
{
    return ImVec2(gamePoint.x, 16384.0f - gamePoint.z);
}
