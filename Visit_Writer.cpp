#include "Visit_Writer.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

static FILE* fp = NULL;
static int useBinary = 0;
static int numInColumn = 0;

static void open_file(const char* filename)
{
    char full_filename[1024];
    if (strstr(filename, ".vtk") != NULL)
    {
        strcpy_s(full_filename, filename);
    }
    else
    {
        sprintf_s(full_filename, "%s.vtk", filename);
    }

    fp = fopen(full_filename, "w+");
}

static void end_line(void)
{
    if (!useBinary)
    {
        char str2[8] = "\n";
        fprintf(fp, "%s", str2);
        numInColumn = 0;
    }
}

static void close_file(void)
{
    end_line();
    fclose(fp);
    fp = NULL;
}

static void write_string(const char* str)
{
    fprintf(fp, "%s", str);
}

static void write_header(void)
{
    fprintf(fp, "# vtk DataFile Version 2.0\n");
    fprintf(fp, "Written using VisIt writer\n");
    if (useBinary)
        fprintf(fp, "BINARY\n");
    else
        fprintf(fp, "ASCII\n");
}

static void force_big_endian(unsigned char* bytes)
{
    static int doneTest = 0;
    static int shouldSwap = 0;
    if (!doneTest)
    {
        int tmp1 = 1;
        unsigned char* tmp2 = (unsigned char*)&tmp1;
        if (*tmp2 != 0)
            shouldSwap = 1;
        doneTest = 1;
    }

    if (shouldSwap & useBinary)
    {
        unsigned char tmp = bytes[0];
        bytes[0] = bytes[3];
        bytes[3] = tmp;
        tmp = bytes[1];
        bytes[1] = bytes[2];
        bytes[2] = tmp;
    }
}

static void write_float(float val)
{
    if (useBinary)
    {
        force_big_endian((unsigned char*)&val);
        fwrite(&val, sizeof(float), 1, fp);
    }
    else
    {
        char str[128];
        sprintf(str, "%20.12e ", val);
        fprintf(fp, "%s", str);
        if (((numInColumn++) % 9) == 8)
        {
            end_line();
        }
    }
}



static void new_section(void)
{
    if (numInColumn != 0)
        end_line();
    numInColumn = 0;
}

void write_variables(int nvars, int* vardim, int* centering,
    const char* const* varname, float** vars,
    int npts, int ncells)
{
    char str[1024];
    int i, j, first_scalar, first_vector;
    int num_scalars, num_vectors;
    int num_field = 0;

    new_section();
    sprintf(str, "CELL_DATA %d\n", ncells);
    write_string(str);

    first_scalar = 0;
    first_vector = 0;
    num_scalars = 0;
    num_vectors = 0;
    /* The field data is where the non-primary scalars and vectors are
     * stored.  They must all be grouped together at the end of the point
     * data.  So write out the primary scalars and vectors first.
     */
    for (i = 0; i < nvars; i++)
    {
        if (centering[i] == 0)
        {
            int num_to_write = 0;
            int should_write = 0;

            if (vardim[i] == 1)
            {
                if (first_scalar == 0)
                {
                    should_write = 1;
                    sprintf(str, "SCALARS %s float\n", varname[i]);
                    write_string(str);
                    write_string("LOOKUP_TABLE default\n");
                    first_scalar = 1;
                }
                else
                    num_scalars++;
            }
            else if (vardim[i] == 3)
            {
                if (first_vector == 0)
                {
                    should_write = 1;
                    sprintf(str, "VECTORS %s float\n", varname[i]);
                    write_string(str);
                    first_vector = 1;
                }
                else
                    num_vectors++;
            }
            else
            {
                printf("Only supported variable dimensions are 1 and 3.\n");
                printf("Ignoring variable %s.\n", varname[i]);
                continue;
            }

            if (should_write)
            {
                num_to_write = ncells * vardim[i];
                for (j = 0; j < num_to_write; j++)
                {
                    write_float(vars[i][j]);
                }
                end_line();
            }
        }
    }

    first_scalar = 0;
    if (num_scalars > 0)
    {
        sprintf(str, "FIELD FieldData %d\n", num_scalars);
        write_string(str);
        for (i = 0; i < nvars; i++)
        {
            int should_write = 0;
            if (centering[i] == 0)
            {
                if (vardim[i] == 1)
                {
                    if (first_scalar == 0)
                    {
                        first_scalar = 1;
                    }
                    else
                    {
                        should_write = 1;
                        sprintf(str, "%s 1 %d float\n", varname[i], ncells);
                        write_string(str);
                    }
                }
            }

            if (should_write)
            {
                int num_to_write = ncells * vardim[i];
                for (j = 0; j < num_to_write; j++)
                {
                    write_float(vars[i][j]);
                }
                end_line();
            }
        }
    }

    first_vector = 0;
    if (num_vectors > 0)
    {
        sprintf(str, "FIELD FieldData %d\n", num_vectors);
        write_string(str);
        for (i = 0; i < nvars; i++)
        {
            int should_write = 0;
            if (centering[i] == 0)
            {
                int num_to_write = 0;

                if (vardim[i] == 3)
                {
                    if (first_vector == 0)
                    {
                        first_vector = 1;
                    }
                    else
                    {
                        should_write = 1;
                        sprintf(str, "%s 3 %d float\n", varname[i], ncells);
                        write_string(str);
                    }
                }
            }

            if (should_write)
            {
                int num_to_write = ncells * vardim[i];
                for (j = 0; j < num_to_write; j++)
                {
                    write_float(vars[i][j]);
                }
                end_line();
            }
        }
    }

    new_section();
    sprintf(str, "POINT_DATA %d\n", npts);
    write_string(str);

    first_scalar = 0;
    first_vector = 0;
    num_scalars = 0;
    num_vectors = 0;
    /* The field data is where the non-primary scalars and vectors are
     * stored.  They must all be grouped together at the end of the point
     * data.  So write out the primary scalars and vectors first.
     */
    for (i = 0; i < nvars; i++)
    {
        if (centering[i] != 0)
        {
            int num_to_write = 0;
            int should_write = 0;

            if (vardim[i] == 1)
            {
                if (first_scalar == 0)
                {
                    should_write = 1;
                    sprintf(str, "SCALARS %s float\n", varname[i]);
                    write_string(str);
                    write_string("LOOKUP_TABLE default\n");
                    first_scalar = 1;
                }
                else
                    num_scalars++;
            }
            else if (vardim[i] == 3)
            {
                if (first_vector == 0)
                {
                    should_write = 1;
                    sprintf(str, "VECTORS %s float\n", varname[i]);
                    write_string(str);
                    first_vector = 1;
                }
                else
                    num_vectors++;
            }
            else
            {
                printf("Only supported variable dimensions are 1 and 3.\n");
                printf("Ignoring variable %s.\n", varname[i]);
                continue;
            }

            if (should_write)
            {
                num_to_write = npts * vardim[i];
                for (j = 0; j < num_to_write; j++)
                {
                    write_float(vars[i][j]);
                }
                end_line();
            }
        }
    }

    first_scalar = 0;
    if (num_scalars > 0)
    {
        sprintf(str, "FIELD FieldData %d\n", num_scalars);
        write_string(str);
        for (i = 0; i < nvars; i++)
        {
            int should_write = 0;
            if (centering[i] != 0)
            {
                if (vardim[i] == 1)
                {
                    if (first_scalar == 0)
                    {
                        first_scalar = 1;
                    }
                    else
                    {
                        should_write = 1;
                        sprintf(str, "%s 1 %d float\n", varname[i], npts);
                        write_string(str);
                    }
                }
            }

            if (should_write)
            {
                int num_to_write = npts * vardim[i];
                for (j = 0; j < num_to_write; j++)
                {
                    write_float(vars[i][j]);
                }
                end_line();
            }
        }
    }

    first_vector = 0;
    if (num_vectors > 0)
    {
        sprintf(str, "FIELD FieldData %d\n", num_vectors);
        write_string(str);
        for (i = 0; i < nvars; i++)
        {
            int should_write = 0;
            if (centering[i] != 0)
            {
                int num_to_write = 0;

                if (vardim[i] == 3)
                {
                    if (first_vector == 0)
                    {
                        first_vector = 1;
                    }
                    else
                    {
                        should_write = 1;
                        sprintf(str, "%s 3 %d float\n", varname[i], npts);
                        write_string(str);
                    }
                }
            }

            if (should_write)
            {
                int num_to_write = npts * vardim[i];
                for (j = 0; j < num_to_write; j++)
                {
                    write_float(vars[i][j]);
                }
                end_line();
            }
        }
    }
}

void write_rectilinear_mesh(const char* filename, int ub, int* dims,
    float* x, float* y, float* z,
    int nvars, int* vardim, int* centering,
    const char* const* varnames, float** vars)
{
    int   i, j;
    char  str[128];
    int npts = dims[0] * dims[1] * dims[2];
    int ncX = (dims[0] - 1 < 1 ? 1 : dims[0] - 1);
    int ncY = (dims[1] - 1 < 1 ? 1 : dims[1] - 1);
    int ncZ = (dims[2] - 1 < 1 ? 1 : dims[2] - 1);
    int ncells = ncX * ncY * ncZ;

    useBinary = ub;
    open_file(filename);
    write_header();

    write_string("DATASET RECTILINEAR_GRID\n");
    sprintf(str, "DIMENSIONS %d %d %d\n", dims[0], dims[1], dims[2]);
    write_string(str);
    sprintf(str, "X_COORDINATES %d float\n", dims[0]);
    write_string(str);
    for (i = 0; i < dims[0]; i++)
        write_float(x[i]);
    new_section();
    sprintf(str, "Y_COORDINATES %d float\n", dims[1]);
    write_string(str);
    for (i = 0; i < dims[1]; i++)
        write_float(y[i]);
    new_section();
    sprintf(str, "Z_COORDINATES %d float\n", dims[2]);
    write_string(str);
    for (i = 0; i < dims[2]; i++)
        write_float(z[i]);

    write_variables(nvars, vardim, centering, varnames, vars, npts, ncells);

    close_file();
}

void write_regular_mesh(const char* filename, int ub, int* dims,
    int nvars, int* vardim, int* centering,
    const char* const* varnames, float** vars)
{
    int  i;

    float* x = (float*)malloc(sizeof(float) * dims[0]);
    float* y = (float*)malloc(sizeof(float) * dims[1]);
    float* z = (float*)malloc(sizeof(float) * dims[2]);

    for (i = 0; i < dims[0]; i++)
        x[i] = (float)i;
    for (i = 0; i < dims[1]; i++)
        y[i] = (float)i;
    for (i = 0; i < dims[2]; i++)
        z[i] = (float)i;

    write_rectilinear_mesh(filename, ub, dims, x, y, z, nvars, vardim,
        centering, varnames, vars);

    free(x);
    free(y);
    free(z);
}