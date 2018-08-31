#ifndef PIMPL_H
#define PIMPL_H

#include <utility>
#include <cstddef>

namespace stdex {

    /**
     * Template class that implements Fast Pimpl idiom.
     * @tparam U Storage type.
     * @tparam N Storage size.
     */
    template <typename U, std::size_t N = 64>
    class pimpl_ptr {
    public:

        /**
         * Constructor with a variable number of arguments.
         * @tparam A Argument type.
         * @param args Arguments for the stored class.
         */
        template <typename... A, typename =
            typename std::enable_if<std::is_constructible<U, A...>{}>::type>
        pimpl_ptr(A&&... args) {
            static_assert(sizeof(U) <= sizeof(store_), "Too big");
            new (static_cast<void*>(&store_)) U(std::forward<A>(args)...);
        }

        /**
         * Template copy constructor.
         * @tparam M Size of the storage to be copied.
         * @tparam K Type of the storage to be copied.
         * @param other Object to copy.
         */
        template <std::size_t M, typename K = U, typename =
            typename std::enable_if<std::is_copy_constructible<K>{}>::type>
        pimpl_ptr(const pimpl_ptr<U, M>& other) {
            static_assert(sizeof(other) <= sizeof(store_), "Too big");
            new (static_cast<void*>(&store_)) U(*other);
        }

        /**
         * Template move constructor.
         * @tparam M Size of the storage to be moved.
         * @tparam K Type of the storage to be moved.
         * @param other Object to move.
         */
        template <std::size_t M, typename K = U, typename =
            typename std::enable_if<std::is_move_constructible<K>{}>::type>
        pimpl_ptr(pimpl_ptr<U, M>&& other) {
            static_assert(sizeof(other) <= sizeof(store_), "Too big");
            new (static_cast<void*>(&store_)) U(std::move(*other));
        }

        /**
         * Copy constructor.
         * @param other Object to move.
         */
        pimpl_ptr(const pimpl_ptr& other) {
            new (static_cast<void*>(&store_)) U(*other);
        }

        /**
         * Destructor.
         */
        ~pimpl_ptr() {
            get()->~U();
        }

        /**
         * Template copy assignment operator.
         * @tparam M Size of the storage to be copied.
         * @tparam K Type of the storage to be copied.
         * @param other Object to copy.
         * @return This object.
         */
        template <std::size_t M, typename K = U, typename =
            typename std::enable_if<std::is_copy_assignable<K>{}>::type>
        pimpl_ptr& operator=(const pimpl_ptr<U, M>& other) {
            static_assert(sizeof(other) <= sizeof(store_), "Too big");
            **this = *other;
            return *this;
        }

        /**
         * Template move assignment operator.
         * @tparam M Size of the storage to be moved.
         * @tparam K Type of the storage to be moved.
         * @param other Object to move.
         * @return This object.
         */
        template <std::size_t M, typename K = U, typename =
            typename std::enable_if<std::is_move_assignable<K>{}>::type>
        pimpl_ptr& operator=(pimpl_ptr<U, M>&& other) {
            static_assert(sizeof(other) <= sizeof(store_), "Too big");
            **this = std::move(*other);
            return *this;
        }

        /**
         * Copy assignment operator.
         * @param other Object to copy.
         * @return This object.
         */
        pimpl_ptr& operator=(const pimpl_ptr& other) {
            **this = *other;
            return *this;
        }

        /**
         * Arrow operator.
         * @return Pointer to the stored class.
         */
        U* operator->() noexcept {
            return reinterpret_cast<U*>(&store_);
        }

        /**
         * Arrow operator.
         * @return Pointer to the stored class.
         */
        const U* operator->() const noexcept {
            return reinterpret_cast<const U*>(&store_);
        }

        /**
         * Dereference operator.
         * @return Reference to the stored class.
         */
        U& operator*() noexcept {
            return *reinterpret_cast<U*>(&store_);
        }

        /**
         * Dereference operator.
         * @return Reference to the stored class.
         */
        const U& operator*() const noexcept {
            return *reinterpret_cast<const U*>(&store_);
        }

        /**
         * Returns a pointer to the stored class.
         * @return Pointer to the stored class.
         */
        U* get() noexcept {
            return reinterpret_cast<U*>(&store_);
        }

        /**
         * Returns a pointer to the stored class.
         * @return Pointer to the stored class.
         */
        const U* get() const noexcept {
            return reinterpret_cast<const U*>(&store_);
        }

    private:
        typename std::aligned_storage<N>::type store_;
    };
}

#endif