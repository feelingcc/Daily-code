#include "echo.hpp"

int main() {

    EchoServer server(8500);
    server.start();

    return 0;
}