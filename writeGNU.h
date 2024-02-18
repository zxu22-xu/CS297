#ifndef write_GNU
#define write_GNU

#include "MD_array.h"
#include <string>

template <class T>
const char* MDWrite(MDarray<T>* a_array);

template <class T> 
void MDWrite(const char* filename, MDarray<T>* a_array);

#endif
