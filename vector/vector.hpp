#pragma once
#include <cstddef>
#include <memory>
#include <utility>

namespace reimplimentation {
    template <typename datatype>
    class vector {
        public:
            vector() {
                _size_ = 0;
                _capacity_ = 0;
                _memory_ = nullptr;
            }

            vector(const vector& other) {
                if (other.size == 0) {
                    _size_ = 0;
                    _capacity_ = 0;
                    _memory_ = nullptr;
                    return;
                }

                _memory_ = copy_allocate(other.memory, other.size);
                _size_ = other.size;
                _capacity_ = other.size;
            }

            vector(vector&& other) noexcept {
                _memory_ = other.memory;
                _size_ = other.size;
                _capacity_ = other.capacity;

                other.memory = nullptr;
                other.size = 0;
                other.capacity = 0;
            }

            ~vector() {
                destroy();
            }

            vector& operator=(const vector& other) {
                if (this == &other) {
                    return *this;
                } else if (other.size == 0) {
                    destroy();
                } else {
                    datatype* new_memory = copy_allocate(other.memory, other.size);

                    destroy();

                    _memory_ = new_memory;
                    _capacity_ = other.size;
                    _size_ = other.size;
                }

                return *this;
            }

            void push_back(datatype data) {
                reserve(_size_ + 1);
                std::allocator_traits<std::allocator<datatype>>::construct(_alloc_, _memory_ + _size_, std::move(data)); // safely (AKA ***** it We BALL) move data to new vector
                _size_++;
            }

            std::size_t length() const {
                return _size_;
            }

        private:
            std::size_t _size_;
            std::size_t _capacity_;
            std::allocator<datatype> _alloc_;
            datatype* _memory_;

            void reserve(const std::size_t requestedCapacity) {
                if (_capacity_ >= requestedCapacity) { // Dont do unneeded work;
                    return;
                }

                std::size_t new_capacity = _capacity_;
                while (new_capacity < requestedCapacity) {
                    new_capacity = (new_capacity == 0) ? 1 : new_capacity * 2;
                }

                datatype* new_memory = _alloc_.allocate(new_capacity); // allocate new memory buffer

                std::size_t constructed = 0;

                try {
                    for (std::size_t i = 0; i < _size_; i++) { // safely move data to new allocated array
                        std::allocator_traits<std::allocator<datatype>>::construct(_alloc_, new_memory + i, std::move_if_noexcept(_memory_[i]));
                        constructed++;
                    }
                } catch (...) { // Catch Exceptions
                    for (std::size_t i = 0; i < constructed; i++) {
                        std::allocator_traits<std::allocator<datatype>>::destroy(_alloc_, new_memory + i);
                    }
                    _alloc_.deallocate(new_memory, new_capacity);
                    throw;
                }

                destroy();

                _size_ = constructed;
                _memory_ = new_memory;
                _capacity_ = new_capacity;
            }

            datatype* copy_allocate(const datatype* source, std::size_t count) {
                if (count == 0) {
                    return nullptr;
                }

                datatype* new_memory = _alloc_.allocate(count);

                std::size_t constructed = 0;

                try {
                    for (std::size_t i = 0; i < count; i++) {
                        std::allocator_traits<std::allocator<datatype>>::construct(_alloc_, new_memory + i, source[i]);
                        constructed++;
                    }
                } catch (...) {
                    for (std::size_t i = 0; i < constructed; i++) {
                        std::allocator_traits<std::allocator<datatype>>::destroy(_alloc_, new_memory + i);
                    }
                    _alloc_.deallocate(new_memory, count);
                    throw;
                }

                return new_memory;
            }

            void destroy() {
                for (std::size_t i = 0; i < _size_; i++) {
                    std::allocator_traits<std::allocator<datatype>>::destroy(_alloc_, _memory_ + i);
                }

                if (_memory_ != nullptr) {
                    _alloc_.deallocate(_memory_, _capacity_);
                }

                _memory_ = nullptr;
                _capacity_ = 0;
                _size_ = 0;
            }
    };
}
