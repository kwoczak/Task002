#include "message_module.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        init_receiver();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
