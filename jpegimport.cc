//source    https://www.quora.com/How-can-we-convert-a-png-jpeg-image-into-a-2D-array-in-C++-If-possible-please-attach-code-snippets

    #include <iostream>
    #include <boost/gil/extension/io/jpeg_io.hpp>
    namespace gil = boost::gil;
    int main()
    {
     gil::rgb8_image_t img;
     gil::jpeg_read_image("demo.jpg", img);
     std::cout << "Read complete, got an image " << img.width()
               << " by " << img.height() << " pixels\n";
     gil::rgb8_pixel_t px = *const_view(img).at(5, 10);
     std::cout << "The pixel at 5,10 is "
               << (int)px[0] << ','
               << (int)px[1] << ','
               << (int)px[2] << '\n';
    }
