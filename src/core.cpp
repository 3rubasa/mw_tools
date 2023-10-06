#include "include/core.h"
#include "pico/stdlib.h"

#include "pico/cyw43_arch.h"

namespace mw_tools {
namespace core {
    void Init() {
        stdio_init_all();
		printf("INIT()\n");
        cyw43_arch_init();
		printf("INIT() 2\n");
    }
}
}