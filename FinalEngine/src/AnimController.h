//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "TimerController.h"
#include "GameObjectAnim.h"
#include "Anim.h"

namespace Azul
{
	class AnimController
	{
	public:
		AnimController(Time delta);
		AnimController(Time delta, Anim *pAnim);
		AnimController(const AnimController &) = delete;
		AnimController &operator = (const AnimController &) = delete;
		~AnimController();

		void Update();

	private:
		TimerController *poTimerControl;
		Anim            *poAnim;

	};
}

#endif

// --- End of File ---
