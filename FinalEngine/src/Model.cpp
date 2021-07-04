//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Model.h"

namespace Azul
{
	Model::Model()
		: numVerts(0),
		numTris(0),    // Keenan(20)
		vao(0),
		vbo_0(0),
		vbo_1(0),
		poRefSphere(new Sphere())
	{
	}

	Model::~Model()
	{
		delete this->poRefSphere;
	}
}

// --- End of File ---
