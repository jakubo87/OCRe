#include <string>
#include <iostream>
#include <filesystem>


int main()
{
    std::string path = "../Testimages";
    for (auto & p : std::filesystem::directory_iterator(path))
        std::cout << p << std::endl;
}
