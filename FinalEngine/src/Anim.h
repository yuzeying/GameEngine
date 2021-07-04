//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_H
#define ANIM_H

#include "Timer.h"
#include "GameObjectAnim.h"
#include "Clip.h"
#include "Skeleton.h"

namespace Azul
{ 
	// For Teddy
	//#define NUM_BONES 20
	//#define BONE_WIDTH 10.0f

	// For Human
	#define NUM_BONES 80
	#define BONE_WIDTH 30.0f

	class Anim
	{
	public:
		Anim();
		Anim(Clip* prClip, Bone* pBoneResult, Skeleton* pSkel, int numBones, float boneWidth);
		Anim(const Anim &) = delete;
		Anim &operator = (const Anim &) = delete;
		~Anim();

		void Animate(Time tCurr);
		void PoseSkeleton(Time tCurr);
		Time FindMaxTime();

	private:
		void privSetBonePose(GameObjectAnim *node);

	private:
		Clip           *pClip;
		Bone           *poBoneResult;
		Skeleton       *poSkeleton;

		int   numBones;
		float boneWidth;
	};


}

#endif

// --- End of File ---
