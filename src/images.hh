#ifndef __IMAGES_H_INCLUDED__
#define __IMAGES_H_INCLUDED__

  #include <iostream>
  #include <boost/gil/extension/io/jpeg/old.hpp> //<--- this???
  //probably missing:
  //#include <boost/gil/extension/io/jpeg/tags.hpp>
  //#include <boost/gil/gil_all.hpp>
  #include <array>
  #include <string>
  #include <numeric>
  #include <algorithm>
  #include <utility>
  //#include "recognition.hh"
  #include "structures.hh"
  #include "gly_scan.hh"

//Globals


//using namespace boost::gil;

//read and scale image
//TODO improve contrast + other stuff

//decltype(auto)
matrix read_img_to_matrix(const std::string & fname){
  boost::gil::gray8_image_t img;
  //jpeg_read_image(fname, img); //jpeg_read_image() ?
  boost::gil::read_and_convert_image(fname, img, boost::gil::jpeg_tag());

//  std::cout << "Read complete, got an image " << img.width()
//            << " by " << img.height() << " pixels\n";
//
//  gil::gray8_pixel_t px = *const_view(img).at(33, 33);
//  std::cout << "The pixel at 33,33 is "
//            << (int)px[0] << '\n';

  int const height=img.height();
  int const width=img.width();

  //instantiating the matrix

  matrix pixels;//={ {(width)} };
  //copying the pixeldata (in case of there being some sort of proxy - thx c++)
  for (int i=0; i<height; ++i){
    std::vector<int> line (width);
    for (int j=0; j<width; ++j){
      line[j]=*const_view(img).at(j,i);
    }
    pixels.push_back(line);
  }

  return pixels; //Move will probably not do, as the problem size is unknown at compile time, but who knows...?
}

int count=0;

template<class M>
void matrix_to_image(M && input){

  std::string filename( "../debug-images/image" );
  filename+=std::to_string(count++);
  filename+=".jpg";
  int H=input.size();
  int W=input[0].size();
  boost::gil::gray8_image_t img ( W, H );

// write data into image
  for (int row=0;row<H;++row){
    for (int col=0;col<W;++col){
      *boost::gil::view(img).at(col, row)=input[row][col];
    }
}

 // write_view( filename, view( img ), jpeg_tag());
  boost::gil::jpeg_write_view( filename, view( img ));
  //std::cout << "turned a matrix to an image.\n";

}
    //fucking include -ljpeg at compiler!



template<class M, class T>
std::string find_lines(M && m, T && t){

  //finding edges by max functions
  const X hsize=m[0].size();
  const Y vsize=m.size();

  std::vector<int> rows(vsize);
  X x_min = hsize;
  X x_max = 0;
  //not to be parallelized writing xmin and x max are critical
  std::transform(m.begin(),m.end(),rows.begin(),
    [&](auto & v){
      //X
      //if you find something thats black on the left from min or right from max change min/max
      for (X x=0;x<x_min;++x)
        if (v[x]<255*0.5)
          x_min=x-1; //margin, you dont want black pixels on the edge ->segvfault
      for (X x=hsize;x>x_max;--x)
        if (v[x]<255*0.5)
          x_max=x+1;
      //Y
      //if you find a black pixel: mark line as black, else white
      if (std::any_of(v.begin()+x_min,v.begin()+x_max+1,[&](auto & e){return e==0;}))
        return 0;
      else
        return 255;
    }
  );

  std::vector<std::pair<Y,Y>> lines; //min-max pairs of Y

  for (auto & a  : rows)
    std::cout << a << "\n";


  Y y_min=0;
  Y y_max=0;

  for (Y y=0;y<vsize-1;++y){
    if (rows[y]==255){
      //for white lines
      if (rows[y+1]==0){
        y_min=y;
      }
    }
    //for black lines
    else{
      if (rows[y+1]==255){
        y_max=y+1; //to avoid black pixels on the edge causing issues...
        lines.push_back(std::make_pair(y_min,y_max)); //keep in mind, that min<max but higher in pos
        std::cout << "y_max found!\n";
      }
    }
  }

//debug output
  std::cout << "x_min: " << x_min <<"\n";
  std::cout << "x_man: " << x_max <<"\n";

  for (auto & a : lines){
    std::cout << "y_min: " << a.first << " y_max: " << a.second << "\n";
  }

/*
maybe a horizontal iterator might be in order... ->locator
however... it may not be very performant to leave aside
 all the other stuff that has to be added anyway aside...
 ->so 1d should be enough for now
*/


  std::vector<std::string> text_lines (lines.size());
  std::transform(lines.begin(),lines.end(),text_lines.begin(),
    [&](auto & p){
      //create new matrix from m and the borders
      matrix n;
      std::for_each(m.begin()+p.first,m.begin()+p.second+1,
        [&](auto & v){
          n.push_back(std::vector<int> (x_max-x_min+1));
          std::transform(v.begin()+x_min,v.begin()+x_max,n.back().begin(),
            [&](auto & e){
              return e;
            }
        );
      });
      //turn to string
      return recognise(std::move(n), t); //dummy
    }

  );
  //putting it together
  std::string res;
  for (auto & s : text_lines)
    res+=s;
  return res;

}



#endif