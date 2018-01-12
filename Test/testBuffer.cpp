#include <catch.hpp>
#include <buffer.h>

TEST_CASE("Buffer testing") {

    /**
     * Testing buffer construction.
     */
    SECTION("Constructors") {
        SECTION("Constructor without parameters") {
            stdex::buffer<int> a;
            REQUIRE(a.data() == nullptr);
            REQUIRE(a.empty());
            REQUIRE(a.capacity() == 0);
        }

        SECTION("Constructor with initializer list") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 5);
            REQUIRE(a.capacity() == 5);
            REQUIRE(a.at(4) == 5);
        }

        SECTION("Constructor with capacity") {
            stdex::buffer<int> a(10);
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.empty());
            REQUIRE(a.capacity() == 10);
        }

        SECTION("Constructor with data array") {
            int array[] = {1, 2, 3, 4, 5};
            stdex::buffer<int> a(array, sizeof(array) / sizeof(int));
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 5);
            REQUIRE(a.capacity() == 5);
            REQUIRE(a.at(0) == 1);
        }

        SECTION("Constructor with data array and capacity") {
            int array[] = {1, 2, 3, 4, 5};
            stdex::buffer<int> a(array, sizeof(array) / sizeof(int), 10);
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 5);
            REQUIRE(a.capacity() == 10);
            REQUIRE(a.at(0) == 1);
        }

        SECTION("Copy constructor") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b = a;
            REQUIRE(b.data() != nullptr);
            REQUIRE(b.size() == 5);
            REQUIRE(b.capacity() == 5);
            REQUIRE(b.at(1) == 2);
        }

        SECTION("Move constructor") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b = std::move(a);
            REQUIRE(b.data() != nullptr);
            REQUIRE(b.size() == 5);
            REQUIRE(b.capacity() == 5);
            REQUIRE(b.at(2) == 3);
            REQUIRE(a.data() == nullptr);
            REQUIRE(a.empty());
            REQUIRE(a.capacity() == 0);
        }
    }

    /**
     * Testing assignment operators.
     */
    SECTION("Assignment operators") {
        SECTION("Copy assignment operator") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b {6, 7, 8, 9, 10};
            b = a;
            REQUIRE(b.size() == 5);
            REQUIRE(b.capacity() == 5);
            REQUIRE(b.at(0) == 1);
            REQUIRE(b.at(4) == 5);
        }

        SECTION("Move assignment operator") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b {6, 7, 8, 9, 10};
            b = std::move(a);
            REQUIRE(b.size() == 5);
            REQUIRE(b.capacity() == 5);
            REQUIRE(b.at(0) == 1);
            REQUIRE(b.at(4) == 5);
            REQUIRE(a.data() == nullptr);
            REQUIRE(a.empty());
            REQUIRE(a.capacity() == 0);
        }

        SECTION("Assignment operator with initializer list") {
            stdex::buffer<int> a;
            a = {1, 2, 3, 4, 5};
            REQUIRE(a.size() == 5);
            REQUIRE(a.capacity() == 5);
            REQUIRE(a.at(0) == 1);
            REQUIRE(a.at(4) == 5);
        }
    }

    /**
     * Testing equality operators.
     */
    SECTION("Equality checking") {
        SECTION("Equality operator") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b {1, 2, 3, 4, 5};
            REQUIRE(a == b);
        }

        SECTION("Inequality operator") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b {6, 7, 8, 9, 10};
            REQUIRE(a != b);
        }
    }

    /**
     * Testing data assignment.
     */
    SECTION("Data assignment") {
        SECTION("Assign an initializer list") {
            stdex::buffer<int> a;
            a.assign({1, 2, 3, 4, 5});
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 5);
            REQUIRE(a.capacity() == 5);
            REQUIRE(a.at(4) == 5);
        }

        SECTION("Assign a capacity") {
            stdex::buffer<int> a;
            a.assign(10);
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.empty());
            REQUIRE(a.capacity() == 10);
        }

        SECTION("Assign a data array") {
            int array[] = {1, 2, 3, 4, 5};
            stdex::buffer<int> a;
            a.assign(array, sizeof(array) / sizeof(int));
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 5);
            REQUIRE(a.capacity() == 5);
            REQUIRE(a.at(0) == 1);
        }

        SECTION("Assign a data array and capacity") {
            int array[] = {1, 2, 3, 4, 5};
            stdex::buffer<int> a;
            a.assign(array, sizeof(array) / sizeof(int), 10);
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 5);
            REQUIRE(a.capacity() == 10);
            REQUIRE(a.at(0) == 1);
        }

        SECTION("Assign an existing object") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b;
            b.assign(a);
            REQUIRE(b.data() != nullptr);
            REQUIRE(b.size() == 5);
            REQUIRE(b.capacity() == 5);
            REQUIRE(b.at(1) == 2);
        }

        SECTION("Assign a temporary object") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b;
            b.assign(std::move(a));
            REQUIRE(b.data() != nullptr);
            REQUIRE(b.size() == 5);
            REQUIRE(b.capacity() == 5);
            REQUIRE(b.at(2) == 3);
            REQUIRE(a.data() == nullptr);
            REQUIRE(a.empty());
            REQUIRE(a.capacity() == 0);
        }

        SECTION("Erase old data") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b {6, 7, 8, 9, 10};
            b.assign(a);
            REQUIRE(b.data() != nullptr);
            REQUIRE(b.size() == 5);
            REQUIRE(b.capacity() == 5);
            REQUIRE(b.at(0) == 1);
            REQUIRE(b[4] == 5);
        }
    }

    /**
     * Testing data appending.
     */
    SECTION("Data appending") {
        SECTION("Append an existing object") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b {6, 7, 8, 9, 10};
            a.append(b);
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 10);
            REQUIRE(a.capacity() == 10);
            REQUIRE(a.at(0) == 1);
            REQUIRE(a[9] == 10);
        }

        SECTION("Append a temporary object") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b {6, 7, 8, 9, 10};
            a.append(std::move(b));
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 10);
            REQUIRE(a.capacity() == 10);
            REQUIRE(a.at(0) == 1);
            REQUIRE(a[9] == 10);
            REQUIRE(b.data() == nullptr);
            REQUIRE(b.empty());
            REQUIRE(b.capacity() == 0);
        }

        SECTION("Append an initializer list") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            a.append({6, 7, 8, 9, 10});
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 10);
            REQUIRE(a.capacity() == 10);
            REQUIRE(a.at(9) == 10);
        }

        SECTION("Append a single value") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            a.append(6);
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 6);
            REQUIRE(a.capacity() == 6);
            REQUIRE(a.at(5) == 6);
        }

        SECTION("Append a data array") {
            int array[] = {6, 7, 8, 9, 10};
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            a.append(array, sizeof(array) / sizeof(int));
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 10);
            REQUIRE(a.capacity() == 10);
            REQUIRE(a.at(9) == 10);
        }

        SECTION("Append a data with reserved capacity") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            a.assign(10);
            a.append({6, 7, 8});
            REQUIRE(a.data() != nullptr);
            REQUIRE(a.size() == 8);
            REQUIRE(a.capacity() == 10);
            REQUIRE(a.at(7) == 8);
            REQUIRE(a[9] == 0);
        }
    }

    /**
     * Testing range access.
     */
    SECTION("Range access") {
        SECTION("Getting the first element") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            REQUIRE(a.first() == 1);
            REQUIRE((a.first() = 10) == 10);
        }

        SECTION("Getting the last element") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            REQUIRE(a.last() == 5);
            REQUIRE((a.last() = 10) == 10);
        }

        SECTION("Iterator loop") {
            stdex::buffer<int> a {1, 2, 3, 4, 5};
            stdex::buffer<int> b;
            for (int element : a) b.append(element);
            REQUIRE(a == b);
        }
    }

    /**
     * Testing data clearing.
     */
    SECTION("Data clearing") {
        stdex::buffer<int> a {1, 2, 3, 4, 5};
        a.clear();
        REQUIRE(a.data() == nullptr);
        REQUIRE(a.empty());
        REQUIRE(a.capacity() == 0);
    }

    /**
     * Testing object swapping.
     */
    SECTION("Object swapping") {
        stdex::buffer<int> a {1, 2, 3, 4, 5};
        stdex::buffer<int> b {6, 7, 8, 9, 10};
        stdex::swap(a, b);
        REQUIRE(a.at(0) == 6);
        REQUIRE(a.at(4) == 10);
        REQUIRE(a.size() == 5);
        REQUIRE(a.capacity() == 5);
        REQUIRE(b.at(0) == 1);
        REQUIRE(b.at(4) == 5);
        REQUIRE(b.size() == 5);
        REQUIRE(b.capacity() == 5);
    }

    /**
     * Testing data releasing.
     */
    SECTION("Data releasing") {
        stdex::buffer<int> a {1, 2, 3, 4, 5};
        int* data = a.release();
        delete[] data;
        REQUIRE(a.data() == nullptr);
        REQUIRE(a.empty());
        REQUIRE(a.capacity() == 0);
    }
}