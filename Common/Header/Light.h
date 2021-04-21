#include "Tools.h"

class Light
{
public:
    float radius;//0 + 4 = 4
    M3DVector3f position;//16(对其16所以4->16) + 16 = 32
    M3DVector3f color;//32 + 16 = 48
};
