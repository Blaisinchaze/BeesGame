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

		//Rotates bee to face "targetLocation" and apply the bee's "m_movementSpeed" to it's forward vector
		void				MoveTowards( DirectX::XMVECTOR targetLocation );
		//Sets the "m_startingHive" as a reference for the bee to access later.
		void				SetStartingHive( Hive* startingHive );
		//Checks whether the bee has been marked be cleaned up by the scene.
		bool				IsMarkedForKill();


	private:		
		//States used for bees to determine what action they should do each frame of Update()
		enum BeeStates
		{
			SPAWNING,		//Initial state for the bees
			SEARCHING,		//Searching for a flower to store as the bee's target at m_targetFlower
			TRAVELLING,		//Move the bee to 1.0f above the target flower at the rate of m_movemenetSpeed
			ARRIVING,		//Check weather the flower is occupied or available - if it is, set it to unavailable, if not return to searching
			DESCEND,		//Move towards target flower
			COLLECTING,		//Collect nectar from the target flower at the rate of m_collectionSpeed
			RETURNING,		//Return to the original hive the bee spawned from
			DEPOSIT,		//Remove all nectar from bee and increase hive's stored nectar
			DESTORY			//mark the bee to be cleaned up by the spawn by changing m_markedFor Kill
		};

		BeeStates					m_currentState = SPAWNING;
		Flower*						m_targetFlower;
		Hive*						m_startingHive;

		static const UINT			NumVertices = 36;
		
		DirectX::XMVECTOR			m_startingPosition = { 0,0,0,0 };
		float						m_movementSpeed;			//The rate of units that the bee moves
		float						m_collectionSpeed;			//The rate at which the bee removes nectar
		float						m_holdingNectar;			//The amount of nectar the bee is holding

		bool						m_markedForKill = false;	//Returned value from "IsMarkedForKill()" - Used for cleanup
	};

} // namespace scene
#pragma once
