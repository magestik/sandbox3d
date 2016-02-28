#include "../RHI.h"

/**
 * @brief Subpass::Subpass
 */
Subpass::Subpass(void)
: m_aDrawBuffers()
{
	// ...
}

/**
 * @brief Move constructor
 * @param subpass
 */
Subpass::Subpass(Subpass && subpass)
: m_aDrawBuffers(std::move(subpass.m_aDrawBuffers))
{
	// ...
}

/**
 * @brief Subpass::Subpass
 * @param aDrawBuffers
 */
Subpass::Subpass(const std::vector<int> & aColorAttachments)
: m_aDrawBuffers()
{
	for (int i : aColorAttachments)
	{
		m_aDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
}

/**
 * @brief Subpass::~Subpass
 */
Subpass::~Subpass(void)
{
	// ...
}

/**
 * @brief Move assignment
 * @param subpass
 * @return
 */
Subpass & Subpass::operator=(Subpass && subpass)
{
	m_aDrawBuffers = std::move(subpass.m_aDrawBuffers);

	return(*this);
}

/**
 * @brief Subpass::Begin
 * @return
 */
#if 0
bool Subpass::Begin(void)
{
	unsigned int size = m_aDrawBuffers.size();

	if (size > 0)
	{
		glDrawBuffers(size, m_aDrawBuffers.data());
	}

	return(true);
}
#endif //0

/**
 * @brief Subpass::End
 */
#if 0
void Subpass::End(void)
{
	glDrawBuffer(GL_BACK);
}
#endif // 0
