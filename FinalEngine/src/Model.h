//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_H
#define MODEL_H

#include "sb7.h"
#include "ModelLink.h"
#include "BoundingSphere.h"

namespace Azul
{
	class Model : public ModelLink
	{
	public:
		Model();
		Model(Model& copyModel) = delete;
		Model& operator = (Model& copyModel) = delete;
		virtual ~Model();

		// Data
		int numVerts;
		int numTris;    // Keenan(20)

		GLuint vao;
		GLuint vbo_0;   // xyzuvn
		GLuint vbo_1;   // trilist

		// Reference sphere... before any transformations are applied
		Sphere *poRefSphere;


	protected:
		// load VAO
		virtual void privCreateVAO(const char* const pModelFileName) = 0;

	private:


	};

}

#endif

// --- End of File ---
