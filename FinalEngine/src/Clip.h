//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_H
#define CLIP_H

#include "Bone.h"

namespace Azul
{
	class Clip
	{
	public:
		struct FrameBucket
		{
			~FrameBucket()
			{
				delete[] poBone;
			}

			FrameBucket *nextBucket;
			FrameBucket *prevBucket;
			Time        KeyTime;
			Bone        *poBone;
			char        pad[4];
		};

	public:
		Clip(int _numBones);
		Clip(int _numBones, FrameBucket* bucketHead);
		Clip() = delete;
		Clip(const Clip &) = delete;
		Clip &operator = (const Clip &) = delete;
		~Clip();

		Time GetTotalTime();
		void AnimateBones(Time tCurr, Bone *pResult);

	private:
		void privSetAnimationData();
		Time privFindMaxTime();
		int  privFindNumFrames();

	private:
		int          numBones;
		int          numFrames;
		Time         TotalTime;
		FrameBucket *poHead;
	};
}

#endif

//--- End of File ---
