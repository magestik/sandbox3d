#include "Scene.h"

#include "SceneListener.h"

/**
 * @brief Scene::Scene
 */
Scene::Scene(void)
{
	m_pLight = new Light::Directionnal(vec3(-20.0f, -20.0f, -20.0f));
}

/**
 * @brief Scene::~Scene
 */
Scene::~Scene(void)
{
	// ...
}

/**
 * @brief Scene::registerListener
 * @param listener
 * @return
 */
bool Scene::registerListener(SceneListener * listener)
{
	m_aListeners.push_back(listener);
	return(true);
}

/**
 * @brief Scene::insert
 * @param object
 * @return
 */
bool Scene::insert(const Object & object)
{
	m_aObjects.push_back(object);
	onObjectInserted(object);
	return(true);
}

/**
 * @brief Scene::remove
 * @param object
 * @return
 */
bool Scene::remove(const Object & object)
{
	for (std::vector<Object>::iterator it = m_aObjects.begin() ; it != m_aObjects.end(); ++it)
	{
		const Object & current = *it;

		if (&current == &object)
		{
			m_aObjects.erase(it);
			onObjectRemoved(object);
			return(true);
		}
	}

	return(false);
}

/**
 * @brief Scene::onObjectInserted
 */
void Scene::onObjectInserted(const Object & object) const
{
	for (SceneListener * listener : m_aListeners)
	{
		listener->onObjectInserted(*this, object);
	}
}

/**
 * @brief Scene::onObjectRemoved
 */
void Scene::onObjectRemoved(const Object & object) const
{
	for (SceneListener * listener : m_aListeners)
	{
		listener->onObjectRemoved(*this, object);
	}
}
