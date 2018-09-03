#ifndef RANGE_ACCESS_HPP
#define RANGE_ACCESS_HPP

namespace stdex {

    /**
     * Returns an iterator pointing to the first element of the container.
     * @tparam T Container type.
     * @param c Container.
     * @return Iterator pointing to the first element of the container.
     */
    template<typename T>
    inline auto begin(T& c) -> decltype(c.begin()) {
        return c.begin();
    }

    /**
     * Returns an iterator pointing to the first element of the container.
     * @tparam T Container type.
     * @param c Container.
     * @return Iterator pointing to the first element of the container.
     */
    template<typename T>
    inline auto begin(const T& c) -> decltype(c.begin()) {
        return c.begin();
    }

    /**
     * Returns an iterator pointing to one past the last element of the
     * container.
     * @tparam T Container type.
     * @param c Container.
     * @return Iterator pointing to one past the last element of the container.
     */
    template<typename T>
    inline auto end(T& c) -> decltype(c.end()) {
        return c.end();
    }

    /**
     * Returns an iterator pointing to one past the last element of the
     * container.
     * @tparam T Container type.
     * @param c Container.
     * @return Iterator pointing to one past the last element of the container.
     */
    template<typename T>
    inline auto end(const T& c) -> decltype(c.end()) {
        return c.end();
    }

    /**
     * Returns an iterator pointing to the first element of the array.
     * @tparam T Array type.
     * @tparam N Array size.
     * @param a Array.
     * @return Iterator pointing to the first element of the array.
     */
    template<typename T, size_t N>
    inline T* begin(T (& a)[N]) {
        return a;
    }

    /**
     * Returns an iterator pointing to one past the last element of the array.
     * @tparam T Array type.
     * @tparam N Array size.
     * @param a Array.
     * @return Iterator pointing to one past the last element of the array.
     */
    template<typename T, size_t N>
    inline T* end(T (& a)[N]) {
        return a + N;
    }
}

#endif
