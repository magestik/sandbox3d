#pragma once

#include <Vector.h>

struct EnvironmentSettings
{
	enum EFeature
	{
		AMBIENT_LIGHT = 0,
		FOG = 1,
	};

	struct FogSettings
	{
		FogSettings(void) : Color(0.8f, 0.8f, 0.8f), Scattering(0.35f), Extinction(0.35f)
		{
			// ...
		}

		vec3 Color;
		float Scattering;
		float Extinction;
	};

	struct AmbientLightSettings
	{
		vec3 Color;
	};

	EnvironmentSettings(void)
	{
		features[AMBIENT_LIGHT] = true;
		features[FOG] = false;
	}

	AmbientLightSettings ambient;
	FogSettings fog;

	void enable(EFeature f)
	{
		features[f] = true;
	}

	void disable(EFeature f)
	{
		features[f] = false;
	}

	bool isEnabled(EFeature f)
	{
		return(features[f]);
	}

private:

	bool features [2];
};
