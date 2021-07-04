//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "PyramidModel.h"
#include "GraphicsObject_TextureLight.h"
#include "GameObjectMan.h"
#include "ModelMan.h"
#include "ShaderMan.h"
#include "GameObjectRigid.h"
#include "GraphicsObject_Null.h"
#include "NullModel.h"
#include "Skeleton.h"
#include "PCSTreeForwardIterator.h"
#include "GameObjectMan.h"

namespace Azul
{
	// Needs to move to binary file


	Skeleton::Skeleton(Bone *pBone, int _numBones)
	: pFirstBone(nullptr),
		numBones(_numBones)
	{
		skel = nullptr;
		this->privSetAnimationHierarchy(pBone);
		assert(pFirstBone);
	}

	Skeleton::Skeleton(Bone* pBone, int _numBones, Data* skelData)
		: pFirstBone(nullptr),
		numBones(_numBones)
	{
		skel = skelData;
		this->privSetAnimationHierarchy(pBone);
		assert(pFirstBone);
	}


	Skeleton::~Skeleton()
	{

	}

	GameObjectAnim *Skeleton::GetFirstBone()
	{
		assert(pFirstBone);
		return this->pFirstBone;
	}

	GameObjectAnim *Skeleton::privFindBoneByIndex(int index)
	{
		GameObjectAnim *pFound = nullptr;

		GameObjectAnim *pObj = this->GetFirstBone();

		// TODO - add test bed for an interator of a tree with only one node
		if (pObj->indexBoneArray == index)
		{
			pFound = pObj;
		}
		else
		{
			PCSTreeForwardIterator pIter(pObj);
			PCSNode *pNode = pIter.First();
			GameObjectAnim *pGameObj = nullptr;

			// walks the anim node does the pose for everything that
			while (pNode != nullptr)
			{
				assert(pNode);
				// Update the game object
				pGameObj = (GameObjectAnim *)pNode;
				if (pGameObj->indexBoneArray == index)
				{
					pFound = pGameObj;
					break;
				}
				pNode = pIter.Next();
			}
		}

		return pFound;
	}

	void Skeleton::privSetAnimationHierarchy(Bone *pBone)
	{
		PyramidModel* pPyramidModel = ModelMan::GetPyramid();
		if (!ModelMan::GetWasPyramidAdded()) {
			// Load the model
			pPyramidModel = new PyramidModel("pyramidModel.azul");
			assert(pPyramidModel);
			ModelMan::Add(pPyramidModel);
			ModelMan::SetWasPyramidAdded();
		}


		// Create/Load Shader 
		ShaderObject *pShaderObject_textureLight = new ShaderObject(ShaderObject::Name::TEXTURE_POINT_LIGHT, "texturePointLightDiff");
		assert(pShaderObject_textureLight);
		ShaderMan::Add(pShaderObject_textureLight);

		ShaderObject *pShaderObject_null = new ShaderObject(ShaderObject::Name::NULL_SHADER, "nullRender");
		assert(pShaderObject_null);
		ShaderMan::Add(pShaderObject_null);

		// GraphicsObject for a specific instance
		GraphicsObject_TextureLight *pGraphics_TextureLight;

		// Create GameObject
		Vect color;
		Vect pos(1, 1, 1);

		// Null object
		NullModel *pNullModel = new NullModel(0);
		ModelMan::Add(pNullModel);
		GraphicsObject_Null *pGraphicsObjNull = new GraphicsObject_Null(pNullModel, pShaderObject_null);
		GameObjectRigid *pGameRigid = new GameObjectRigid(pGraphicsObjNull);
		pGameRigid->SetName("Rigid");
		GameObjectMan::Add(pGameRigid, GameObjectMan::GetRoot());

		// Human from txt file
		color.set(1.0f, 0.0f, 0.0f, 1.0f);

		pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
		GameObjectAnim *pObj = new GameObjectAnim(pGraphics_TextureLight, pBone);
		pObj->SetIndex(skel[0].index);
		pObj->SetName(skel[0].name);
		GameObjectMan::Add(pObj, pGameRigid);
		this->pFirstBone = pObj;

		for (int i = 1; i < this->numBones; i++)
		{
			pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
			pObj = new GameObjectAnim(pGraphics_TextureLight, pBone);
			pObj->SetIndex(skel[i].index);
			pObj->SetName(skel[i].name);

			GameObjectAnim *pParent = this->privFindBoneByIndex(skel[i].parentIndex);
			assert(pParent);
			GameObjectMan::Add(pObj, pParent);
		}

	}
}

// --- End of File ---
