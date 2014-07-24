#pragma once
#include "Collider.h"

namespace ElysiumEngine
{
    void narrowPhase(ICollider *one, ICollider *two);
    //void broadPhase();
    
    bool intersectRay(ICollider *collider, Vec4 dir, Vec4 origin);
}