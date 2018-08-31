#include <catch.hpp>
#include <buffer.hpp>
#include <range_access.hpp>

TEST_CASE("Range access testing") {

    /**
     * Testing range access for containers.
     */
    SECTION("Range access for containers") {
        stdex::buffer<int> a {1, 2, 3, 4, 5};
        stdex::buffer<int> b;
        for (int* i = stdex::begin(a); i != stdex::end(a) ; ++i) b.append(*i);
        REQUIRE(a == b);
    }

    /**
     * Testing range access for raw arrays.
     */
    SECTION("Range access for raw arrays") {
        int a[] {1, 2, 3, 4, 5};
        int b[] {0, 0, 0, 0, 0};
        int j;
        int* i;
        for (j = 0, i = stdex::begin(a); i != stdex::end(a); ++i, ++j)
            b[j] = *i;
        REQUIRE(a[0] == b[0]);
    }
}