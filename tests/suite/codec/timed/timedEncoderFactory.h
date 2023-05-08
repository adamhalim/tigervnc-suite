#ifndef __SUITE_TIMEDENCODERFACTORY_H__
#define __SUITE_TIMEDENCODERFACTORY_H__

#include "TimedEncoder.h"
#include "rfb/SConnection.h"

namespace suite {
  rfb::Encoder* constructEncoder(EncoderClass encoder, rfb::SConnection* conn);
}

#endif // __SUITE_TIMEDENCODERFACTORY_H__
