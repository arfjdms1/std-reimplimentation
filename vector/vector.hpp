#pragma once
#include <cstddef>
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

            ~vector() {
                if (memory != nullptr) {
                    for (std::size_t i = 0; i < size; i++) {
                        std::allocator_traits<std::allocator<datatype>>::destroy(alloc, memory + i);
                    }
                    alloc.deallocate(memory, capacity);
                }
            }

            void push_back(datatype data) {
                reserve(size + 1);
                std::allocator_traits<std::allocator<datatype>>::construct(alloc, memory + size, std::move(data)); // safely (AKA Fuck it We BALL) move data to new vector
                size++;
            }

            int length() {
                return size;
            }

        private:
            std::size_t size;
            std::size_t capacity;
            std::allocator<datatype> alloc;
            datatype* memory;

            void reserve(std::size_t requestedCapacity) {
                if (capacity >= requestedCapacity) { // Dont do unneeded work;
                    return;
                }

                std::size_t new_capacity = capacity;
                while (new_capacity < requestedCapacity) {
                    new_capacity = (new_capacity == 0) ? 1 : new_capacity * 2;
                }

                datatype* new_memory = alloc.allocate(new_capacity); // allocate new memory buffer

                std::size_t constructed = 0;

                try {
                    for (std::size_t i = 0; i < size; i++) { // safely move data to new allocated array
                        std::allocator_traits<std::allocator<datatype>>::construct(alloc, new_memory + i, std::move_if_noexcept(memory[i]));
                        constructed++;
                    }
                } catch (...) { // Catch Exeptions
                    for (std::size_t i = 0; i < constructed; i++) {
                        std::allocator_traits<std::allocator<datatype>>::destroy(alloc, new_memory + i);
                    }
                    alloc.deallocate(new_memory, new_capacity);
                    throw;
                }

                for (std::size_t i = 0; i < size; i++) { // dealoc objects of old array
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
