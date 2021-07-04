//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_MAN_H
#define MODEL_MAN_H

#include "Model.h"
#include <PyramidModel.h>

namespace Azul
{
	class ModelMan
	{
	public:
		static void Add(Model* pModel);
		static void Create();
		static void Destroy();

		bool wasAdded = false;
		PyramidModel* pP = nullptr;
		static PyramidModel* GetPyramid() {
			ModelMan* pModelMan = ModelMan::privGetInstance();
			assert(pModelMan);
			return pModelMan->pP;
		}
		static bool GetWasPyramidAdded() {
			ModelMan* pModelMan = ModelMan::privGetInstance();
			assert(pModelMan);
			return pModelMan->wasAdded;
		}
		static void SetWasPyramidAdded() {
			ModelMan* pModelMan = ModelMan::privGetInstance();
			assert(pModelMan);
			pModelMan->wasAdded = true;
		}
	private:  // methods

		static ModelMan* privGetInstance();
		ModelMan();

		void privAddToFront(Model* node, Model*& head);
		void privRemove(Model* node, Model*& head);

	private:  // add

		Model* active;

	};

}

#endif

// ---  End of File ---
