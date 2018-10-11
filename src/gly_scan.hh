#ifndef __GLY_SCAN_HH_INCLUDED__
#define __GLY_SCAN_HH_INCLUDED__

#include <unordered_set>
#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include <boost/filesystem.hpp>
#include <climits>
#include "images.hh"
#include "structures.hh"

//from structures.hh
extern const int WHITE;
extern const int BLACK;
extern const double T;

//from images.hh
extern matrix read_img_to_matrix(const std::string & fname);
//extern template<class M> void to_image(M && input);


//forward declarations
class glyph;
using gly_string = std::vector<glyph>;
template<class M> decltype(auto) gly_scan(M && input);


using trans_tab = std::pair<std::vector<matrix>,std::vector<char>>;


template<class M> auto similarity(M && input,M && comp);
template<class M> auto similarity2(M && input,M && comp);


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
}; //direct proximity



template<class M>
auto similarity(M && input,M && comp){
  int H=input.size();
  int W=std::begin(input)->size();
  int result=0;

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
  return -std::sqrt(result);
}

template<class P>
auto eukl_dist(P && p1, P && p2){ //points basically
  return std::sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

template<class P>
auto length(P && p){
  return eukl_dist(std::forward<P>(p),point{0,0});
}


template<class M>
auto similarity2(M && input, M && comp){
  int H=input.size();
  int W=input[0].size();
  double dist=0;

  //highly inefficient and highly redundant
  //wanted some sort of dynamic time warping in 2d but thats not that easy im afraid
  //2d-dtw would be very helpful if scanning whole areas instead of (resized) glyphs


  for (Y y1=0; y1<H;++y1){
    for (X x1=0;x1<W;++x1){
      //for white pixels the outside can be closest pixel
      //for black pixels only inside
      //have a full circle around of x1|y1 i.e. x2|y2 (relative coordinates)
      if (input[y1][x1]>=WHITE){
        // distance to border as upper limit (outside is always white)
        double dist_min=std::min(std::min(x1+1,y1+1),std::min(W-x1,H-y1));
        for (Y y2=0; y2<dist_min ;++y2){
          for (X x2=0; x2<dist_min ;++x2){
            if (comp[y1+y2][x1+x2]>=WHITE ||
                comp[y1-y2][x1-x2]>=WHITE ||
                comp[y1-y2][x1+x2]>=WHITE ||
                comp[y1+y2][x1-x2]>=WHITE){
              dist_min=std::min(dist_min, length(point{x2,y2}));
            }
          }
        }
        dist+=dist_min;
      }
      else{
        double dist_min=W+H; //Max manhattan dist as an upper limit
        for (Y y2=0; y2<dist_min ;++y2){
          for (X x2=0; x2<dist_min ;++x2){
            if (x1-x2>=0 && x1+x2<W &&
                y1-y2>=0 && y1+y2<H){
              if (comp[y1+y2][x1+x2]<WHITE*T ||
                  comp[y1-y2][x1-x2]<WHITE*T ||
                  comp[y1-y2][x1+x2]<WHITE*T ||
                  comp[y1+y2][x1-x2]<WHITE*T){
                dist_min=std::min(dist_min, length(point{x2,y2}));
              }
            }
          }
        }
        dist+=dist_min;
      }
    }
  }
  dist/=H*W;//norm
  //std::cout << "score= " << -std::sqrt(result) << "\n";
  return -dist;
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
  glyph(point p, M && input)
  {findall(std::forward<M>(input),p);} //top will remain, bottom, left and right can change


  bool contains(const point & p) const{
    return (std::find(_data.begin(), _data.end(), p)!=_data.end());
  }

  bool operator==(const self_t & other)const {
    return other.contains(*(_data.begin()));
  }

  bool operator!=(const self_t & other)const {
    return !(*this==other);
  }

  void fuse(const glyph other){ //in case of i, j, ;, : and the likes
    std::for_each(other.data().begin(), other.data().end(),[&](auto p){
      this->insert(p);
    });
  }

  //constructs a matrix from a glyph
  decltype(auto) to_matrix() const {
    return matrix(data());
  }

    int left() const   {return _left;}
    int right() const  {return _right;}
    int top() const    {return _top;}
    int bottom() const {return _bottom;}

  const Cont & data() const{return _data;}

  private:
    int _left=INT_MAX;
    int _right=INT_MIN;
    int _top=INT_MAX;
    int _bottom=INT_MAX;
    //std::unordered_set<point,MyHash> _data;
    Cont _data;


  void insert(point p){
    if(!contains(p))
      _data.push_back(p);
    if (p.x<_left)    _left=p.x;
    if (p.x>_right)   _right=p.x;
    if (p.y<_top)     _top=p.y;
    if (p.y>_bottom)  _bottom=p.y;
  }


  //only to be used in the beginning when _x,_y is the first pixel to be touched
  template<class M>
  void findall(M && input,point p){
    const int height=input.size();
    const int width=std::begin(input)->size();

    //std::cout << height << " "<< input[0][0] <<"\n";

    insert(p);
    std::vector<point> queue;
    //initial value to get it started
    queue.push_back(p); //yet-to-be-checked, actually not a queue, but a lifo
    for (int i=0;i<queue.size();++i){
      for (auto d : dir_prox){ //check all the neighbours
        int x = queue[i].x+d.x;
        int y = queue[i].y+d.y;
        if (x >=0 && x<width &&
            y>=0 && y<height &&
            !contains(point{x,y})) //only add pixels, that have not been visited before
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

  template<class M, class Tr>
  decltype(auto) to_char(M && m, Tr && trans){
    char best=' ';
    int init_score=-500;
    double score=init_score;

    auto mask = matrix(std::forward<M>(m),MaskH, MaskW);
    //to_image(mask); //for debugging and demontration purposes
    for (int i=0;i<trans.first.size();++i){
      auto curr= similarity(mask,trans.first[i]);
      //auto curr= similarity2(comp,trans.first[i]);
      if (curr>score){
        score =curr; //max
        best=trans.second[i];
      }
      //if (score==0) break;
    }
    return std::move(std::make_pair(best,score));
  }




//algorithm, scan for glyphs, outputs an rvalue vector of glyphs aka gly_string
template<class M>
decltype(auto) gly_scan(M && input){
  int height=input.size();
  int width=std::begin(input)->size();
  gly_string text; //

  //std::cout << "gly scan" << input[0][0] <<"\n";

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
          text.push_back(glyph(point{x,y}, input)); //create new glyph and add to vector
        }
      }
    }
  }


  //find composite glyphs to be fused and resize the container

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
          //std::cout << "fused glyphs!\n";
          --i1;
          --i2;
        } //reverse order would be more performant and less repartitioning
    }
  }
  text.shrink_to_fit();
  //std::cout << "found "<< text.size() << " glyphs in image.\n";
  return text;
}


// recognition part


decltype(auto) make_masks(){
  // for both jpegs in folder Trainingimages make a mask according to ascii numbers of chars
  //->vector of ascii char matrixes to compare with
  std::vector<char> chars;
  std::vector<matrix> masks;
  std::string path = "../TrainingimagesASCII";

  for (auto & p : boost::filesystem::directory_iterator(path)){ //C++17 & -lstc++fs for linking
    const std::string path=p.path().string();
    masks.push_back(
      matrix(//resize
          gly_scan(
            read_img_to_matrix(path))
          .back().to_matrix(),
        MaskH,MaskW
      )
    );
    //crop path and ending from 'path'
    //turn string to int and to corresponding ASCII char
    int len=path.length();
    std::string st;
    int end=len;//again: 1 behind last
    int start=0;
    for (int i=len;i>=0;--i){
      if (path[i]=='.') end=i;
      if (path[i]=='/'){
        start=i+1;
        break;
      }
    }
    for (int i=start;i<end;++i)
      st.push_back(path[i]);

    chars.push_back(static_cast<char>(std::stoi(st)));

    //std::cout << "mask finished for" << path << " -> "<< path[len-5] << "\n";
  }
  trans_tab trans=std::make_pair(masks,chars);
  return trans;
}


//using trans_tab = std::pair<std::vector<matrix>,std::vector<char>>;
template<class M,class Tt>
decltype(auto) recognise(M && m, Tt && tran){

//first scan the line for glyphs
  auto gly_s=gly_scan(m);

// secondly sort the string. It's unsorted due to scanning order (linewise top->bottom), so the taller letters always go first
  std::sort(gly_s.begin(),gly_s.end(), [&](auto & l, auto & n){return l.left()<n.left();});
  std::string res (gly_s.size(),'_');

//lastly translate the glyphs to chars
  //std::transform(std::execution::par_unseq,gly_s.begin(), gly_s.end(), s.begin() //since c++17
  std::transform(gly_s.begin(), gly_s.end(), res.begin(),                           //before c++17
    [&](auto & g){
      return to_char(g.to_matrix(),tran).first;
      //}
      //std::cout << "found an unrecognizable glyph!\n";
      //return '_';
    }
  );
  res+="\n";

  //empty spaces - (not by pink floyd...)
  int i=0;
  for (auto it=gly_s.begin();it!=gly_s.end()-1;++it){
    int tot = ((it+1)->right()-it->left());
    int sum_let = (it->right()-it->left()+(it+1)->right()-(it+1)->left());
    if (tot>sum_let+5)
      res.insert(++i," ");
    ++i;
  }
  return res;
}




//recognition end

#endif