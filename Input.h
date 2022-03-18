#pragma once


class Input
{
private:
	static BYTE oldKeyState[256];
	static BYTE keyState[256];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

};
