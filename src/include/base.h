#ifndef BASE_H
#define BASE_H

// context cracking
# if defined(_WIN32) || defined(_WIN64)
#  define OS_WINDOWS 1
# elif defined(__linux__) || defined(__gnu_linux__)
#  define OS_LINUX 1
# elif defined(__APPLE__) && defined(__MACH__)
# define OS_MAC 1
# endif

# if defined(__M_AMD64) || defined(__amd64__)
#  define ARCH_X64 1
# elif defined(__M_I86) || defined(__i386__)
#  define ARCH_X86 1
# elif defined(__M_ARM) || defined(__arm__)
#  define ARCH_ARM 1
# elif defined(__aarch64__)
#  define ARCH_ARM64 1
# endif

# ifdef __clang__
#  define COMPILER_CLANG 1
# elif __GNUC__
#  define COMPILER_GCC 1
# endif

# if defined(OS_WINDOWS)
#  include "raylib.h"
#  define CLEAR system("cls")
# elif defined(OS_LINUX)
#  include "raylib.h"
#  define CLEAR system("clear")
# endif

// aliases

# define ArrayCount(a) (sizeof(a)/sizeof(*(a)))

# define global     static
# define local      static
# define function   static

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

typedef void VoidFunc(void);

int ap_int(int a, int b);
int ap_float(float a, float b);

# define OVERLOADING
# ifdef OVERLOADING
#  define ap(a, b) _Generic((a),    \
    int     :   ap_int((a), (b)),   \
    float   :   ap_float((a), (b))  \
)
# endif


// debug tools
# define ENABLE_ASSERT
# ifdef  ENABLE_ASSERT
#  define ASSERT(c) if (!(c)) { printf("[-] Assertion failed at line %d.\n", __LINE__); exit(1); }
# else
#  define ASSERT(c)
# endif

#endif //BASE_H
