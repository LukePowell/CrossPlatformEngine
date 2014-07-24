#pragma once

namespace ElysiumEngine
{
    #ifndef C11_UNIX
    #pragma message("Using Microsoft Compiler no constexpr")
    #define constexpr
    #endif
    
    static constexpr unsigned int FNV_Offset = 2166136261;
    static constexpr unsigned int FNV_Prime = 16777619;
    
    static constexpr unsigned int hashOne(unsigned int hash, char current,const char *next)
    {
        return current == 0 ? hash : hashOne((hash ^ current) * FNV_Prime,next[0],next+1);
    }
    
    static constexpr unsigned int hash(const char* string)
    {
        return hashOne(FNV_Offset, string[0], string + 1);
    }
}