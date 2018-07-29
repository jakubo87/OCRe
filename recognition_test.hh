#include <array>
#include <vector>
//#include "_chars.hh"

//Globals

template<int height,int width>
class matrix{
public:
  //ctors
  matrix(const std::array<int, height*width> & data):_data(data){};

  const std::array<int, height*width> & _data;
  const int _height=height;
  const int _width=width;
};

struct xy_char{
  int x; //maybe better to have usign long..?
  int y;
  char value;
  //opt padding or reduce x...?
};

std::vector<xy_char> output;



//20x10 Matrix
std::array<int,200> testmatrixIN_true
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
  0, 0, 0 ,0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//20x10 Matrix
std::array<int,200> testmatrixIN_false
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1 ,1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


  //matrix needs to be iterated over area of interest
  //for now has fixed size -> templates (TODO)?
  //input would be a view!! TODO
  template <int H,int W>
  int similarity_measure_x100(matrix<H,W> input,matrix<H,W> comp)
  {
  int result=0;
  for (int i=0;i<H;++i)
    for (int j=0;j<W;++j)
      if (input._data[i*W+j]==comp._data[i*W+j])
        ++result;
//TODO std::transform, lines, matrix representation, dot product(!!)
  //max result is 13x9=117
  result=result*100/(H*W);
  return result;
  }


//iterates over all the input matrix and saves hits of over 90% in vector as a position value pair (to be turned into string)
template<int height,int width> //<-confusion? Matrix indices
void iterate(const matrix<height,width> & M_in, const matrix<13,9> & M_comp){
  //as mentioned: for now the comparing matrix (stamp) is fixed in size
  const int W=M_comp._width; //stamp width
  const int H=M_comp._height; //stamp height
  const int max_score=W*H;

  const int max_x=width-W+1; //#steps horizontally
  const int max_y=height-H+1; //#steps vertically
//TODO something more sophisticated maybe... transform and the likes
  for (int y=0; y<max_y; ++y)
    for (int x=0; x<max_x;++x)
      {
        //keep it "simple" for now
        //almost copied from simliarity_measure_x100 above

        int result=0;
        for (int i=0;i<H;++i)
          for (int j=0;j<W;++j)
          //form
          // row offset =y(stamp)+i(within stamp)
          // column offset=x(stamp)+j(within stamp)
          if (M_in._data[(y+i)*width+x+j]==M_comp._data[i*W+j])
            result+=1;
        //max result is 13x9=117
        result=result*100/max_score;
//simliarity score should provide a quite decent tool to extract chars -> blessing of dimensionality, iid distributed dimensions tend to have the same distance, but these here are not iid ;-)
  std::cout<<"x: "<<x<<" y: "<<y<<" similarity score: "<<result<<"\n";
        if (result>90){
          std::cout<<"char 7 found!\n";
          //write most likely result into output
          output.push_back(xy_char{x,y,'7'});
        }
      }

}




//20x10 Matrix
std::array<int,800> TM_IN
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1 ,1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
  0, 0, 0 ,0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1 ,1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
  0, 0, 0 ,0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0};



const std::array<int,117> ar_char_7
{ 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0,
  0, 0 ,0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0};



//13x9 Matrix
const std::array<int,117> ar_char_0
{ 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 1, 1, 0, 1, 1, 0, 0,
  0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 1 ,1, 0, 0, 0, 1, 1, 0,
  0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 1, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 1, 1, 0, 1, 1, 0, 0,
  0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0};


const matrix<80,10> test_IN (TM_IN);

//13x9 Matrix
const matrix<13,9> char_7 (ar_char_7);

const matrix<13,9> char_0 (ar_char_0);




  