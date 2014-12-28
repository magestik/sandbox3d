#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

class Shader;

class Bloom
{
public:

    Bloom(void);
    ~Bloom(void);

    bool	init	(const GPU::Texture<GL_TEXTURE_2D> * pTexture1, const GPU::Texture<GL_TEXTURE_2D> * pTexture2);
    void	free	(void);

    bool	begin	(void);
    bool	end		(void);

    void    prepare_blur_h    (void);
    void    prepare_blur_v    (void);

    GLuint	GetObject(void) { return(m_uCurrentObject); }

    Shader * GetShader(void) { return(m_pCurrentShader); }

private:

    GLuint m_uObject [2];

    unsigned int m_uCurrentObject;

    Shader * m_pShader_bright;
    Shader * m_pShader_blur;

    Shader * m_pCurrentShader;
};
