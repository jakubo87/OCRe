#ifndef __STRUCTURES_H_INCLUDED__
#define __STRUCTURES_H_INCLUDED__

#include <vector>
#include <algorithm>
#include <vector>
#include <cmath>



using X = int;
using Y = int;


// because array wants a size at compile time
using matrix= std::vector<std::vector<int>>;

struct point{
  X x;
  Y y;
};

struct xy_char{
  int x; //maybe better to have usign long..?
  int y;
  char value;
  //opt padding or reduce x...?
};


//global
std::vector<matrix> masks;


decltype(auto) resize_matrix(const matrix & input, X tar_w, Y tar_h){
  Y cur_h=input.size();
  X cur_w=input[0].size();

//ratio only for compile time known stuff...

  double Dx=tar_w/cur_w;
  double Dy =tar_h/cur_h;


  //initialize resuling matrix
  matrix res;
  res[0].reserve(tar_w);
  res.reserve(tar_h);
  //std::transform..?
  for (int i=0;i<cur_h;++i){
    for (int j=0; j<cur_w;++j){
      //add value to corresponding element in resized matrix
      res[i/Dy][j/Dx]+=input[i][j];
    }
  }
  //average over elements
  for (auto & v:res)
    for (auto & e :v)
    // smaller target has to get lower values<-> higher accumulation rate
      e=std::round(e/Dx/Dy);
  return res;
}

#endif