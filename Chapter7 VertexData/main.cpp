#include "../Tools/Header/Tools.h"
#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Camera.h"

BaseShaderParam param;
ShaderMgr* shaderMgr;
GLShaderManager glShaderMgr;

GLBatch rectangle;
GLMatrixStack modelStack;
NormalCamera normalCamera;
float color[] = {0.5f,0,0,1};

GLuint vao;
GLuint vbo;

GLuint _vbo;
GLuint ebo;
float vertexs[] = {
    1,0,0, 1,0,0, 0,0,1,//位置——颜色——法线
    2,0,0, 0,1,0, 0,0,1,
    1,1,0, 0,0,1, 0,0,1
};

float vertex_data[] = {
    2,0,0, 1,0,0, //位置_颜色
    3,0,0, 0,1,0,
    3,1,0, 0,0,1,
    2,1,0, 0,1,0,
};
unsigned int vertex_elem[] = {
    0,1,2, 0,2,3
};

static void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,1);

    param.SetDiffuseColor(color);
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(modelStack));
    shaderMgr->UseDiffuse(param);
    rectangle.Draw();

    glBindVertexArray(vao);
    //使用vao+vbo
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    //使用vao+vbo+ebo
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glutSwapBuffers();
}

static void Idle(void)
{
    glutPostRedisplay();
}

void InitVBO()
{
    //生成vbo
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

void InitEBO()
{
    //ebo+vbo
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    //顶点位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //顶点颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_elem), vertex_elem, GL_STATIC_DRAW);
}

void OnShutup()
{
    normalCamera.OnUnInit();
    shaderMgr->OnUnInit();
}

void OnStartup()
{
    shaderMgr = ShaderMgr::GetInstance();
    glShaderMgr.InitializeStockShaders();

    normalCamera.OnInit(640, 480, 50, 2, 1);
    modelStack.Translate(0,0,-6);

    //init rectangle
    rectangle.Begin(GL_TRIANGLES, 3, 1);
    rectangle.Color4f(1,0,0,1);
    rectangle.Vertex3f(0,0,0);
    rectangle.Color4f(0,1,0,1);
    rectangle.Vertex3f(1,0,0);
    rectangle.Color4f(0,0,1,1);
    rectangle.Vertex3f(0,1,0);
    rectangle.End();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //InitVBO();
    InitEBO();
}

void KeyboardFn(unsigned char key, int x, int y){normalCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y){normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y){normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h){normalCamera.Resize(w, h);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("vbo");

    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);

    if (glewInit() != GLEW_OK)
    {
        printf("glew init fail...\n");
        return 0;
    }

    OnStartup();
    glutMainLoop();
    OnShutup();

    return EXIT_SUCCESS;
}
