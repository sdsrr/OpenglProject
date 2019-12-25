#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "../Header/Tools.h"

class GObject
{
public:
    GObject();
    GLMatrixStack modelStack;
    virtual void Draw(){printf("draw in Object \n");}
};

class BatchGObject : public GObject
{
public:
    BatchGObject();
    GLBatch batch;
    void CreateCube(float length, float width, float height, int textureUnit);
    void CreateTriangle(float length, float width, int textureUnit);
    virtual void Draw();
};

class TriangleGObject : public GObject
{
public:
    TriangleGObject();
    GLTriangleBatch batch;
    virtual void Draw();
};

class TriangleStripGObject : public GObject
{
    int vertexCnt;
public:
    TriangleStripGObject();
    TriangleStripGObject(int count);
    GLuint vao;
    GLuint vbo;
    virtual void Draw();
};

#endif // GAMEOBJECT
