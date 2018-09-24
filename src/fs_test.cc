#include <string>
#include <iostream>
#include <boost/filesystem.hpp>


int main()
{
    std::string path = "../Trainingimages";
    for (auto & p : boost::filesystem::directory_iterator(path))
        std::cout << p << std::endl;
}
