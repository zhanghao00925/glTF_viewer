/************************/
/*  FILE NAME: image.h  */
/************************/
#ifndef _IMAGE_H_
#define _IMAGE_H_

/**************/
/*  INCLUDES  */
/**************/
#include <string>
#include <vector>

/*************************/
/*  CLASS NAME: Image  */
/*************************/
class Image
{
public:
    Image();
    Image(const Image& other);

public:
    int width;
    int height;
    int component;
    std::vector<unsigned char> data;
    std::string name;
}; // class Image
#endif // !_IMAGE_H_