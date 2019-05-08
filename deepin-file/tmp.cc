#include <iostream>
#include <vector>

int
main(void)
{
    std::vector<char> p;
    std::cout << p.size() << ' ' << p.capacity() << std::endl;
    p.push_back('x');
    p.push_back('x');
    p.push_back('x');
    p.push_back('x');
    std::cout << p.size() << ' ' << p.capacity() << std::endl;
    p.push_back('x');
    std::cout << p.size() << ' ' << p.capacity() << std::endl;
}
