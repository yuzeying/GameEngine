//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectRigid.h"
#include "GraphicsObject.h"

namespace Azul
{
	GameObjectRigid::GameObjectRigid(GraphicsObject *pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject != 0);

		this->pScale = new Vect(1.0f, 1.0f, 1.0f);
		assert(this->pScale);

		this->pPos = new Vect(0.0f, 0.0f, 0.0f);
		assert(this->pPos);

		this->angle = 0.0f;
	}

	GameObjectRigid::~GameObjectRigid()
	{
		delete this->pPos;
		delete this->pScale;
	}

	void GameObjectRigid::privUpdate(Time)
	{
		// for Humanoid
		Vect Trans = Vect(-280.172638f, 1288.917847f - 500, 526.212402f);
		Matrix T = Matrix(Matrix::Trans::XYZ, -Trans);
		Matrix M = T;
		Matrix R = Matrix(Matrix::Rot1::Z, 0);
		Matrix S = Matrix(Matrix::Scale::XYZ, 0.5, 0.5, 0.5);

		*this->poWorld = M * R;


		// Special transform for Teddy
		//Matrix Rx = Matrix(Matrix::Rot1::X, MATH_PI2);
		//Matrix Rz = Matrix(Matrix::Rot1::Z, 1.7f);
		//Matrix M = Rx * Rz;

		//*this->poWorld = M;
	}

	void GameObjectRigid::Update(Time currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		this->baseUpdateBoundingSphere();

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}
}

// --- End of File ---
