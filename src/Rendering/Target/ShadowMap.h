#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include "../Pass.h"

class ShadowMap : public Pass
{
public:

    ShadowMap(const Pipeline * pipeline);
    ~ShadowMap(void);

    bool	init	(unsigned int width, unsigned height);
    void	free	(void);

    GLuint	GetObject	(void) { return(m_uFramebufferObject); }

    const GPU::Texture<GL_TEXTURE_2D> &	GetTexture	(void) { return(m_texture); }

    const mat4x4 &	GetProjection (void) const { return(m_matProjection); }

protected:

    void updateProjection(void);

private:

    mat4x4 m_matProjection;

    GPU::Texture<GL_TEXTURE_2D> m_texture;
};
