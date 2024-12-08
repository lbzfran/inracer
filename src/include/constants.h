#ifndef CONSTANTS_H
#define CONSTANTS_H

// constants
# define SCREEN_WIDTH 1080
# define SCREEN_HEIGHT 720

# define TARGET_FPS 60

// context cracking
# ifdef __linux__
   #include "raylib.h"
#  define CLEAR system("clear")
# endif

// aliases
# define AND &&
# define OR ||
# define NOT !

# include <stdio.h>
# include <stdint.h>
typedef float       f32;
typedef double      f64;
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;
typedef size_t      usize;
typedef ssize_t     isize;

// debug tools
# define ENABLE_ASSERT
# ifdef  ENABLE_ASSERT
#  define ASSERT(c) if (!(c)) { printf("[-] Assertion failed at line %d.\n", __LINE__); exit(1); }
# else
#  define ASSERT(c)
# endif

#endif //CONSTANTS_H
