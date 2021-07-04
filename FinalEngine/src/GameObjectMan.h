//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObject.h"
#include "PCSTree.h"
#include "Time.h"

namespace Azul
{

	// Singleton
	class GameObjectMan
	{
	public:
		static void Add(GameObject *pObj, GameObject *pParent);
		static void Draw(void);
		static void Update(Time currentTime);
		static GameObject *GetRoot(void);
		static PCSTree *GetPCSTree();

		static void Destroy();
		static void Create();
		~GameObjectMan();

	private:
		GameObjectMan();

		static GameObjectMan* privGetInstance();

		// data
		PCSTree* poRootTree;
	};

}

#endif


// --- End of File ---
