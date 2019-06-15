#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"

#include "ft2build.h"
#include FT_FREETYPE_H

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;

struct CharTex
{
public:
    GLuint texture;
    GLfloat width;
    GLfloat height;
    M3DVector2d position;
};

struct Char
{
public:
    GLuint vao;
    GLuint vbo;
    GLuint texture;
    float vertex[30];
};

char* fontPath = (char*)"Tools/Font/msyh.ttf";
int fontSize = 40;
FT_Library ft;
FT_Face face;

std::map<char, CharTex> texMap;
std::vector<Char> charList;

GLfloat color[] ={1,0,0,0};
char text[] = "hello world";

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelviewStack->PushMatrix();
    modelviewStack->Translate(0, 0, -160);

    for (int i = 0; i < charList.size() - 1; i++)
    {
        Char ch = charList[i];
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
        param.SetNormalMatrix(normalCamera.GetNormalMatrix());
        param.SetMVMatrix(normalCamera.GetModelviewMatrix());
        param.colorMap[0] = 0;

        glBindVertexArray(ch.vao);
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        shaderMgr.UseFont(param, color);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //Util::PrintBuffer(GL_ARRAY_BUFFER, 30);
        glBindVertexArray(0);
    }
    modelviewStack->PopMatrix();
    glutSwapBuffers();
}


void OnInit()
{
    if (FT_Init_FreeType(&ft))
    {
        printf("init freetype font library error\n");
        return;
    }
    if (FT_New_Face(ft, Util::GetFullPath(fontPath), 0, &face))
    {
        printf("load font error\n");
        return;
    }
    //font size
    FT_Set_Pixel_Sizes(face, 0, fontSize);
}

void InitText(char* text, int count, float x, float y, float space)
{
    for (int i = 0; i < count; i++)
    {
        std::map<char, CharTex>::iterator iter = texMap.find(text[i]);
        if (iter != texMap.end())
            continue;
        if (FT_Load_Char(face, text[i], FT_LOAD_RENDER))//
        {
            printf("load char %c error\n", text[i]);
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        CharTex charTex;
        charTex.texture = texture;
        charTex.position[0] = face->glyph->bitmap_left;
        charTex.position[1] = face->glyph->bitmap_top;
        charTex.height = face->glyph->bitmap.rows;
        charTex.width = face->glyph->bitmap.width;
        texMap[text[i]] = charTex;
        //printf("... %s\n", face->glyph->bitmap.buffer);
    }

    for (int i = 0; i < count; i++)
    {
        std::map<char, CharTex>::iterator iter = texMap.find(text[i]);
        if (iter == texMap.end())
        {
            printf("char %c not exist\n", text[i]);
            continue;
        }
        Char newChar;
        float width = iter->second.width;
        float height = iter->second.height;
        float posx = x + iter->second.position[0];
        float posy = y - (height - iter->second.position[1]);

        float vertex[30] = {
            posx,posy+height,0, 0,0,
            posx,posy,0, 0,1,
            posx+width,posy,0, 1,1,

            posx,posy+height,0, 0,0,
            posx+width,posy,0, 1,1,
            posx+width,posy+height,0, 1,0,
        };

        memcpy((void*)newChar.vertex, (void*)vertex, sizeof(vertex));
        x += width + space;

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(newChar.vertex), newChar.vertex, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        Util::PrintBuffer(GL_ARRAY_BUFFER, 30);

        newChar.vbo = vbo;
        newChar.vao = vao;
        newChar.texture = iter->second.texture;
        charList.push_back(newChar);
    }
}

void OnStartUp()
{
    //禁用字节对齐限制
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    OnInit();
    InitText(text, sizeof(text), 0, 0, 0.2f);

    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();
}

void OnShutUp()
{
    for (std::vector<Char>::iterator iter = charList.begin(); iter != charList.end(); iter++)
    {
        glDeleteTextures(1, &iter->texture);
        glDeleteBuffers(1, &iter->vbo);
    }

    shaderMgr.OnUnInit();
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
    glutCreateWindow("freetype font");

    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
