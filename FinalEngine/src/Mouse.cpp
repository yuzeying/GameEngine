//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GameMan.h"
#include "Mouse.h"

namespace Azul
{

	Mouse::Mouse()
	{

	}

	// Use this to read mouse buttons
	bool Mouse::GetKeyState(Button button)
	{
		bool value;
		
		if (GameMan::GetGame()->GetMouseButton(button) == GLFW_PRESS)
		{
			value = true;
		}
		else
		{
			value = false;
		}
		return value;
	}

	void Mouse::GetCursor(float& xPos, float& yPos)
	{
		// get mouse position
		GameMan::GetGame()->GetCursorPos(xPos, yPos);
	}

}

// --- End of File ---