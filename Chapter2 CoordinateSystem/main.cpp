#include "../Tools/Header/ShaderMgr.h"

ShaderMgr shaderMgr;
GLShaderManager shaderMgr_;
GLBatch batch;
GLFrustum frustum;
M3DVector3f cameraPosition={0,0,2};
float angle = 0;
M3DMatrix44f modelMatrix;
M3DMatrix44f viewMatrix;

//ʹ�ù��߷�ʽ
GLMatrixStack modelviewMatrixStack;
GLMatrixStack projectMatrixStack;
GLGeometryTransform transformPipeline;

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    frustum.SetPerspective(60, width/height, 1, 100);
    projectMatrixStack.LoadMatrix(frustum.GetProjectionMatrix());
    //frustum.SetOrthographic(-1, 1, -1 ,1, -1, 1);
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    //�ƶ����
    float speed = 0.5f;
    switch (key)
    {
        std::cout<<key<<"\n";
    case 'a':
        cameraPosition[0] -= speed;
        modelviewMatrixStack.Translate(speed,0,0);
        break;
    case 'd':
        cameraPosition[0] += speed;
        modelviewMatrixStack.Translate(-speed,0,0);
        break;
    case 'w':
        cameraPosition[1] += speed;
        modelviewMatrixStack.Translate(0,-speed,0);
        break;
    case 's':
        cameraPosition[1] -= speed;
        modelviewMatrixStack.Translate(0,speed,0);
        break;
    }
    //�ƶ����
    m3dTranslationMatrix44(viewMatrix, -cameraPosition[0], -cameraPosition[1], -cameraPosition[2]);

    Util::printmatrix44f(modelviewMatrixStack.GetMatrix());
    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

GLfloat vertexs[] = {-0.5f,0.0f,0.0f, 0.5f,0.0f,0.0f, 0.0f,0.5f,0.0f};
GLfloat vRed[] = {1.0f, 0.0f, 0.0f ,0.0f};
M3DVector4f color = {0.0f, 0.5f ,1.0f ,1.0f};
float lastTime = 0;

//�Լ�����任����
static void display_normal()
{
    const double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    float delta = time - lastTime;
    lastTime = time;

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //��ת������
    angle += delta*30;
    m3dRotationMatrix44(modelMatrix, angle, 0, 1, 0);

    M3DMatrix44f mvMatrix;
    m3dMatrixMultiply44(mvMatrix, viewMatrix, modelMatrix);

    M3DMatrix44f mvpMatrix;
    m3dMatrixMultiply44(mvpMatrix, frustum.GetProjectionMatrix(), mvMatrix);
    shaderMgr.UseDiffuse(color, mvpMatrix);
    //shaderMgr_.UseStockShader(GLT_SHADER_FLAT, mvpMatrix, vRed);
    batch.Draw();
    glutSwapBuffers();
}

//ʹ�ü��α任����
static void display_transformpiple()
{
    const double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    float delta = time - lastTime;
    lastTime = time;

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //��ת������
    angle += delta*30;

    modelviewMatrixStack.PushMatrix();
    modelviewMatrixStack.Rotate(angle, 0, 1, 0);
    shaderMgr.UseDiffuse(color, transformPipeline.GetModelViewProjectionMatrix());
    modelviewMatrixStack.PopMatrix();

    batch.Draw();
    glutSwapBuffers();
}


void OnStartUp()
{
    //��ʼ��Ϊ��λ����
    m3dLoadIdentity44(modelMatrix);
    m3dLoadIdentity44(viewMatrix);

    //�󶨾����ջ�뼸�α任����
    transformPipeline.SetMatrixStacks(modelviewMatrixStack, projectMatrixStack);

    //��ʼ���
    //frustum.SetOrthographic(-1, 1, -1 ,1, -1, 1);
    frustum.SetPerspective(60, 1, 1, 100);
    projectMatrixStack.LoadMatrix(frustum.GetProjectionMatrix());
    modelviewMatrixStack.Translate(0,0,-3);

    shaderMgr.OnInit();
    shaderMgr_.InitializeStockShaders();
    batch.Begin(GL_TRIANGLES, 3);
    batch.CopyVertexData3f(vertexs);
    batch.End();
}

void OnShutUp()
{
    shaderMgr.OnUnInit();
}


//ʹ������ͶӰ�����Ⱦ�����壬ʹ�ü����л��ӽ�
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("coordinate system");

    //�ص�����
    glutReshapeFunc(resize);
    //glutDisplayFunc(display_normal);
    glutDisplayFunc(display_transformpiple);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

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