//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_MAN_H
#define SHADER_MAN_H

#include "ShaderObject.h"

namespace Azul
{
	class ShaderMan
	{
	public:
		static void Add(ShaderObject* pShader);
		static void Create();
		static void Destroy();

	private:  // methods

		static ShaderMan* privGetInstance();
		ShaderMan();

		void privAddToFront(ShaderObject* node, ShaderObject*& head);
		void privRemove(ShaderObject* node, ShaderObject*& head);

	private:  // add

		ShaderObject* active;
	};

}

#endif

// ---  End of File ---
