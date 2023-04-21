#include "Manager.h"
#include "rfb/SConnection.h"

namespace suite {
  
  Manager::Manager(class rfb::SConnection *conn) :
    EncodeManager(conn)
  {
  }

  Manager::~Manager()
  {
  }
}
