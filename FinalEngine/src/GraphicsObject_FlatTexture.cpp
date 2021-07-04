//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject_FlatTexture.h"
#include "Texture.h"
#include "Model.h"
#include "CameraMan.h"

namespace Azul
{
	GraphicsObject_FlatTexture::GraphicsObject_FlatTexture(const Model* const _pModel, const ShaderObject* const _pShaderObj, Texture::Name _name)
		: GraphicsObject(_pModel, _pShaderObj), textName(_name)
	{
		// check the base class
		assert(this->pModel);
		assert(this->pShaderObj);
		assert(this->poWorld);
	}

	void GraphicsObject_FlatTexture::SetState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		GLuint textureID = Texture::Find(this->textName);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

	void GraphicsObject_FlatTexture::SetDataGPU()
	{
		// use this shader
		this->pShaderObj->SetActive();

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		Camera *pCam = CameraMan::GetCurrent();

		Matrix world = this->GetWorld();
		Matrix view = pCam->getViewMatrix();
		Matrix proj = pCam->getProjMatrix();

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&proj);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&view);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float*)&world);;
	}

	void GraphicsObject_FlatTexture::Draw()
	{
		// draw
		//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numTris));
		
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_FlatTexture::RestoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

}

//--- End of File -------------------------------------------------------------

