#include <iostream>
#include "App.h"


int main() {
    std::cout << "Renderer geladen!\n";
    App app(1920, 1080);
    app.Run();
    return 0;
}