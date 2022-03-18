
#include "Main.h"
#include "Input.h"


BYTE Input::oldKeyState[256];
BYTE Input::keyState[256];


void Input::Init()
{
	memset( oldKeyState, 0, 256 );
	memset( keyState, 0, 256 );
}

void Input::Uninit()
{
}

void Input::Update()
{
	memcpy( oldKeyState, keyState, 256 );

	GetKeyboardState( keyState );
}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (keyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((keyState[KeyCode] & 0x80) && !(oldKeyState[KeyCode] & 0x80));
}
