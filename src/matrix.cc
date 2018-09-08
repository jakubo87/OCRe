#include "structures.hh"
#include <iostream>
#include <algorithm>
int main (){

//befragen einer Matrix fester Größe
  const matrix bla ={{1,2},{3,4}};
  for (auto a : bla)
    for (auto b : a)
      std::cout<<b<<"\n";
  std::cout<<"\n";

  std::cout << "or..\n";

  std::for_each(bla.begin(),bla.end(),[&](auto i){
    std::for_each(i.begin(),i.end(),[&](auto j){
      std::cout << j <<"\n";
    });
  });

  std::cout << "or..\n";

  for (int i=0;i<bla.size();++i)
    for (int j=0;j<bla[i].size();++j)
      std::cout << bla[i][j] << "\n";

  return 0;
}
