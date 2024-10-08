/// Logic for handling interactions with emscripten

#ifdef __EMSCRIPTEN__

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>

extern "C" void nativeHandleDroppedData(uint8_t *data, size_t len) {
    for (size_t i = 0; i < std::min<size_t>(len, 10); ++i) {
        std::cout << (int)data[i] << std::endl;
    }
    std::cout << len << std::endl;
}

#endif
