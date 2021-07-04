
//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Clip.h"
#include "Time.h"
#include "Bone.h"
#include "Mixer.h"

namespace Azul
{
	Clip::Clip(int _numBones)
		: numBones(_numBones),
		numFrames(0),
		TotalTime(Time::Duration::ZERO),
		poHead(nullptr)
	{
		this->privSetAnimationData();
		this->TotalTime = this->privFindMaxTime();
		this->numFrames = this->privFindNumFrames();
	}

	Clip::Clip(int _numBones, FrameBucket* bucketHead)
		: numBones(_numBones),
		numFrames(0),
		TotalTime(Time::Duration::ZERO),
		poHead(bucketHead)
	{
		this->privSetAnimationData();
		this->TotalTime = this->privFindMaxTime();
		this->numFrames = this->privFindNumFrames();
	}

	Clip::~Clip()
	{
		FrameBucket *pTmp = this->poHead;

		while (pTmp != nullptr)
		{
			FrameBucket *pDeleteMe = pTmp;
			pTmp = pTmp->nextBucket;
			delete pDeleteMe;
		}
	}

	void Clip::AnimateBones(Time tCurr, Bone *pResult)
	{
		// First one 
		FrameBucket *pTmp = this->poHead;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		FrameBucket *pA = pTmp->prevBucket;
		FrameBucket *pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		Mixer::Blend(pResult, pA->poBone, pB->poBone, tS, this->numBones);

	}

	int Clip::privFindNumFrames()
	{
		int count = 0;
		FrameBucket *pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			count++;
			pTmp = pTmp->nextBucket;
		}
		return count;
	}

	Time Clip::privFindMaxTime()
	{
		Time tMax;
		FrameBucket *pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;

		return tMax;
	}

	Time Clip::GetTotalTime()
	{
		return this->TotalTime;
	}

	void Clip::privSetAnimationData()
	{
		// USING ALREADY LOADED DATA
		if (this->poHead != nullptr) return;
	}
}

// --- End of File ---
