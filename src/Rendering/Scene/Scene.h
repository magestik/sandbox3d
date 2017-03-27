#pragma once

#include "Object.h"
#include "../Light/Light.h"

#include <vector>

class SceneListener;

class Scene
{
public:

	Scene(void);
	~Scene(void);

	bool registerListener(SceneListener * listener);

	bool insert(const Object & object);
	bool remove(const Object & object);

	inline const std::vector<Object> & getObjects(void) const
	{
		return(m_aObjects);
	}

	inline unsigned int getObjectCount(void) const
	{
		return(m_aObjects.size());
	}

private:

	void onObjectInserted(const Object & object) const;
	void onObjectRemoved(const Object & object) const;

public: //private:

	Light::Directionnal * m_pLight;

private:

	std::vector<Object> m_aObjects;

	std::vector<SceneListener*> m_aListeners;
};
