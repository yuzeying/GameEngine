//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "sb7.h"
#include "TextureNodeLink.h"

namespace Azul
{
	class Texture
	{
	public:
		static const unsigned int ASSET_NAME_SIZE = 64;

		enum class Name
		{
			STONES,
			RED_BRICK,
			DUCKWEED,
			ROCKS,
			NOT_INITIALIZED
		};

	public:
		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;
		~Texture() = default;

		static void Create();
		static void Destroy();
		static void Add(const char* const _assetName, const Texture::Name _name);
		static void DeleteAllTextures();
		static GLuint Find(const Texture::Name _name);

	private:
		// methods
		Texture();
		static Texture* privGetInstance();

		// helper private methods
		void privLoadTexture(const char* const _assetName, GLuint*& textureID);
		bool privLoadTGATexture(const char* szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode);
		void privAddToFront(TextureNodeLink* node, TextureNodeLink*& head);
		GLbyte* gltReadTGABits(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);

	private:

		TextureNodeLink* active;
	};

}

#endif

// --- End of File --------------------------------------------------
