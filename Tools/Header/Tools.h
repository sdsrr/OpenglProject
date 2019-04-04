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
    static void PrintString(int count, char* str, ...);
    static void PrintMatrix44f(M3DMatrix44f matrix);
    static void PrintMatrix44f(const M3DMatrix44f matrix);
    static void LoadTGATexture(const char* filepath, GLenum filter, GLenum wrapMode);
    static void LoadJPGTexture(const char* filepath, GLenum filter, GLenum wrapMode);
    static void LoadTGATextureArray(const char* filepath[], GLint count, GLenum filter, GLenum wrapMode);
    static void LoadTGACubemap(const char* filepath[], GLenum magFilter, GLenum minFilter, GLenum wrapMode);
    static bool CompareMatrix(const M3DMatrix44f matrixa, const M3DMatrix44f matrixb);
    static void CheckErrors(std::string desc);
    static void UpdateFrameRate();
    static void CheckFBO();
    // file
    static bool FileExists(const std::string& abs_filename);
    static std::string GetBaseDir(const std::string& filepath);
    //vector
    static void NormalizeVector(vec3 &v);//单位化向量v
    static void Roate(vec2& v, float angle);
};

enum MouseMotion
{
    MMNone,
    MMLeftMousePress,
    MMMedMousePress,
    MMRightMousePress,
};


class NormalCamera
{
private:
    float moveSpeed = 1;
    float roateSpeed = 2;
    float width, height, fov;
    int prevMouseX, prevMouseY;
    MouseMotion mouseMotion;

    GLFrustum frustum;
    GLMatrixStack modelviewStack;
    GLMatrixStack projectStack;
    GLGeometryTransform transformPiple;
public:
    void OnUnInit();
    void OnInit(float w, float h, float fov, float moveSp, float roateSp);
    void KeyboardFn(unsigned char key, int x, int y);
    void MouseClick(int button, int action, int x, int y);
    void MotionFunc(int mouse_x, int mouse_y);
    void Resize(int w, int h);
    GLMatrixStack* GetModelviewStack();
    const M3DMatrix44f& GetModelviewprojectMatrix();
    const M3DMatrix44f& GetModeviewMatrix();
    const M3DMatrix33f& GetNormalMatrix();
};

#endif // TOOLS__



