#include "Object3D.h"

Object3D::PipelineSet Object3D::ObjectCreateGraphicsPipeline(ID3D12Device* dev)
{
	HRESULT result;

	//���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/BasicVS.hlsl",  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	//���_�V�F�[�_�̃G���[��\��
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	//�s�N�Z���V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/BasicPS.hlsl",   // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	//�s�N�Z���V�F�[�_�̃G���[��\��
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ //xyz���W
				"POSITION", //�Z�}���e�B�b�N��
				0, //�����Z�}���e�B�b�N�����������鎞�Ɏg���C���f�b�N�X(0�ł悢)
				DXGI_FORMAT_R32G32B32_FLOAT, //�v�f���ƃr�b�g��
				0, //���̓X���b�g�C���f�b�N�X(0�ł悢)
				D3D12_APPEND_ALIGNED_ELEMENT, //�f�[�^�̃I�t�Z�b�g�l
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, //���̓f�[�^���
				0}, //xyz
			{ //�@���x�N�g��
				"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			}, //�@���x�N�g��
			{ //uv���W
				"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			}, //uv

	};


	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//���_�V�F�[�_
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	//�s�N�Z���V�F�[�_
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�i�W���ݒ�(�w�ʃJ�����O/�h��Ԃ�/�[�x�N���b�s���O�L��)�j
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //�W���ݒ�

	//���ʐݒ�
	blenddesc.BlendEnable = true; //�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; //���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE; //�\�[�X(�V�����F)�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO; //�f�X�g(������F)�̒l��0%�g��

	////���Z�����i�����ɕʕ��j
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE; //�\�[�X�̒l100%
	//blenddesc.DestBlend = D3D12_BLEND_ONE; //�f�X�g�̒l100%

	////���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; //�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE; //�\�[�X�̒l100%
	//blenddesc.DestBlend = D3D12_BLEND_ONE; //�f�X�g�̒l100%

	////�F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; //1.0f - �f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO; //�g��Ȃ�

	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA; //�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; //1.0f - �\�[�X�̃A���t�@�l

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	//�W���ݒ�i�[�x�e�X�g���s��/�������݋���/�[�x����������΍��i�j
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g

	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); //t0���W�X�^
	//���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0); //�萔�o�b�t�@�r���[�Ƃ��ď�����(b0���W�X�^)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	//�e�N�X�`���T���v���[�i�X�^�e�B�b�N�T���v���[�j
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
							   D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//�p�C�v���C���ƃ��[�g�V�O�l�`���̃Z�b�g
	PipelineSet pipelineSet;

	ComPtr<ID3DBlob> rootSigBlob;
	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
												   &rootSigBlob, &errorBlob);
	//���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
									  IID_PPV_ARGS(&pipelineSet.rootsignature));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = pipelineSet.rootsignature.Get();

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelinestate));

	return pipelineSet;
}

Object3D::ObjectCommon Object3D::ObjectCommonCreate(ID3D12Device * dev, int window_width, int window_height)
{
	HRESULT result = S_FALSE;
	//�V���ȃI�u�W�F�N�g���ʃf�[�^�𐶐�
	ObjectCommon objectCommon{};
	//�I�u�W�F�N�g�p�p�C�v���C������
	objectCommon.pipelineSet = ObjectCreateGraphicsPipeline(dev);
	//�������e�ϊ�
	objectCommon.matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		window_width / window_height,
		0.1f, 1000.0f
	);

	//�f�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&objectCommon.descHeap));

	return objectCommon;
}

void Object3D::ObjectCommonLoadTexture(ObjectCommon & objectCommon, UINT texnumber, const wchar_t * filename, ID3D12Device * dev)
{
	assert(texnumber <= spriteSRVCount - 1);
	HRESULT result;

	//�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	const Image *img = scratchImg.GetImage(0, 0, 0);

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels);

	// �e�N�X�`���o�b�t�@����
	result = dev->CreateCommittedResource(	//GPU���\�[�X�̐���
										  &CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
										  D3D12_HEAP_FLAG_NONE,
										  &texresDesc,
										  D3D12_RESOURCE_STATE_GENERIC_READ,	//�e�N�X�`���p�w��
										  nullptr,
										  IID_PPV_ARGS(&objectCommon.texBuff[texnumber]));

	// �e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = objectCommon.texBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		img->pixels,	//���f�[�^�A�h���X
		(UINT)img->rowPitch,	//1���C���T�C�Y
		(UINT)img->slicePitch	//�S�T�C�Y
	);


	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture1D.MipLevels = 1;

	//�V�F�[�_�[���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(
		objectCommon.texBuff[texnumber].Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,	//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(objectCommon.descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);
}

void Object3D::ObjectCommonBeginDraw(ID3D12GraphicsCommandList * cmdList, const ObjectCommon & objectCommon)
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(objectCommon.pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(objectCommon.pipelineSet.rootsignature.Get());
	//�g�|���W�[
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap *ppHeaps[] = { objectCommon.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

Object3D::Object Object3D::ObjectCreate(int index, ID3D12Device* dev, int window_width, int window_height, ID3D12DescriptorHeap* descHeap)
{
	HRESULT result;

	Object object;

	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object.constBuff));

	//�f�X�N���v�^1���̃T�C�Y
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//CBV��CPU�f�X�N���v�^�n���h�����v�Z
	object.cpuDescHandleCBV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		descHeap->GetCPUDescriptorHandleForHeapStart(), index, descHandleIncrementSize);
	//CBV��GPU�f�X�N���v�^�n���h�����v�Z
	object.gpuDescHandleCBV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descHeap->GetGPUDescriptorHandleForHeapStart(), index, descHandleIncrementSize);

	//�萔�o�b�t�@�r���[�̍쐬
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = object.constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)object.constBuff->GetDesc().Width;
	dev->CreateConstantBufferView(&cbvDesc, object.cpuDescHandleCBV);

	//�萔�o�b�t�@
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(Object3D::ConstBufferData)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&object.constBuff)
	);

	//�萔�o�b�t�@�Ƀf�[�^��]��
	ConstBufferData *constMap = nullptr;
	result = object.constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);
	//���t���e�s��
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f
	);
	object.constBuff->Unmap(0, nullptr);

	return object;
}

void Object3D::ObjectUpdate(Object& object, const XMMATRIX& matView, const ObjectCommon& objectCommon)
{
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(object.scale.x, object.scale.y, object.scale.z); //�X�P�[�����O

	matRot = XMMatrixIdentity(); //��]�s��
	matRot *= XMMatrixRotationZ(XMConvertToRadians(object.rotation.z)); //Z������ɉ�]
	matRot *= XMMatrixRotationX(XMConvertToRadians(object.rotation.x)); //X������ɉ�]
	matRot *= XMMatrixRotationY(XMConvertToRadians(object.rotation.y)); //Y������ɉ�]

	matTrans = XMMatrixTranslation(object.position.x, object.position.y, object.position.z); //���s�ړ�

	//���[���h�s��̍���
	object.matWorld = XMMatrixIdentity(); //�P�ʍs��
	object.matWorld *= matScale; //���[���h���W�ɃX�P�[�����O�𔽉f
	object.matWorld *= matRot; //���[���h�s��ɉ�]�𔽉f
	object.matWorld *= matTrans; //���[���h�s��ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (object.parent != nullptr)
	{
		//�e�I�u�W�F�N�g�̃��[���h�s����|����
		object.matWorld *= object.parent->matWorld;
	}

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	if (SUCCEEDED(object.constBuff->Map(0, nullptr, (void**)&constMap)))
	{
		constMap->color = XMFLOAT4(1, 1, 1, 1.0f);
		constMap->mat = object.matWorld * matView * objectCommon.matProjection;
		object.constBuff->Unmap(0, nullptr);
	}
}

void Object3D::ObjectDraw(const Object &object, const ObjectCommon &objectCommon, ID3D12Device *dev, ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices)
{
	//�I�u�W�F�N�g�p���ʃO���t�B�b�N�X�R�}���h
	Object3D::ObjectCommonBeginDraw(cmdList, objectCommon);
	//���_�o�b�t�@�̐���
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, object.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, 
											CD3DX12_GPU_DESCRIPTOR_HANDLE(
												objectCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
												object.texNumber,
												dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
											));
	//�`��R�}���h
	cmdList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}
