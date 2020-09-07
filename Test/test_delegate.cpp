#include <catch.hpp>
#include <delegate.hpp>

// Simple function for testing purposes.
int data(int value) {
    return value;
}

// Simple class for testing purposes.
class A {
public:
    explicit A() { data1_ = data2_ = 0; }
    explicit A(int data) { data1_ = data2_ = data; }
    explicit A(int data1, int data2) { data1_ = data1, data2_ = data2; }
    int data1() { return data1_; }
    int data2() const { return data2_; }
    int data3(int data) { return data + data1_ + data2_; }

private:
    int data1_;
    int data2_;
};

// Testing of delegate.
TEST_CASE("Testing of delegate") {

    // Testing of delegates with member functions.
    SECTION("Delegates with member functions") {
        A object(1, 2);
        auto a = stdex::delegate<int()>::create<A, &A::data1>(object);
        auto b = stdex::delegate<int()>::create<A, &A::data2>(object);
        REQUIRE(a() == 1);
        REQUIRE(b() == 2);
    }

    // Testing of delegates with free functions.
    SECTION("Delegates with free functions") {
        auto c = stdex::delegate<int(int)>::create<&data>();
        REQUIRE(c(3) == 3);
    }

    // Testing of delegates with lambda functions.
    SECTION("Delegates with lambda functions") {
        stdex::delegate<int()> c = []() -> int { return 4; };
        REQUIRE(c() == 4);
    }

    // Testing of equality operators.
    SECTION("Equality operators") {
        A object;
        auto a = stdex::delegate<int()>::create<A, &A::data1>(&object);
        auto b = stdex::delegate<int()>::create<A, &A::data1>(&object);
        REQUIRE(a == b);
    }

    // Testing of multicast delegates.
    SECTION("Multicast delegates") {
        A object(1, 2);
        auto a = stdex::delegate<int(int)>::create<&data>();
        auto b = stdex::delegate<int(int)>::create<A, &A::data3>(object);
        stdex::multidelegate<int(int)> ab;
        ab += a;
        ab += b;
        ab(2);
        REQUIRE(ab.size() == 2);
    }
}
