/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include "../Common/Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"
#include "windows.h"
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

// Local function prototypes.
void DisplayContent(FbxScene *pScene);
void DisplayContent(FbxNode *pNode);
void DisplayTarget(FbxNode *pNode);
void DisplayTransformPropagation(FbxNode *pNode);
void DisplayGeometricTransform(FbxNode *pNode);
void DisplayMetaData(FbxScene *pScene);

static bool gVerbose = true;

// Keenan(14)
#include "ModelEnum.h"
Model model = Model::Undefined;


DWORD g_BytesTransferred = 0;

VOID CALLBACK FileIOCompletionRoutine(
    __in  DWORD dwErrorCode,
    __in  DWORD dwNumberOfBytesTransfered,
    __in  LPOVERLAPPED lpOverlapped
);

VOID CALLBACK FileIOCompletionRoutine(
    __in  DWORD dwErrorCode,
    __in  DWORD dwNumberOfBytesTransfered,
    __in  LPOVERLAPPED lpOverlapped)
{
    _tprintf(TEXT("Error code:\t%x\n"), dwErrorCode);
    _tprintf(TEXT("Number of bytes:\t%x\n"), dwNumberOfBytesTransfered);
    g_BytesTransferred = dwNumberOfBytesTransfered;
}






int main(int argc, char **argv)
{
    FbxManager *lSdkManager = NULL;
    FbxScene *lScene = NULL;
    bool lResult;

    // Keenan(14)
    if (strcmp(argv[1], "Teddy_TGA.fbx") == 0)
    {
        model = Model::Teddy;
    }
    else if (strcmp(argv[1], "humanoid.fbx") == 0)
    {
        model = Model::Humanoid;
    }
    else
    {
        model = Model::Undefined;
    }

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);
    // Load the scene.

    // The example can take a FBX file as an argument.
	FbxString lFilePath("");
	for( int i = 1, c = argc; i < c; ++i )
	{
		if( FbxString(argv[i]) == "-test" ) gVerbose = false;
		else if( lFilePath.IsEmpty() ) lFilePath = argv[i];
	}

	if( lFilePath.IsEmpty() )
	{
        lResult = false;
        FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
	}

    if(lResult == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene...");
    }
    else 
    {
        // Display the scene.
        DisplayMetaData(lScene);

        FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

        if( gVerbose ) DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

        if( gVerbose ) DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

        if( gVerbose ) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");





































        
        struct Data
        {
            char name[64];
            int  index;
            int  parentIndex;
        };

        struct ChunkHeader {
            char name[16];
            int num;
            int size;
        };

        char* outputFileName = argc > 2 ? argv[2] : "out_pack.azul";

        HANDLE hFile;
        DWORD dwBytesWritten = 0;
        BOOL bErrorFlag = FALSE;

        hFile = CreateFile(outputFileName,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, 
            NULL);

        int heirNum = 0;
        Data* heirBuf = new Data[256];
        DisplayHierarchy(lScene, (char*)heirBuf, heirNum);
        heirNum -= 3; // ignore the camera and light in data
        int heirSize = heirNum * sizeof(Data);
        struct ChunkHeader chunk = {
            "",
            heirNum,
            heirSize,
        };
        strcpy_s(chunk.name, "Hier");
        bErrorFlag = WriteFile(
            hFile,
            &chunk,
            sizeof(struct ChunkHeader),
            &dwBytesWritten,
            NULL);
        dwBytesWritten = 0;
        bErrorFlag = WriteFile(
            hFile,
            heirBuf,
            chunk.size,
            &dwBytesWritten,
            NULL);
        dwBytesWritten = 0;
        //FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");
        // if( gVerbose ) DisplayContent(lScene);
        //FBXSDK_printf("\n\n----\nPose\n----\n\n");
        // if( gVerbose ) DisplayPose(lScene);
        //FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

        int numFrames = 60;
        int maxFrames = 0;
        BoneCollection** bones = (BoneCollection**)calloc(numFrames, sizeof(BoneCollection*));
        for (int i = 0; i < numFrames; i++) *(bones + i) = (BoneCollection*)calloc(sizeof(BoneCollection), heirNum + 5);
        

        // USE TO GET ALL OF THE ANIMATION DATA INTO THE BONES BUFFER
        DisplayAnimation(maxFrames, bones, lScene);
        maxFrames++; // one extra for the 1st frame






        chunk = {
            "",
            (int)sizeof(BoneCollection) * heirNum,
            maxFrames,
        };
        strcpy_s(chunk.name, "Clip");
        bErrorFlag = WriteFile(
            hFile,
            &chunk,
            sizeof(struct ChunkHeader),
            &dwBytesWritten,
            NULL);
        dwBytesWritten = 0;

        for (int i = 0; i < maxFrames; i+=5) {
            for (int j = 0; j < heirNum; j+=5) {
                printf("\t%d\n", j);
                printf("\t\t%f %f %f\n", bones[i][j].T.x, bones[i][j].T.y, bones[i][j].T.z);
                printf("\t\t%f %f %f\n", bones[i][j].Q.x, bones[i][j].Q.y, bones[i][j].Q.z);
                printf("\t\t%f %f %f\n", bones[i][j].S.x, bones[i][j].S.y, bones[i][j].S.z);
            }
        }

        for (int i = 0; i < maxFrames; i++) {
            bErrorFlag = WriteFile(
                hFile,
                bones[i],
                sizeof(BoneCollection) * ((unsigned int)heirNum + 5),
                &dwBytesWritten,
                NULL);
            dwBytesWritten = 0;
        }

        printf("%d\n", ((unsigned int)heirNum + 5));
        printf("%d\n", ((unsigned int)heirNum + 5));
        printf("%d\n", ((unsigned int)heirNum + 5));

        for (int i = 0; i < numFrames; i++) delete (*(bones + i));
        free(bones);
        delete heirBuf;
        CloseHandle(hFile);
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, lResult);

    return 0;
}

void DisplayContent(FbxScene* pScene)
{
    int i;
    FbxNode* lNode = pScene->GetRootNode();

    if(lNode)
    {
        for(i = 0; i < lNode->GetChildCount(); i++)
        {
            DisplayContent(lNode->GetChild(i));
        }
    }
}

void DisplayContent(FbxNode* pNode)
{
    FbxNodeAttribute::EType lAttributeType;
    int i;

    if(pNode->GetNodeAttribute() == NULL)
    {
        FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
	    default:
	        break;
        case FbxNodeAttribute::eMarker:  
            DisplayMarker(pNode);
            break;

        case FbxNodeAttribute::eSkeleton:  
            DisplaySkeleton(pNode);
            break;

        case FbxNodeAttribute::eMesh:      
            DisplayMesh(pNode);
            break;

        case FbxNodeAttribute::eNurbs:      
            DisplayNurb(pNode);
            break;

        case FbxNodeAttribute::ePatch:     
            DisplayPatch(pNode);
            break;

        case FbxNodeAttribute::eCamera:    
            DisplayCamera(pNode);
            break;

        case FbxNodeAttribute::eLight:     
            DisplayLight(pNode);
            break;

        case FbxNodeAttribute::eLODGroup:
            DisplayLodGroup(pNode);
            break;
        }   
    }

    DisplayUserProperties(pNode);
    DisplayTarget(pNode);
    DisplayPivotsAndLimits(pNode);
    DisplayTransformPropagation(pNode);
    DisplayGeometricTransform(pNode);

    for(i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayContent(pNode->GetChild(i));
    }
}


void DisplayTarget(FbxNode* pNode)
{
    if(pNode->GetTarget() != NULL)
    {
        DisplayString("    Target Name: ", (char *) pNode->GetTarget()->GetName());
    }
}

void DisplayTransformPropagation(FbxNode* pNode)
{
    FBXSDK_printf("    Transformation Propagation\n");

    // 
    // Rotation Space
    //
    EFbxRotationOrder lRotationOrder;
    pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

    FBXSDK_printf("        Rotation Space: ");

    switch (lRotationOrder)
    {
    case eEulerXYZ: 
        FBXSDK_printf("Euler XYZ\n");
        break;
    case eEulerXZY:
        FBXSDK_printf("Euler XZY\n");
        break;
    case eEulerYZX:
        FBXSDK_printf("Euler YZX\n");
        break;
    case eEulerYXZ:
        FBXSDK_printf("Euler YXZ\n");
        break;
    case eEulerZXY:
        FBXSDK_printf("Euler ZXY\n");
        break;
    case eEulerZYX:
        FBXSDK_printf("Euler ZYX\n");
        break;
    case eSphericXYZ:
        FBXSDK_printf("Spheric XYZ\n");
        break;
    }

    //
    // Use the Rotation space only for the limits
    // (keep using eEulerXYZ for the rest)
    //
    FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
        pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


    //
    // Inherit Type
    //
    FbxTransform::EInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);

    FBXSDK_printf("        Transformation Inheritance: ");

    switch (lInheritType)
    {
    case FbxTransform::eInheritRrSs:
        FBXSDK_printf("RrSs\n");
        break;
    case FbxTransform::eInheritRSrs:
        FBXSDK_printf("RSrs\n");
        break;
    case FbxTransform::eInheritRrs:
        FBXSDK_printf("Rrs\n");
        break;
    }
}

void DisplayGeometricTransform(FbxNode* pNode)
{
	FBX_UNUSED(pNode);

// Keenan(7)
#if 0

    FbxVector4 lTmpVector;

    FBXSDK_printf("    Geometric Transformations\n");

    //
    // Translation
    //
    lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
    FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

// Keenan(7)
#endif
}


void DisplayMetaData(FbxScene* pScene)
{
    FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
        FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

        FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            FBXSDK_printf("    Thumbnail:\n");

            switch (thumbnail->GetDataFormat())
            {
            case FbxThumbnail::eRGB_24:
                FBXSDK_printf("        Format: RGB\n");
                break;
            case FbxThumbnail::eRGBA_32:
                FBXSDK_printf("        Format: RGBA\n");
                break;
            }

            switch (thumbnail->GetSize())
            {
	        default:
	            break;
            case FbxThumbnail::eNotSet:
                FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e64x64:
                FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e128x128:
                FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
            }
        }
    }
}

