#pragma once

#include <GL3/gl3.h>

#include <assert.h>

namespace GPU
{

/**
 * @brief isDepthFormat
 * @param internalFormat
 * @return
 */
inline bool isDepthFormat(GLint internalFormat)
{
    switch (internalFormat)
    {
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32F:
        {
            return(true);
        }
        break;

        default:
        {
            return(false);
        }
    }

    return(false);
}

/**
 * @brief isStencilFormat
 * @param internalFormat
 * @return
 */
inline bool isStencilFormat(GLint internalFormat)
{
    switch (internalFormat)
    {
        case GL_STENCIL_INDEX:
        case GL_STENCIL_INDEX1:
        case GL_STENCIL_INDEX4:
        case GL_STENCIL_INDEX8:
        case GL_STENCIL_INDEX16:
        {
            return(true);
        }
        break;

        default:
        {
            return(false);
        }
    }

    return(false);
}

/**
 * @brief isDepthStencilFormat
 * @param internalFormat
 * @return
 */
inline bool isDepthStencilFormat(GLint internalFormat)
{
    switch (internalFormat)
    {
        case GL_DEPTH_STENCIL:
        case GL_DEPTH24_STENCIL8:
        case GL_DEPTH32F_STENCIL8:
        {
            return(true);
        }
        break;

        default:
        {
            return(false);
        }
    }

    return(false);
}

/**
 * @brief getFormat
 * @todo constexpr
 * @param internalFormat
 * @return
 */
inline GLenum getFormat(GLint internalFormat)
{
    switch (internalFormat)
    {
        case GL_R8:
        case GL_R8_SNORM:
        case GL_R16F:
        case GL_R32F:
        {
            return(GL_RED);
        }
        break;

        case GL_R8UI:
        case GL_R8I:
        case GL_R16UI:
        case GL_R16I:
        case GL_R32UI:
        case GL_R32I:
        {
            return(GL_RED_INTEGER);
        }
        break;

        case GL_RG8:
        case GL_RG8_SNORM:
        case GL_RG16F:
        case GL_RG32F:
        {
            return(GL_RG);
        }
        break;

        case GL_RG8UI:
        case GL_RG8I:
        case GL_RG16UI:
        case GL_RG16I:
        case GL_RG32UI:
        case GL_RG32I:
        {
            return(GL_RG_INTEGER);
        }
        break;

        case GL_RGB8:
        case GL_SRGB8:
#ifdef GL_RGB565
        case GL_RGB565:
#endif
        case GL_RGB8_SNORM:
        case GL_R11F_G11F_B10F:
        case GL_RGB9_E5:
        case GL_RGB16F:
        case GL_RGB32F:
        {
            return(GL_RGB);
        }
        break;

        case GL_RGB8UI:
        case GL_RGB8I:
        case GL_RGB16UI:
        case GL_RGB16I:
        case GL_RGB32UI:
        case GL_RGB32I:
        {
            return(GL_RGB_INTEGER);
        }
        break;

        case GL_RGBA8:
        case GL_SRGB8_ALPHA8:
        case GL_RGBA8_SNORM:
        case GL_RGB5_A1:
        case GL_RGBA4:
        case GL_RGB10_A2:
        case GL_RGBA16F:
        case GL_RGBA32F:
        {
            return(GL_RGBA);
        }
        break;

        case GL_RGBA8UI:
        case GL_RGBA8I:
        case GL_RGB10_A2UI:
        case GL_RGBA16UI:
        case GL_RGBA16I:
        case GL_RGBA32I:
        case GL_RGBA32UI:
        {
            return(GL_RGBA_INTEGER);
        }
        break;

        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32F:
        {
            return(GL_DEPTH_COMPONENT);
        }
        break;

        case GL_DEPTH24_STENCIL8:
        case GL_DEPTH32F_STENCIL8:
        {
            return(GL_DEPTH_STENCIL);
        }
        break;

        default:
        {
            assert(false);
        }
    }
}

/**
 * @brief getType
 * @todo constexpr
 * @param internalFormat
 * @return
 */
inline GLenum getType(GLint internalFormat)
{
    switch (internalFormat)
    {
        case GL_R8:
        case GL_R8UI:
        case GL_RG8:
        case GL_RG8UI:
        case GL_RGB8:
        case GL_SRGB8:
        case GL_RGB8UI:
        case GL_RGBA8:
        case GL_SRGB8_ALPHA8:
        case GL_RGBA8UI:
        {
            return(GL_UNSIGNED_BYTE);
        }
        break;

        case GL_R8_SNORM:
        case GL_R8I:
        case GL_RG8_SNORM:
        case GL_RG8I:
        case GL_RGB8_SNORM:
        case GL_RGB8I:
        case GL_RGBA8_SNORM:
        case GL_RGBA8I:
        {
            return(GL_BYTE);
        }
        break;

        case GL_R16UI:
        case GL_RG16UI:
        case GL_RGB16UI:
        case GL_RGBA16UI:
        case GL_DEPTH_COMPONENT16:
        {
            return(GL_UNSIGNED_SHORT);
        }
        break;

        case GL_R16I:
        case GL_RG16I:
        case GL_RGB16I:
        case GL_RGBA16I:
        {
            return(GL_SHORT);
        }
        break;

        case GL_R32UI:
        case GL_RG32UI:
        case GL_RGB32UI:
        case GL_RGBA32UI:
        case GL_DEPTH_COMPONENT24:
        {
            return(GL_UNSIGNED_INT);
        }
        break;

        case GL_R32I:
        case GL_RG32I:
        case GL_RGB32I:
        case GL_RGBA32I:
        {
            return(GL_INT);
        }
        break;

        case GL_R16F:
        case GL_RG16F:
        case GL_RGB16F:
        case GL_RGBA16F:
        {
            return(GL_HALF_FLOAT);
        }
        break;

        case GL_R32F:
        case GL_RG32F:
        case GL_RGB32F:
        case GL_RGBA32F:
        case GL_DEPTH_COMPONENT32F:
        {
            return(GL_FLOAT);
        }
        break;

#ifdef GL_RGB565
        case GL_RGB565:
        {
            return(GL_UNSIGNED_SHORT_5_6_5);
        }
        break;
#endif

        case GL_R11F_G11F_B10F:
        {
            return(GL_UNSIGNED_INT_10F_11F_11F_REV);
        }
        break;

        case GL_RGB9_E5:
        {
            return(GL_UNSIGNED_INT_5_9_9_9_REV);
        }
        break;

        case GL_RGB5_A1:
        {
            return(GL_UNSIGNED_SHORT_5_5_5_1);
        }
        break;

        case GL_RGBA4:
        {
            return(GL_UNSIGNED_SHORT_4_4_4_4);
        }
        break;

        case GL_RGB10_A2:
        case GL_RGB10_A2UI:
        {
            return(GL_UNSIGNED_INT_2_10_10_10_REV);
        }
        break;

        case GL_DEPTH24_STENCIL8:
        {
            return(GL_UNSIGNED_INT_24_8);
        }
        break;

        case GL_DEPTH32F_STENCIL8:
        {
            return(GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
        }
        break;

        default:
        {
            assert(false);
        }
    }
}

}
