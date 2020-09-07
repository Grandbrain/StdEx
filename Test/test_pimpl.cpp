#include <catch.hpp>
#include <pimpl.hpp>

// Simple structure for testing purposes.
struct A {
public:
    explicit A() { data1_ = data2_ = 0; }
    explicit A(int data) { data1_ = data2_ = data; }
    explicit A(int data1, int data2) { data1_ = data1, data2_ = data2; }
    int data1() const { return data1_; }
    int data2() const { return data2_; }
private:
    int data1_;
    int data2_;
};

// Testing of pimpl smart pointer.
TEST_CASE("Testing of pimpl smart pointer") {

    // Testing of constructors.
    SECTION("Constructors") {

        SECTION("Constructor with variable number of parameters") {
            stdex::pimpl_ptr<A> a;
            stdex::pimpl_ptr<A, 16> b(1, 2);
            REQUIRE(a->data1() == 0);
            REQUIRE(a->data2() == 0);
            REQUIRE(b->data1() == 1);
            REQUIRE(b->data2() == 2);
        }

        SECTION("Copy constructor") {
            stdex::pimpl_ptr<A> a(1, 2);
            stdex::pimpl_ptr<A, 128> b(a);
            REQUIRE(a->data1() == 1);
            REQUIRE(a->data2() == 2);
            REQUIRE(b->data1() == 1);
            REQUIRE(b->data2() == 2);
        }

        SECTION("Move constructor") {
            stdex::pimpl_ptr<A> a(1, 2);
            stdex::pimpl_ptr<A, 128> b(std::move(a));
            REQUIRE(b->data1() == 1);
            REQUIRE(b->data2() == 2);
        }
    }

    // Testing of assignment operators.
    SECTION("Assignment operators") {

        SECTION("Copy assignment operator") {
            stdex::pimpl_ptr<A> a(1, 2);
            stdex::pimpl_ptr<A> b;
            b = a;
            REQUIRE(b->data1() == 1);
            REQUIRE(b->data2() == 2);
        }

        SECTION("Move assignment operator") {
            stdex::pimpl_ptr<A> a(1, 2);
            stdex::pimpl_ptr<A> b;
            b = std::move(a);
            REQUIRE(b->data1() == 1);
            REQUIRE(b->data2() == 2);
        }
    }

    // Testing of dereference operator.
    SECTION("Dereference operator") {
        stdex::pimpl_ptr<A> a(1, 2);
        A structure = *a;
        REQUIRE(structure.data1() == 1);
        REQUIRE(structure.data2() == 2);
    }

    // Testing of various operations.
    SECTION("Various operations") {

        SECTION("Swap operation") {
            stdex::pimpl_ptr<A> a(1, 2);
            stdex::pimpl_ptr<A> b(3);
            std::swap(a, b);
            REQUIRE(b->data1() == 1);
            REQUIRE(b->data2() == 2);
            REQUIRE(a->data1() == 3);
            REQUIRE(a->data2() == 3);
        }
    }
}
