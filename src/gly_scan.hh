#ifndef __GLY_SCAN_H_INCLUDED__
#define __GLY_SCAN_H_INCLUDED__

#include <unordered_set>
#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include <boost/filesystem.hpp>
#include "images.hh"
#include "structures.hh"


//#include "recognition.hh"


//forward declarations
class glyph;
//a container of glyphs (unordered)
using gly_string = std::vector<glyph>;
template<class M> decltype(auto) gly_scan(M && input);

extern matrix read_img_to_matrix(const std::string & fname);
template<class M> void matrix_to_image(M && input);

//using matrix = std::vector<std::vector<int>>;
using trans_tab = std::pair<std::vector<matrix>,std::vector<char>>;


template<class M, class C> auto similarity(M && input,C && comp);


//needed for hashing of composite structures
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



template<class M, class C>
auto similarity(M && input,C && comp){
  int H=input.size();
  int W=input[0].size();
  int result=0;
  //the hard way
	//used 2 accumulates here to not get race conditions from simultaneously writing to accum value
	//hopefully to be used in a parallel way-> execution policy parallel: std::execution::par_unseq
	//in more civilised days...
  std::vector<int> rows(input.size());
  std::transform(input.begin(), input.end(), comp.begin(), rows.begin() ,
    //lambda to fill row sum vector
    [&](const auto & /*vector*/ v1, const auto & v2){
      std::vector<int> cols(v1.size()); //temporary vector
      std::transform(v1.begin(),v1.end(),v2.begin(), cols.begin(),
        //lambda to fill col sum vector
        [&](const auto & i1, const auto & i2){
          return (i1-i2)*(i1-i2);
        }
      );
      return std::accumulate(cols.begin(), cols.end(),0);
      }
    );
  result= std::accumulate(rows.begin(), rows.end(), 0);
  result/=H*W;//norm
  //std::cout << "score= " << -std::sqrt(result) << "\n";
  return -(std::sqrt(result));
}











////////////////////
/////GLYPH CLASS////
////////////////////
//glyph: connected black pixels...
class glyph{
public:

  using self_t  = glyph;
  using Cont    = std::vector<point>;

  //ctor
  template<class M>
  glyph(Y y,X x, M && input)
  //glyph(Y y,X x, const matrix & input)
  :_top(y),_left(x), _bottom(y), _right(x) //init variables
  {findall(std::forward<M>(input));} //top will remain, bottom, left and right can change


  bool contains(const point & p) const{
    //if (_data.find(point{x,y})!=_data.end()) return true;
    return (std::find(_data.begin(), _data.end(), p)!=_data.end());
  }

  bool operator==(const self_t & other)const {
    return other.contains(*(_data.begin()));
  }

  bool operator!=(const self_t & other)const {
    return !(*this==other);
  }


  decltype(auto) to_matrix() const {
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
    matrix_to_image(m);
    return m;
  }

//TODO
  void fuse(const glyph other){ //in case of i and ä and the likes
    std::for_each(other.data().begin(), other.data().end(),[&](auto p){
      this->insert(p);
    });
  }

  template<class T>
  std::pair<char,int> to_char(T && trans) const {
    char best=' ';
    int init_score=-200;
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

  const Cont & data() const{return _data;}

  private:
    Y _top; //line of the glyph (also top)
    X _left; //leftmost pixel (column)
    Y _bottom;
    X _right;
    //std::unordered_set<point,MyHash> _data;
    Cont _data;


  void insert(point p){
    if(!contains(p))
      _data.push_back(p);
      if (p.x<_left) _left=p.x;
      else if(p.x>_right)  _right=p.x;
      if (p.y>_bottom) _bottom=p.y;
      else if(p.y<_top) _top=p.y;
  }


  //only to be used in the beginning when _x,_y is the first pixel to be touched
  template<class M>
  void findall(M && input){
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
            insert(point{x,y}); //wont go into the queue a second time
          }
      }
    };
  }
}; //end class glyph



//algorithm, scan for glyphs, outputs an rvalue vector of glyphs aka gly_string
template<class M>
decltype(auto) gly_scan(M && input){
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


  //find composite glyphs to be fused and resize the container
  //TODO quick'n'dirty
  for (int i1=0;i1<text.size();++i1){
    for (int i2=0;i2<text.size();++i2){
      if (text[i2].left()<=text[i1].right() &&
          text[i2].right()>=text[i1].left()&&
          i2!=i1 &&
          text[i2].bottom()<text[i1].top()+2
          )
        { //i2 is the suspected upper one
          text[i1].fuse(text[i2]);
          text.erase(text.begin()+i2);
          std::cout << "fused glyphs!\n";
          --i1;
          --i2;
        } //reverse order would be more performant and less repartitioning
    }
  }
  text.shrink_to_fit();
  std::cout << "found "<< text.size() << " glyphs in image.\n";
  return text;
}


// recognition part


trans_tab make_masks(){
  // for both jpegs in folder Trainingimages make a mask according to ascii numbers of chars
  //->vector of ascii char matrixes to compare with
  trans_tab trans;
  std::vector<char> chars;
  std::vector<matrix> masks;
  std::string path = "../Trainingimages";

  for (auto & p : boost::filesystem::directory_iterator(path)){ //C++17 & -lstc++fs for linking
    const std::string path=p.path().string();
    masks.push_back(
      resize_matrix(
        gly_scan(
          read_img_to_matrix(path)
        ).back().to_matrix(),
        MaskW,
        MaskH
      )
    );
    int len=path.length();
    chars.push_back(path[len-5]);
    //std::cout << "mask finished for" << path << " -> "<< path[len-5] << "\n";
  }
  trans.first=masks;
  trans.second=chars;
  return trans;
}




//using trans_tab = std::pair<std::vector<matrix>,std::vector<char>>;
template<class M,class T>
decltype(auto) recognise(point UL, point LR, M && m, T && tran){

//first scan the line for glyphs
  auto gly_s=gly_scan(m);

// secondly sort the string. It's unsorted due to scanning order (linewise top->bottom), so the taller letters always go first
  std::sort(gly_s.begin(),gly_s.end(), [&](auto & m, auto & n){return m.left()<n.left();});
  std::string res (gly_s.size(),'_');

//lastly translate the glyphs to chars
  //std::transform(std::execution::par_unseq,gly_s.begin(), gly_s.end(), s.begin() //since c++17
  std::transform(gly_s.begin(), gly_s.end(), res.begin(),                           //before c++17
    [&](auto & g){
      const auto & c= g.to_char(tran); //c is pair of char and confidence
      //if (c.second>-100){
      std::cout << "char found: " << c.first <<"\n";
      return c.first;
      //}
      //std::cout << "found an unrecognizable glyph!\n";
      //return '_';
    }
  );
  res+="\n";
  return res;
  //TODO finding empty spaces.
}




//recognition end

#endif