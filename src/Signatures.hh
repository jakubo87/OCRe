#include "structures.hh"
#include "images.hh"
#include "gly_scan.hh"


//collection of signatures to check for inconsistencies...

//from images.hh

decltype(auto) read_img_to_matrix(const std::string & fname);

template<class M> void matrix_to_image(M && input);


//from gly_scan.hh

template<class M> decltype(auto) gly_scan(M && input);

trans_tab make_masks();

template<class M,class T> decltype(auto) recognise(point UL, point LR, M && m, T && tran);

template<class M, class C> auto similarity(M && input,C && comp);

template<class M> glyph(Y y,X x, M && input);