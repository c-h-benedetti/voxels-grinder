#include <iostream>
#include <stdexcept>

class A {
public:
    virtual ~A() {}

    A() = default;
};

class B : public A {
public:
    ~B() override {}

    B() = default;
};

class C : public A {
public:
    ~C() override {}

    C() = default;
};

class D : public A {
public:
    ~D() override {}

    D() = default;
};

template <typename I, typename V>
void threshold(I* img, V val) {

}

class Operator {
public:

    Operator(A* a);

    template <typename T>
    void run(T a);
};

template <>
void Operator::run<B*>(B* a) {
    B* b = a;
    std::cout << "This is a B." << std::endl;
    threshold<B, B>(b, *b);
}

template <>
void Operator::run<C*>(C* a) {
    C* b = a;
    std::cout << "This is a C." << std::endl;
    threshold<C, C>(b, *b);
}

Operator::Operator(A* a) {
    if (B* b = dynamic_cast<B*>(a)) {
        run<B*>(b);
        return;
    }
    if (C* b = dynamic_cast<C*>(a)) {
        run<C*>(b);
        return;
    }
    throw std::invalid_argument("Data type not handled by this operator.");
}

int main() {
    B* b = new B;
    C* c = new C;
    D* d = new D;

    Operator o1(b);
    Operator o2(c);
    Operator o3(d);

    delete b;
    delete c;
    delete d;

    return 0;
}