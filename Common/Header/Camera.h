#ifndef CAMERAHEADER
#define CAMERAHEADER

#include "../Header/Tools.h"
#include "../Header/GameObject.h"
#include "../Header/ShaderMgr.h"
#include "../Header/Lib/FreetypeFont.h"

enum MouseMotion
{
    MMNone,
    MMLeftMousePress,
    MMMedMousePress,
    MMRightMousePress,
};

enum ECameraObject
{
    COLine,
    COBox,
    COMax,
};
class BaseCamera
{
protected:
    GLfloat width,height,nearCamera,farCamera;
    BaseShaderParam param;
    BatchGObject gObject[(int)COMax];
    CharText charTex;
protected:
    GLGeometryTransform transformPiple;
    GLMatrixStack projectStack;
    GLFrustum frustum;
    GLFrame frame;
public:
    void ResetFrame();
    void DrawBox(const M3DMatrix44f modelMatrix,const M3DMatrix44f viewMatrix, const M3DMatrix44f projectMatrix);
    void DrawLine(const M3DMatrix44f modelMatrix,const M3DMatrix44f viewMatrix, const M3DMatrix44f projectMatrix);
    void ShowInfo(float x, float y);

    void Rotate(float angle, float x, float y, float z);
    void Translate(float x, float y, float z);
    void RefreshProjectStack();

    const M3DMatrix44f& GetProjectMatrix();
    const M3DMatrix44f& GetViewMatrix();
    const M3DMatrix44f& GetModelviewMatrix(GLMatrixStack& modelStack);
    const M3DMatrix44f& GetModelviewprojectMatrix(GLMatrixStack& modelStack);
    const M3DMatrix33f& GetNormalMatrix(GLMatrixStack& modelStack);
    GLMatrixStack& GetModeviewStack();

    virtual void RebuildGObject(){}
    virtual void Resize(int w, int h){}
    virtual void OnUnInit();
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
    virtual void OnUnInit();
    void OnInit(float w, float h, float fov, float moveSp, float roateSp);
    virtual void Resize(int w, int h);
    virtual void RebuildGObject();
    void BuildPlaneVertex(float ditance, float fov, M3DVector3f& p1, M3DVector3f& p2, M3DVector3f& p3, M3DVector3f& p4, M3DVector3f& origin, M3DVector3f& foward, M3DVector3f& right, M3DVector3f& up);
};

class UICamera : public BaseCamera
{
public:
    virtual void OnUnInit();
    void OnInit(float w, float h);
    virtual void Resize(int w, int h);
    virtual void RebuildGObject();
};

#endif
