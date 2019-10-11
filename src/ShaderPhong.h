#pragma once

#include "ShaderFlat.h"

class CScene;

class CShaderPhong : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 * @param ka The ambient coefficient
	 * @param kd The diffuse reflection coefficients
	 * @param ks The specular refelection coefficients
	 * @param ke The shininess exponent
	 */
	CShaderPhong(CScene &scene, Vec3f color, float ka, float kd, float ks, float ke)
		: CShaderFlat(color), m_scene(scene), m_ka(ka), m_kd(kd), m_ks(ks), m_ke(ke)
	{
	}
	virtual ~CShaderPhong(void) = default;

	virtual Vec3f Shade(const Ray &ray) const override
	{
		// --- PUT YOUR CODE HERE ---

		//Concept and formula followed from
		//https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF
		Vec3f AmbientV, DiffuseV, SpecularV;

		//AmbientV = ka.ca.La
		AmbientV = m_ka * CShaderFlat::Shade(ray); //modeling ca.La = Color returned
		//Need initialized sum values to calculate summation for diff and spectral
		Vec3f DiffusionSum = 0;
		Vec3f SpectralSum = 0;

		Ray Iray;

		//n = no of light sources = size of primitives of light array
		for (int i = 0; i < m_scene.m_vpLights.size(); i++)
		{
			Iray.org = ray.org + ray.t * ray.dir;
			//Getting light radiance for the incident ray
			std::optional<Vec3f> RadianceLight = m_scene.m_vpLights[i]->Illuminate(Iray); //kd.cd

			Iray.t = INFINITY; //setting a max limit

			if (!m_scene.Occluded(Iray))
			{
				if (RadianceLight)
				{
					float prodIN;
					//Only consider light sources that illuminate the primitive from its front-side (i.e. Il·N > 0)
					prodIN = max(0.0f, Iray.dir.dot(ray.hit->GetNormal(ray)));

					//summation of Σl=0 to n-1: L(I·N)
					DiffusionSum += *RadianceLight * prodIN;
				}
			}
		}

		//kd.cd. Σl=0 to n-1: L(I·N)
		DiffuseV = m_kd * DiffusionSum.mul(CShaderFlat::Shade(ray));

		Ray Sray; // specular part //incident ray
		for (int i = 0; i < m_scene.m_vpLights.size(); i++)
		{
			Sray.org = ray.org + ray.t * ray.dir;

			std::optional<Vec3f> RadianceLight = m_scene.m_vpLights[i]->Illuminate(Sray);

			Sray.t = std::numeric_limits<float>::infinity();

			//If the light is not blocked
			if (!m_scene.Occluded(Sray))
			{
				if (RadianceLight)
				{
					// R = I - 2*(I.N)*N
					Vec3f R = Sray.dir - 2 * (Sray.dir.dot(ray.hit->GetNormal(ray))) * ray.hit->GetNormal(ray);
					//check if I.N > 0
					float I_R = max(0.0f, ray.dir.dot(R));
					float ke = pow(I_R, m_ke); // calculate (Il·R)^ke
					SpectralSum += *RadianceLight * ke;
					// sum up the products of each light
				}
			}
		}

		SpecularV = m_ks * RGB(1, 1, 1).mul(SpectralSum);
		//Use cs = (1, 1, 1) for white highlights

		//Lr = kacaLa + kdcd Σl=0n-1 Ll(Il·N)+ kscs Σl=0n-1 Ll(Il·R)ke
		Vec3f LR = AmbientV + DiffuseV + SpecularV;

		return LR;
		//return RGB(0, 0, 0);
	}

private:
	CScene &m_scene;
	float m_ka; ///< ambient coefficient
	float m_kd; ///< diffuse reflection coefficients
	float m_ks; ///< specular refelection coefficients
	float m_ke; ///< shininess exponent
};
