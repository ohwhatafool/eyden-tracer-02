#pragma once

#include "ILight.h"
#include "Prim.h"
#include "CameraPerspective.h"

/**
 * @brief Scene class
 */
class CScene
{
public:
	/**
	 * @brief Constructor
	 * @param camera The reference to the camera
	 * @todo Background may be image
	 */
	CScene(Vec3f bgColor = RGB(0, 0, 0))
		: m_bgColor(bgColor), m_pCamera(std::make_unique<CCameraPerspective>(Vec3f(0, 0, 8), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 60, Size(800, 600)))
	{
	}
	~CScene(void) = default;

	/**
	 * @brief Adds a new primitive to the scene
	 * @param prim Pointer to the primitive
	 */
	void Add(const std::shared_ptr<CPrim> pPrim)
	{
		// --- PUT YOUR CODE HERE ---
		m_vpPrims.push_back(pPrim); //pushing back new shared pointers
	}
	/**
	 * @brief Adds a new light to the scene
	 * @param pLight Pointer to the light
	 */
	void Add(const std::shared_ptr<ILight> pLight)
	{
		// --- PUT YOUR CODE HERE ---
		m_vpLights.push_back(pLight);
		//pushing back new shared pointers
	}

	/**
	 * @brief Checks intersection of ray \b ray with all contained objects
	 * @param ray The ray
	 * @retval true If ray \b ray intersects any object
	 * @retval false otherwise
	 */
	bool Intersect(Ray &ray) const
	{
		// --- PUT YOUR CODE HERE ---
		//Since Intersect is a virtual function

		//Checking if Ray intersects primitive or not, by looping through all primitives
		for (auto index = m_vpPrims.begin(); index != m_vpPrims.end(); index++)
		{
			if ((*index)->Intersect(ray))
			{
				return true;
			}
		}
		return false;
	}
	/**
	 * find occluder
	 */
	bool Occluded(Ray &ray)
	{
		// --- PUT YOUR CODE HERE ---
		return Intersect(ray);
		// check if something blocks the light.
	}

	/**
	 trace the given ray and shade it and
	 return the color of the shaded ray
	 */
	Vec3f RayTrace(Ray &ray) const
	{
		// --- PUT YOUR CODE HERE ---
		Vec3f col = m_bgColor; // background color

		//Return the color depending on the hit
		for (auto i = m_vpPrims.begin(); i != m_vpPrims.end(); i++)
		{
			if ((*i)->Intersect(ray))
			{
				col = ray.hit->getShader()->Shade(ray);
				// color = Vec3f(1,1,1); // trying Initially
			}
		}
		return col;
	}

public:
	std::unique_ptr<CCameraPerspective> m_pCamera;
	std::vector<std::shared_ptr<ILight>> m_vpLights; ///< lights

private:
	Vec3f m_bgColor = RGB(0, 0, 0);				   ///< background color
	std::vector<std::shared_ptr<CPrim>> m_vpPrims; ///< primitives
};