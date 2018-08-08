#ifndef BUFFER_H
#define BUFFER_H

#include <stdexcept>
#include <range_access.h>

namespace stdex {

    /**
     * Template class that provides dynamic storage for various data types.
     * @tparam T Type of stored data.
     */
    template <typename T>
    class buffer {
    public:

        /**
         * Constructor without parameters.
         */
        buffer() noexcept
                : data_(nullptr), size_(0), capacity_(0) {
        }

        /**
         * Copy constructor.
         * @param other Object to copy.
         */
        buffer(const buffer& other)
                : data_(nullptr), size_(0), capacity_(0) {
            assign(other);
        }

        /**
         * Move constructor.
         * @param other Object to move.
         */
        buffer(buffer&& other) noexcept
                : data_(nullptr), size_(0), capacity_(0) {
            assign(std::move(other));
        }

        /**
         * Constructor with initializer list.
         * @param list Initializer list.
         */
        buffer(const std::initializer_list<T>& list)
                : data_(nullptr), size_(0), capacity_(0) {
            assign(list);
        }

        /**
         * Constructor with capacity.
         * @param capacity Capacity.
         */
        buffer(size_t capacity)
                : data_(nullptr), size_(0), capacity_(0) {
            assign(capacity);
        }

        /**
         * Constructor with data array.
         * @param data Data pointer.
         * @param size Data size.
         */
        buffer(const T* data, size_t size)
                : data_(nullptr), size_(0), capacity_(0) {
            assign(data, size);
        }

        /**
         * Constructor with data array and capacity.
         * @param data Data pointer.
         * @param size Data size.
         * @param capacity Capacity.
         */
        buffer(const T* data, size_t size, size_t capacity)
                : data_(nullptr), size_(0), capacity_(0) {
            assign(data, size, capacity);
        }

        /**
         * Destructor.
         */
        ~buffer() noexcept {
            clear();
        }

        /**
         * Returns an element at the specified index.
         * @param index Index.
         * @return Element at the specified index.
         */
        T& operator[](const size_t index) noexcept {
            return data()[index];
        }

        /**
         * Returns an element at the specified index.
         * @param index Index.
         * @return Element at the specified index.
         */
        T operator[](const size_t index) const noexcept {
            return data()[index];
        }

        /**
         * Copy assignment operator.
         * @param other Object to copy.
         * @return This object.
         */
        buffer& operator=(const buffer& other) {
            assign(other);
            return *this;
        }

        /**
         * Move assignment operator.
         * @param other Object to move.
         * @return This object.
         */
        buffer& operator=(buffer&& other) noexcept {
            assign(std::move(other));
            return *this;
        }

        /**
         * Assignment operator with initializer list.
         * @param list Initializer list.
         * @return This object.
         */
        buffer& operator=(const std::initializer_list<T>& list) {
            assign(list);
            return *this;
        }

        /**
         * Assigns an existing object.
         * @param other Object to copy.
         */
        void assign(const buffer& other) {
            if (this != &other)
                assign(other.data_, other.size_, other.capacity_);
        }

        /**
         * Assigns a temporary object.
         * @param other Object to move.
         */
        void assign(buffer&& other) noexcept {
            if (this != &other) {
                std::swap(data_, other.data_);
                std::swap(size_, other.size_);
                std::swap(capacity_, other.capacity_);
                other.clear();
            }
        }

        /**
         * Assigns an initializer list.
         * @param list Initializer list.
         */
        void assign(const std::initializer_list<T>& list) {
            assign(list.begin(), list.size());
        }

        /**
         * Assigns a capacity.
         * @param capacity Capacity.
         */
        void assign(size_t capacity) {
            auto size = capacity > size_ ? size_ : capacity;
            auto tmp = capacity > 0 ? new T[capacity]() : nullptr;

            if (data_ && tmp && size > 0)
                std::copy(data_, data_ + size, tmp);

            delete[] data_;
            data_ = tmp;
            size_ = tmp ? size : 0;
            capacity_ = tmp ? capacity : 0;
        }

        /**
         * Assigns a data array.
         * @param data Data pointer.
         * @param size Data size.
         */
        void assign(const T* data, size_t size) {
            assign(data, size, size);
        }

        /**
         * Assigns a data array and capacity.
         * @param data Data pointer.
         * @param size Data size.
         * @param capacity Capacity.
         */
        void assign(const T* data, size_t size, size_t capacity) {
            capacity = size > capacity ? size : capacity;

            auto tmp = capacity > 0 ? new T[capacity]() : nullptr;

            if (tmp && data && size > 0)
                std::copy(data, data + size, tmp);

            delete[] data_;
            data_ = tmp;
            size_ = tmp ? size : 0;
            capacity_ = tmp ? capacity : 0;
        }

        /**
         * Appends an existing object.
         * @param other Object to copy.
         */
        void append(const buffer& other) {
            append(other.data_, other.size_);
        }

        /**
         * Appends a temporary object.
         * @param other Object to move.
         */
        void append(buffer&& other) {
            append(other.data_, other.size_);
            other.clear();
        }

        /**
         * Appends an initializer list.
         * @param list Initializer list.
         */
        void append(const std::initializer_list<T>& list) {
            append(list.begin(), list.size());
        }

        /**
         * Appends a single value.
         * @param value Value.
         */
        void append(const T& value) {
            append(&value, 1);
        }

        /**
         * Appends a data array.
         * @param data Data pointer.
         * @param size Data size.
         */
        void append(const T* data, size_t size) {
            auto capacity = size + size_;
            auto exceeds = capacity > capacity_;
            auto tmp = exceeds ? new T[capacity]() : data_;

            if (exceeds && tmp && data_ && size_ > 0)
                std::copy(data_, data_ + size_, tmp);

            if (tmp && data && size > 0)
                std::copy(data, data + size, tmp + size_);

            if(exceeds) delete[] data_, data_ = tmp;
            size_ = size + size_;
            capacity_ = exceeds ? capacity : capacity_;
        }

        /**
         * Clears and destroys object.
         */
        void clear() noexcept {
            T* tmp = data_;
            data_ = nullptr;
            size_ = capacity_ = 0;
            delete[] tmp;
        }

        /**
         * Releases data pointer.
         * @return Data pointer.
         */
        T* release() noexcept {
            T* data = data_;
            data_ = nullptr;
            size_ = capacity_ = 0;
            return data;
        }

        /**
         * Swaps with another object.
         * @param other Object to swap.
         */
        void swap(buffer& other) noexcept {
            buffer tmp(std::move(other));
            other = std::move(*this);
            *this = std::move(tmp);
        }

        /**
         * Returns an element at the specified index.
         * @param index Index.
         * @return Element at the specified index.
         */
        T& at(const size_t index) {
            if (index >= size_)
                throw std::out_of_range("index out of range");

            return data_[index];
        }

        /**
         * Returns an element at the specified index.
         * @param index Index.
         * @return Element at the specified index.
         */
        T at(const size_t index) const {
            if (index >= size_)
                throw std::out_of_range("index out of range");

            return data_[index];
        }

        /**
         * Returns the first element.
         * @return First element.
         */
        T& first() {
            return at(0);
        }

        /**
         * Returns the first element.
         * @return First element.
         */
        T first() const {
            return at(0);
        }

        /**
         * Returns the last element.
         * @return Last element.
         */
        T& last() {
            return at(size() - 1);
        }

        /**
         * Returns the last element.
         * @return Last element.
         */
        T last() const {
            return at(size() - 1);
        };

        /**
         * Returns size.
         * @return Size.
         */
        size_t size() const noexcept {
            return size_;
        }

        /**
         * Returns capacity.
         * @return Capacity.
         */
        size_t capacity() const noexcept {
            return capacity_;
        }

        /**
         * Returns data pointer.
         * @return Data pointer.
         */
        T* data() noexcept {
            return data_;
        }

        /**
         * Returns data pointer.
         * @return Data pointer.
         */
        const T* data() const noexcept {
            return data_;
        }

        /**
         * Indicates if data is empty.
         * @return True if data is empty and false otherwise.
         */
        bool empty() const noexcept {
            return size() == 0;
        }

        /**
         * Returns an iterator pointing to the first element of the container.
         * @return Iterator pointing to the first element of the container.
         */
        T* begin() noexcept {
            return data();
        }

        /**
         * Returns an iterator pointing to the first element of the container.
         * @return Iterator pointing to the first element of the container.
         */
        const T* begin() const noexcept {
            return data();
        }

        /**
         * Returns an iterator pointing to one past the last element of the
         * container.
         * @return Iterator pointing to one past the last element of the
         * container.
         */
        T* end() noexcept {
            return begin() + size();
        }

        /**
         * Returns an iterator pointing to one past the last element of the
         * container.
         * @return Iterator pointing to one past the last element of the
         * container.
         */
        const T* end() const noexcept {
            return begin() + size();
        }

    private:

        /**
         * Buffer data.
         */
        T* data_;

        /**
         * Buffer size.
         */
        size_t size_;

        /**
         * Buffer capacity.
         */
        size_t capacity_;
    };

    /**
     * Byte buffer type.
     */
    using byte_buffer = buffer<uint8_t>;

    /**
     * Char buffer type.
     */
    using char_buffer = buffer<char>;

    /**
     * Swaps objects.
     * @tparam T Object type.
     * @param a First object to swap.
     * @param b Second object to swap.
     */
    template <typename T>
    inline void swap(buffer<T>& a, buffer<T>& b) noexcept {
        a.swap(b);
    }

    /**
     * Equality operator.
     * @tparam T Object type.
     * @param a First object to compare.
     * @param b Second object to compare.
     * @return True if objects are equal and false otherwise.
     */
    template <typename T>
    inline bool operator==(const buffer<T>& a, const buffer<T>& b) {
        return (a.size() == b.size()) &&
                std::equal(a.begin(), a.end(), b.begin());
    }

    /**
     * Equality operator.
     * @tparam T Object type.
     * @param a First object to compare.
     * @param b Second object to compare.
     * @return True if objects aren't equal and false otherwise.
     */
    template <typename T>
    inline bool operator!=(const buffer<T>& a, const buffer<T>& b) {
        return !(a == b);
    }
}

#endif
