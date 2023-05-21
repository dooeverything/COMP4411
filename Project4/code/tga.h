#ifndef TGA_H_
#define TGA_H_

#include <fstream>
#include <iostream>
#include <string>



extern unsigned char* loadTGA(const char* fname, int& width, int& height, int& dataType);


#endif
