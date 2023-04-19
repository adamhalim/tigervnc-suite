#include "Manager.h"
#include "rfb/SConnection.h"

Manager::Manager(class rfb::SConnection *conn) :
  EncodeManager(conn)
{
}

Manager::~Manager()
{
}
