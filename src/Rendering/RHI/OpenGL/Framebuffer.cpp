#include "../RHI.h"

/**
 * @brief Default constructor
 */
RHI::Framebuffer::Framebuffer(void)
: m_uFramebufferObject(0)
{

}

/**
 * @brief Move constructor
 * @param fb
 */
RHI::Framebuffer::Framebuffer(Framebuffer && fb)
: m_uFramebufferObject(fb.m_uFramebufferObject)
{
	fb.m_uFramebufferObject = 0;
}

/**
 * @brief Constructor
 * @param aTextures
 */
RHI::Framebuffer::Framebuffer(const std::vector<const GPU::Texture<GL_TEXTURE_2D> *> & aTextures)
: m_uFramebufferObject(0)
{
	if (aTextures.size() > 0)
	{
		GLenum current_color_attachment = GL_COLOR_ATTACHMENT0;

		glGenFramebuffers(1, &m_uFramebufferObject);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);

		for (auto texture : aTextures)
		{
			GLenum a = current_color_attachment;

			const GLint format = texture->getFormat();

			if (GPU::isDepthStencilFormat(format))
			{
				a = GL_DEPTH_STENCIL_ATTACHMENT;
			}
			else if (GPU::isDepthFormat(format))
			{
				a = GL_DEPTH_ATTACHMENT;
			}
			else if (GPU::isStencilFormat(format))
			{
				a = GL_STENCIL_ATTACHMENT;
			}
			else
			{
				current_color_attachment += 1;
				//m_aDrawBuffers.push_back(a);
			}

			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, a, GL_TEXTURE_2D, texture->GetObject(), 0);
		}

		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		assert(GL_FRAMEBUFFER_COMPLETE == status);
	}
}
/**
 * @brief Constructor
 * @param aTextures
 */
RHI::Framebuffer::Framebuffer(GLuint framebuffer)
: m_uFramebufferObject(framebuffer)
{

}

/**
 * @brief Destructor
 */
RHI::Framebuffer::~Framebuffer(void)
{
	glDeleteFramebuffers(1, &m_uFramebufferObject);
}

/**
 * @brief Move assignement
 * @param fb
 * @return
 */
RHI::Framebuffer & RHI::Framebuffer::operator=(Framebuffer && fb)
{
	m_uFramebufferObject = fb.m_uFramebufferObject;
	fb.m_uFramebufferObject = 0;
	return(*this);
}
