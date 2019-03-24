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
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

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
    static void PrintString(int count, char* str, ...);
    static void PrintMatrix44f(M3DMatrix44f matrix);
    static void PrintMatrix44f(const M3DMatrix44f matrix);
    static void LoadTGATexture(const char* filepath, GLenum filter, GLenum wrapMode);
    static void LoadTGATextureArray(const char* filepath[], GLint count, GLenum filter, GLenum wrapMode);
    static void LoadTGACubemap(const char* filepath[], GLenum magFilter, GLenum minFilter, GLenum wrapMode);
    static void ExecuteKeyFn(unsigned char key, int x, int y, GLMatrixStack& modelviewStack);
    static void CheckErrors(std::string desc);
    static bool FileExists(const std::string& abs_filename);
    static std::string GetBaseDir(const std::string& filepath);
};

#endif // TOOLS__



