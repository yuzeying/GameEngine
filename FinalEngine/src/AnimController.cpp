//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimController.h"
#include "Anim.h"

namespace Azul
{
	AnimController::AnimController(Time delta)
	{
		// Animation object
		this->poAnim = new Anim();
		assert(this->poAnim);

		// TimerController
		Time maxTime = poAnim->FindMaxTime();
		Time minTime = Time(Time::Duration::ZERO);
		this->poTimerControl = new TimerController(delta, minTime, maxTime);
		assert(this->poTimerControl);

	}

	AnimController::AnimController(Time delta, Anim *pAnim)
	{
		// Animation object
		this->poAnim = pAnim;
		assert(this->poAnim);

		// TimerController
		Time maxTime = poAnim->FindMaxTime();
		Time minTime = Time(Time::Duration::ZERO);
		this->poTimerControl = new TimerController(delta, minTime, maxTime);
		assert(this->poTimerControl);

	}

	AnimController::~AnimController()
	{
		delete poAnim;
		delete poTimerControl;
	}

	void AnimController::Update()
	{
		this->poTimerControl->Update();

		// interpolate all bones
		this->poAnim->Animate(this->poTimerControl->GetCurrTime());

		// pose skeleton
		this->poAnim->PoseSkeleton(this->poTimerControl->GetCurrTime());
	}
}

// --- End of File ---
