#pragma once

#include <vector>

#include <GPU.h> // todo : remove this

namespace RHI
{

class Framebuffer
{
public:

	friend class CommandBuffer;

	//
	// Constructors / Destructor
	Framebuffer		(void);
	Framebuffer		(const Framebuffer & fb) = delete;
	Framebuffer		(Framebuffer && fb);
	Framebuffer		(const std::vector<const GPU::Texture<GL_TEXTURE_2D> *> & aTextures);
	~Framebuffer	(void);

	//
	// Operators
	Framebuffer &	operator =		(const Framebuffer & pass) = delete;
	Framebuffer &	operator =		(Framebuffer && pass);

protected:

#if HAVE_OPENGL
	GLenum m_uFramebufferObject;
#endif // HAVE_OPENGL

#if HAVE_VULKAN
	VkFramebuffer m_framebuffer;
#endif // HAVE_VULKAN
};

}
