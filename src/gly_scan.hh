#ifndef __GLY_SCAN_H_INCLUDED__
#define __GLY_SCAN_H_INCLUDED__

#include <unordered_set>
#include <vector>
#include <utility>
#include <algorithm>
#include "structures.hh"
#include "jpegimportGIL.hh"
#include "recognition.hh"

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
double T=0.5; //threshold for contrast, to be worked on later

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
  //ctor
  glyph(Y y,X x, matrix input)
  :_top(y),_left(x), _bottom(y), _right(x) //init variables
  {findall(input);} //top will remain, bottom, left and right can change

  bool contains(point p){
    //if (_data.find(point{x,y})!=_data.end()) return true;
    return (std::find(_data.begin(), _data.end(), p)!=_data.end());
  }


  matrix to_matrix(){
  //initialize the matrix containing only the glyph
    matrix m;
    for (int i=0;i<_bottom-_top+1;++i){
      m.push_back(std::vector<int> (_right-_left+1));
      std::fill(m[i].begin(),m[i].end(),255);
    }
    std::for_each(_data.begin(),_data.end(),[&](auto i){
    //std::cout <<"i.y=" << i.y <<"top="<< _top << "\n";
    //std::cout <<"i.x=" << i.x <<"left="<< _left << "\n";
      m[i.y-_top][i.x-_left]=0;
    });
  //for testing
    //matrix_to_image(m);
    return m;
  }

//TODO
  void fuse(glyph other){ //in case of i and ä and the likes
    _data.insert(_data.end(),other._data.begin(), other._data.end());
  }

//TODO
  std::pair<char,int> recognize(const trans_tab & trans){ //std::string because if nothing fits the string can be empty
    char best=' ';
    int init_score=-100;
    double score=init_score;
    auto comp= resize_matrix(this->to_matrix(),MaskW,MaskH);
    //matrix_to_image(comp); //for debugging and demontration purposes
    for (int i=0;i<trans.first.size();++i){
      auto curr= similarity(comp,trans.first[i]);
      if (curr>score){
        score =curr; //max
        best=trans.second[i];
      }
      if (score==0) break;
    }
    return std::make_pair(best,score);
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
    //std::unordered_set<point,MyHash> _data;
    std::vector<point> _data;


  //only to be used in the beginning when _x,_y is the first pixel to be touched
  void findall(matrix input){
    //_data.insert(point{_left,_top});
    _data.push_back(point{_left,_top});
    std::vector<point> queue;
    //initial value to get it started
    queue.push_back(point{_left, _top}); //yet-to-be-checked, actually not a queue, but a lifo
    for (int p=0;p<queue.size();++p){
      for (auto i : dir_prox){ //check all the neighbours
        int x = queue[p].x+i.x;
        int y = queue[p].y+i.y;
        if (!contains(point{x,y})) //only add pixels, that have not been visited before
          if(T*255>input[y][x]){
            //new black pixel in queue and in glyph
            queue.push_back(point{x,y});
            //_data.insert(point{x,y}); //wont go into the queue a second time
            _data.push_back(point{x,y}); //wont go into the queue a second time
            //writing edge rows and lines (bottom, top, ..)
            if (x<_left) _left=x;
            if(x>_right) {
              _right=x;
             // std::cout << "right value changed to " << _right << "\n";
            }
            if (y>_bottom){
              _bottom=y; //top not necessary, as we are going linewise top to bottom
             // std::cout << "bottom value changed to " << _bottom << "\n";
            }
            if(y<_top) _top=y;
           // std::cout << "adding point " << x << " " << y  << "to the list\n";

          }
      }
    };
  }
};

//the string of glyphs
using gly_string = std::vector<glyph>;

//algorithm, scan for glyphs
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
          cont=text[g].contains(point{x,y});
          if (cont) break;
        }
        //find all pixels for the new glyph through neighbourhood
        if (!cont){
          //std::cout << "adding new glyph for coord" << x <<" "<< y <<"\n";
          text.push_back(glyph(y,x, input)); //create new glyph and add to vector
        }
      }
    }
  }

  std::cout << "found "<< text.size() << " glyphs in image.\n";
  return text;
}


//comments
/* to avoid checks if glyphs need to be unified i first scan the whole glyph via density connection
possibly object to change

idea
scan through all the black pixels and label them. if they happen to connect use the smaller label on both and erase the larger label

*/



#endif