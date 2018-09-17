#ifndef __GLY_SCAN_H_INCLUDED__
#define __GLY_SCAN_H_INCLUDED__

#include <unordered_set>
#include <vector>
#include <utility>
#include <algorithm>
#include "structures.hh"

bool operator==(const point & lhs, const point & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

//Hash function for combined types
struct MyHash
{
  std::size_t operator()(const point & p) const noexcept
  {
    std::size_t h1 = std::hash<int>{}(p.x);
    std::size_t h2 = std::hash<int>{}(p.y);
    return h1 ^ (h2 << 1); // or use boost::hash_combine (see Discussion)
  }
};



//variables
double T=0.2; //threshold for contrast, to be worked on later

//const
const std::vector<point> dir_prox {
    point{-1,-1},
    point{-1,0},
    point{-1,1},
    point{0,-1},
    point{0,0},
    point{0,1},
    point{1,-1},
    point{1,0},
    point{1,1}
}; //direct proximity //TODO can lead to segvfault if last pixel hori. or ver. is black



//black pixels of a glyph ...for fast checks if in glyph or not
class glyph{
public:

  glyph(Y y,X x, matrix input)
  :_top(y),_left(x)
  {findall(input);} //y will remain, x can change

  bool contains(X x,Y y){
    if (_data.find(point{y,x})!=_data.end()) return true;
    return false;
  }

  Y top(){ return _top;}
  X left(){ return _left;}
  Y bottom(){return _bottom;}
  X right(){return _right;}
  //assuming that 0,0 is the left top corner of the image
  int h_size(){return right()-left();}
  int v_size(){return bottom()-top();}

  const auto & data(){return _data;}

  private:
    Y _top; //line of the glyph (also top)
    X _left; //leftmost pixel (column)
    Y _bottom;
    X _right;
    //(_xl,_yt) does not need to be contained later on
    std::unordered_set<point,MyHash> _data;


  //only to be used in the beginning when _x,_y is the first pixel to be touched
  void findall(matrix input){
    _data.insert(point{_top,_left});
    std::vector<point> queue; //yet-to-be-checked
    for (auto p :queue){
      for (auto i : dir_prox){ //check all the neighbours
        int x = p.x+i.x;
        int y = p.y+i.y;
        if (!contains(x,y)) //only add pixels, that have not been visited before
          if(T*255>input[y][x]){
            //new black pixel in queue and in glyph
            queue.push_back(point{y,x});
            _data.insert(point{y,x});
          }
      }
    };
  }
};


//the string of glyphs
using gly_string = std::vector<glyph>;


//algorithm
gly_string gly_scan(const matrix & input){
  int height=input.size();
  int width=input[0].size();
  gly_string text; //

  for (Y y=0;y<height; ++y){ // for every line
    for (X x=0;x<width;++x){ // for every pixel in that line
      if (input[y][x]<T*255)
      {
        bool cont=false; //contained
        for (int g=0;g<text.size();++g){ //for every glyph found
          cont=(text[g].contains(x,y));
          if (cont) break;
        }
        //find all pixels for the new glyph through neighbourhood
        if (!cont)
        text.push_back(glyph(y,x, input)); //create new glyph and add to vector
      }
    }
  }


  return text;
}


//comments
/* to avoid checks if glyphs need to be unified i first scan the whole glyph via density connection
possibly object to change

idea
scan through all the black pixels and label them. if they happen to connect use the smaller label on both and erase the larger label

*/



#endif