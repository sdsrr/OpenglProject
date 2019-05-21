#ifndef UTILTIMER
#define UTILTIMER

#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <mmsystem.h>
#ifdef __cplusplus
}
#endif
#pragma comment(lib, "winmm.lib")
#else
#if defined(__unix__) || defined(__APPLE__)
#include <sys/time.h>
#else
#include <ctime>
#endif
#endif

class UtilTimer
{
public:
#ifdef _WIN32
    typedef DWORD time_t;

    UtilTimer()
    {
        ::timeBeginPeriod(1);
    }
    ~UtilTimer()
    {
        ::timeEndPeriod(1);
    }

    void start()
    {
        t_[0] = ::timeGetTime();
    }
    void end()
    {
        t_[1] = ::timeGetTime();
    }

    time_t sec()
    {
        return (time_t)((t_[1] - t_[0]) / 1000);
    }
    time_t msec()
    {
        return (time_t)((t_[1] - t_[0]));
    }
    time_t usec()
    {
        return (time_t)((t_[1] - t_[0]) * 1000);
    }
    time_t current()
    {
        return ::timeGetTime();
    }
#else
#if defined(__unix__) || defined(__APPLE__)
    typedef unsigned long int time_t;

    void start()
    {
        gettimeofday(tv + 0, &tz);
    }
    void end()
    {
        gettimeofday(tv + 1, &tz);
    }

    time_t sec()
    {
        return (time_t)(tv[1].tv_sec - tv[0].tv_sec);
    }
    time_t msec()
    {
        return this->sec() * 1000 +
               (time_t)((tv[1].tv_usec - tv[0].tv_usec) / 1000);
    }
    time_t usec()
    {
        return this->sec() * 1000000 + (time_t)(tv[1].tv_usec - tv[0].tv_usec);
    }
    time_t current()
    {
        struct timeval t;
        gettimeofday(&t, NULL);
        return (time_t)(t.tv_sec * 1000 + t.tv_usec);
    }

#else  // C timer
    // using namespace std;
    typedef clock_t time_t;

    void start()
    {
        t_[0] = clock();
    }
    void end()
    {
        t_[1] = clock();
    }

    time_t sec()
    {
        return (time_t)((t_[1] - t_[0]) / CLOCKS_PER_SEC);
    }
    time_t msec()
    {
        return (time_t)((t_[1] - t_[0]) * 1000 / CLOCKS_PER_SEC);
    }
    time_t usec()
    {
        return (time_t)((t_[1] - t_[0]) * 1000000 / CLOCKS_PER_SEC);
    }
    time_t current()
    {
        return (time_t)clock();
    }

#endif
#endif

private:
#ifdef _WIN32
    DWORD t_[2];
#else
#if defined(__unix__) || defined(__APPLE__)
    struct timeval tv[2];
    struct timezone tz;
#else
    time_t t_[2];
#endif
#endif
};

class FrameTimer
{
private:
    int baseTime = 0;
    int nowTime = 0;
    int tickCount = 0;
    int lastTime = 0;
public:
    int Update()
    {
        nowTime = glutGet(GLUT_ELAPSED_TIME);
        tickCount++;
        if (nowTime - baseTime >= 1000)
        {
            printf("frame count %d \n", tickCount);
            baseTime = nowTime;
            tickCount = 0;
        }
        int delta = nowTime - lastTime;
        lastTime = nowTime;
        return delta;
    }
};

#endif
