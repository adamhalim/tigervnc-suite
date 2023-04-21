#ifndef __MANAGER_H__
#define __MANAGER_H__
#include "rfb/EncodeManager.h"
#include "rfb/SConnection.h"

namespace suite {
    
    class Manager : public rfb::EncodeManager 
    {
    public:
        Manager(class rfb::SConnection *conn);
        ~Manager();
    };
}
#endif
