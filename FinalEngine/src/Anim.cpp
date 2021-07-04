//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Anim.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Mixer.h"
#include "PCSTreeForwardIterator.h"
#include "GameObjectMan.h"
#include "Clip.h"

namespace Azul
{
	//extern Clip *pRunClip;

	Anim::Anim()
		:	pClip(nullptr),
		poBoneResult(nullptr),
		poSkeleton(nullptr),
		numBones(NUM_BONES),
		boneWidth(BONE_WIDTH)
	{
		this->poBoneResult = new Bone[(unsigned int)numBones]();
		assert(this->poBoneResult);

		this->poSkeleton = new Skeleton(this->poBoneResult, numBones);
		assert(this->poSkeleton);

		// Get this from the AnimMan
		this->pClip = nullptr; // BROKE THIS INTENTIALLY

		assert(pClip);
	}

	Anim::Anim(Clip * prClip, Bone *pBoneResult, Skeleton *pSkel, int numBones, float boneWidth) : 
		pClip(prClip),
		poBoneResult(pBoneResult),
		poSkeleton(pSkel),
		numBones(numBones),
		boneWidth(boneWidth) {

	}

	Anim::~Anim()
	{
		delete[] poBoneResult;
		delete poSkeleton;
	}

	Time Anim::FindMaxTime()
	{
		assert(pClip);
		return this->pClip->GetTotalTime();
	}

	void Anim::Animate(Time tCurr)
	{
		this->pClip->AnimateBones(tCurr, this->poBoneResult);
	}

	void Anim::PoseSkeleton(Time tCurr)
	{
		PCSTreeForwardIterator pIter(this->poSkeleton->GetFirstBone());
		PCSNode *pNode = pIter.First();
		GameObjectAnim *pGameObj = nullptr;

		// walks the anim node does the pose for everything that
		while (pNode != nullptr)
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObjectAnim *)pNode;
			privSetBonePose(pGameObj);

			pNode = pIter.Next();
		}
	}

	void Anim::privSetBonePose(GameObjectAnim *node)
	{
		// Now get the world matrices
		// getting the parent from current node
		GameObjectAnim *childNode = (GameObjectAnim *)node;
		GameObjectAnim *parentNode;
		if (node->GetParent()->GetParent()->GetParent() != nullptr)
		{
			parentNode = (GameObjectAnim *)node->GetParent();
		}
		else
		{
			parentNode = nullptr;
		}

		PCSTree *pTree = GameObjectMan::GetPCSTree();
		assert(pTree);
		if (parentNode == pTree->GetRoot())
			return;

		if (parentNode != nullptr && childNode != nullptr)
		{
			// starting point
			Vect start(0.0f, 0.0f, 0.0f);

			//  At this point, Find the two bones initial positions in world space
			//  Now get the length and directions

			Vect ptA = start * *parentNode->GetWorld();
			Vect ptB = start * *childNode->GetWorld();

			// direction between the anchor points of the respective bones
			Vect dir = (ptA - ptB);

			// length of the bone 0
			float mag = dir.mag();

			Matrix S(Matrix::Scale::XYZ, BONE_WIDTH, BONE_WIDTH, mag);
			Quat Q(Quat::Orient::LocalToWorld, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f));
			Matrix T(Matrix::Trans::XYZ, ptB);

			Matrix BoneOrient = S * Q * T;

			childNode->SetBoneOrientation(BoneOrient);
		}
	}


}

//--- End of File ----
