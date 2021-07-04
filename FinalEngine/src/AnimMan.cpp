//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimMan.h"
#include "Anim.h"
#include <AnimController.h>
namespace Azul
{
	AnimMan::AnimMan()
	{
	}

	AnimMan* AnimMan::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static AnimMan AnimMan;
		return &AnimMan;
	}

	void AnimMan::Create()
	{
		// Get the instance to the manager
		AnimMan* pCameraMan = AnimMan::privGetInstance();
		assert(pCameraMan);
		AZUL_UNUSED_VAR(pCameraMan);
		pCameraMan->curIndex = 0;
	}

	void AnimMan::Add(Anim* pAnim, Time delta)
	{
		// Get the instance to the manager
		AnimMan* pCameraMan = AnimMan::privGetInstance();
		assert(pCameraMan);

		// Create a TextureNode
		AnimNode* pNode = new AnimNode();
		assert(pNode);

		// initialize it
		assert(pAnim);
		pNode->set(pAnim);

		// Now add it to the manager
		pCameraMan->privAddToFront(pNode, pCameraMan->poActive);
		pCameraMan->times[pCameraMan->curIndex] = delta;
		pCameraMan->controllers[pCameraMan->curIndex] = new AnimController(delta, pAnim);
		pCameraMan->curIndex++;
	}

	void AnimMan::Destroy()
	{
		// Get the instance to the manager
		AnimMan* pCameraMan = AnimMan::privGetInstance();
		assert(pCameraMan);

		AnimLink* pTmp = pCameraMan->poActive;
		while (pTmp != nullptr)
		{
			AnimLink* pDeleteMe = pTmp;
			pTmp = pTmp->next;
			delete pDeleteMe;
		}

	}

	void AnimMan::Update()
	{
		AnimMan* pCameraMan = AnimMan::privGetInstance();
		assert(pCameraMan);

		if (pCameraMan->shouldPlay) {
			for (int i = 0; i < pCameraMan->curIndex; i++) {
				pCameraMan->controllers[i]->Update();
			}
		}
	}

	void AnimMan::Toggle(bool changeTo)
	{
		AnimMan* pCameraMan = AnimMan::privGetInstance();
		assert(pCameraMan);

		pCameraMan->shouldPlay = changeTo;
	}

	void AnimMan::privAddToFront(AnimLink* node, AnimLink*& head)
	{
		assert(node != nullptr);

		if (head == nullptr)
		{
			head = node;
			node->next = nullptr;
			node->prev = nullptr;
		}
		else
		{
			node->next = head;
			head->prev = node;
			head = node;
		}
	}

	void AnimMan::privRemove(AnimLink* pNode, AnimLink*& poHead)
	{
		assert(pNode);

		if (pNode->prev != nullptr)
		{	// middle or last node
			pNode->prev->next = pNode->next;
		}
		else
		{  // first
			poHead = (AnimLink*)pNode->next;
		}

		if (pNode->next != nullptr)
		{	// middle node
			pNode->next->prev = pNode->prev;
		}
	}

}

// --- End of File ---
