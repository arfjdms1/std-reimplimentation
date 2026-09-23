#pragma once
#include <iostream>
#include <memory>
#include <utility>

namespace reimplimentation {
    template <typename datatype>
    class vector {
        public:
            vector() {
                size = 0;
                capacity = 0;
                memory = nullptr;
            }

            ~vector() = default;

            void push_back(datatype data) {
                if (size == capacity) {
                    reserve(size + 1);
                }

                std::allocator_traits<std::allocator<datatype>>::construct(alloc, memory + size, data); // copy new data
                size++;
            }
        private:
            unsigned int size;
            unsigned int capacity;
            std::allocator<datatype> alloc;
            datatype* memory;

            void reserve(int requestedCapacity) {
                size_t new_capacity = capacity;
                while (new_capacity < requestedCapacity) {
                    new_capacity = (new_capacity == 0) ? 1 : new_capacity * 2;
                }

                datatype* new_memory = alloc.allocate(new_capacity); // allocate new memory buffer

                for (size_t i = 0; i < size; i++) { // copy data to new allocated array
                    std::allocator_traits<std::allocator<datatype>>::construct(alloc, new_memory + i, memory[i]);
                }

                for (size_t i = 0; i < size; i++) { // dealoc objects of old array
                    std::allocator_traits<std::allocator<datatype>>::destroy(alloc, memory + i);
                }

                if (memory != nullptr) {
                    alloc.deallocate(memory, capacity);
                }
                memory = new_memory;
                capacity = new_capacity;
            }
    };
}