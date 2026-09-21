#include <iostream>

namespace reimplimentation {
    template <typename datatype>
    class vector {
        public:
            vector() {}
            ~vector() {}
        private:
            unsigned int size = 0;
            unsigned int capacity = 0;
    };
}

int main() {
    std::cout << "Hello, World! ";
    return 0;
}
