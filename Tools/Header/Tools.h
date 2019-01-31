#ifndef TOOLS__LIBRARY
#define TOOLS__LIBRARY _

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <math3d.h>
#include <stdarg.h>
#include <string.h>
#include <GLTools.h>
#include <GLBatch.h>
#include <GLFrustum.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <iostream>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#define random(x) (rand()%x)
#define clamp(ret, x, min, max) if(x<min)ret=min; else if(x>max) ret=max;else ret=x;
class Util
{
public:
    static void printstring(int count, char* str, ...);
    static void printmatrix44f(M3DMatrix44f matrix);
    static void printmatrix44f(const M3DMatrix44f matrix);
    static void LoadTGATexture(const char* filepath, GLenum filter, GLenum wrapMode);
    static void LoadTGATextureArray(const char* filepath[], GLint count, GLenum filter, GLenum wrapMode);
};





#endif // TOOLS__



