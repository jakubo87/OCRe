#include <iostream>
#include "recognition_test.hh"

int main (){

//general function testing
  std::cout<< "Test for my naive approach on recognition.\n";

  std::cout<< "similarity measure for input matrix containing a '7' is:\n";
  std::cout<< similarity_measure_x100(testmatrix7, testmatrix7)   <<"\n";

  std::cout<< "similarity measure for input matrix NOT containing a '7' but a '0' instead is:\n";
  std::cout<< similarity_measure_x100(testmatrix0, testmatrix7)   <<"\n";


//issue the command for true
std::cout<< "finding a 7 in a wider array\n";
iterate<20,10>(testmatrixIN_true,testmatrix7);

for (xy_char i: output)
  std::cout<< "x: "<<i.x<< " y: "<<i.y<< "char: "<<i.value<<"\n";

for (xy_char i: output)
  std::cout<<i.value;
std::cout<<"\n\n";



//issue the command for false
std::cout<< "finding a 7 in a wider array\n";
iterate<20,10>(testmatrixIN_false,testmatrix7);

for (xy_char i: output)
  std::cout<< "x: "<<i.x<< " y: "<<i.y<< "char: "<<i.value<<"\n";

for (xy_char i: output)
  std::cout<<i.value;
std::cout<<"\n";

}



