int externe(int (*f)(int), int x)
{
    return f(x);
}

class s
{
public:
    inline static int a;

    static int f(int x)
    {
        return a * x;
    }

    int g()
    {
        return externe(s::f, 3);
    }
};

#include <iostream>

int main()
{
    struct s ss;
    ss.a = 2;
    std::cout << ss.g() << std::endl;
    std::cout<< ss.f(3) << std::endl;
    return 0;
}