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
		//Void function that allows returning bees to deposit their sourced nectar - called from the bee itself 
		void				AddNectar( float nectarToStore );

	private:
		static const UINT	NumVertices = 32;
		float				m_spawnTimer;			//Rate at which bees are spawned
		int					m_beeCount;				//The number of currently dispatched bees from this hive
		float				m_collectedNectar;		//The amount of nectar deposited by bees - used by the "AddNectar" function.
		DirectX::XMFLOAT4	m_colour;				//The colour of which the hive is spawned at
		
	};

} // namespace scene
#pragma once
