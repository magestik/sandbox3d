#pragma once

class Scene;
class Object;

class SceneListener
{
public:
	virtual void onObjectInserted	(const Scene & scene, const Object & object) = 0;
	virtual void onObjectRemoved	(const Scene & scene, const Object & object) = 0;
};
