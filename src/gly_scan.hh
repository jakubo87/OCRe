#ifndef __GLY_SCAN_H_INCLUDED__
#define __GLY_SCAN_H_INCLUDED__

#include <unordered_set>
#include <vector>
#include <utility>
#include <algorithm>

using X = int;
using Y = int;


//variables
double T=0.2; //threshold for contrast, to be worked on later

//const
const std::vector<std::pair<Y,X>> dir_prox {
    std::make_pair(-1,-1),
    std::make_pair(-1,0),
    std::make_pair(-1,1),
    std::make_pair(0,-1),
    std::make_pair(0,0),
    std::make_pair(0,1),
    std::make_pair(1,-1),
    std::make_pair(1,0),
    std::make_pair(1,1)
    }; //direct proximity //TODO can lead to segvfault if last pixel hori. or ver. is black



//black pixels of a glyph ...for fast checks if in glyph or not
class glyph{
public:

glyph(Y y,X x)
:_top(y),_left(x)
{findall();} //y will remain, x can change

bool contains(X x,Y y){
  if (_data.find(std::make_pair(y,x))!=_data.end()) return true;
  return false;
}

Y top(){ return _top;}
X left(){ return _left;}
Y bottom(){return _bottom;}
X right(){return _right;}
//assuming that 0,0 is the left top corner of the image
int h_size(){return right()-left();}
int v_size(){return bottom()-top();}

const std::unordered_set<std::pair<Y, X>> & data(){return _data;}

private:
  Y _top; //line of the glyph (also top)
  X _left; //leftmost pixel (column)
  Y _bottom;
  X _right;
  //(_xl,_yt) does not need to be contained later on
  std::unordered_set<std::pair<Y, X>> _data;


  //only to be used in the beginning when _x,_y is the first pixel to be touched
  void findall(){
  _data.insert(std::make_pair(_top,_left));
  //auto ytbc= _data; //yet-to-be-checked
  std::all_of(_data.begin(),_data.end(),[&](std::pair<Y,X> p){
    for (auto i : dir_prox){
      int x=p.second+i.second;
      int y =p.first+i.first;
      //if (!_data.contains(x,y)) not needed
      _data.insert(std::make_pair(y,x));
    }
  });

  }
};


//the string of glyphs
using gly_string = std::vector<glyph>;


//algorithm
template<class AAA>
gly_string gly_scan(AAA input, int height,int width){
  gly_string text; //

  for (Y y=0;y<height; ++y){ // for every line
    for (X x=0;x<width;++x){ // for every pixel in that line
      if (input[y][x]>T) //<- view on pixels //TODO| if black then do proceed
      {
        bool cont=false; //contained
        for (int g=0;g<text.size();++g){ //for every glyph found
          cont=(text[g].contains(x,y));
          if (cont) break;
        }
        //find all pixels for the new glyph through neighbourhood
        if (!cont)
        text.push_back(glyph(y,x)); //create new glyph and add to vector
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