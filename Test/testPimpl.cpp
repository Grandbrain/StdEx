#include <catch.hpp>
#include <pimpl.hpp>

TEST_CASE("Pimpl testing") {

    /**
     * Simple structure for testing purposes.
     */
    struct A {
    public:
        A() {data1_ = data2_ = 0;}
        A(int data) {data1_ = data2_ = data;}
        A(int data1, int data2) {data1_ = data1, data2_ = data2;}
        int data1() {return data1_;}
        int data2() {return data2_;}
    private:
        int data1_;
        int data2_;
    };

    /**
     * Testing of constructors.
     */
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

    /**
     * Testing assignment operators.
     */
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

    /**
     * Testing dereference operator.
     */
    SECTION("Dereference operator") {
        stdex::pimpl_ptr<A> a(1, 2);
        A structure = *a;
        REQUIRE(structure.data1() == 1);
        REQUIRE(structure.data2() == 2);
    }
}