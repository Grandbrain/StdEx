/// \file delegate.hpp
/// \brief Classes and functions that implement delegate.
/// \bug No known bugs.

#ifndef DELEGATE_HPP
#define DELEGATE_HPP

#include <algorithm>
#include <list>

/// Contains classes and functions that extend the C++ STL.
namespace stdex {

    /// Non-specialized template class that implements delegate base.
    /// \tparam T Object type.
    template <typename T>
    class delegate_base;

    /// Template class that implements delegate base.
    /// \tparam R Return type.
    /// \tparam P Argument type.
    template <typename R, typename ...P>
    class delegate_base<R(P...)> {
    protected:

        /// Callback function type.
        using function_type = R(*)(void*, P...);

        /// Internal class that stores pointers to an object and a function.
        class invocation final {
        public:

            /// Constructor without arguments.
            invocation() noexcept
                : object_(nullptr), function_(nullptr) {
            }

            /// Constructor with pointers to an object and a function.
            /// \param object Object pointer.
            /// \param function Function pointer.
            invocation(void* object, const function_type function) noexcept
                : object_(object), function_(function) {
            }

            /// Boolean operator.
            /// \return True if function pointer is not null and false
            /// otherwise.
            explicit operator bool() const noexcept {
                return !empty();
            }

            /// Equality operator.
            /// \return True if function pointer is null and false otherwise.
            bool operator==(std::nullptr_t) const noexcept {
                return empty();
            }

            /// Equality operator.
            /// \return True if function pointer is not null and false
            /// otherwise.
            bool operator!=(std::nullptr_t) const noexcept {
                return !(*this == nullptr);
            }

            /// Equality operator.
            /// \param object Object to compare.
            /// \return True if objects are equal and false otherwise.
            bool operator==(const invocation& object) const noexcept {
                return object_ == object.object_ &&
                       function_ == object.function_;
            }

            /// Equality operator.
            /// \param object Object to compare.
            /// \return True if objects aren't equal and false otherwise.
            bool operator!=(const invocation& object) const noexcept {
                return !(*this == object);
            }

            /// Indicates if function pointer is null.
            /// \return True if function pointer is null and false otherwise.
            bool empty() const noexcept {
                return function_ == nullptr;
            }

            /// Returns a pointer to an object.
            /// \return Pointer to an object.
            void* object() const noexcept {
                return object_;
            }

            /// Returns a pointer to a function.
            /// \return Pointer to a function.
            function_type function() const noexcept {
                return function_;
            }

        private:

            /// Pointer to an object.
            void* object_;

            /// Pointer to a function.
            function_type function_;
        };
    };

    /// Non-specialized template class that implements delegate.
    /// \tparam T Object type.
    template <typename T>
    class delegate;

    /// Non-specialized template class that implements multicast delegate.
    /// \tparam T Object type.
    template <typename T>
    class multidelegate;

    /// Template class that implements delegate.
    /// \tparam R Return type.
    /// \tparam P Parameter type.
    template <typename R, typename ...P>
    class delegate<R(P...)> final : delegate_base<R(P...)> {
    public:

        /// Constructor without arguments.
        delegate() noexcept : invocation_() {
        }

        /// Constructor with the lambda object.
        /// \tparam L Lambda type.
        /// \param object Object.
        template <typename L>
        delegate(const L& object) noexcept
            : invocation_() {
            *this = object;
        }

        /// Assignment operator with the lambda object.
        /// \tparam L Lambda type.
        /// \param object Object.
        /// \return This object.
        template <typename L>
        delegate& operator=(const L& object) noexcept {
            invocation_ = typename delegate_base<R(P...)>::invocation(
                (void*) (&object), lambda < L > );
            return *this;
        }

        /// Boolean operator.
        /// \return True if function pointer is not null and false otherwise.
        explicit operator bool() const noexcept {
            return !empty();
        }

        /// Equality operator.
        /// \return True if function pointer is null and false otherwise.
        bool operator==(std::nullptr_t) const noexcept {
            return empty();
        }

        /// Equality operator.
        /// \return True if function pointer is not null and false otherwise.
        bool operator!=(std::nullptr_t) const noexcept {
            return !(*this == nullptr);
        }

        /// Equality operator.
        /// \param object Object to compare.
        /// \return True if objects are equal and false otherwise.
        bool operator==(const delegate& object) const noexcept {
            return invocation_ == object.invocation_;
        }

        /// Equality operator.
        /// \param object Object to compare.
        /// \return True if objects aren't equal and false otherwise.
        bool operator!=(const delegate& object) const noexcept {
            return !(*this == object);
        }

        /// Equality operator.
        /// \param object Object to compare.
        /// \return True if objects are equal and false otherwise.
        bool operator==(const multidelegate<R(P...)>& object) const noexcept {
            return object == *this;
        }

        /// Equality operator.
        /// \param object Object to compare.
        /// \return True if objects aren't equal and false otherwise.
        bool operator!=(const multidelegate<R(P...)>& object) const noexcept {
            return object != *this;
        }

        /// Function call operator.
        /// \param arg Possible arguments of the function.
        /// \return Return value of the function.
        R operator()(P... arg) const {
            return (*invocation_.function())(invocation_.object(), arg...);
        }

        /// Indicates if function pointer is null.
        /// \return True if function pointer is null and false otherwise.
        bool empty() const noexcept {
            return !invocation_;
        }

        /// Returns a delegate that points to a member function.
        /// \tparam T Object type.
        /// \tparam M Member function.
        /// \param object Object.
        /// \return Delegate that points to a member function.
        template <typename T, R(T::*M)(P...)>
        static delegate create(T& object) noexcept {
            return delegate(&object, method < T, M > );
        }

        /// Returns a delegate that points to a const member function.
        /// \tparam T Object type.
        /// \tparam M Const member function.
        /// \param object Object.
        /// \return Delegate that points to a const member function.
        template <typename T, R(T::*M)(P...) const>
        static delegate create(const T& object) noexcept {
            return delegate(const_cast<T*>(&object), const_method < T, M > );
        }

        /// Returns a delegate that points to a free function.
        /// \tparam M Free function.
        /// \return Delegate that points to a free function.
        template <R(* M)(P...)>
        static delegate create() noexcept {
            return delegate(nullptr, free_function < M > );
        }

        /// Returns a delegate that points to a lambda function.
        /// \tparam L Lambda type.
        /// \param object Object.
        /// \return Delegate that points to a lambda function.
        template <typename L>
        static delegate create(const L& object) noexcept {
            return delegate(&object, lambda < L > );
        }

        /// Returns a delegate that points to a member function.
        /// \tparam T Object type.
        /// \tparam M Member function.
        /// \param object Object pointer.
        /// \return Delegate that points to a member function.
        template <typename T, R(T::*M)(P...)>
        static delegate create(T* object) noexcept {
            return delegate(object, method < T, M > );
        }

        /// Returns a delegate that points to a const member function.
        /// \tparam T Object type.
        /// \tparam M Const member function.
        /// \param object Object pointer.
        /// \return Delegate that points to a const member function.
        template <typename T, R(T::*M)(P...) const>
        static delegate create(const T* object) noexcept {
            return delegate(const_cast<T*>(object), const_method < T, M > );
        }

    private:

        /// Constructor with the pointers to an object an a function.
        /// \param object Object.
        /// \param function Function.
        delegate(void* object,
                 typename delegate_base<R(P...)>::function_type function)
        noexcept : invocation_(object, function) {
        }

        /// Called a member function with the specified arguments.
        /// \tparam T Object type.
        /// \tparam M Member function.
        /// \param object Object.
        /// \param args Arguments.
        /// \return Return value of the function.
        template <typename T, R(T::*M)(P...)>
        static R method(void* object, P... args) {
            return (static_cast<T*>(object)->*M)(args...);
        }

        /// Calls a const member function with the specified arguments.
        /// \tparam T Object type.
        /// \tparam M Const member function.
        /// \param object Object.
        /// \param args Arguments.
        /// \return Return value of the function.
        template <typename T, R(T::*M)(P...) const>
        static R const_method(void* object, P... args) {
            return (static_cast<const T*>(object)->*M)(args...);
        }

        /// Calls a free function with the specified arguments.
        /// \tparam M Free function.
        /// \param object Object.
        /// \param args Arguments.
        /// \return Return value of the function.
        template <R(* M)(P...)>
        static R free_function(void* object, P... args) {
            return (M)(args...);
        }

        /// Calls a lambda function with the specified arguments.
        /// \tparam L Lambda type.
        /// \param object Object.
        /// \param args Arguments.
        /// \return Return value of the function.
        template <typename L>
        static R lambda(void* object, P... args) {
            return (static_cast<L*>(object)->operator())(args...);
        }

        /// Storage of the pointers to an object and a function.
        typename delegate_base<R(P...)>::invocation invocation_;

        /// Friend class multicast delegate for member access.
        friend class multidelegate<R(P...)>;
    };

    /// Template class that implements multicast delegate.
    /// \tparam R Return type.
    /// \tparam P Parameter type.
    template <typename R, typename ...P>
    class multidelegate<R(P...)> final : delegate_base<R(P...)> {
    public:

        /// Container type.
        using container_type =
        typename std::list<typename delegate_base<R(P...)>::invocation>;

        /// Container size type.
        using size_type = typename container_type::size_type;

        /// Constructor without arguments.
        multidelegate() noexcept
            : invocations_() {
        }

        /// Boolean operator.
        /// \return True if invocations is not empty and false otherwise.
        explicit operator bool() const noexcept {
            return !empty();
        }

        /// Equality operator.
        /// \return True if invocations is empty and false otherwise.
        bool operator==(std::nullptr_t) const noexcept {
            return empty();
        }

        /// Equality operator.
        /// \return True if invocations is not empty and false otherwise.
        bool operator!=(std::nullptr_t) const noexcept {
            return !(*this == nullptr);
        }

        /// Equality operator.
        /// \param object Object to compare.
        /// \return True if objects are equal and false otherwise.
        bool operator==(const multidelegate& object) const noexcept {
            return invocations_ == object.invocations_;
        }

        /// Equality operator.
        /// \param object Object to compare.
        /// \return True if objects aren't equal and false otherwise.
        bool operator!=(const multidelegate& object) const noexcept {
            return !(*this == object);
        }

        /// Equality operator.
        /// \param object Object to compare.
        /// \return True if objects are equal and false otherwise.
        bool operator==(const delegate<R(P...)>& object) const noexcept {
            if (!(*this) && !object) return true;
            if (size() != 1 || !object) return false;
            return object.invocation_ == invocations_.front();
        }

        /// Equality operator.
        /// \param object Object to compare.
        /// \return True if objects aren't equal and false otherwise.
        bool operator!=(const delegate<R(P...)>& object) const noexcept {
            return !(*this == object);
        }

        /// Adds delegate to the queue.
        /// \param object Multicast delegate to add.
        /// \return This object.
        multidelegate& operator+=(const multidelegate& object) {
            for (const auto& invocation : object.invocations_)
                invocations_.push_back(invocation);
            return *this;
        }

        /// Adds delegate to the queue.
        /// \param object Delegate to add.
        /// \return This object.
        multidelegate& operator+=(const delegate<R(P...)>& object) {
            if (object) invocations_.push_back(object.invocation_);
            return *this;
        }

        /// Removes delegate from the queue.
        /// \param object Multicast delegate to remove.
        /// \return This object.
        multidelegate& operator-=(const multidelegate& object) {
            for (const auto& invocation : object.invocations_) {
                auto iterator = std::find(invocations_.begin(),
                                          invocations_.end(), invocation);

                if (iterator != invocations_.end())
                    invocations_.erase(iterator);
            }
            return *this;
        }

        /// Removes delegate from the queue.
        /// \param object Delegate to remove.
        /// \return This object.
        multidelegate& operator-=(const delegate<R(P...)>& object) {
            auto iterator = std::find(invocations_.begin(),
                                      invocations_.end(), object.invocation_);

            if (iterator != invocations_.end())
                invocations_.erase(iterator);

            return *this;
        }

        /// Function call operator.
        /// The returned parameters are ignored.
        /// \param args Possible arguments of the functions.
        void operator()(P... args) const {
            for (const auto& invocation : invocations_)
                (*invocation.function())(invocation.object(), args...);
        }

        /// Function call operator.
        /// The returned parameters are handled by callback handler.
        /// \tparam H Handler type.
        /// \param args Possible arguments of the functions.
        /// \param handler Callback handler.
        template <typename H>
        void operator()(P... args, H handler) const {
            size_type index { 0 };
            for (const auto& invocation : invocations_) {
                R item = (*invocation.function())(invocation.object(), args...);
                handler(index++, &item);
            }
        }

        /// Indicates if invocations is empty.
        /// \return True if invocations is empty and false otherwise.
        bool empty() const noexcept {
            return invocations_.empty();
        }

        /// Returns the queue size.
        /// \return Queue size.
        size_type size() const noexcept {
            return invocations_.size();
        }

        /// Clears the queue.
        void clear() noexcept {
            invocations_.clear();
        }

    private:

        /// Storage of the invocations.
        container_type invocations_;
    };
}

#endif
