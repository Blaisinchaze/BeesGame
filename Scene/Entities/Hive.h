#pragma once

#include "Scene\Entity.h"

namespace scene
{

	class Hive : public Entity
	{
	public:
		Hive();

		virtual void		Initialise();
		virtual void		Shutdown();

		virtual void		Update();
		virtual void		Render();
		void				AddNectar(float nectarToStore);

	private:
		static const UINT	NumVertices = 32;
		float				m_spawnTimer;
		int					m_beeCount;
		float				m_collectedNectar;
		DirectX::XMFLOAT4	m_colour;
		
	};

} // namespace scene
#pragma once
