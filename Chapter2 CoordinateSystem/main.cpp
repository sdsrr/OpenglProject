#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Camera.h"
#include "../Tools/Header/UtilPrint.h"

BaseShaderParam param;
ShaderMgr* shaderMgr;
GLShaderManager shaderMgr_;
GLBatch batch;
GLFrustum frustum;
M3DVector3f cameraPosition={0,0,2};
float angle = 0;
M3DMatrix44f modelMatrix;
M3DMatrix44f viewMatrix;

//使用管线方式
GLMatrixStack modelMatrixStack;
GLMatrixStack projectMatrixStack;
GLGeometryTransform transformPipeline;

static void Resize(int width, int height)
{
    glViewport(0, 0, width, height);
    frustum.SetPerspective(60, width/height, 1, 100);
    projectMatrixStack.LoadMatrix(frustum.GetProjectionMatrix());
    //frustum.SetOrthographic(-1, 1, -1 ,1, -1, 1);
    glutSwapBuffers();
}

static void Key(unsigned char key, int x, int y)
{
    //移动相机
    float speed = 0.5f;
    switch (key)
    {
        std::cout<<key<<"\n";
    case 'a':
        cameraPosition[0] -= speed;
        modelMatrixStack.Translate(speed,0,0);
        break;
    case 'd':
        cameraPosition[0] += speed;
        modelMatrixStack.Translate(-speed,0,0);
        break;
    case 'w':
        cameraPosition[1] += speed;
        modelMatrixStack.Translate(0,-speed,0);
        break;
    case 's':
        cameraPosition[1] -= speed;
        modelMatrixStack.Translate(0,speed,0);
        break;
    }
    //移动相机
    m3dTranslationMatrix44(viewMatrix, -cameraPosition[0], -cameraPosition[1], -cameraPosition[2]);

    UtilPrint::PrintMatrix44f(modelMatrixStack.GetMatrix());
    glutPostRedisplay();
}

static void Idle(void)
{
    glutPostRedisplay();
}

GLfloat vRed[] = {1.0f, 0.0f, 0.0f ,0.0f};
M3DVector4f color = {0.0f, 0.5f ,1.0f ,1.0f};
float lastTime = 0;

//自己构造变换矩阵
static void Display_normal()
{
    const double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    float delta = time - lastTime;
    lastTime = time;

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //旋转三角形
    angle += delta*30;
    //移动以相对坐标移动
    m3dTranslationMatrix44(modelMatrix, -5, 0, 0);
    //旋转以自身为中心旋转
    //先移动再旋转,否则效果是绕世界坐标原点旋转
    m3dRotationMatrix44(modelMatrix, angle, 0, 1, 0);

    param.SetModelMatrix(modelMatrix);
    param.SetViewMatrix(viewMatrix);
    param.SetProjectMatrix(frustum.GetProjectionMatrix());
    param.SetLightDirection(0,0,1);
    param.SetDiffuseColor(color);
    shaderMgr->UseDiffuse(param);
    //shaderMgr_.UseStockShader(GLT_SHADER_FLAT, mvpMatrix, vRed);
    batch.Draw();
    glutSwapBuffers();
}

//draw triangle
static void Display_transformpiple()
{
    const double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    float delta = time - lastTime;
    lastTime = time;

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //roate triangle
    angle += delta*30;

    modelMatrixStack.PushMatrix();
    modelMatrixStack.Translate(0,0,-5);
    modelMatrixStack.Rotate(angle, 0, 1, 0);

    const float model[4][4]=
    {
		1.0f, 0.0f, 0.0f, -5,
		0.0f, 1.0f, 0.0f, -1,
		0.0f, 0.0f, 1.0f, 0.0f,
		0, 0, 0, 0,
    };

    //param.SetModelMatrix(model);
    param.SetModelMatrix(modelMatrixStack.GetMatrix());
    param.SetViewMatrix(viewMatrix);
    param.SetProjectMatrix(frustum.GetProjectionMatrix());
    param.SetDiffuseColor(color);
    param.SetLightDirection(0,0,1);
    shaderMgr->UseDiffuse(param);
    modelMatrixStack.PopMatrix();

    batch.Draw();
    glutSwapBuffers();
}


void OnStartUp()
{
    //初始化为单位矩阵
    m3dLoadIdentity44(modelMatrix);
    m3dLoadIdentity44(viewMatrix);

    //绑定矩阵堆栈与几何变换管线
    transformPipeline.SetMatrixStacks(modelMatrixStack, projectMatrixStack);

    //初始相机
    //frustum.SetOrthographic(-1, 1, -1 ,1, -1, 1);
    frustum.SetPerspective(60, 640/480.0f, 1, 100);
    projectMatrixStack.LoadMatrix(frustum.GetProjectionMatrix());

    shaderMgr = ShaderMgr::GetInstance();
    shaderMgr_.InitializeStockShaders();
    batch.Begin(GL_TRIANGLES, 3);
    batch.Color4f(1,0,0,1);
    batch.Normal3f(0,0,1);
    batch.Vertex3f(-0.5f,0.0f,0.0f);
    batch.Color4f(0,1,0,1);
    batch.Normal3f(0,0,1);
    batch.Vertex3f(0.5f,0.0f,0.0f);
    batch.Color4f(0,0,1,1);
    batch.Normal3f(0,0,1);
    batch.Vertex3f(0.0f,0.5f,0.0f);
    batch.End();
}

void OnShutUp()
{
    shaderMgr->OnUnInit();
}


//使用正交投影相机渲染正方体，使用键盘切换视角
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("coordinate system");

    //回调函数
    glutReshapeFunc(Resize);
    //glutDisplayFunc(Display_normal);
    glutDisplayFunc(Display_transformpiple);
    glutKeyboardFunc(Key);
    glutIdleFunc(Idle);
    //glEnable(GL_CULL_FACE);

    if (GLEW_OK != glewInit())
    {
		fprintf(stderr, "GLEW Error: \n");
		return 1;
    }

    OnStartUp();
    glutMainLoop();
    OnShutUp();

    return EXIT_SUCCESS;
}
