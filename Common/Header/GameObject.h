#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "Tools.h"


class Action
{
public:
    virtual void Run(GLMatrixStack* modelStack){}
};

class YoyoMoveAction: public Action
{
private:
    float _speed;
    M3DVector3f _pos;
    M3DVector3f _dir;
    M3DVector3f _offset;
public:

    YoyoMoveAction(float speed, M3DVector3f offset);
    virtual void Run(GLMatrixStack* modelStack);
};

class SequenceAction : public Action
{
private:
    std::list<Action> _children;
public:
    void AddAction(Action action){_children.push_back(action);}
    virtual void Run(GLMatrixStack* modelStack);
};


class GObject
{
private:
    Action* _action;
public:
    GObject();
    GLMatrixStack modelStack;
    virtual void Draw(){printf("draw in object \n");}

    virtual void Translate(float x, float y, float z){modelStack.Translate(x, y, z);}
    // angleÊÇ¶ÈÊý
    virtual void Rotation(float angle, float x, float y, float z){modelStack.Rotate(angle, x, y, z);}
    virtual void Scale(float x, float y, float z){modelStack.Scale(x, y ,z);}
    virtual void SetAction(Action* action){_action = action;}
    void RunAction();
    void GetMatrix(M3DMatrix44f matrix){modelStack.GetMatrix(matrix);}
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
