#ifndef CAMERAHEADER
#define CAMERAHEADER

#include "../Header/Tools.h"
#include "../Header/GameObject.h"
#include "../Header/ShaderMgr.h"

enum MouseMotion
{
    MMNone,
    MMLeftMousePress,
    MMMedMousePress,
    MMRightMousePress,
};

class BaseCamera
{
protected:
    GLfloat width,height,nearCamera,farCamera;
    BaseShaderParam param;
    BatchGObject gObject;
protected:
    GLGeometryTransform transformPiple;
    GLMatrixStack projectStack;
    GLFrustum frustum;
    GLFrame frame;
public:
    void ResetFrame();
    void Draw(const M3DMatrix44f mvMatrix, const M3DMatrix44f projectMatrix, const M3DMatrix33f normalMatrix);
    void Rotate(float angle, float x, float y, float z);
    void Translate(float x, float y, float z);
    void RefreshProjectStack();

    const M3DMatrix44f& GetProjectMatrix();
    const M3DMatrix44f& GetModelviewprojectMatrix(GLMatrixStack& modelviewStack);
    const M3DMatrix33f& GetNormalMatrix(GLMatrixStack& modelviewStack);
    GLMatrixStack& GetModeviewStack();

    virtual void RebuildGObject(){}
    virtual void Resize(int w, int h){}

protected:
    float moveSpeed = 1;
    float roateSpeed = 2;
    int prevMouseX, prevMouseY;
    MouseMotion mouseMotion;
public:
    void KeyboardFn(unsigned char key, int x, int y);
    void MouseClick(int button, int action, int x, int y);
    void MotionFunc(int mouse_x, int mouse_y);
};

class NormalCamera : public BaseCamera
{
private:
    float fov;
public:
    void OnUnInit();
    void OnInit(float w, float h, float fov, float moveSp, float roateSp);

    virtual void Resize(int w, int h);
    virtual void RebuildGObject();
};

class UICamera : public BaseCamera
{
public:
    void OnInit(float w, float h);
    void OnUnInit();
    virtual void Resize(int w, int h);
    virtual void RebuildGObject();
};

#endif
