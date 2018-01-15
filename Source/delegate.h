#ifndef DELEGATE_H
#define DELEGATE_H

namespace stdex {

    /**
     * Template class that implements delegate base.
     * Non-specialized version.
     * @tparam T Object type.
     */
    template <typename T>
    class delegate_base;


    /**
     * Template class that implements delegate base.
     * @tparam R Return type.
     * @tparam P Argument type.
     */
    template <typename R, typename ...P>
    class delegate_base<R(P...)> {
    protected:

        /**
         * Type of the called function.
         */
        using function_type = R(*)(void*, P...);

        /**
         * Internal class that stores pointers to an object and a function.
         */
        class invocation final {
        public:

            /**
             * Constructor without arguments.
             */
            invocation() noexcept
                    : object_(nullptr), function_(nullptr) {
            }

            /**
             * Copy constructor.
             * @param other Object to copy.
             */
            invocation(const invocation& other) noexcept
                    : object_(other.object_), function_(other.function_) {
            }

            /**
             * Constructor with pointers to an object and a function.
             * @param object Object pointer.
             * @param function Function pointer.
             */
            invocation(void* object, function_type function) noexcept
                    : object_(object), function_(function) {
            }

            /**
             * Indicates if objects are equal.
             * @param other Object to compare.
             * @return True if objects are equal and false otherwise.
             */
            bool equal(const invocation& other) const noexcept {
                return object_ == other.object_ && function_ == other.function_;
            }

            /**
             * Returns a pointer to an object.
             * @return Pointer to an object.
             */
            void* object() const noexcept {
                return object_;
            }

            /**
             * Returns a pointer to a function.
             * @return Pointer to a function.
             */
            function_type function() const noexcept {
                return function_;
            }

        private:

            /**
             * Pointer to an object.
             */
            void* object_;

            /**
             * Pointer to a function.
             */
            function_type function_;
        };
    };


    /**
     * Template class that implements delegate.
     * Non-specialized version.
     * @tparam T Object type.
     */
    template <typename T>
    class delegate;


    /**
     * Template class that implements delegate.
     * @tparam R Return type.
     * @tparam P Parameter type.
     */
    template <typename R, typename ...P>
    class delegate<R(P...)> final : private delegate_base<R(P...)> {
    public:

        /**
         * Constructor without arguments.
         */
        delegate() noexcept
                : invocation_() {
        }

        /**
         * Boolean operator.
         * @return True if function pointer not null and false otherwise.
         */
        explicit operator bool() const noexcept {
            return invocation_.function() != nullptr;
        }

        /**
         * Equality operator.
         * @param other Object to compare.
         * @return True if objects are equal and false otherwise.
         */
        bool operator==(const delegate& other) const noexcept {
            return invocation_.equal(other.invocation_);
        }

        /**
         * Equality operator.
         * @param other Object to compare.
         * @return True if objects aren't equal and false otherwise.
         */
        bool operator!=(const delegate& other) const noexcept {
            return !invocation_.equal(other.invocation_);
        }

        /**
         * Function call operator.
         * @param arg Possible arguments of the function.
         * @return Return value of the function.
         */
        R operator()(P... arg) const {
            return (*invocation_.function())(invocation_.object(), arg...);
        }

        /**
         * Returns a delegate that points to a member function.
         * @tparam T Object type.
         * @tparam M Member function.
         * @param object Object.
         * @return Delegate that points to a member function.
         */
        template <typename T, R(T::*M)(P...)>
        static delegate create(T& object) noexcept {
            return delegate(&object, method<T, M>);
        }

        /**
         * Returns a delegate that points to a const member function.
         * @tparam T Object type.
         * @tparam M Const member function.
         * @param object Object.
         * @return Delegate that points to a const member function.
         */
        template <typename T, R(T::*M)(P...) const>
        static delegate create(const T& object) noexcept {
            return delegate(const_cast<T*>(&object), const_method<T, M>);
        }

        /**
         * Returns a delegate that points to a free function.
         * @tparam M Free function.
         * @return Delegate that points to a free function.
         */
        template <R(*M)(P...)>
        static delegate create() noexcept {
            return delegate(nullptr, free_function<M>);
        }

    private:

        /**
         * Constructor with the pointers to an object an a function.
         * @param object Object.
         * @param function Function.
         */
        delegate(void* object,
                 typename delegate_base<R(P...)>::function_type function)
                noexcept : invocation_(object, function) {
        }

        /**
         * Called a member function with the specified arguments.
         * @tparam T Object type.
         * @tparam M Member function.
         * @param object Object.
         * @param args Arguments.
         * @return Return value of the function.
         */
        template <typename T, R(T::*M)(P...)>
        static R method(void* object, P... args) {
            return (static_cast<T*>(object)->*M)(args...);
        };

        /**
         * Calls a const member function with the specified arguments.
         * @tparam T Object type.
         * @tparam M Const member function.
         * @param object Object.
         * @param args Arguments.
         * @return Return value of the function.
         */
        template <typename T, R(T::*M)(P...) const>
        static R const_method(void* object, P... args) {
            return (static_cast<const T*>(object)->*M)(args...);
        };

        /**
         * Calls a free function with the specified arguments.
         * @tparam M Free function.
         * @param object Object.
         * @param args Arguments.
         * @return Return value of the function.
         */
        template <R(*M)(P...)>
        static R free_function(void* object, P... args) {
            return (M)(args...);
        }

    private:

        /**
         * Storage of the pointers to an object and a function.
         */
        typename delegate_base<R(P...)>::invocation invocation_;
    };
}

#endif