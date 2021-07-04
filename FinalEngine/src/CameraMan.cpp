//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Camera.h"
#include "CameraNode.h"
#include "CameraMan.h"

namespace Azul
{
	CameraMan::CameraMan()
	{
		this->poActive = nullptr;
		this->currCamera = nullptr;
	}

	CameraMan *CameraMan::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static CameraMan cameraMan;
		return &cameraMan;
	}

	void CameraMan::Create()
	{
		// Get the instance to the manager
		CameraMan *pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);
		AZUL_UNUSED_VAR(pCameraMan);
	}

	void CameraMan::Destroy()
	{
		// Get the instance to the manager
		CameraMan *pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);

		CameraLink *pTmp = pCameraMan->poActive;
		while (pTmp != nullptr)
		{
			CameraLink *pDeleteMe = pTmp;
			pTmp = pTmp->next;
			delete pDeleteMe;
		}

	}

	void CameraMan::Add(Camera::Name name, Camera *pCamera)
	{
		// Get the instance to the manager
		CameraMan *pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);

		// Create a TextureNode
		CameraNode *pNode = new CameraNode();
		assert(pNode);

		// initialize it
		assert(pCamera);
		pCamera->setName(name);
		pNode->set(pCamera);

		// Now add it to the manager
		pCameraMan->privAddToFront(pNode, pCameraMan->poActive);
	}

	Camera *CameraMan::Find(Camera::Name _name)
	{
		// Get the instance to the manager
		CameraMan *pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);

		CameraNode *pNode = (CameraNode *)pCameraMan->poActive;
		while (pNode != 0)
		{
			if (pNode->poCamera->getName() == _name)
			{
				// found it
				break;
			}

			pNode = (CameraNode *)pNode->next;
		}
		assert(pNode);
		return pNode->poCamera;
	}

	void CameraMan::SetCurrent(const Camera::Name name)
	{
		CameraMan *pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);

		Camera *pCam = CameraMan::Find(name);
		assert(pCam);

		pCameraMan->currCamera = pCam;
	}

	Camera *CameraMan::GetCurrent()
	{
		CameraMan *pCameraMan = CameraMan::privGetInstance();
		assert(pCameraMan);

		assert(pCameraMan->currCamera);
		return pCameraMan->currCamera;
	}

	void CameraMan::privAddToFront(CameraLink *node, CameraLink *&head)
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

	void CameraMan::privRemove(CameraLink *pNode, CameraLink *&poHead)
	{
		assert(pNode);

		if (pNode->prev != nullptr)
		{	// middle or last node
			pNode->prev->next = pNode->next;
		}
		else
		{  // first
			poHead = (CameraLink *)pNode->next;
		}

		if (pNode->next != nullptr)
		{	// middle node
			pNode->next->prev = pNode->prev;
		}
	}

}

// --- End of File ---
