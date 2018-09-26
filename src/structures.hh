#ifndef __STRUCTURES_H_INCLUDED__
#define __STRUCTURES_H_INCLUDED__

#include <vector>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>

int MaskW=20;
int MaskH=30;

using X = int;
using Y = int;


// because array wants a size at compile time

//"Matrix" is a preliminary solution to later generalize the signatures
//will probably go from [][] to .at(x,y)
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
//works for downsizing only
  Y cur_h=input.size();
  X cur_w=input[0].size();
//ratio only for compile time known stuff...
  double Dx = static_cast<double>(tar_w)/cur_w;
  double Dy = static_cast<double>(tar_h)/cur_h;
//  std::cout <<"cur_w:"<<cur_w<<" cur_h:"<<cur_h<<"\n";
//  std::cout <<"tar_w:"<<tar_w<<" tar_h:"<<tar_h<<"\n";
//  std::cout <<"Dx:"<<Dx<<" Dy:"<<Dy<<"\n";
  //initialize resuling matrix
  matrix res;
  //std::cout << "got a problem?\n";
  for (int i=0;i<tar_h;++i)
    res.push_back(std::vector<int> (tar_w));
  //res.reserve(tar_h);

  //std::transform..?
  for (int i=0;i<cur_h;++i){
    for (int j=0; j<cur_w;++j){
      //add value to corresponding element in resized matrix
      //std::cout << "adding " <<input[i][j]<<"\n";
      res[i*Dy][j*Dx]+=input[i][j];
      //std::cout << "entry: " <<j*Dx <<" "<<i*Dy <<" is now " <<res[i*Dy][j*Dx]<<"\n";
    }
  }
  //average over elements
  for (auto & v : res)
    for (auto & e : v)
    // smaller target has to get lower values<-> higher accumulation rate
      e=std::round(e*Dx*Dy); //Jacobi
  return res;
}

#endif