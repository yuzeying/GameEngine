//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Camera.h"
#include "CameraNode.h"

namespace Azul
{
	// Singleton
	class CameraMan
	{
	public:
		static void Add(Camera::Name name, Camera *pCamera);
		static Camera *Find(Camera::Name name);
		static void SetCurrent(Camera::Name name);
		static Camera *GetCurrent();

		static void Create();
		static void Destroy();

	private:  // methods

		static CameraMan *privGetInstance();
		CameraMan();
		void privAddToFront(CameraLink *node, CameraLink *&head);
		void privRemove(CameraLink *pNode, CameraLink *&poHead);

	private:  // add

		CameraLink *poActive;
		Camera *currCamera;

	};

}

#endif

// --- End of File ---
