//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <AnimNode.h>
#include "AnimController.h"
#ifndef ANIM_MANAGER_H
#define ANIM_MANAGER_H


namespace Azul
{
	// Singleton
	class AnimMan
	{
	public:
		static void Create();
		static void Destroy();
		static void Update();
		static void Add(Anim* pAnim, Time delta);
		AnimController* controllers[100];
		int curIndex = 0;
		Time times[100];
		bool shouldPlay = true;
		bool shouldReverse = false;
		int shouldSlow = false;
		static void Toggle(bool changeTo);
		static void ToggleReverse(bool changeTo) {
			AnimMan* pCameraMan = AnimMan::privGetInstance();
			assert(pCameraMan);
			pCameraMan->shouldReverse = changeTo;
		}
		static void ToggleSlow(int changeTo) {
			AnimMan* pCameraMan = AnimMan::privGetInstance();
			assert(pCameraMan);
			pCameraMan->shouldSlow = changeTo;
		}
		static bool getShouldReverse() {
			AnimMan* pCameraMan = AnimMan::privGetInstance();
			assert(pCameraMan);
			return pCameraMan->shouldReverse;
		}
		static int getShouldSlow() {
			AnimMan* pCameraMan = AnimMan::privGetInstance();
			assert(pCameraMan);
			return pCameraMan->shouldSlow;
		}
	private:  // methods

		static AnimMan* privGetInstance();
		AnimMan();
		void privAddToFront(AnimLink* node, AnimLink*& head);
		void privRemove(AnimLink* pNode, AnimLink*& poHead);

	private:  // add
		AnimLink* poActive;
		Anim* currAnim;

	};

}

#endif

// --- End of File ---
