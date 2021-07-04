/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

#include <fbxsdk.h>
#include <assert.h>

#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

// Keenan(14)
#include "ModelEnum.h"
extern Model model;

// Keenan(15)
#include <map>
#include "../Common/Common.h"
struct BoneName
{
	char name[64];

	BoneName(char *pBuff)
	{
		memset(name, 0, 64);
		memcpy(name, pBuff, strlen(pBuff));
	}

	bool operator == (const BoneName &other) const
	{
		return (strcmp(this->name, other.name) == 0);
	}

	bool operator < (const BoneName &other) const
	{
		return (strcmp(this->name, other.name) < 0);
	}
};
std::map<BoneName, int> BoneNames;


static int displayHierarchyIndex = 0;

// Local functions prototype.
void DisplayHierarchy(FbxScene* pScene, char* buf, int& convCount);
void DisplayHierarchy(FbxNode* pParent, FbxNode* pNode, int pDepth, char* buf, int& convCount);

void DisplayHierarchy(FbxScene* pScene, char * buf, int& convCount)
{
	// Keenan(14)
	if (model == Model::Humanoid)
	{
		displayHierarchyIndex = -2;
	}
	else if (model == Model::Teddy)
	{
		displayHierarchyIndex = 0;
	}
	else
	{
		assert(false);
	}

    int i;
    FbxNode* lRootNode = pScene->GetRootNode();

    for( i = 0; i < lRootNode->GetChildCount(); i++)
    {
		// Keenan(7)
		DisplayHierarchy(lRootNode, lRootNode->GetChild(i), 0, buf, convCount);
    }
}

// Keenan(9) - rework below
void DisplayHierarchy(FbxNode *pParent, FbxNode* pNode, int pDepth, char* buf, int &convCount)
{
	FbxString lString;

	// Big Hack to remove "Humanoid:" from each name
	char buff[64];
	char buff2[64];
	char origBuff[64];

	// Get original
	// Clean up house cleaning... remove extra stuff on Humanoid
	// Replace spaces with "_" so the names are valid
	strcpy(origBuff, pNode->GetName());

	// test if contains string...
	if (strncmp(origBuff, "Humanoid:", 9) == 0)
	{
		// trim it.
		strcpy(buff, origBuff + 9);
	}
	else
	{
		// leave it
		strcpy(buff, origBuff);
	}

	for (int i = 0; i < (int)strlen(buff); i++)
	{
		// Find a space... replace with an underscore -- quick and dirty
		if (buff[i] == 32)
		{
			buff[i] = 95;
		}
	}

	// Get original
	strcpy(origBuff, pParent->GetName());

	// test if contains string...
	if (strncmp(origBuff, "Humanoid:", 9) == 0)
	{
		// trim it.
		strcpy(buff2, origBuff + 9);
	}
	else
	{
		// leave it
		strcpy(buff2, origBuff);
	}
	for (int i = 0; i < (int)strlen(buff); i++)
	{	
		// Find a space... replace with an underscore -- quick and dirty
		if (buff2[i] == 32)
		{
			buff2[i] = 95;
		}
	}

	/* Goal: 

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
	p1 = new SimpleGameObject(pGraphics_TextureLight);
	p1->SetName("Green");
	p1->SetIndex(1);

	GameObjectMan::Add(p1, p0);

	*/

	// Keenan(12) - Human
	//lString += "pGraphics_TextureLight = new GraphicsObject_TextureLight( pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);\n";
	//lString += "GameObjectAnim *p";
	//lString += buff;
	//lString += " = new GameObjectAnim( pGraphics_TextureLight );\n";

	//lString += "p";
	//lString += buff;
	//lString += "->SetIndex(";
	//lString += displayHierarchyIndex;
	//lString += ");\n";

	//lString += "p";
	//lString += buff;
	//lString += "->SetName(\"";
	//lString += buff;
	//lString += "\");\n";

	//lString += "GameObjectMan::Add(  p";
	//lString += buff;
	//lString += ", p";
	//lString += buff2;
	//lString += " );\n\n";

	//lString += "pGraphics_TextureLight = new GraphicsObject_TextureLight( pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);\n";
	//lString += "GameObjectAnim *p";
	//lString += buff;
	//lString += " = new GameObjectAnim( pGraphics_TextureLight );\n";

	// Add to Map
	BoneName bn(buff);
	BoneNames.emplace(bn, displayHierarchyIndex);

	// See if buff2 is in map
	BoneName bn2(buff2);
	auto it = BoneNames.find(bn2);

	// Can't find it... make -1
	int buff2Index = -1;
	if (it == BoneNames.end())
	{
		buff2Index = -1;
	}
	else
	{
		buff2Index = it->second;
	}

	//Data data[]
	//{
	//	{"Apple",5,6},
	//	{"dog",1,2},
	//	{"tree", 7, 8}
	//};
	
	lString += "   { ";
	lString += "\"";
	lString += buff;
	lString += "\",";
	lString += " ";
	lString += displayHierarchyIndex;
	lString += ", ";
	lString += buff2Index;
	lString += " },";
	lString += "\n";

	//strcpy_s(skelData[skelDataSize].name, 64, buff);
	//skelData[skelDataSize].index = displayHierarchyIndex;
	//skelData[skelDataSize].parentIndex = buff2Index;
	//skelDataSize++;
	

	struct Data
	{
		char name[64];
		int  index;
		int  parentIndex;
	};

	if (displayHierarchyIndex >= 0) {
		Data* dArr = (Data*)buf;
		dArr[convCount].index = displayHierarchyIndex;
		dArr[convCount].parentIndex = buff2Index;
		strcpy_s(dArr[convCount].name, 64, buff);
		convCount++;
	}
	//lString += "p";
	//lString += buff;
	//lString += "->SetName(\"";
	//lString += buff;
	//lString += "\");\n";

	//lString += "GameObjectMan::Add(  p";
	//lString += buff;
	//lString += ", p";
	//lString += buff2;
	//lString += " );\n\n";

	printf(lString.Buffer());
	displayHierarchyIndex++;

	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayHierarchy(pNode, pNode->GetChild(i), pDepth + 1, buf, convCount);
	}
}
