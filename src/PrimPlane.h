// Plane Geaometrical Primitive class
// Written by Sergey Kosov in 2005 for Rendering Competition
#pragma once

#include "Prim.h"

/**
 * @brief The Plane Geaometrical Primitive class
 */
class CPrimPlane : public CPrim
{
public:
	/**
	 * @brief Constructor
	 * @param origin Point on the plane
	 * @param normal Normal to the plane
	 */
	CPrimPlane(Vec3f origin, Vec3f normal, std::shared_ptr<IShader> pShader)
		: CPrim(pShader), m_normal(normal), m_origin(origin)
	{
		normalize(m_normal);
	}
	virtual ~CPrimPlane(void) = default;

	virtual bool Intersect(Ray &ray) override
	{
		float t = (m_origin - ray.org).dot(m_normal) / ray.dir.dot(m_normal);
		if (t < Epsilon || t > ray.t)
			return false;
		ray.t = t;
		/* After a ray has been successfully intersected with a primitive, 
		store the primitive’s address in hit */

		ray.hit = this;

		return true;
	}

	virtual Vec3f GetNormal(const Ray &ray) const override
	{
		// --- PUT YOUR CODE HERE ---
		//normalize the normal to plane
		return normalize(m_normal);
	}

private:
	Vec3f m_normal; ///< Normal to the plane
	Vec3f m_origin; ///< Point on the plane
};
