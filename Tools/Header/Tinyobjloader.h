#ifndef TINYOBJLOADER
#define TINYOBJLOADER

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <map>
#include <vector>

#include "../Header/Tools.h"
#include "../Header/UtilTimer.h"

#include "stb_image.h"
#include "tiny_obj_loader.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

typedef struct
{
    GLuint vb_id;  // vertex buffer id
    int numTriangles;
    size_t material_id;
} DrawObject;

struct vec3
{
    float v[3];
    vec3()
    {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
    }
};

class Tinyobjloader
{
private:
    UtilTimer tm;
    std::vector<tinyobj::material_t> materials;
    std::map<std::string, GLuint> textures;
    std::vector<DrawObject> gDrawObjects;
public:
    void Draw();
    void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);
    void NormalizeVector(vec3 &v);
    bool HasSmoothingGroup(const tinyobj::shape_t& shape);
    void ComputeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape, std::map<int, vec3>& smoothVertexNormals);
    bool LoadObjAndConvert(float bmin[3], float bmax[3], const char* filename);
};

#endif // TINYOBJLOADER
