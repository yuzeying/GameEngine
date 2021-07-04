//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObject.h"
#include "Game.h"
#include "GraphicsObject.h"
#include "Model.h"

namespace Azul
{
	GameObject::GameObject(GraphicsObject *pGraphicsObject)
		: poGraphicsObject(pGraphicsObject)
	{
		this->poWorld = new Matrix(Matrix::Special::Identity);
		assert(poWorld);

		assert(pGraphicsObject != nullptr);

		this->mDrawEnable = true;
	}

	GameObject::~GameObject()
	{
		delete this->poWorld;
		delete this->poGraphicsObject;
	}

	bool GameObject::GetDrawEnable()
	{
		return this->mDrawEnable;
	}

	void GameObject::SetDrawEnable(bool val)
	{
		this->mDrawEnable = val;
	}

	void GameObject::baseUpdateBoundingSphere()
	{
		// Get the ref Sphere
		Sphere *pRefSphere = poGraphicsObject->GetModel()->poRefSphere;
		assert(pRefSphere);

		// Get the curr Sphere
		Sphere *pCurrSphere = poGraphicsObject->poCurrSphere;
		assert(pCurrSphere);

		// Update... cntr process through the world
		// Assuming a uniform scaling
		Vect A = pRefSphere->cntr;
		Vect radius(1.0f, 0.0f, 0.0f);
		Vect B = A + radius;
		Vect A_out = A * (*this->poWorld);
		Vect B_out = B * (*this->poWorld);

		pCurrSphere->rad = (B_out[x] - A_out[x]) * pRefSphere->rad;
		pCurrSphere->cntr = A_out;
	}

	Matrix *GameObject::GetWorld()
	{
		return this->poWorld;
	}

	GraphicsObject *GameObject::GetGraphicsObject()
	{
		return this->poGraphicsObject;
	}

	void GameObject::SetWorld(Matrix *_pWorld)
	{
		assert(_pWorld);
		*this->poWorld = *_pWorld;
	}

	void GameObject::Draw()
	{
		GraphicsObject *pGraphicsObj = this->GetGraphicsObject();
		assert(pGraphicsObj);

		pGraphicsObj->Render();
	}

}

// --- End of File ---
