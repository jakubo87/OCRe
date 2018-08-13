#include <iostream>
#include "recognition_test.hh"
#include "writetofile.hh"

int main (){

//prepare chars //vector of patterns to be compared
chars.push_back(char0);
chars.push_back(char7);


//general function testing
  std::cout<< "Test for my naive approach on recognition.\n";

  std::cout<< "similarity measure for input matrix containing a '7' is:\n";
  std::cout<< similarity_measure_x100<13,9>(char_7, char_7)   <<"\n";

  std::cout<< "similarity measure for input matrix NOT containing a '7' but a '0' instead is:\n";
  std::cout<< similarity_measure_x100(char_0, char_7)   <<"\n";


//issue the command
std::cout<< "finding a 7 in a wider array\n";
iterate(test_IN,char_7);
for (xy_char i: output)
  std::cout<< "x: "<<i.x<< " y: "<<i.y<< " char: "<<i.value<<"\n";
for (xy_char i: output)
  std::cout<<i.value;
std::cout<<"\n\n";


writetofile(output);


output.clear();


}



