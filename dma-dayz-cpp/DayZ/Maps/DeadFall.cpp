#include "DeadFall.h"

ImVec2 DayZ::DeadFall::translatePoint(const DMARender::Vector3& gamePoint)
{
    return ImVec2(gamePoint.x, 10000.0f - gamePoint.z);
}
