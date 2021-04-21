#include"../Header/UtilTimer.h"

/*
1.创建uicamera 120*120
2.使用fontshader
*/
void FrameTimer::Update()
{
    UpdateFrame();
    ShowFrame();
}

void FrameTimer::UpdateFrame()
{
    nowTime = glutGet(GLUT_ELAPSED_TIME);
    tickCount++;
    if (nowTime - baseTime >= 1000)
    {
        currentFrame = tickCount;
        //printf("frame count %d \n", tickCount);
        baseTime = nowTime;
        tickCount = 0;
    }
    int delta = nowTime - lastTime;
    lastTime = nowTime;
}

void FrameTimer::ShowFrame()
{
    static char str[20]={0};
    sprintf(str, "frame count %d", currentFrame);
    charTex.CreateText(str, strlen(str), -60, 40, 0.3f, 20, 0.5f);
    charTex.Draw();
}
