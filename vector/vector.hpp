#include <iostream>
#include <memory>
#include <utility>

namespace reimplimentation {
    template <typename datatype>
    class vector {
        public:
            vector() = default;
            ~vector() = default;
        
            void push_back(datatype data) {
                if (size == capacity) {
                    size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
                    size_t total_bytes = new_capacity * size_of(datatype);
                    datatype* new_memory = alloc.allocate(new_capacity); // allocate new memory buffer

                    for (size_t i = 0; i < size; i++) {
                        //std::allocator_traits<std::allocator<datatype>>
                    }
                }
            }
        private:
            unsigned int size = 0;
            unsigned int capacity = 0;
            std::allocator<datatype> alloc;
            datatype* memory = nullptr;
    };
}

int main() {
    std::cout << "Hello, World! ";
    return 0;
}
