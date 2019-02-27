#include "../Tools/Header/Tools.h"
#include "../Tools/Header/ShaderMgr.h"

ShaderMgr shaderMgr;
GLShaderManager glShaderMgr;

GLFrustum frustum;
GLMatrixStack modelviewStack;
GLMatrixStack projectStack;
GLGeometryTransform transformPiple;

GLBatch rectangle;
float color[] = {0.5f,0,0,1};

GLuint vbo;
float vertexs[] = {1,0,0, 1,0,0, 0,0,1,//位置——颜色——法线
                   2,0,0, 0,1,0, 0,0,1,
                   1,1,0, 0,0,1, 0,0,1};

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    frustum.SetPerspective(50, width/height, 0.1f, 100);
    glutPostRedisplay();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    shaderMgr.UseDiffuse(color, transformPiple.GetModelViewProjectionMatrix());
    rectangle.Draw();


    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    Util::executeKeyFn(key, x, y, modelviewStack);
}

static void idle(void)
{
    glutPostRedisplay();
}

void onStartup()
{
    shaderMgr.OnInit();
    glShaderMgr.InitializeStockShaders();

    frustum.SetPerspective(50, 640/480, 0.1f, 100);
    transformPiple.SetMatrixStacks(modelviewStack, projectStack);
    projectStack.LoadMatrix(frustum.GetProjectionMatrix());
    modelviewStack.Translate(0,0,-6);

    //init rectangle
    rectangle.Begin(GL_TRIANGLES, 3, 1);
    rectangle.Color4f(1,0,0,1);
    rectangle.Vertex3f(0,0,0);
    rectangle.Color4f(0,1,0,1);
    rectangle.Vertex3f(1,0,0);
    rectangle.Color4f(0,0,1,1);
    rectangle.Vertex3f(0,1,0);
    rectangle.End();

    glGenBuffers(1, & vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
    //设置顶点位置
    //shader中location对应序号，数据类型长度，数据类型类型(这里是vec3 = 3个float), 步长(下一组顶点数据间隔长度，中间隔了3个float颜色，3个float法线), 起始位置偏移
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //设置顶点颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    //设置顶点法线
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(2);
}

void onShutup()
{
    shaderMgr.OnUnInit();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("vbo");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    if (glewInit() != GLEW_OK)
    {
        printf("glew init fail...\n");
        return 0;
    }

    onStartup();
    glutMainLoop();
    onShutup();

    return EXIT_SUCCESS;
}
