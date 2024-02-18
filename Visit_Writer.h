#ifndef Visit_Writer 
#define Visit_Writer

#define VISIT_VERTEX         1
#define VISIT_LINE           3
#define VISIT_TRIANGLE       5
#define VISIT_QUAD           9
#define VISIT_TETRA         10
#define VISIT_HEXAHEDRON    12
#define VISIT_WEDGE         13
#define VISIT_PYRAMID       14

void write_regular_mesh(const char* filename, int useBinary, int* dims,
    int nvars, int* vardim, int* centering,
    const char* const* varnames, float** vars);

#endif 
