/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _DISPLAY_ANIMATION_H
#define _DISPLAY_ANIMATION_H

#include "DisplayCommon.h"

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

void DisplayAnimation(int &maxFrames, BoneCollection** bones, FbxScene* pScene);
void DisplayDefaultAnimation(FbxNode* pNode);

#endif // #ifndef _DISPLAY_ANIMATION_H


