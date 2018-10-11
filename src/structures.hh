#ifndef __STRUCTURES_H_INCLUDED__
#define __STRUCTURES_H_INCLUDED__

#include <vector>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>
#include <iostream>
#include <utility>
#include <climits>
#include "images.hh"


//variables
const int WHITE =255;
const int BLACK=0;
const double T=0.6; //threshold for contrast, to be worked on later

using X = int;
using Y = int;

struct point{
  X x;
  Y y;
};


X MaskW=19;
Y MaskH=23;


//"Matrix" is a preliminary solution to later generalize the signatures
//will probably go from [][] to .at(x,y)
using matrix = std::vector<std::vector<int>>;
//ideas
//template <class T> using matrix = std::vector<std::vector<T>>;

//forward declarations and external functions
template<class M> void to_image(M && input);


//template<class T>
decltype(auto) init_Matrix(Y height, X width){
  matrix m;
  for (int i=0;i<height;++i)
    m.push_back({std::vector<int>(width, WHITE)});
  //return std::move(m);
  return m;
}

template<class S> //initializes a new matrix with size from a Set
decltype(auto) init_Matrix(S && s) {
  int bt=bottom(s);//is the last VALID pixel -> +1
  int tp=top(s);
  int rt=right(s);
  int lt=left(s);
  return init_Matrix(bt-tp+1,rt-lt+1);
}


template<class M>
//decltype(auto) resize_matrix_(M && input, M && res){
decltype(auto) resize_matrix(M && input, X tar_w, Y tar_h){

  auto res=init_Matrix(tar_h,tar_w);

  Y cur_h=input.size();
  X cur_w=std::begin(input)->size();
 // Y tar_h=res.size();
 // X tar_w=std::begin(res)->size();

//std::ratio only for compile time known stuff...
  double Dx = static_cast<double>(tar_w)/cur_w;
  double Dy = static_cast<double>(tar_h)/cur_h;

  for (int i=0;i<cur_h;++i){
    for (int j=0; j<cur_w;++j){
      //fills up corresponding areas
      for (int dx=0;dx<Dx;++dx)
        for (int dy=0;dy<Dy;++dy)
          res[i*Dy+dy][j*Dx+dx]=input[i][j];
    }
  }

//for testing
  to_image(res);
  //return std::move(res);
  return res;
}


// for the sets S (which are not really sets but sequences/lists.. of points
// the coordinates xy are ordered left to right and top to bottom
// i.e. down -> larger y
template<class S> //turns a set of points to a matrix
decltype(auto) to_matrix(S && s) {
  int tp=top(s);
  int lt=left(s);

  auto m=init_Matrix(s);

  std::for_each(std::begin(s),std::end(s),[&](auto & i){
    m[i.y-tp][i.x-lt]=0;
  });
  //for testing
    //to_image(m);
    //return std::move(m);
  return m;
}



template<class S>
int left(S && s){
  int min=std::begin(s)->x;
    std::for_each(s.begin(),s.end(),
      [&](auto & p){if (p.x<min) min = p.x;}
    );
  return min;
}

template<class S>
int right(S && s){
  int max=std::begin(s)->x;
    std::for_each(s.begin(),s.end(),
      [&](auto & p){if (p.x>max) max = p.x;}
    );
  return max;
}

template<class S>
int top(S && s){
  int min=std::begin(s)->x;
    std::for_each(s.begin(),s.end(),
      [&](auto & p){if (p.y<min) min = p.y;}
    );
  return min;
}

template<class S>
int bottom(S && s){
  int max=std::begin(s)->x;
    std::for_each(s.begin(),s.end(),
      [&](auto & p){if (p.y>max) max = p.y;}
    );
  return max;
}



#endif