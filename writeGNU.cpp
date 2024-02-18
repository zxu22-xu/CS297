#include "writeGNU.h"
#include "Visit_Writer.h"
#include "MD_array.h"
#include <cstdio>

template <class T> 
const char* MDWrite(MDarray<T>* a_array) {
	static int fileCount = 0;
	static char nameBuffer[10];

	if (a_array == NULL) {
		return nameBuffer;
	}

	sprintf(nameBuffer, "md%d", fileCount);
	MDWrite(nameBuffer, a_array);
	fileCount++;
	return nameBuffer;
}

template <class T> 
void MDWrite(const char* a_filename, MDarray<T>* a_array) {
	if (a_filename == NULL || a_array == NULL) {
		return;
	}

	int dim[3];
	int vardims[1] = { 1 };
	int centering[1] = { 1 };
	float* vars[1];
	const char* vr = "variable";
	const char* const varnames[] = { "cellCentered" };
	int lo[DIM];
	int hi[DIM];
	const Box& box = a_array->getBox();
	box.getLowCorner(lo);
	box.getHighCorner(hi);

	for (int i = 0; i < DIM; i++) {
		dim[i] = hi[i] - lo[i] + 1;
	}

	float& val = a_array->operator[](0);
	vars[0] = &val; //because vars is an array of float ptrs...
	write_regular_mesh(a_filename, 1, dim, 1, vardims, centering, varnames, vars);
}

