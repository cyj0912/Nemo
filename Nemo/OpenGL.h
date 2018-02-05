#pragma once

#include <Platform.h>

#if TC_OS == TC_OS_MAC_OS_X
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif
