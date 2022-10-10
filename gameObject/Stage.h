#pragma once
#include "Object3d.h"

#include "GameScene.h"

class Stage
{
private: // �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Stage();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Stage();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize(const USHORT& sceneNo);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private: // �����o�ϐ�
	// �I�u�W�F�N�g
	Object3d* obj = nullptr;
	// ���f��
	Model* model = nullptr;
};

