//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include "sb7.h"
#include "ShaderLink.h"

namespace Azul
{
	class ShaderObject : public ShaderLink
	{
	public:
		enum class Name
		{
			NULL_SHADER,
			COLOR_LIGHT,
			COLOR_SINGLE,
			TEXTURE_SIMPLE,
			TEXTURE_POINT_LIGHT,
			CONST_COLOR
		};

	public:
		ShaderObject(ShaderObject::Name shaderName, const char* const pShaderBaseFileName);

		ShaderObject() = delete;
		ShaderObject(const ShaderObject&) = delete;
		ShaderObject& operator = (ShaderObject&) = delete;
		virtual ~ShaderObject() = default;

		void SetActive() const;
		GLint GetLocation(const char* const pUniformName) const;

	private:
		bool privCreateShader(GLuint& programObject, const char* const pShaderBaseFileName);
		bool privLoadNCompile(GLuint& shaderObject, const char* const pShaderFileName, GLenum shader_type);

	public:
		Name  name;
		GLuint		programObject;

	};
}

#endif

// ---  End of File ---
