/// \file string_utils.hpp
/// \brief Classes and functions that extend STL string class.
/// \bug No known bugs.

#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <vector>
#include <algorithm>

/// Contains classes and functions that extend the C++ STL.
namespace stdex {

    /// A helper structure that checks whether the type is a string type.
    /// \tparam T Arbitrary type.
    template <typename T>
    struct is_string {

        /// The value indicates whether the type is a string type.
        static constexpr bool value =
            std::is_same<T, std::string>::value ||
            std::is_same<T, std::wstring>::value;

        /// The type involved in SFINAE checks.
        using type = typename std::enable_if<value, T>::type;
    };

    /// Trims a string in left.
    /// \tparam T String type.
    /// \param[in,out] str String to trim.
    template <typename T, typename = typename stdex::is_string<T>::type>
    inline void ltrim(T& str) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    /// Trims a string in right.
    /// \tparam T String type.
    /// \param[in,out] str String to trim.
    template <typename T, typename = typename stdex::is_string<T>::type>
    inline void rtrim(T& str) {
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), str.end());
    }

    /// Trims a string.
    /// \tparam T String type.
    /// \param[in,out] str String to trim.
    template <typename T, typename = typename stdex::is_string<T>::type>
    inline void trim(T& str) {
        ltrim(str);
        rtrim(str);
    }

    /// Trims a string in left.
    /// \tparam T String type.
    /// \param str String to trim.
    /// \return Trimmed string.
    template <typename T, typename = typename stdex::is_string<T>::type>
    inline T ltrim_copy(T str) {
        ltrim(str);
        return str;
    }

    /// Trims a string in right.
    /// \tparam T String type.
    /// \param str String to trim.
    /// \return Trimmed string.
    template <typename T, typename = typename stdex::is_string<T>::type>
    inline T rtrim_copy(T str) {
        rtrim(str);
        return str;
    }

    /// Trims a string.
    /// \tparam T String type.
    /// \param str String to trim.
    /// \return Trimmed string.
    template <typename T, typename = typename stdex::is_string<T>::type>
    inline T trim_copy(T str) {
        trim(str);
        return str;
    }

    /// Splits a string.
    /// \tparam T String type.
    /// \tparam C Character type.
    /// \param str String to split.
    /// \param delims Delimiters.
    /// \param[out] lines Output list of strings.
    template <typename T, typename C = typename T::value_type,
        typename = typename stdex::is_string<T>::type>
    inline void split(const T& str,
                      const std::initializer_list<C>& delims,
                      std::vector<T>& lines) {

        auto iter = str.begin();

        while (iter != str.end()) {

            auto pos = std::find_if(iter, str.end(), [delims](C ch) {
                return std::any_of(delims.begin(), delims.end(), [ch](C d) {
                    return d == ch;
                });
            });

            if (pos != iter) {
                T line(iter, pos);
                trim(line);
                if (!line.empty()) lines.emplace_back(line);
            }

            iter = pos == str.end() ? pos : std::next(pos);
        }
    }

    /// Splits a string.
    /// \tparam T String type.
    /// \tparam C Character type.
    /// \param str String to split.
    /// \param delims Delimiters.
    /// \return Output list of strings.
    template <typename T, typename C = typename T::value_type,
        typename = typename stdex::is_string<T>::type>
    inline std::vector<T> split_copy(const T& str,
                                     const std::initializer_list<C>& delims) {

        std::vector<T> lines;
        split(str, delims, lines);
        return lines;
    }
};

#endif
