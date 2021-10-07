#pragma once
#include<DirectXTex.h>
#include<wrl.h>

#include<d3d12.h>
#include<d3dx12.h>

#include<DirectXMath.h>

#include<d3dcompiler.h>

using namespace Microsoft::WRL;
using namespace DirectX;
#pragma comment(lib,"d3dcompiler.lib")


class Object3D
{
public:
	//�p�C�v���C���Z�b�g�\����
	struct PipelineSet
	{
		//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
	};
	//3D�I�u�W�F�N�g�p�p�C�v���C������
	static PipelineSet ObjectCreateGraphicsPipeline(ID3D12Device* dev);

	//3D�I�u�W�F�N�g�^
	struct Object
	{
		//���_�o�b�t�@
		ComPtr<ID3D12Resource> vertBuff;
		//���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW vbView{};

		//�萔�o�b�t�@
		ComPtr<ID3D12Resource> constBuff;
		//�萔�o�b�t�@�r���[�̃n���h��(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;
		//�萔�o�b�t�@�r���[�̃n���h��(GPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;

		//�A�t�B���ϊ����
		XMFLOAT3 scale = { 1,1,1 };
		XMFLOAT3 rotation = { 90,0,0 };
		XMFLOAT3 position = { 0,0,0 };
		//���[���h�ϊ����W
		XMMATRIX matWorld;
		//�F RGBA
		XMFLOAT4 color = { 1,1,1,1 };

		//�e�I�u�W�F�N�g�ւ̃|�C���^
		Object* parent = nullptr;

		//�e�N�X�`���ԍ�
		UINT texNumber = 0;
	};
	//3D�I�u�W�F�N�g���ʃf�[�^
	static const int spriteSRVCount = 512; //�e�N�X�`���̍ő喇��
	struct ObjectCommon
	{
		//�p�C�v���C���Z�b�g
		PipelineSet pipelineSet;
		//�ˉe�s��
		XMMATRIX matProjection{};

		//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
		ComPtr<ID3D12DescriptorHeap> descHeap;
		//�e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j�̐���
		ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	};

	//�萔�o�b�t�@�f�[�^�\���́i���肽���f�[�^���������炱���ɏ����j
	struct ConstBufferData
	{
		XMFLOAT4 color; //�F(RGBA)
		XMMATRIX mat; //3D�ϊ��s��
	};


	////3D�I�u�W�F�N�g�����p�֐�
	 //�X�v���C�g���ʃf�[�^����
	static ObjectCommon ObjectCommonCreate(ID3D12Device* dev, int window_width, int window_height);
	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	static void ObjectCommonLoadTexture(ObjectCommon& objectCommon, UINT texnumber, const wchar_t* filename, ID3D12Device* dev);
	//�X�v���C�g�p���ʃO���t�B�b�N�X�R�}���h
	static void ObjectCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const ObjectCommon& objectCommon);


	//3D�I�u�W�F�N�g������
	static Object ObjectCreate(int index, ID3D12Device* dev, int window_width, int window_height, ID3D12DescriptorHeap* descHeap);
	//�I�u�W�F�N�g�X�V����
	static void ObjectUpdate(Object& object, const XMMATRIX& matView, const ObjectCommon& objectCommon);
	//�I�u�W�F�N�g�`�揈��
	static void ObjectDraw(const Object &object, const ObjectCommon &objectCommon, ID3D12Device *dev, ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);


};

