#include "../Header/Camera.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BaseCamera::DrawBox(const M3DMatrix44f mvMatrix, const M3DMatrix44f projectMatrix,const M3DMatrix33f normalMatrix)
{
    ShaderMgr* shaderMgr = ShaderMgr::GetInstance();
    param.SetDiffuseColor(ShaderMgr::red);
    param.SetMVMatrix(mvMatrix);
    param.SetProjectMatrix(projectMatrix);
    param.SetNormalMatrix(normalMatrix);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    shaderMgr->UseCameraLine(param);
    gObject[(int)COBox].Draw();
    glDisable(GL_CULL_FACE);

    shaderMgr->UseCameraBox(param);
    gObject[(int)COBox].Draw();
}

void BaseCamera::DrawLine(const M3DMatrix44f mvpMatrix)
{
    ShaderMgr* shaderMgr = ShaderMgr::GetInstance();
    param.SetDiffuseColor(ShaderMgr::red);
    param.SetMVPMatrix(mvpMatrix);
    shaderMgr->UseSolidColor(param);
    gObject[(int)COLine].Draw();
}

void BaseCamera::ResetFrame()
{
    frame.SetOrigin(0,0,0);
    frame.SetForwardVector(0,0,-1);
    frame.SetUpVector(0,1,0);
}

void BaseCamera::Rotate(float angle, float x, float y, float z)
{
    frame.RotateWorld(m3dDegToRad(-angle), x, y, z);
    projectStack.Rotate(angle,x,y,z);
}

void BaseCamera::Translate(float x, float y, float z)
{
    frame.TranslateWorld(-x, -y, -z);
    projectStack.Translate(x,y,z);
}

GLMatrixStack& BaseCamera::GetModeviewStack()
{
    static GLMatrixStack matrixStack;
    matrixStack.LoadMatrix(frame);
    //printf("modeview\n");
    //Util::PrintMatrix44f(matrixStack.GetMatrix());
    return matrixStack;
}

const M3DMatrix44f& BaseCamera::GetProjectMatrix()
{
    const M3DMatrix44f& matrix = projectStack.GetMatrix();
    return matrix;
}

const M3DMatrix44f& BaseCamera::GetModelviewprojectMatrix(GLMatrixStack& modelviewStack)
{
    transformPiple.SetModelViewMatrixStack(modelviewStack);
    const M3DMatrix44f& matrix = transformPiple.GetModelViewProjectionMatrix();
    return matrix;
}

const M3DMatrix33f& BaseCamera::GetNormalMatrix(GLMatrixStack& modelviewStack)
{
    transformPiple.SetModelViewMatrixStack(modelviewStack);
    const M3DMatrix33f& matrix = transformPiple.GetNormalMatrix(true);
    return matrix;
}


void BaseCamera::MouseClick(int button, int action, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (action == GLUT_DOWN) {
            mouseMotion=MMLeftMousePress;
        }
        else if (action == GLUT_UP) {
            mouseMotion = MMNone;
        }
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (action == GLUT_DOWN) {
            mouseMotion = MMRightMousePress;
        }
        else if (action == GLUT_UP) {
            mouseMotion = MMNone;
        }
    }
    if (button == GLUT_MIDDLE_BUTTON)
    {
        if (action == GLUT_DOWN) {
            mouseMotion = MMMedMousePress;
        }
        else if (action == GLUT_UP) {
            mouseMotion = MMNone;
        }
    }
    prevMouseX = x;
    prevMouseY = y;
}

void BaseCamera::KeyboardFn(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
        Translate(moveSpeed,0,0);
        break;
    case 'd':
        Translate(-moveSpeed,0,0);
        break;
    case 'w':
        Translate(0,-moveSpeed,0);
        break;
    case 's':
        Translate(0,moveSpeed,0);
        break;
    case 'q':
        Translate(0,0,-moveSpeed);
        break;
    case 'e':
        Translate(0,0,moveSpeed);
        break;
    }

    //M3DVector3f ma;
    //camera.GetOrigin(ma);
    //printf("%f %f %f\n",ma[0], ma[1], ma[2]);
    glutPostRedisplay();
}

void BaseCamera::MotionFunc(int mouse_x, int mouse_y)
{
    float deltaX = mouse_x - prevMouseX;
    float deltaY = mouse_y - prevMouseY;
    if (mouseMotion == MMRightMousePress)
    {
        if (abs(deltaX) >= abs(deltaY))
            deltaY = 0;
        else
            deltaX = 0;
        if (deltaX != 0)
            Rotate(deltaX/roateSpeed, 0, 1, 0);
        else
            Rotate(deltaY/roateSpeed, 1, 0, 0);
    }
    //printf("%d  %d\n", mouse_x, mouse_y);
    prevMouseX = mouse_x;
    prevMouseY = mouse_y;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NormalCamera::OnInit(float w, float h, float fov, float moveSp, float roateSp)
{
    this->nearCamera = 0.1f;
    this->farCamera = 1000;
    this->fov = fov;
    this->moveSpeed = moveSp;
    this->roateSpeed = roateSp;
    Resize(w, h);
    RebuildGObject();
}

void NormalCamera::Resize(int w, int h)
{
    this->width = w;
    this->height = h;
    glViewport(0, 0, w, h);
    frustum.SetPerspective(this->fov, width/height, nearCamera, farCamera);
    transformPiple.SetProjectionMatrixStack(projectStack);
    projectStack.LoadMatrix(frustum.GetProjectionMatrix());
    ResetFrame();
    RebuildGObject();
    glutPostRedisplay();
}

void NormalCamera::OnUnInit()
{

}

void NormalCamera::RebuildGObject()
{
    GLFrame camera;
    M3DVector3f origin,foward,up,right;
    frustum.Transform(camera);
    camera.GetOrigin(origin);
    camera.GetZAxis(foward);
    camera.GetYAxis(up);
    camera.GetXAxis(right);

    m3dNormalizeVector3(foward);
    m3dNormalizeVector3(up);
    m3dNormalizeVector3(right);

    M3DVector3f p1,p2,p3,p4, p5,p6,p7,p8;
    BuildPlaneVertex(nearCamera, fov/2, p1, p2, p3, p4, origin, foward, right, up);
    BuildPlaneVertex(farCamera, fov/2, p5, p6, p7, p8, origin, foward, right, up);

    {
        GLBatch& batch = gObject[(int)COLine].batch;
        batch.Reset();
        batch.Begin(GL_LINES, 24, 0);

        batch.Vertex3fv(p1);
        batch.Vertex3fv(p2);
        batch.Vertex3fv(p2);
        batch.Vertex3fv(p3);
        batch.Vertex3fv(p3);
        batch.Vertex3fv(p4);
        batch.Vertex3fv(p4);
        batch.Vertex3fv(p1);

        batch.Vertex3fv(origin);
        batch.Vertex3fv(p5);
        batch.Vertex3fv(origin);
        batch.Vertex3fv(p6);
        batch.Vertex3fv(origin);
        batch.Vertex3fv(p7);
        batch.Vertex3fv(origin);
        batch.Vertex3fv(p8);

        batch.Vertex3fv(p5);
        batch.Vertex3fv(p6);
        batch.Vertex3fv(p6);
        batch.Vertex3fv(p7);
        batch.Vertex3fv(p7);
        batch.Vertex3fv(p8);
        batch.Vertex3fv(p8);
        batch.Vertex3fv(p5);
        batch.End();
    }

}

void NormalCamera::BuildPlaneVertex(float ditance, float fov, M3DVector3f& p1, M3DVector3f& p2, M3DVector3f& p3, M3DVector3f& p4, M3DVector3f& origin, M3DVector3f& foward, M3DVector3f& right, M3DVector3f& up)
{
    float hlafHeight = ditance * tan(m3dDegToRad(fov));
    float halfWidth = hlafHeight * width / height;

    M3DVector3f planeOrigin,rightMed,leftMed;
    Util::MoveVector(planeOrigin, origin, foward, ditance);

    Util::MoveVector(rightMed, planeOrigin, right, halfWidth);
    Util::MoveVector(p1, rightMed, up, hlafHeight);
    Util::MoveVector(p2, rightMed, up, -hlafHeight);
    Util::MoveVector(leftMed, planeOrigin, right, -halfWidth);
    Util::MoveVector(p3, leftMed, up, -hlafHeight);
    Util::MoveVector(p4, leftMed, up, hlafHeight);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UICamera::OnInit(float w, float h)
{
    this->nearCamera = -100;
    this->farCamera = 100;
    Resize(w, h);
    RebuildGObject();
}

void UICamera::OnUnInit()
{

}

void UICamera::Resize(int w, int h)
{
    this->width = w;
    this->height = h;
    glViewport(0, 0, w, h);
    frustum.SetOrthographic(-w/2.0, w/2.0, -h/2.0, h/2.0, nearCamera, farCamera);
    transformPiple.SetProjectionMatrixStack(projectStack);
    projectStack.LoadMatrix(frustum.GetProjectionMatrix());
    ResetFrame();
    glutPostRedisplay();
}

void UICamera::RebuildGObject()
{
    GLFrame camera;
    M3DVector3f origin,foward,up,right;
    frustum.Transform(camera);
    camera.GetOrigin(origin);
    camera.GetZAxis(foward);
    camera.GetYAxis(up);
    camera.GetXAxis(right);

    m3dNormalizeVector3(foward);
    m3dNormalizeVector3(up);
    m3dNormalizeVector3(right);

    M3DVector3f p1,p2,p3,p4,p5,p6,p7,p8;
    M3DVector3f rightMed,leftMed;

    //vertex
    Util::MoveVector(rightMed, origin, right, width/2);
    Util::MoveVector(p1, rightMed, up, height/2);
    Util::MoveVector(p2, rightMed, up, -height/2);

    Util::MoveVector(leftMed, origin, right, -width/2);
    Util::MoveVector(p3, leftMed, up, -height/2);
    Util::MoveVector(p4, leftMed, up, height/2);

    Util::MoveVector(p5, p1, foward, farCamera);
    Util::MoveVector(p6, p2, foward, farCamera);
    Util::MoveVector(p7, p3, foward, farCamera);
    Util::MoveVector(p8, p4, foward, farCamera);

    Util::MoveVector(p1, p1, foward, nearCamera);
    Util::MoveVector(p2, p2, foward, nearCamera);
    Util::MoveVector(p3, p3, foward, nearCamera);
    Util::MoveVector(p4, p4, foward, nearCamera);

    //normal
    M3DVector3f normalP1,normalP2,normalP3,normalP4,normalP5,normalP6,normalP7,normalP8;
    Util::CalcNormal(normalP1, p1, p2, p4, p5);
    Util::CalcNormal(normalP2, p2, p1, p3, p6);
    Util::CalcNormal(normalP3, p3, p2, p4, p7);
    Util::CalcNormal(normalP4, p4, p1, p3, p8);

    Util::CalcNormal(normalP5, p5, p1, p6, p8);
    Util::CalcNormal(normalP6, p6, p2, p5, p7);
    Util::CalcNormal(normalP7, p7, p3, p6, p8);
    Util::CalcNormal(normalP8, p8, p7, p4, p5);


    {
        //set data
        glLineWidth(2);
        GLBatch& batch = gObject[(int)COBox].batch;
        batch.Reset();
        batch.Begin(GL_TRIANGLES, 36, 1);
        //front
        batch.Normal3fv(normalP3);
        batch.Vertex3fv(p3);
        batch.Normal3fv(normalP2);
        batch.Vertex3fv(p2);
        batch.Normal3fv(normalP1);
        batch.Vertex3fv(p1);
        batch.Normal3fv(normalP3);
        batch.Vertex3fv(p3);
        batch.Normal3fv(normalP1);
        batch.Vertex3fv(p1);
        batch.Normal3fv(normalP4);
        batch.Vertex3fv(p4);
        //back
        batch.Normal3fv(normalP5);
        batch.Vertex3fv(p5);
        batch.Normal3fv(normalP6);
        batch.Vertex3fv(p6);
        batch.Normal3fv(normalP7);
        batch.Vertex3fv(p7);
        batch.Normal3fv(normalP5);
        batch.Vertex3fv(p5);
        batch.Normal3fv(normalP7);
        batch.Vertex3fv(p7);
        batch.Normal3fv(normalP8);
        batch.Vertex3fv(p8);

        //right
        batch.Normal3fv(normalP3);
        batch.Vertex3fv(p3);
        batch.Normal3fv(normalP4);
        batch.Vertex3fv(p4);
        batch.Normal3fv(normalP8);
        batch.Vertex3fv(p8);
        batch.Normal3fv(normalP3);
        batch.Vertex3fv(p3);
        batch.Normal3fv(normalP8);
        batch.Vertex3fv(p8);
        batch.Normal3fv(normalP7);
        batch.Vertex3fv(p7);

        //button
        batch.Normal3fv(normalP6);
        batch.Vertex3fv(p6);
        batch.Normal3fv(normalP2);
        batch.Vertex3fv(p2);
        batch.Normal3fv(normalP3);
        batch.Vertex3fv(p3);
        batch.Normal3fv(normalP6);
        batch.Vertex3fv(p6);
        batch.Normal3fv(normalP3);
        batch.Vertex3fv(p3);
        batch.Normal3fv(normalP7);
        batch.Vertex3fv(p7);

        //left
        batch.Normal3fv(normalP1);
        batch.Vertex3fv(p1);
        batch.Normal3fv(normalP2);
        batch.Vertex3fv(p2);
        batch.Normal3fv(normalP6);
        batch.Vertex3fv(p6);
        batch.Normal3fv(normalP1);
        batch.Vertex3fv(p1);
        batch.Normal3fv(normalP6);
        batch.Vertex3fv(p6);
        batch.Normal3fv(normalP5);
        batch.Vertex3fv(p5);

        //top
        batch.Normal3fv(normalP4);
        batch.Vertex3fv(p4);
        batch.Normal3fv(normalP1);
        batch.Vertex3fv(p1);
        batch.Normal3fv(normalP5);
        batch.Vertex3fv(p5);
        batch.Normal3fv(normalP4);
        batch.Vertex3fv(p4);
        batch.Normal3fv(normalP5);
        batch.Vertex3fv(p5);
        batch.Normal3fv(normalP8);
        batch.Vertex3fv(p8);
        batch.End();
    }

    {
        GLBatch& batch = gObject[(int)COLine].batch;
        batch.Reset();
        batch.Begin(GL_LINES, 36, 0);
        batch.Vertex3fv(p1);
        batch.Vertex3fv(p2);
        batch.Vertex3fv(p2);
        batch.Vertex3fv(p3);
        batch.Vertex3fv(p3);
        batch.Vertex3fv(p4);
        batch.Vertex3fv(p4);
        batch.Vertex3fv(p1);

        batch.Vertex3fv(p1);
        batch.Vertex3fv(p5);
        batch.Vertex3fv(p2);
        batch.Vertex3fv(p6);
        batch.Vertex3fv(p3);
        batch.Vertex3fv(p7);
        batch.Vertex3fv(p4);
        batch.Vertex3fv(p8);

        batch.Vertex3fv(p5);
        batch.Vertex3fv(p6);
        batch.Vertex3fv(p6);
        batch.Vertex3fv(p7);
        batch.Vertex3fv(p7);
        batch.Vertex3fv(p8);
        batch.Vertex3fv(p8);
        batch.Vertex3fv(p5);
        batch.End();
    }

}
