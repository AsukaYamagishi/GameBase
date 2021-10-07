#pragma once
#include<Windows.h>
#include<string>

#include"Object2D.h"

class DebugText
{
public:
	//�萔�̐錾
	//�f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 256; //1�t���[���ł̍ő�\��������
	static const int fontWidth = 9; //1�������̉���
	static const int fontHeight = 18; //1�������̏c��
	static const int fontLineCount = 14; //�摜��1�s���̕�����

private:
	//�����o�ϐ�
	//�X�v���C�g�̔z��
	Object2D::Sprite textSprites[maxCharCount];
	//�z��̓Y����
	int textIndex = 0;


public:
	//�����o�֐�
	//������
	void Init(ID3D12Device* dev, int window_wigth, int window_height, UINT texnumber, const Object2D::SpriteCommon& spriteCommon);
	//������̒ǉ�
	void PrintDebugText(const Object2D::SpriteCommon& spriteCommon, const std::string& text, float x, float y, float scale = 1.0f);
	//������̕\��
	void DrawAll(ID3D12GraphicsCommandList* cmdList, const Object2D::SpriteCommon& spriteCommon, ID3D12Device* dev);

};

