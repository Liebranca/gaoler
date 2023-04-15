#ifndef __2A_CONSTS_H__
#define __2A_CONSTS_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---

namespace Gaol::Mode {

  cx8 NOCOL   = 0x00;
  cx8 STATIC  = 0x01;
  cx8 DYNAMIC = 0x02;

};

namespace Gaol::Limit {

  cxr32 NORMAL = 0.075f;
  cxr32 MARGIN = 0.25f;
  cxr32 CLAMP  = 0.004f;
  cxr32 DEACC  = 0.95f;

};

// ---   *   ---   *   ---

#endif //__GAOL_CONSTS_H__
