#include <catch.hpp>
#include <string_utils.hpp>
#include <iostream>

// Testing of string utilites.
TEST_CASE("Testing of string utilites") {

    // Testing of trimming.
    SECTION("Trimming") {

        // String.
        std::string str("Hello, world!");

        // Wide string.
        std::wstring wstr(L"Привет, мир!");

        // Whitespace characters.
        std::string space(" \t\n\v\f\r");

        // Whitespace wide characters.
        std::wstring wspace(L" \t\n\v\f\r");

        // Testing of left trimming.
        SECTION("Left trimming") {
            std::string a(space + str);
            std::wstring b(wspace + wstr);
            REQUIRE(stdex::ltrim_copy(a) == str);
            REQUIRE(stdex::ltrim_copy(b) == wstr);
            stdex::ltrim(a);
            stdex::ltrim(b);
            REQUIRE(a == str);
            REQUIRE(b == wstr);
        }

        // Testing of right trimming.
        SECTION("Right trimming") {
            std::string a(str + space);
            std::wstring b(wstr + wspace);
            REQUIRE(stdex::rtrim_copy(a) == str);
            REQUIRE(stdex::rtrim_copy(b) == wstr);
            stdex::rtrim(a);
            stdex::rtrim(b);
            REQUIRE(a == str);
            REQUIRE(b == wstr);
        }

        // Testing of full trimming.
        SECTION("Full trimming") {
            std::string a(space + str + space);
            std::wstring b(wspace + wstr + wspace);
            REQUIRE(stdex::trim_copy(a) == str);
            REQUIRE(stdex::trim_copy(b) == wstr);
            stdex::trim(a);
            stdex::trim(b);
            REQUIRE(a == str);
            REQUIRE(b == wstr);
        }
    }

    // Testing of splitting.
    SECTION("Splitting") {
        std::string str("a b\nc");
        std::initializer_list<std::string::value_type> delims {' ', '\n'};
        std::vector<std::string> lines;
        stdex::split(str, delims, lines);
        REQUIRE(lines.size() == 3);
        REQUIRE(lines[0] == "a");
        REQUIRE(lines[1] == "b");
        REQUIRE(lines[2] == "c");
        lines = stdex::split_copy(str, delims);
        REQUIRE(lines.size() == 3);
        REQUIRE(lines[0] == "a");
        REQUIRE(lines[1] == "b");
        REQUIRE(lines[2] == "c");
    }
}
