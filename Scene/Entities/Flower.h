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

		//Sets the flowers availability to "changeValue".
		void				CollectionChange( bool changeValue ); 
		//Check whether the target flower is available to use - Returns true if in use.
		bool				IsFlowerInUse();
		//Removes the "rate" from the target flower - Returns true if more nectar is available.
		bool				CollectNectar( float rate );

	private:

		static const UINT	NumPetals = 20;				//Int for how many petals each plant has.
		static const UINT	NumVerticesPerPetal = 3;	//Inf for how many vertices each petal has.
		static const UINT	NumVertices = NumPetals * NumVerticesPerPetal; //Int that calculates and stores how many vertices each flower has. 
		float				BaseRadius = 0.75f;

		DirectX::XMFLOAT4	m_insideColour = { 0.0f,0.0f ,0.0f ,0.0f };		//Color of the inside vertices of the flower;
		DirectX::XMFLOAT4	m_outsideColour = { 0.0f, 0.0f, 0.0f, 0.0f };	//Color of the outside vertices of the flower;

		bool				m_beingCollected;		//Bool used to check whether a plant is in use or not
		float				m_nectarStored;			//Float used to keep track of how much nectar the flower has
		float				m_maxNectarStored;		//Float used to keep track of the max nectar that the plant can store
		float				m_nectarGainRate;		//Float used to determine the rate nectar is gained
	};

} // namespace scene
