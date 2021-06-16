#include "KnxHelper.h"
#include "hardware.h"


// That is not a final list. In case you need more dpt types, you can add this dpt to this Array
static Dpt sDpt[] = {Dpt(1, 1), Dpt(2, 1), Dpt(5, 10), Dpt(5, 1), Dpt(6, 1), Dpt(7, 1), Dpt(8, 1), Dpt(9, 2), Dpt(16, 1), Dpt(17, 1), Dpt(232, 600), Dpt(10,1,1), Dpt(11,1), Dpt(3,7), Dpt(3,8), Dpt(9, 4), Dpt(1, 19), Dpt(9, 20), Dpt(1, 18), Dpt(9, 8), Dpt(9, 7), Dpt(9, 1)};

Dpt &getDPT(uint8_t iDptIndex)
{
    return sDpt[iDptIndex];
}



// ensure correct time delta check
// cannot be used in interrupt handler
bool delayCheck(uint32_t iOldTimer, uint32_t iDuration)
{
    return millis() - iOldTimer >= iDuration;
}






