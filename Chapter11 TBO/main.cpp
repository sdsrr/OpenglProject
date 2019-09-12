#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Camera.h"

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;
NormalCamera normalCamera;

GLuint tbo;
GLuint texture;
GLfloat angle;
BatchGObject triangle;

const char* filepath = "Chapter11 TBO/Marble.tga";
const char* datapath = "Chapter11 TBO/LumTan.data";
bool useTexture = false;
int maxWidth = 0;
int maxHeight = 0;

static float* LoadFloatData(const char *szFile, int *count)
{
	GLint lineCount = 0;
	FILE *fp;
	float* data = 0;

    // Open the shader file
    fp = fopen(szFile, "r");
    if(fp != NULL)
	{
		char szFloat[1024];
        while ( fgets ( szFloat, sizeof szFloat, fp ) != NULL )
            lineCount++;

        // Go back to beginning of file
        rewind(fp);

        // Allocate space for all data
		data = (float*)malloc((lineCount)*sizeof(float));
		if (data != NULL)
		{
			int index = 0;
			while ( fgets ( szFloat, sizeof szFloat, fp ) != NULL )
			{
				data[index] = (float)atof(szFloat);
				index++;
			}
			count[0] = index;
		}
        fclose(fp);
	}
    else
        return 0;

    return data;
}

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glBindTexture(GL_TEXTURE_BUFFER, texture);
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(triangle.modelStack));
    param.colorMap[0] = 0;
    shaderMgr->UseTboShader(param, maxWidth, maxHeight);
    triangle.Draw();

    glutSwapBuffers();
}

void OnStartUp()
{
    //init triangle
    gltMakeCube(triangle.batch, 2);
    triangle.modelStack.Translate(0,0,-20);

    //init texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_BUFFER, texture);

    //int tbo
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_TEXTURE_BUFFER, tbo);
    if (useTexture)
    {
        GLenum eFormat;
        GLint components;
        GLbyte* pBits = gltReadTGABits(Util::GetFullPath(filepath), &maxWidth, &maxHeight, &components, &eFormat);
        if (maxWidth != 0 && maxHeight != 0)
        {
            glBufferData(GL_TEXTURE_BUFFER, sizeof(GLbyte)*maxWidth*maxHeight*3, pBits, GL_STATIC_DRAW);
            free(pBits);
        }
        printf("width=%d height=%d eFormat=%d\n", maxWidth, maxHeight, eFormat);
        //bind texture with tbo 数据还是GL_RG8格式不对导致，显示的纹理错误
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RG8, tbo);
    }
    else
    {
        float* fileData = LoadFloatData(Util::GetFullPath(datapath), &maxWidth);
        if (maxWidth > 0)
        {
            glBufferData(GL_TEXTURE_BUFFER, sizeof(float)*maxWidth, fileData, GL_STATIC_DRAW);
            free(fileData);
        }
        //bind texture with tbo
        glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, tbo);
    }

    //init shadermgr
    glShaderMgr.InitializeStockShaders();
    shaderMgr = ShaderMgr::GetInstance();
    normalCamera.OnInit(640, 480, 50, 1, 2);
}

void OnShutUp()
{
    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &tbo);
    shaderMgr->OnUnInit();
    normalCamera.OnUnInit();
}

void Idle(void) {glutPostRedisplay();}
void KeyboardFn(unsigned char key, int x, int y) {normalCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y) {normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h) {normalCamera.Resize(w, h);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("fbo");


    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (glewInit() != GLEW_OK)
    {
        printf("glew init error...\n");
        return 0;
    }

    OnStartUp();
    glutMainLoop();
    OnShutUp();

    return EXIT_SUCCESS;
}
