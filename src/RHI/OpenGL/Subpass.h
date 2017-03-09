#pragma once

#include <vector>

class Subpass
{

public:

	//
	// Constructors / Destructor
	Subpass		(void);
	Subpass		(const Subpass & subpass) = delete;
	Subpass		(Subpass && subpass);
	Subpass		(const std::vector<int> & aColorAttachments);
	~Subpass	(void);

	//
	// Operators
	Subpass &	operator =		(Subpass && subpass);

	std::vector<GLenum> m_aDrawBuffers;

};
