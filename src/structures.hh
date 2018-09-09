#ifndef __STRUCTURES_H_INCLUDED__
#define __STRUCTURES_H_INCLUDED__

#include <vector>

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

struct mask{
matrix arr;
char value;
};



#endif