#ifndef _CPU_EROSION_H_
#define _CPU_EROSION_H_

float* heightmap_to_vertices(const char* heightmap_file, unsigned int** indices_loc, float** tex_coords_loc, int* n_vertices, int* n_indices, int* n_strips, int* n_tris_strips);

#endif