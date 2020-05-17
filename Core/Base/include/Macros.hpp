#ifndef ALIT_MACROS_HPP
#define ALIT_MACROS_HPP


/// Debug macros
#ifdef ALIT_DEBUG
#include <cassert>
#include <iostream>
#include <string>
#include <chrono>

static std::chrono::steady_clock::time_point start;

#define ALIT_ASSERT(test, msg)                                                                                         \
    if(!test)                                                                                                          \
        LOG(msg);                                                                                                      \
    assert(test)
#define LOG(msg) (std::cerr << msg << std::endl)
#define STR(v) std::to_string(v)
#define VEC_STR(v) (STR(v.x) + " " + STR(v.y) + " " + STR(v.z) + " ")

#define START_CLOCK() start = std::chrono::steady_clock::now()
#define ELAPSED()                                                                                                      \
    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count() / 1000.f

#ifdef __gl_h_
#define WAIT_END() glFinish()
#define GL_CHECK_ERROR()                                                                                               \
    {                                                                                                                  \
        GLenum error;                                                                                                  \
        while((error = glGetError()) != GL_NO_ERROR) {                                                                 \
            std::string errorMessage;                                                                                  \
            switch(error) {                                                                                            \
            case GL_INVALID_ENUM:                                                                                      \
                errorMessage = "Invalid enum";                                                                         \
                break;                                                                                                 \
            case GL_INVALID_VALUE:                                                                                     \
                errorMessage = "Invalid value";                                                                        \
                break;                                                                                                 \
            case GL_INVALID_OPERATION:                                                                                 \
                errorMessage = "Invalid operation";                                                                    \
                break;                                                                                                 \
            case GL_INVALID_FRAMEBUFFER_OPERATION:                                                                     \
                errorMessage = "Invalid framebuffer operation";                                                        \
                break;                                                                                                 \
            }                                                                                                          \
            LOG("[OGL ERROR]: " + errorMessage);                                                                       \
        }                                                                                                              \
    }
#endif

#else

// clang-format off
#define NONE while(false)
// clang-format on

#define ALIT_ASSERT(test, msg) NONE
#define LOG(msg) NONE
#define STR(v) NONE
#define VEC_STR(v) NONE

#define START_CLOCK() NONE
#define ELAPSED() NONE

#ifdef __gl_h_
#define WAIT_END() NONE
#define GL_CHECK_ERROR() NONE
#endif

#endif


/// DLL macros
// clang-format off
#ifdef _MSC_VER
#    ifdef DLL_EXPORT
#        define ALIT_API __declspec(dllexport)
#    else
#        define ALIT_API __declspec(dllimport)
#    endif
#else
#    define ALIT_API
#endif
// clang-format on


#endif