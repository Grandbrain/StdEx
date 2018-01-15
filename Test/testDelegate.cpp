#include <catch.hpp>
#include <delegate.h>

/**
 * Simple function for testing purposes.
 */
int data() {
    return 3;
}

TEST_CASE("Delegate testing") {

    /**
     * Simple class for testing purposes.
     */
    class A {
    public:
        A() {data1_ = data2_ = 0;}
        A(int data) {data1_ = data2_ = data;}
        A(int data1, int data2) {data1_ = data1, data2_ = data2;}
        int data1() {return data1_;}
        int data2() const {return data2_;}
        int data3(int data) {return data;}
    private:
        int data1_;
        int data2_;
    };

    /**
     * Testing delegates with member functions.
     */
    SECTION("Delegates with member functions") {
        A object(1, 2);
        auto a = stdex::delegate<int()>::create<A, &A::data1>(object);
        auto b = stdex::delegate<int()>::create<A, &A::data2>(object);
        REQUIRE(a() == 1);
        REQUIRE(b() == 2);
    }

    /**
     * Testing delegates with member functions.
     */
    SECTION("Delegates with free functions") {
        auto c = stdex::delegate<int()>::create<&data>();
        REQUIRE(c() == 3);
    }
}