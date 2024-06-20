#pragma once

#include "Scene\Entity.h"

namespace scene
{

	class Flower : public Entity
	{
	public:
		Flower();

		virtual void		Initialise();
		virtual void		Shutdown();

		virtual void		Update();
		virtual void		Render();
		void				CollectionChange(bool changeValue);
		bool				IsFlowerInUse();
		bool				CollectNectar(float rate);

	private:
		static const UINT	NumVertices = 18;
		static const UINT	NumPetals = 6;
		static const UINT	NumVerticesPerPetal = 3;
		float				BaseRadius = 1.0f;
		bool				m_beingCollected;
		float				m_nectarStored;
		float				m_maxNectarStored;
		float				m_nectarGainRate;
	};

} // namespace scene
