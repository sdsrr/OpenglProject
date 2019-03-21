#include "../Tools/Header/Tools.h"
#include "../Tools/Header/ShaderMgr.h"


GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;

GLFrustum frustum;
GLMatrixStack modelviewStack;
GLMatrixStack projectStack;
GLGeometryTransform transformPiple;

const int MaxStarNum = 30;
GLBatch points[MaxStarNum];
M3DVector2f pointsSize[MaxStarNum];

GLuint texture;
char* texturePath = "Chapter6 SpritePoint/Texture/star.tga";
float color[] = {1,1,1,1};


static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    frustum.SetPerspective(50, width/height, 0.1f, 100);
    glutPostRedisplay();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);

    glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_POINT_SPRITE_ARB);
    glEnable(GL_PROGRAM_POINT_SIZE);

    for (int i = 0; i < MaxStarNum; i++)
    {
        pointsSize[i][0]--;
        if (pointsSize[i][0] <= 0)
            pointsSize[i][0] = pointsSize[i][1];
        shaderMgr.UseSpritePoint(transformPiple.GetModelViewProjectionMatrix(), texture, pointsSize[i][0]);
        points[i].Draw();
    }

    glDisable(GL_POINT_SPRITE_ARB);
    glutSwapBuffers();
    glutPostRedisplay();
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
    //init matrix
    transformPiple.SetMatrixStacks(modelviewStack, projectStack);
    frustum.SetPerspective(50, 640/480, 0.1f, 100);
    projectStack.LoadMatrix(frustum.GetProjectionMatrix());
    modelviewStack.Translate(-2,-2,-12);

    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();

    //init points
    for (int i = 0; i < MaxStarNum; i++)
    {
        points[i].Begin(GL_POINTS, 1, 1);
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

        points[i].Color4fv(vColor);
        points[i].Vertex3fv(vVertex);

        points[i].End();
        // point size
        float size = rand()%200;
        pointsSize[i][0] = size;
        pointsSize[i][1] = size;


        //printf("%f %f %f\n", vColor[0],vColor[1],vColor[2]);
    }

    //init tetxure
    glActiveTexture(0);
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    Util::LoadTGATexture(texturePath, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

void onShutup()
{
    glDeleteTextures(1,&texture);
    shaderMgr.OnUnInit();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("sprite point");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    // set point size
    //glPointSize(20);

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
