#pragma once
#include<Windows.h>

//�C���N���[�h�֌W
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�o�[�W�����w��
#include<dinput.h>

class KeyboardInput
{
public:
	
	//������
	static void Init(HWND hwnd);

	//���t���[���X�V
	static void Update();

	//���̗��p
	static bool PressKeyTrigger(int key);
	static bool ReleaseKeyTrigger(int key);
	static bool PressKey(int key);
	static bool ReleaseKey(int key);


private:
	static int pressKey[256];
	static int releaseKey[256];
};