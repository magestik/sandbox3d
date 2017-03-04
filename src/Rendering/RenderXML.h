#pragma once

#include "Rendering.h"

class RenderXML
{

public:

	//
	// Constructor / Destructor
	RenderXML		(const char * filename);
	~RenderXML		(void);

	//
	// Parsing
	void initializeTargets		(Rendering & rendering);
	void initializeFramebuffers	(Rendering & rendering);
	void initializeQueue		(Rendering & rendering);

protected:

	void createFramebuffer		(const void * pNode, Rendering & rendering);
	void setParameters			(const void * pNode, GraphicsAlgorithm * pAlgo, Rendering & rendering);

private:

	void * m_pData;
};
