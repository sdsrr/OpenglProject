#ifndef FREETYPEFONT
#define FREETYPEFONT

#include "../Header/Tools.h"

#include "ft2build.h"
#include FT_FREETYPE_H

class CharTexture
{
public:
    GLuint texture;
    GLfloat width;
    GLfloat height;
    M3DVector2d position;
};

class Char
{
public:
    GLuint vao;
    GLuint vbo;
    GLuint texture;
};

class CharTextureManager
{
private:
    FT_Library ft;
    FT_Face face;
    float fontSize = 40;
    char* fontPath = (char*)"Tools/Font/msyh.ttf";
    std::map<char, CharTexture> texMap;
    static CharTextureManager* _instance;
public:
    void OnInit();
    void LoadCharTexture(char text[], int count);
    CharTexture* GetCharTexture(char ch);
    static CharTextureManager* GetInstance();
    float GetFontSize();
};

class CharText
{
private:
    std::vector<Char> charList;
public:
    void Show();
    void CreateText(char text[], int count, float x, float y, float space);
    void OnUnInit();
};

#endif

