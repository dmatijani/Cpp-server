#include "server.h"

int main() {
    Server server("0.0.0.0", 12345);
    server.run();
    return 0;
}
