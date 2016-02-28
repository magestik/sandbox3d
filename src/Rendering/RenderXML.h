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
	void initializePipelines	(Rendering & rendering);
	void initializeTargets		(Rendering & rendering);
	void initializePasses		(Rendering & rendering);
	void initializeFramebuffers	(Rendering & rendering);

protected:

	void createPass				(const void * pNode, Rendering & rendering);
	void createFramebuffer		(const void * pNode, Rendering & rendering);

	void getSubpassDescription	(const void * pNode, RHI::RenderPass::SubpassDescription & subpassDesc);

private:

	void * m_pData;
};
