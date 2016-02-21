#include "Subpass.h"

#include "Rendering.h"

#include <tinyxml2.h>
#include "StrToGL.h"

using namespace tinyxml2;

/**
 * @brief Subpass::Subpass
 */
Subpass::Subpass(void)
: m_uFramebufferObject(0)
, m_Pipeline(nullptr)
{
	// ...
}

/**
 * @brief Move constructor
 * @param subpass
 */
Subpass::Subpass(Subpass && subpass)
: m_uFramebufferObject(subpass.m_uFramebufferObject)
, m_Pipeline(subpass.m_Pipeline)
, m_aDrawBuffers(std::move(subpass.m_aDrawBuffers))
{
	subpass.m_uFramebufferObject = 0;
	subpass.m_Pipeline = nullptr;
}

/**
 * @brief Subpass::Subpass
 * @param pipeline
 */
Subpass::Subpass(const Pipeline * pipeline)
: m_uFramebufferObject(0)
, m_Pipeline(pipeline)
{
	// ...
}

/**
 * @brief Subpass::Subpass
 * @param pipeline
 * @param element
 * @param rendering
 */
Subpass::Subpass(const Pipeline * pipeline, const XMLElement * element, const Rendering & rendering)
: m_uFramebufferObject(0)
, m_Pipeline(pipeline)
{
	//
	// Framebuffer
	{
		const XMLElement * output = element->FirstChildElement("output");

		if (NULL != output)
		{
			glGenFramebuffers(1, &m_uFramebufferObject);

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);

			GLenum current_color_attachment = GL_COLOR_ATTACHMENT0;

			while (NULL != output)
			{
				const char * texture = output->Attribute("texture");

				assert(nullptr != texture);

				if (nullptr != texture)
				{
					GLenum a = current_color_attachment;

					const GPU::Texture<GL_TEXTURE_2D> * pRenderTexture = rendering.GetRenderTexture(texture);

					const GLint format = pRenderTexture->getFormat();

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
						m_aDrawBuffers.push_back(a);
					}

					glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, a, GL_TEXTURE_2D, pRenderTexture->GetObject(), 0);
				}

				output = output->NextSiblingElement("output");
			}

			GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

			assert(GL_FRAMEBUFFER_COMPLETE == status);
		}
	}
}

/**
 * @brief Subpass::~Subpass
 */
Subpass::~Subpass(void)
{
	glDeleteFramebuffers(1, &m_uFramebufferObject);
	m_uFramebufferObject = 0;
}

/**
 * @brief Move assignment
 * @param subpass
 * @return
 */
Subpass & Subpass::operator=(Subpass && subpass)
{
	m_uFramebufferObject = subpass.m_uFramebufferObject;
	subpass.m_uFramebufferObject = 0;

	m_Pipeline = subpass.m_Pipeline;
	subpass.m_Pipeline = nullptr;

	m_aDrawBuffers = std::move(subpass.m_aDrawBuffers);

	return(*this);
}

/**
 * @brief Subpass::Begin
 * @return
 */
bool Subpass::Begin(void)
{
	unsigned int size = m_aDrawBuffers.size();

	if (size > 0)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uFramebufferObject);
		glDrawBuffers(m_aDrawBuffers.size(), m_aDrawBuffers.data());
	}

	m_Pipeline->Bind();

	return(true);
}

/**
 * @brief Subpass::End
 */
void Subpass::End(void)
{
	m_Pipeline->UnBind();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
}

/**
 * @brief Subpass::ReadPixel
 * @param pos
 * @param result
 * @return
 */
bool Subpass::ReadPixel(const ivec2 & pos, unsigned int & result)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_uFramebufferObject);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glReadPixels(pos.x, pos.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &result);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glReadBuffer(GL_BACK);

	return(true);
}