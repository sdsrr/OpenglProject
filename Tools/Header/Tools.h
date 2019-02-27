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
#include <GLFrame.h>
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


class Util
{
public:
    static const char* projectPath;
    static char* GetFullPath(const char* relativePath);
    static void printstring(int count, char* str, ...);
    static void printmatrix44f(M3DMatrix44f matrix);
    static void printmatrix44f(const M3DMatrix44f matrix);
    static void LoadTGATexture(const char* filepath, GLenum filter, GLenum wrapMode);
    static void LoadTGATextureArray(const char* filepath[], GLint count, GLenum filter, GLenum wrapMode);
    static void LoadTGACubemap(const char* filepath[], GLenum magFilter, GLenum minFilter, GLenum wrapMode);
    static void executeKeyFn(unsigned char key, int x, int y, GLMatrixStack& modelviewStack);
};



#endif // TOOLS__



