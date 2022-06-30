#include <iostream>

template<typename A, typename B>
struct Pair1;

template<typename A, typename B>
struct Pair2 {
    A x;
    B y;

    constexpr auto swap() const {
        auto r = new Pair1<A, B>;
        r.x = x;
        r.y = y;
        // in Tyr, we would get an error here; in C++, this is fine
        r.z = 3;
        return r;
    }
};

template<typename A, typename B>
struct Pair1 {
    A x;
    B y;

    constexpr auto swap() const {
        auto r = new Pair2<B, A>;
        r.x = x;
        r.y = y;
        // in Tyr, we would get an error here; in C++, this is fine
        r.z = 3;
        return r;
    }
};

int main() {
    std::cout << "Hi!" << std::endl;

    // the program will work with this line and it will create a somewhat useful
    // representation of Pair1 ...
    auto p = new Pair1<int, float>();
    // ... However, if swap is actually used, the compailer will complain on
    // instantiation of swap that it is unable to access z.
    // p->swap();

    return 0;
}
