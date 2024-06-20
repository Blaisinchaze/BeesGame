#pragma once

#include "Scene\Entity.h"

namespace scene
{

	class Ground : public Entity
	{
	public:
		Ground();

		virtual void		Initialise();
		virtual void		Shutdown();

		virtual void		Update();
		virtual void		Render();

	private:
		static const UINT NumVertices = 36;

	};

} // namespace scene
