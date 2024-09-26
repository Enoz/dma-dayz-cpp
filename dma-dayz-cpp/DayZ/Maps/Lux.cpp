#include "Lux.h"

ImVec2 DayZ::Lux::translatePoint(const DMARender::Vector3& gamePoint)
{
    return ImVec2(gamePoint.x, 15360.0f - gamePoint.z);
}
