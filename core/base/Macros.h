#pragma once

#ifdef _WIN32

#endif

#define IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 
#define IS_LITTLE_ENDIAN (!(IS_BIG_ENDIAN))

#define BREAK_IF(cond) if (cond) break
#define RETURN_IF(cond) if (cond) return