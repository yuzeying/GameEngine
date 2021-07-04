//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "sb7.h"
#include "GameMan.h"
#include "MathEngine.h"
#include "GameObject.h"
#include "ShaderObject.h"
#include "CubeModel.h"
#include "SphereModel.h"
#include "PyramidModel.h"
#include "GraphicsObject.h"
#include "GraphicsObject_ColorByPosition.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_TextureLight.h"
#include "GraphicsObject_ConstColor.h"
#include "GraphicsObject_WireframeConstantColor.h"
#include "Texture.h"
#include "InputMan.h"
#include "InputTest.h"
#include "Camera.h"
#include "GameObjectMan.h"
#include "GameObjectAnim.h"
#include "ModelMan.h"
#include "ShaderMan.h"
#include "BoundingSphereGameObject.h"
#include "CameraMan.h"
#include "CameraModel.h"
#include "CameraDemoInput.h"
#include "Time.h"
#include "Timer.h"
#include "VectApp.h"
#include "QuatApp.h"
#include "Anim.h"
#include "GraphicsDebugMessage.h"
#include "TimerController.h"
#include "AnimController.h"
#include "Clip.h"
#include "Bone.h"
#include "Mixer.h"
#include "AnimMan.h"
#include "windows.h"
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <sys/types.h>
#include <sys/stat.h>
namespace Azul
{
	DWORD g_BytesTransferred = 0;

	VOID CALLBACK FileIOCompletionRoutine(
		__in  DWORD dwErrorCode,
		__in  DWORD dwNumberOfBytesTransfered,
		__in  LPOVERLAPPED lpOverlapped
	);

	VOID CALLBACK FileIOCompletionRoutine(
		__in  DWORD,
		__in  DWORD dwNumberOfBytesTransfered,
		__in  LPOVERLAPPED)
	{
		g_BytesTransferred = dwNumberOfBytesTransfered;
	}

	//-----------------------------------------------------------------------------
	//  Game::Game()
	//		Game Engine Constructor
	//-----------------------------------------------------------------------------
	Game::Game(const char *const pWindowName, const int Width, const int Height)
		:Engine(pWindowName, Width, Height)
	{
		assert(pWindowName);
		this->globalTimer.Tic();
		this->intervalTimer.Tic();
	}

	//-----------------------------------------------------------------------------
	// Game::Initialize()
	//		Allows the engine to perform any initialization it needs to before 
	//      starting to run.  This is where it can query for any required services 
	//      and load any non-graphic related content. 
	//-----------------------------------------------------------------------------
	void Game::Initialize()
	{

	}

	struct SmalVec {
		double x;
		double y;
		double z;
	};

	struct BoneCollection {
		SmalVec T;
		SmalVec Q; // Quat::Rot3::XYZ
		SmalVec S;
	};

	struct ChunkHeader {
		char name[16];
		int num;
		int size;
	};

	void EatAnimationAzul(char* filename, float offx = 0, float offy = 0, float offz = 0) {
		HANDLE hFile;
		BOOL bErrorFlag = FALSE;
		OVERLAPPED ol = { 0 };

		hFile = CreateFile(filename,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		struct stat filestatus;
		stat(filename, &filestatus);

		int fileSize = filestatus.st_size;
		char* fileBuffer = (char*)malloc((size_t)fileSize);

		bErrorFlag = ReadFileEx(hFile,
			fileBuffer,
			(DWORD)fileSize,
			&ol,
			FileIOCompletionRoutine);

		CloseHandle(hFile);

		struct ChunkHeader* skelChunk = (struct ChunkHeader*)fileBuffer;
		fileBuffer += sizeof(struct ChunkHeader);

		int skelDataNumBones = skelChunk->num;
		Skeleton::Data* skelDataFile = (Skeleton::Data*) fileBuffer;
		fileBuffer += skelChunk->size;

		struct ChunkHeader* clipChunk = (struct ChunkHeader*)fileBuffer;
		fileBuffer += sizeof(struct ChunkHeader);

		BoneCollection** fileAnimBones = (BoneCollection**)calloc((int)sizeof(BoneCollection*), (size_t)clipChunk->num);
		fileBuffer += sizeof(BoneCollection) * (skelChunk->num + 5);
		for (int i = 0; i < clipChunk->num; i++) {
			fileAnimBones[i] = (BoneCollection*)fileBuffer;
			fileBuffer += sizeof(BoneCollection) * (skelChunk->num + 5);
		}

		Clip::FrameBucket* pFrames = new Clip::FrameBucket();
		pFrames->prevBucket = nullptr;
		pFrames->nextBucket = nullptr;
		pFrames->KeyTime = 0 * Time(Time::Duration::NTSC_30_FRAME);
		pFrames->poBone = (Bone*)calloc(sizeof(Bone), (size_t)skelDataNumBones + 5) + 3;
		auto headFileClip = pFrames;

		int animFramCount = 30;
		for (int x = 0; x < animFramCount; x += 1) {
			int i = 0;
			if (x == 1) i = 5;
			if (x == 2) i = 10;
			i = x;

			Trace::out("%d\n", i);
			if (i != 0) {
				Clip::FrameBucket* pTmp22 = new Clip::FrameBucket();
				pTmp22->prevBucket = pFrames;
				pTmp22->nextBucket = nullptr;
				pTmp22->KeyTime = i * Time(Time::Duration::NTSC_30_FRAME);
				pTmp22->poBone = (Bone*)calloc(sizeof(Bone), (size_t)skelDataNumBones + 5) + 3;
				pFrames->nextBucket = pTmp22;
				pFrames = pTmp22;
			}

			for (int j = 3; j < skelDataNumBones + 5; j++) {
				pFrames->poBone[j - 3].T = Vect((float)fileAnimBones[i][j].T.x + offx, (float)fileAnimBones[i][j].T.y + offy, (float)fileAnimBones[i][j].T.z + offz);
				pFrames->poBone[j - 3].Q = Quat(Quat::Rot3::XYZ, (float)fileAnimBones[i][j].Q.x, (float)fileAnimBones[i][j].Q.y, (float)fileAnimBones[i][j].Q.z);
				pFrames->poBone[j - 3].S = Vect((float)fileAnimBones[i][j].S.x, (float)fileAnimBones[i][j].S.y, (float)fileAnimBones[i][j].S.z);
			}
		}

		float humanBoneWidth = 30.0f;
		Time humanDelta = 0.1f * Time(Time::Duration::NTSC_30_FRAME);
		int humanNumBones = skelDataNumBones;
		Skeleton::Data* humanSkelData = skelDataFile;
		Clip::FrameBucket* humanFrames = headFileClip;
		Clip* humanClip = new Clip(humanNumBones, humanFrames);
		Bone* humanBoneResult = new Bone[(unsigned int)humanNumBones]();
		Skeleton* humanSkel = new Skeleton(humanBoneResult, humanNumBones, humanSkelData);
		Anim* humanAnim = new Anim(humanClip, humanBoneResult, humanSkel, humanNumBones, humanBoneWidth);
		AnimMan::Add(humanAnim, humanDelta);
	}

	//-----------------------------------------------------------------------------
	// Game::LoadContent()
	//		Allows you to load all content needed for your engine,
	//	    such as objects, graphics, etc.
	//-----------------------------------------------------------------------------
	void Game::LoadContent()
	{
		// Start the managers
		CameraMan::Create();
		ModelMan::Create();
		ShaderMan::Create();
		Texture::Create();
		GameObjectMan::Create();
		CameraMan::Create();
		InputMan::Create();
		AnimMan::Create();

		// Camera setup

		Camera *pCam0 = new Camera();
		pCam0->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);

		// For Human
		pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 20000.0f);
		pCam0->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 0.0f), Vect(4000.0f, 0.0f, 0.0f));

		// For Teddy
		//pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 20000.0f);
		//pCam0->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 50.0f), Vect(400.0f, 0.0f, 0.0f));

		pCam0->updateCamera();
		CameraMan::Add(Camera::Name::CAMERA_0, pCam0);
		CameraMan::SetCurrent(Camera::Name::CAMERA_0);

		// Load the model

		// Create/Load Shader 

		// Textures
		Texture::Add("Rocks.tga", Texture::Name::ROCKS);
		Texture::Add("Stone.tga", Texture::Name::STONES);
		Texture::Add("RedBrick.tga", Texture::Name::RED_BRICK);
		Texture::Add("Duckweed.tga", Texture::Name::DUCKWEED);

		EatAnimationAzul("humanoid.azul");
		EatAnimationAzul("humanoid.azul");
	}

	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------
	void Game::Update(float)
	{
		// ------------------------------------------------
		// Update Camera
		// ------------------------------------------------
		Camera *pCam = CameraMan::GetCurrent();
		pCam->updateCamera();


		if (InputMan::GetKeyboard()->GetKeyState(Keyboard::Key::KEY_0)) {
			AnimMan::Toggle(false);
		}
		else if (InputMan::GetKeyboard()->GetKeyState(Keyboard::Key::KEY_1)) {
			AnimMan::Toggle(true);
		}

		if (InputMan::GetKeyboard()->GetKeyState(Keyboard::Key::KEY_2)) {
			AnimMan::ToggleReverse(false); //MAKE FORWARDS
		}
		else if (InputMan::GetKeyboard()->GetKeyState(Keyboard::Key::KEY_3)) {
			AnimMan::ToggleReverse(true); //MAKE BACKWARDS
		}

		if (InputMan::GetKeyboard()->GetKeyState(Keyboard::Key::KEY_4)) {
			AnimMan::ToggleSlow(false); //MAKE NORMAL SPEED
		}
		else if (InputMan::GetKeyboard()->GetKeyState(Keyboard::Key::KEY_5)) {
			AnimMan::ToggleSlow(true); //MAKE SLOW
		}
		else if (InputMan::GetKeyboard()->GetKeyState(Keyboard::Key::KEY_6)) {
			AnimMan::ToggleSlow(2); //MAKE FAST
		}

		// ------------------------------------------------
		// Animate ME
		// ------------------------------------------------
		AnimMan::Update();

		// ------------------------------------------------
		// GameObjects update
		// ------------------------------------------------
		GameObjectMan::Update(this->globalTimer.Toc());

	}

	//-----------------------------------------------------------------------------
	// Game::Draw()
	//		This function is called once per frame
	//	    Use this for draw graphics to the screen.
	//      Only do rendering here
	//-----------------------------------------------------------------------------
	void Game::Draw()
	{
		GameObjectMan::Draw();
	}


	//-----------------------------------------------------------------------------
	// Game::UnLoadContent()
	//       unload content (resources loaded above)
	//       unload all content that was loaded before the Engine Loop started
	//-----------------------------------------------------------------------------
	void Game::UnLoadContent()
	{
		// Todo fix this - verify OpenGL removal

		//  glDeleteVertexArrays(1, &vao);
		//  glDeleteProgram(pShaderObject->programObject);
		//  glDeleteBuffers(1, &buffer);

		Texture::Destroy();
		InputMan::Destroy();
		GameObjectMan::Destroy();
		ModelMan::Destroy();
		ShaderMan::Destroy();
		CameraMan::Destroy();
		AnimMan::Destroy();
	}

	//------------------------------------------------------------------
	// Game::ClearBufferFunc()
	// Allows user to change the way the clear buffer function works
	//------------------------------------------------------------------
	void Game::ClearBufferFunc()
	{
		const GLfloat grey[] = { 0.7f, 0.7f, 0.8f, 1.0f };
		const GLfloat one = 1.0f;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, grey);
		glClearBufferfv(GL_DEPTH, 0, &one);
	}

}

// --- End of File ---
