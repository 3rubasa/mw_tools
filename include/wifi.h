#pragma once

#include <string> 

namespace mw_tools {
namespace wifi {
    void Init();
    void Connect(std::string ssid, std::string pass, uint32_t timeout);
}
}

