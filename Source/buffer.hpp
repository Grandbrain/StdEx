/// \file buffer.hpp
/// \brief Classes and functions that implement buffer structure.
/// \bug No known bugs.

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <initializer_list>
#include <stdexcept>
#include <memory>

/// Contains classes and functions that extend the C++ STL.
namespace stdex {

    /// Template class that provides dynamic storage for various data types.
    /// \tparam T Type of stored data.
    template <typename T>
    class buffer {
    public:

        /// Constructor without parameters.
        buffer() noexcept
            : data_(nullptr), size_(0), capacity_(0) {
        }

        /// Copy constructor.
        /// \param object Object to copy.
        buffer(const buffer& object)
            : data_(nullptr), size_(0), capacity_(0) {
            assign(object);
        }

        /// Move constructor.
        /// \param object Object to move.
        buffer(buffer&& object) noexcept
            : data_(nullptr), size_(0), capacity_(0) {
            assign(std::move(object));
        }

        /// Constructor with initializer list.
        /// \param list Initializer list.
        buffer(const std::initializer_list<T>& list)
            : data_(nullptr), size_(0), capacity_(0) {
            assign(list);
        }

        /// Constructor with data array.
        /// \param data Data pointer.
        /// \param size Data size.
        buffer(const T* data, std::size_t size)
            : data_(nullptr), size_(0), capacity_(0) {
            assign(data, size);
        }

        /// Constructor with data array and capacity.
        /// \param data Data pointer.
        /// \param size Data size.
        /// \param capacity Capacity.
        buffer(const T* data, std::size_t size, std::size_t capacity)
            : data_(nullptr), size_(0), capacity_(0) {
            assign(data, size, capacity);
        }

        /// Constructor with capacity.
        /// \param capacity Capacity.
        buffer(std::size_t capacity)
            : data_(nullptr), size_(0), capacity_(0) {
            assign(capacity);
        }

        /// Destructor.
        ~buffer() noexcept {
            clear();
        }

        /// Returns an element at the specified index.
        /// \param index Index.
        /// \return Element at the specified index.
        T& operator[](const std::size_t index) noexcept {
            return data()[index];
        }

        /// Returns an element at the specified index.
        /// \param index Index.
        /// \return Element at the specified index.
        T operator[](const std::size_t index) const noexcept {
            return data()[index];
        }

        /// Copy assignment operator.
        /// \param object Object to copy.
        /// \return This object.
        buffer& operator=(const buffer& object) {
            if (&object != this) assign(object);
            return *this;
        }

        /// Move assignment operator.
        /// \param object Object to move.
        /// \return This object.
        buffer& operator=(buffer&& object) noexcept {
            assign(std::move(object));
            return *this;
        }

        /// Assignment operator with initializer list.
        /// \param list Initializer list.
        /// \return This object.
        buffer& operator=(const std::initializer_list<T>& list) {
            assign(list);
            return *this;
        }

        /// Assigns an existing object.
        /// \param object Object to copy.
        void assign(const buffer& object) {
            if (this != &object)
                assign(object.data_, object.size_, object.capacity_);
        }

        /// Assigns a temporary object.
        /// \param object Object to move.
        void assign(buffer&& object) noexcept {
            if (this != &object) {
                std::swap(data_, object.data_);
                std::swap(size_, object.size_);
                std::swap(capacity_, object.capacity_);
                object.clear();
            }
        }

        /// Assigns an initializer list.
        /// \param list Initializer list.
        void assign(const std::initializer_list<T>& list) {
            assign(list.begin(), list.size());
        }

        /// Assigns a capacity.
        /// \param capacity Capacity.
        void assign(std::size_t capacity) {
            if (capacity != capacity_)
                assign(data_, size_, capacity);
        }

        /// Assigns a data array.
        /// \param data Data pointer.
        /// \param size Data size.
        void assign(const T* data, std::size_t size) {
            assign(data, size, size);
        }

        /// Assigns a data array and capacity.
        /// \param data Data pointer.
        /// \param size Data size.
        /// \param capacity Capacity.
        void assign(const T* data, std::size_t size, std::size_t capacity) {
            size = capacity > size ? size : capacity;

            std::unique_ptr<T[]> safe_tmp {
                capacity > 0 ? new T[capacity]() : nullptr
            };

            if (safe_tmp && data && size > 0)
                std::copy(data, data + size, safe_tmp.get());

            auto tmp = safe_tmp.release();
            safe_tmp.reset(data_);
            data_ = tmp;

            size_ = data_ ? size : 0;
            capacity_ = data_ ? capacity : 0;
        }

        /// Appends an existing object.
        /// \param object Object to copy.
        void append(const buffer& object) {
            append(object.data_, object.size_);
        }

        /// Appends an initializer list.
        /// \param list Initializer list.
        void append(const std::initializer_list<T>& list) {
            append(list.begin(), list.size());
        }

        /// Appends a single value.
        /// \param value Value.
        void append(const T& value) {
            append(&value, 1);
        }

        /// Appends a data array.
        /// \param data Data pointer.
        /// \param size Data size.
        void append(const T* data, std::size_t size) {
            insert(data, size, size_);
        }

        /// Inserts an existing object at the specified position.
        /// \param object Object to copy.
        /// \param position Insertion position.
        void insert(const buffer& object, std::size_t position) {
            insert(object.data_, object.size_, position);
        }

        /// Inserts an initializer list at the specified position.
        /// \param list Initializer list.
        /// \param position Insertion position.
        void insert(const std::initializer_list<T>& list,
                    std::size_t position) {

            insert(list.begin(), list.size(), position);
        }

        /// Inserts a single value at the specified position.
        /// \param value Value.
        /// \param position Insertion position.
        void insert(const T& value, std::size_t position) {
            insert(&value, 1, position);
        }

        /// Inserts a data array at the specified position.
        /// \param data Data pointer.
        /// \param size Data size.
        /// \param position Insertion position.
        void insert(const T* data, std::size_t size, std::size_t position) {
            if (position > size_)
                throw std::invalid_argument(
                    "the insertion position must be less than or equal to "
                    "the size of the container");

            auto capacity = size + size_;
            auto exceeds = capacity > capacity_;

            if (exceeds) assign(capacity);

            if (data && size > 0) {
                std::copy_backward(data_ + position,
                                   data_ + size_,
                                   data_ + capacity);

                std::copy(data, data + size, data_ + position);
            }

            size_ = capacity;
        }

        /// Reduces container's capacity to fit its size.
        void shrink_to_fit() {
            if (capacity_ != size_)
                assign(data_, size_);
        }

        /// Clears and destroys object.
        void clear() noexcept {
            T* tmp = data_;
            data_ = nullptr;
            size_ = capacity_ = 0;
            delete[] tmp;
        }

        /// Acquires data array to manage.
        /// \param data Data pointer.
        /// \param size Data size.
        /// \warning The function doesn't check if the data matches the size.
        /// Memory will be cleared as if it was allocated by operator new.
        void acquire(T* data, std::size_t size) {
            acquire(data, size, size);
        }

        /// Acquires data array to manage.
        /// \param data Data pointer.
        /// \param size Data size.
        /// \param capacity Capacity.
        /// \throw std::invalid_argument if any of the arguments is null.
        /// \warning The function doesn't check if the data matches the size.
        /// Memory will be cleared as if it was allocated by operator new.
        void acquire(T* data, std::size_t size, std::size_t capacity) {
            if (!data || size == 0 || size > capacity)
                throw std::invalid_argument("arguments are invalid");

            clear();

            data_ = data;
            size_ = size;
            capacity_ = capacity;
        }

        /// Releases data pointer.
        /// \return Data pointer.
        T* release() noexcept {
            T* data = data_;
            data_ = nullptr;
            size_ = capacity_ = 0;
            return data;
        }

        /// Swaps with another object.
        /// \param object Object to swap.
        void swap(buffer& object) noexcept {
            buffer tmp(std::move(object));
            object = std::move(*this);
            *this = std::move(tmp);
        }

        /// Returns an element at the specified index.
        /// \param index Index.
        /// \return Element at the specified index.
        /// \throw std::out_of_range if the index is out of range.
        T& at(const std::size_t index) {
            if (index >= size_)
                throw std::out_of_range("index is out of range");

            return data_[index];
        }

        /// Returns an element at the specified index.
        /// \param index Index.
        /// \return Element at the specified index.
        /// \throw std::out_of_range if the index is out of range.
        T at(const std::size_t index) const {
            if (index >= size_)
                throw std::out_of_range("index is out of range");

            return data_[index];
        }

        /// Returns the first element.
        /// \return First element.
        T& first() {
            return at(0);
        }

        /// Returns the first element.
        /// \return First element.
        T first() const {
            return at(0);
        }

        /// Returns the last element.
        /// \return Last element.
        T& last() {
            return at(size() - 1);
        }

        /// Returns the last element.
        /// \return Last element.
        T last() const {
            return at(size() - 1);
        };

        /// Returns size.
        /// \return Size.
        std::size_t size() const noexcept {
            return size_;
        }

        /// Returns capacity.
        /// \return Capacity.
        std::size_t capacity() const noexcept {
            return capacity_;
        }

        /// Returns data pointer.
        /// \return Data pointer.
        T* data() noexcept {
            return data_;
        }

        /// Returns data pointer.
        /// \return Data pointer.
        const T* data() const noexcept {
            return data_;
        }

        /// Indicates if data is empty.
        /// \retval true if data is empty.
        /// \retval false if data is not empty.
        bool empty() const noexcept {
            return size() == 0;
        }

        /// Returns an iterator pointing to the first element of the container.
        /// \return Iterator pointing to the first element of the container.
        T* begin() noexcept {
            return data();
        }

        /// Returns an iterator pointing to the first element of the container.
        /// \return Iterator pointing to the first element of the container.
        const T* begin() const noexcept {
            return data();
        }

        /// Returns an iterator pointing to one past the last element of the
        /// container.
        /// \return Iterator pointing to one past the last element of the
        /// container.
        T* end() noexcept {
            return begin() + size();
        }

        /// Returns an iterator pointing to one past the last element of the
        /// container.
        /// \return Iterator pointing to one past the last element of the
        /// container.
        const T* end() const noexcept {
            return begin() + size();
        }

    private:

        /// Buffer data.
        T* data_;

        /// Buffer size.
        std::size_t size_;

        /// Buffer capacity.
        std::size_t capacity_;
    };

    /// Char buffer type.
    using char_buffer = buffer<char>;

#if __cplusplus >= 201703L
    /// Byte buffer type.
    using byte_buffer = buffer<std::byte>;
#endif

    /// Swaps objects.
    /// \tparam T Object type.
    /// \param a First object to swap.
    /// \param b Second object to swap.
    template <typename T>
    inline void swap(buffer<T>& a, buffer<T>& b) noexcept {
        a.swap(b);
    }

    /// Equality operator.
    /// \tparam T Object type.
    /// \param a First object to compare.
    /// \param b Second object to compare.
    /// \retval true if objects are equal.
    /// \retval false if objects are not equal.
    template <typename T>
    inline bool operator==(const buffer<T>& a, const buffer<T>& b) {
        return (a.size() == b.size()) &&
               std::equal(a.begin(), a.end(), b.begin());
    }

    /// Equality operator.
    /// \tparam T Object type.
    /// \param a First object to compare.
    /// \param b Second object to compare.
    /// \retval true if objects are not equal.
    /// \retval false if objects are equal.
    template <typename T>
    inline bool operator!=(const buffer<T>& a, const buffer<T>& b) {
        return !(a == b);
    }
}

#endif
