#ifndef __STRUCTURES_HH_INCLUDED__
#define __STRUCTURES_HH_INCLUDED__

#include <vector>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>
#include <iostream>
#include <utility>


//variables
const int WHITE =255;
const int BLACK=0;
const double T=0.6; //threshold for contrast, to be worked on later
//the larger T the more outliers may be included
//the smaller T the more holes in glyphs can occurr

using X = int;
using Y = int;

struct point{
  X x;
  Y y;
};


X MaskW=19;
Y MaskH=23;


//"Matrix" is a preliminary solution to later generalize the signatures
//will probably go from [][] to .at(x,y) and image views
class matrix : public std::vector<std::vector<int>>{
public:
 //ctors

  matrix()=default;
 //empty matrix
 matrix(Y height, X width){
  for (int i=0;i<height;++i)
    push_back({std::vector<int>(width, WHITE)});
  }

  //Sequence of points to matrix - of unknown size -> calculated
  template<class S>
  matrix(S && s) {
    const X lt=left(s);
    const Y tp=top(s);
    const Y height = bottom(s)-top(s)+1;//is the last VALID pixel -> +1
    const X width = right(s)-left(s)+1;

    for (int i=0;i<height;++i)
      push_back({std::vector<int>(width, WHITE)});

    std::for_each(std::begin(s),std::end(s),[&](auto & i){
      (*this)[i.y-tp][i.x-lt]=0;
    });
  }

  //resize
  template <class M>
  matrix(M && input, X tar_w, Y tar_h){

    for (int i=0;i<tar_h;++i)
      push_back({std::vector<int>(tar_w, WHITE)});

    const Y cur_h=input.size();
    const X cur_w=std::begin(input)->size();

  //std::ratio only for compile time known stuff...
    double Dx = static_cast<double>(tar_w)/cur_w;
    double Dy = static_cast<double>(tar_h)/cur_h;

    for (int i=0;i<cur_h;++i){
      for (int j=0; j<cur_w;++j){
        //fills up corresponding areas
        for (int dx=0;dx<Dx;++dx)
          for (int dy=0;dy<Dy;++dy)
            (*this)[i*Dy+dy][j*Dx+dx]=input[i][j];
      }
    }
  }
}; //end matrix class

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