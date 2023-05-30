#ifndef __SUITE_TIMEDENCODERFACTORY_H__
#define __SUITE_TIMEDENCODERFACTORY_H__

#include "TimedEncoder.h"
#include "rfb/SConnection.h"

namespace suite {
  TimedEncoder* constructTimedEncoder(EncoderClass encoder,
                                      rfb::SConnection* conn,
                                      Manager* manager);
}

#endif // __SUITE_TIMEDENCODERFACTORY_H__
