#include <string>
#include <iostream>
#include <filesystem>


int main()
{
    std::string path = "../Trainingimages";
    for (auto & p : std::filesystem::directory_iterator(path))
        std::cout << p << std::endl;
}
