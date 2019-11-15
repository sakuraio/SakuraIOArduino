#include "SakuraIOUtils.h"

namespace SakuraIOUtils {

  float double2float(uint64_t d){

    uint8_t sign = d>>63;
    uint32_t exponent = (d>>52) & 0x7ff;
    uint64_t fraction = d & 0xfffffffffffff;


    uint32_t fv = 0;
    fv |= (uint32_t)sign << 31;
    fv |= (uint32_t)(exponent>>3)<<23;
    fv |= (uint32_t)(fraction>>29);

    float *pf = (float *)&fv;
    return *pf;
  }

}
