#pragma once
#include<Windows.h>
#include<xaudio2.h>
#include<wrl.h>

class Audio
{
private:
	//Microsoft::WRL::���ȗ�����
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: //�ϐ�
	//ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;


public: //�\����
	ComPtr<IXAudio2> xAudio2;
	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4]; //�`�����N����ID
		int size; //�`�����N�T�C�Y
	};
	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk; //"RIFF"
		char type[4]; //"WAVE"
	};
	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk; //"fmt"
		WAVEFORMATEX fmt; //�g�`�t�H�[�}�b�g
	};
	//�����f�[�^
	struct SoundData
	{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;
		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;
		//�o�b�t�@�̃T�C�Y
		unsigned int bufferSize;
	};

public: //�֐�
	//�����֌W������
	void Init();
	//�����f�[�^�ǂݍ���
	SoundData SoundLoadWave(const char* filename);
	//�����Đ�
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);
	//�����f�[�^���
	void SoundUnLoad(SoundData* soundData);


};