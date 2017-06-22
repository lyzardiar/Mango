#pragma once

#ifdef _WIN32

#endif

#define IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 
#define IS_LITTLE_ENDIAN (!(IS_BIG_ENDIAN))

#define BREAK_IF(cond) if (cond) break
#define RETURN_IF(cond) if (cond) return
#define SAFE_DELETE(obj) do { if (obj) delete (obj); (obj) = nullptr; } while(0)
#define SAFE_FREE(obj)   do { if (obj) free (obj); (obj) = nullptr; } while(0)

#define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            printf("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)