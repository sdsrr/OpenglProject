#ifndef TOOLS__LIBRARY
#define TOOLS__LIBRARY

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <math3d.h>
#include <stdarg.h>
#include <string.h>
#include <GLTools.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
#include <list>
#include <ctime>

// OpenEXR headers
#include <ImfRgbaFile.h>
#include <ImfArray.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#define random(x) (rand()%x)
#define PI 3.1415926
#define Enum2String(x) (#x)

struct vec3
{
    float v[3] = {0,0,0};
};

struct vec2
{
    float v[2] = {0,0};
};

class Util
{
public:
    static const char* projectPath;
    static char* GetFullPath(const char* relativePath);

    static void LoadTGATexture(const char* filepath, GLenum filter, GLenum wrapMode);
    static void LoadJPGTexture(const char* filepath, GLenum filter, GLenum wrapMode);
    static void LoadTGATextureArray(const char* filepath[], GLint count, GLenum filter, GLenum wrapMode);
    static void LoadTGACubemap(const char* filepath[], GLenum magFilter, GLenum minFilter, GLenum wrapMode);
    static void LoadOpenEXR(char*filepath, GLenum filter, GLenum wrapMode, int& texWidth, int& texHeight);
    static void LoadVector3f(M3DVector3f vector, float x, float y, float z);

    static void RandomVector3(M3DVector3f& vector, int maxX, int maxY, int maxZ);
    static void RandomCol(M3DVector3f& color);
    static bool CompareMatrix(const M3DMatrix44f matrixa, const M3DMatrix44f matrixb);
    static void CheckErrors(std::string desc);
    static void UpdateFrameRate();
    static void CheckFBO();
    //result = origin + direction * distance
    static void MoveVector(M3DVector3f& result, const M3DVector3f& origin, const M3DVector3f& direction, GLfloat distance);
    //result = ((p1-origin)+(p2-origin)+(p3-origin))/3
    static void CalcNormal(M3DVector3f& result, const M3DVector3f& origin, const M3DVector3f& p1, const M3DVector3f& p2, const M3DVector3f& p3);
    // file
    static bool FileExists(const std::string& abs_filename);
    static std::string GetBaseDir(const std::string& filepath);

    //vector
    static void NormalizeVector(vec3 &v);//单位化向量v
    static void Roate(vec2& v, float angle);
};
#endif // TOOLS__



