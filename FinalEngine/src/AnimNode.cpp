//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimNode.h"
#include "Anim.h"

namespace Azul
{
	AnimNode::AnimNode()
	{
		this->poAnim = nullptr;
	}

	AnimNode::~AnimNode()
	{
		delete this->poAnim;
	}

	void AnimNode::set(Anim* pAnim)
	{
		assert(pAnim);
		this->poAnim = pAnim;
	}

}

// --- End of File ---
