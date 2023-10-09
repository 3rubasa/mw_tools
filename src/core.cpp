#include "include/core.h"
#include "pico/stdlib.h"

#include "pico/cyw43_arch.h"
#include <lwip/apps/httpd.h>

namespace mw_tools {
namespace core {
    void Init() {
        stdio_init_all();
		printf("stdio initialized\n");
        cyw43_arch_init();
		printf("cyw43_arch initialized\n");
        httpd_init();
        printf("Httpd server initialised\n");
    }
}
}