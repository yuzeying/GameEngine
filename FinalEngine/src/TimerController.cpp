//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TimerController.h"
#include "AnimMan.h"

namespace Azul
{

	TimerController::TimerController(Time Delta, Time Min, Time Max)
		: tCurr(Time::Duration::ZERO),
		tDelta(Delta),
		tMax(Max),
		tMin(Min)
	{
	}

	void TimerController::Update()
	{
		Time ttDelta = tDelta;
		if (AnimMan::getShouldReverse()) {
			ttDelta *= -1;
		}
		
		if (AnimMan::getShouldSlow()) {
			if (AnimMan::getShouldSlow() == 2) {
				ttDelta *= 3;
			}
			else {
				ttDelta /= 3;
			}
		}

		tCurr += ttDelta;

		// protection for time values for looping
		if (tCurr < tMin)
		{
			tCurr = tMax;
		}
		else if (tCurr > tMax)
		{
			tCurr = tMin;
		}
		else
		{
			// do nothing
		}
	}

	void TimerController::SetCurrTime(const Time time)
	{
		this->tCurr = time;
	}

	Time TimerController::GetCurrTime() const
	{
		return this->tCurr;
	}

	void TimerController::SetDeltaTime(const Time time)
	{
		this->tDelta = time;
	}

	Time TimerController::GetDeltaTime() const
	{
		return this->tDelta;
	}

	void TimerController::SetMaxTime(const Time time)
	{
		this->tMax = time;
	}

	Time TimerController::GetMaxTime() const
	{
		return this->tMax;
	}

	void TimerController::SetMinTime(const Time time)
	{
		this->tMin = time;
	}

	Time TimerController::GetMinTime() const
	{
		return this->tMin;
	}

}

// --- End of File ---
