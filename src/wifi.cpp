#include "include/wifi.h"

#include <iostream>
#include <string>

#include "pico/cyw43_arch.h"

using namespace std;

namespace mw_tools {
namespace wifi {

void Connect(string ssid, string pass, uint32_t timeout) {
    printf("Hello");
    cout << "Setting WiFi station mode... " << endl;
    cyw43_arch_enable_sta_mode();

    cout << "Connecting to WiFi AP with SSID: " << ssid << endl;

    int err = 0;
    while((err = cyw43_arch_wifi_connect_timeout_ms(ssid.c_str(), pass.c_str(), CYW43_AUTH_WPA2_AES_PSK, timeout)) != 0){
        cout << "Error while connecting: " << err << endl;
        if (err == PICO_ERROR_TIMEOUT) {
            cout << "Probably SSID was not found: " << ssid << endl;
        } else if (err == PICO_ERROR_BADAUTH) {
            cout << "Probably password is incorrect: " << pass << endl;
        }

        cout << "Attempting to connect to WIFI " << ssid << "..." << endl;
    }
};
} // namespace wifi
} // namespace mw_tools