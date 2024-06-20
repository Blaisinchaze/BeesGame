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
		float				BaseRadius = 0.75f;
		DirectX::XMFLOAT4	m_insideColour = { 0.0f,0.0f ,0.0f ,0.0f };
		DirectX::XMFLOAT4	m_outsideColour = { 0.0f, 0.0f, 0.0f, 0.0f };

		bool				m_beingCollected = false;
		float				m_nectarStored = 50.0f;
		float				m_maxNectarStored = 0.0f;
		float				m_nectarGainRate = 2.0f;
	};

} // namespace scene
