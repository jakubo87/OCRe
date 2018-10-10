#include <array>
#include <iostream>
#include <algorithm>
#include "gly_scan.hh"


//fwd decl
void print_2d_array(std::array<std::array<int,4>,4> out);


//testing gly_scan
int main() {

//unit-test
  std::array<std::array<int,4>,4> testm ={{
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
  }};

  //print array
  print_2d_array(testm);



  auto text = gly_scan(testm,4,4);

  std::array<std::array<int,4>,4> result ={{
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
  }};

  for (auto gly : text){
    for (auto pix : gly.data()){
      result[pix.first][pix.second]=1;
    }
  }
  print_2d_array(result);


  return 0;
}

void print_2d_array(std::array<std::array<int,4>,4> out){
 for (auto r : out){
    for(auto c : r)
      std::cout << c << " ";
    std::cout << "\n";
  }

}
