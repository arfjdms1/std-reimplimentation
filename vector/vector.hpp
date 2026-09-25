#pragma once
#include <cstddef>
#include <memory>
#include <utility>

namespace reimplimentation {
    template <typename datatype>
    class vector {
        public:
            vector() {
                size_ = 0;
                capacity_ = 0;
                memory_ = nullptr;
            }

            vector(const vector& other) {
                if (other.size_ == 0) {
                    size_ = 0;
                    capacity_ = 0;
                    memory_ = nullptr;
                    return;
                }

                memory_ = copy_allocate(other.memory_, other.size_);
                size_ = other.size_;
                capacity_ = other.size_;
            }

            vector(vector&& other) noexcept {
                memory_ = other.memory_;
                size_ = other.size_;
                capacity_ = other.capacity_;

                other.memory_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            }

            ~vector() {
                destroy();
            }

            vector& operator=(const vector& other) {
                if (this == &other) {
                    return *this;
                } else if (other.size_ == 0) {
                    destroy();
                } else {
                    datatype* new_memory = copy_allocate(other.memory_, other.size_);

                    destroy();

                    memory_ = new_memory;
                    capacity_ = other.size_;
                    size_ = other.size_;
                }

                return *this;
            }

            vector& operator=(vector&& other) noexcept {
                if (this != &other) {
                    destroy();

                    memory_ = other.memory_;
                    size_ = other.size_;
                    capacity_ = other.capacity_;

                    other.memory_ = nullptr;
                    other.size_ = 0;
                    other.capacity_ = 0;
                }

                return *this;
            }

            void push_back(datatype data) {
                reserve(size_ + 1);
                std::allocator_traits<std::allocator<datatype>>::construct(alloc_, memory_ + size_, std::move(data)); // safely (AKA ***** it We BALL) move data to new vector
                size_++;
            }

            void reserve(const std::size_t requestedCapacity) {
                if (capacity_ >= requestedCapacity) { // Dont do unneeded work;
                    return;
                }

                std::size_t new_capacity = capacity_;
                while (new_capacity < requestedCapacity) {
                    new_capacity = (new_capacity == 0) ? 1 : new_capacity * 2;
                }

                datatype* new_memory = alloc_.allocate(new_capacity); // allocate new memory buffer

                std::size_t constructed = 0;

                try {
                    for (std::size_t i = 0; i < size_; i++) { // safely move data to new allocated array
                        std::allocator_traits<std::allocator<datatype>>::construct(alloc_, new_memory + i, std::move_if_noexcept(memory_[i]));
                        constructed++;
                    }
                } catch (...) { // Catch Exceptions
                    for (std::size_t i = 0; i < constructed; i++) {
                        std::allocator_traits<std::allocator<datatype>>::destroy(alloc_, new_memory + i);
                    }
                    alloc_.deallocate(new_memory, new_capacity);
                    throw;
                }

                destroy();

                size_ = constructed;
                memory_ = new_memory;
                capacity_ = new_capacity;
            }

            void resize(std::size_t amount) {
                if (amount < size_) {
                    for (std::size_t i = amount; i < size_; i++) {
                        std::allocator_traits<std::allocator<datatype>>::destroy(alloc_, memory_ + i);
                    }
                    size_ = amount;
                } else if (amount > size_) {
                    reserve(amount);

                    std::size_t constructed = 0;

                    try {
                        for (std::size_t i = size_; i < amount; i++) {
                            std::allocator_traits<std::allocator<datatype>>::construct(alloc_, memory_ + i);
                            constructed++;
                        }
                    } catch (...) {
                        for (std::size_t i = size_; i < size_ + constructed; i++) {
                            std::allocator_traits<std::allocator<datatype>>::destroy(alloc_, memory_ + i);
                        }
                        throw;
                    }

                    size_ = amount;
                }
            }

            std::size_t size() const {
                return size_;
            }

        private:
            std::size_t size_;
            std::size_t capacity_;
            std::allocator<datatype> alloc_;
            datatype* memory_;

            datatype* copy_allocate(const datatype* source, std::size_t count) {
                if (count == 0) {
                    return nullptr;
                }

                datatype* new_memory = alloc_.allocate(count);

                std::size_t constructed = 0;

                try {
                    for (std::size_t i = 0; i < count; i++) {
                        std::allocator_traits<std::allocator<datatype>>::construct(alloc_, new_memory + i, source[i]);
                        constructed++;
                    }
                } catch (...) {
                    for (std::size_t i = 0; i < constructed; i++) {
                        std::allocator_traits<std::allocator<datatype>>::destroy(alloc_, new_memory + i);
                    }
                    alloc_.deallocate(new_memory, count);
                    throw;
                }

                return new_memory;
            }

            void destroy() {
                for (std::size_t i = 0; i < size_; i++) {
                    std::allocator_traits<std::allocator<datatype>>::destroy(alloc_, memory_ + i);
                }

                if (memory_ != nullptr) {
                    alloc_.deallocate(memory_, capacity_);
                }

                memory_ = nullptr;
                capacity_ = 0;
                size_ = 0;
            }
    };
}
