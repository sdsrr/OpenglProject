#ifndef UTILPRINT
#define UTILPRINT
#include "Tools.h"

class UtilPrint
{
public:
    static void PrintString(int count, char* str, ...);
    static void PrintMatrix44f(M3DMatrix44f matrix, std::string ext="");
    static void PrintMatrix44f(const M3DMatrix44f matrix, std::string ext="");
    static void PrintVector3f(M3DVector3f vector, std::string ext="");
    static void PrintBuffer(GLenum type, int bufferSize);
};



#endif // UTILPRINT
