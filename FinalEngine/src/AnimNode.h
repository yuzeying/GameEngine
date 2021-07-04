//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Anim_NODE_H
#define Anim_NODE_H

namespace Azul
{
	class Anim;

	class AnimLink
	{
	public:
		AnimLink()
		{
			this->next = nullptr;
			this->prev = nullptr;
		}

		virtual ~AnimLink() = default;

		AnimLink* next;
		AnimLink* prev;
	};

	class AnimNode : public AnimLink
	{
	public:
		AnimNode();
		~AnimNode();
		void set(Anim* pAnim);

	public:
		Anim* poAnim;
	};

}

#endif

// --- End of File ---
