#include "../Tools/Header/Tools.h"
#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/GameObject.h"

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;

const int MaxStarNum = 30;
BatchGObject points[MaxStarNum];
M3DVector2f pointsSize[MaxStarNum];

GLuint texture;
char* texturePath = "Chapter6 SpritePoint/Texture/star.tga";
float color[] = {1,1,1,1};

static void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);

    glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_POINT_SPRITE_ARB);
    glEnable(GL_PROGRAM_POINT_SIZE);

    for (int i = 0; i < MaxStarNum; i++)
    {
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(points[i].modelviewStack));
        param.colorMap[0] = 0;
        pointsSize[i][0]--;
        if (pointsSize[i][0] <= 0)
            pointsSize[i][0] = pointsSize[i][1];
        shaderMgr.UseSpritePoint(param, pointsSize[i][0]);
        points[i].Draw();
    }

    glDisable(GL_POINT_SPRITE_ARB);
    glutSwapBuffers();
    glutPostRedisplay();
}

static void Idle(void)
{
    glutPostRedisplay();
}

void OnStartup()
{
    //init matrix
    normalCamera.OnInit(640, 480, 50, 2, 1);
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();

    //init points
    for (int i = 0; i < MaxStarNum; i++)
    {
        points[i].modelviewStack.Translate(-2,-2,-12);
        points[i].batch.Begin(GL_POINTS, 1, 1);
        //point position
        M3DVector3f vVertex;
        vVertex[0] = (float)(rand()%6);
        vVertex[1] = (float)(rand()%6);
        vVertex[2] = (float)(rand()%6);
        //point color
        M3DVector4f vColor;
        vColor[0] = rand()%10/10.0f;
        vColor[1] = rand()%10/10.0f;
        vColor[2] = rand()%10/10.0f;
        vColor[3] = 1;

        points[i].batch.Color4fv(vColor);
        points[i].batch.Vertex3fv(vVertex);
        points[i].batch.End();
        // point size
        float size = rand()%200;
        pointsSize[i][0] = size;
        pointsSize[i][1] = size;
        //printf("%f %f %f\n", vColor[0],vColor[1],vColor[2]);
    }

    //init tetxure
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    Util::LoadTGATexture(texturePath, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

void OnShutup()
{
    glDeleteTextures(1,&texture);
    shaderMgr.OnUnInit();
    normalCamera.OnUnInit();
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
    glutCreateWindow("sprite point");

    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    // set point size
    //glPointSize(20);

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
