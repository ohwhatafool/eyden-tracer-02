#pragma once

#include "ILight.h"

/**
 * @brief Point light source class
 */
class CLightPoint : public ILight
{
public:
	/**
	 * @brief Constructor
	 * @param position The position (origin) of the light source
	 * @param intensity The emission color and strength of the light source
	 */
	CLightPoint(Vec3f intensity, Vec3f position)
		: m_intensity(intensity), m_position(position)
	{
	}
	virtual ~CLightPoint(void) = default;

	virtual std::optional<Vec3f> Illuminate(Ray &ray) override
	{
		// --- PUT YOUR CODE HERE ---
		//Document referenced to get the light intensity is:
		//https://computergraphics.stackexchange.com/questions/5166/how-to-find-light-direction-from-a-point-on-a-surface-to-an-area-light

		Vec3f intensity;
		Vec3f directionLight = m_position - ray.org;

		intensity = m_intensity / pow(cv::norm(directionLight), 2);

		ray.dir = normalize(directionLight);

		//normalized form is v important
		return normalize(intensity);
	}

private:
	Vec3f m_intensity; ///< emission (red, green, blue)
	Vec3f m_position;  ///< The light source origin
};
