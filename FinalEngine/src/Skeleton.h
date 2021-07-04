//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKELETON_H
#define SKELETON_H

#include "GameObjectAnim.h"
#include "Bone.h"

namespace Azul
{
	class Skeleton
	{
	public:
		struct Data
		{
			char name[64];
			int  index;
			int  parentIndex;
		};

	public:
		Skeleton(Bone *pBone, int numBones);

		Skeleton(Bone* pBone, int _numBones, Data* skelData);

		Skeleton() = delete;
		Skeleton(const Skeleton &) = delete;
		Skeleton &operator = (const Skeleton &) = delete;
		~Skeleton();

		GameObjectAnim *GetFirstBone();
		
	private:
		Data* skel;
		void privSetAnimationHierarchy(Bone *pBone);
		GameObjectAnim *privFindBoneByIndex(int index);

	private:
		GameObjectAnim *pFirstBone;
		int             numBones;
	};
}

#endif

// --- End of File ---
