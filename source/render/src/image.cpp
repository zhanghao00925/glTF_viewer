/**************************/
/*  FILE NAME: image.cpp  */
/**************************/
#include "image.h"

/* default constructor */
Image::Image()
    : width()
    , height()
    , component()
    , data()
    , name()
{ /* empty */ }

/* copy constructor */
Image::Image(const Image& other)
    : width(other.width)
    , height(other.height)
    , component(other.component)
    , data(other.data)
    , name(other.name)
{ /* empty */ }