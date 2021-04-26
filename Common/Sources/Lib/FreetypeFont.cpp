#include "../../Header/Lib/FreetypeFont.h"

#include "ft2build.h"
#include FT_FREETYPE_H


void CharTextureManager::OnInit()
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

    //开启混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //禁用字节对齐限制
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void CharTextureManager::LoadCharTexture(char text[], int count)
{
    for (int i = 0; i < count; i++)
    {
        std::map<char, CharTexture>::iterator iter = texMap.find(text[i]);
        if (iter != texMap.end())
            continue;
        if (FT_Load_Char(face, text[i], FT_LOAD_RENDER))//
        {
            printf("load char %c error\n", text[i]);
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE31);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        CharTexture charTex;
        charTex.texture = texture;
        charTex.position[0] = face->glyph->bitmap_left;
        charTex.position[1] = face->glyph->bitmap_top;
        charTex.height = face->glyph->bitmap.rows;
        charTex.width = face->glyph->bitmap.width;
        texMap[text[i]] = charTex;
        //printf("... %s\n", face->glyph->bitmap.buffer);
    }
}

CharTexture* CharTextureManager::GetCharTexture(char ch)
{
    std::map<char, CharTexture>::iterator iter = texMap.find(ch);
    if (iter == texMap.end())
        return NULL;
    return &iter->second;
}

CharTextureManager* CharTextureManager::_instance = NULL;
CharTextureManager* CharTextureManager::GetInstance()
{
    if (_instance == NULL)
    {
        _instance = new CharTextureManager();
        _instance->OnInit();
    }
    return _instance;
}

float CharTextureManager::GetFontSize() { return fontSize; };
void CharTextureManager::SetFontSize(float fontSize)
{
    if (this->fontSize == fontSize)
        return;
    this->fontSize = fontSize;
    OnInit();
}

void CharText::CreateText(char text[], int count, float x, float y, float space, float fontSize, float scale)
{
    CharTextureManager* instance = CharTextureManager::GetInstance();
    instance->SetFontSize(fontSize);
    instance->LoadCharTexture(text, count);
    charList.clear();
    for (int i = 0; i < count; i++)
    {
        CharTexture* tex = instance->GetCharTexture(text[i]);
        if (tex == NULL)
        {
            printf("char %c not exist\n", text[i]);
            continue;
        }
        if (text[i] == ' ')
        {
            float fontSize = instance->GetFontSize();
            x += fontSize/2*scale;
            continue;
        }

        Char newChar;
        float width = tex->width * scale;
        float height = tex->height;
        float posx = x + tex->position[0];
        float posy = y - (height - tex->position[1])*scale;

        float vertex[30] = {
            posx,posy+height*scale,0, 0,0,
            posx,posy,0, 0,1,
            posx+width,posy,0, 1,1,

            posx,posy+height*scale,0, 0,0,
            posx+width,posy,0, 1,1,
            posx+width,posy+height*scale,0, 1,0,
        };
        x += width + space;

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        //Util::PrintBuffer(GL_ARRAY_BUFFER, 30);

        newChar.vbo = vbo;
        newChar.vao = vao;
        newChar.texture = tex->texture;
        charList.push_back(newChar);
    }
    //printf("%d  %d \n", count, charList.size());
}

void CharText::Draw()
{
    for (int i = 0; i < (int)charList.size(); i++)
    {
        Char ch = charList[i];
        glBindVertexArray(ch.vao);
        glActiveTexture(GL_TEXTURE31);
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void CharText::OnUnInit()
{
    for (std::vector<Char>::iterator iter = charList.begin(); iter != charList.end(); iter++)
    {
        glDeleteTextures(1, &iter->texture);
        glDeleteBuffers(1, &iter->vbo);
    }
}
