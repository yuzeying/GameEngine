//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TIMER_CONTROLLER_H
#define TIMER_CONTROLLER_H

#include "Timer.h"

namespace Azul
{
	class TimerController
	{
	public:
		TimerController(Time Delta, Time Min, Time Max);

		TimerController() = delete;
		TimerController(const TimerController &) = delete;
		TimerController &operator = (const TimerController &) = delete;
		~TimerController() = default;

		void Update();

		// Accessors
		void SetCurrTime(const Time time);
		Time GetCurrTime() const;
		void SetDeltaTime(const Time time);
		Time GetDeltaTime() const;
		void SetMaxTime(const Time time);
		Time GetMaxTime() const;
		void SetMinTime(const Time time);
		Time GetMinTime() const;

	private:
		Time tCurr;
		Time tDelta;
		Time tMax;
		Time tMin;
	};
}

#endif

// --- End of File ---

