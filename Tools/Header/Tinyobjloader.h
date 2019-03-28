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

class Tinyobjloader
{
private:
    UtilTimer tm;
    std::vector<tinyobj::material_t> materials;
    std::map<std::string, GLuint> textures;
    std::vector<DrawObject> gDrawObjects;
public:
    void DrawGraph();
    void DefaultDrawGraph();
    void DrawLine(GLfloat r, GLfloat g, GLfloat b);
    //���ݶ���v0(xyz),v1(xyz),v2(xyz)���㷨��N
    void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);
    bool HasSmoothingGroup(const tinyobj::shape_t& shape);
    //����������shape��������ķ���
    void ComputeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape, std::map<int, vec3>& smoothVertexNormals);
    //����model�����ذ�Χ�еļ�С/�����
    bool LoadObjAndConvert(float bmin[3], float bmax[3], const char* filename);
    bool LoadObjAndConvert(const char* filename);
};

#endif // TINYOBJLOADER
