#pragma once

#include "Scene\Entity.h"
#include "Flower.h"
#include "Hive.h"
namespace scene
{
	class Flower;
	class Hive;
	class Bee : public Entity
	{
	public:
		Bee();

		virtual void		Initialise();
		virtual void		Shutdown();

		virtual void		Update();
		virtual void		Render();

		void				MoveTowards(DirectX::XMVECTOR targetLocation);
		bool				setToDestory = false;
		void				SetStartingHive(Hive* startingHive);

	private:		
		enum BeeStates
		{
			SPAWNING, 
			SEARCHING, 
			TRAVELLING, 
			ARRIVING,
			COLLECTING,
			RETURNING, 
			DEPOSIT,
			DESTORY
		};
		static const UINT			NumVertices = 36;
		DirectX::XMVECTORF32		m_targetPosition = {0,0,0,0};
		DirectX::XMVECTOR			m_startingPosition = {0,0,0,0};
		float						m_movementSpeed;
		float						m_collectionSpeed;
		float						m_holdingNectar;
		BeeStates					m_currentState = SPAWNING;
		Flower*						m_targetFlower;
		Hive*						m_startingHive;
	};

} // namespace scene
#pragma once
