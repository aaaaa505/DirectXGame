#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "GameScene.h"
#include "TitleScene.h" 
#include "SelectScene.h"
#include "PlayScene.h"
#include "LightGroup.h"
#include "ParticleManager.h"
#include "LoadSprite.h"

/// <summary>
/// �Q�[���ŗL�̃N���X
/// </summary>
class Illegal
{
public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Illegal();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual~Illegal();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �Q�[�����[�v
	/// </summary>
	void Loop();

protected: // �����o�֐�
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �V�[���؂�ւ�����
	/// </summary>
	void ChangeScene();

protected: // �����o�֐�
	// WindowsAPI��ՃN���X
	WinApp* win = nullptr;
	// DirectX12��ՃN���X
	DirectXCommon* dxCommon = nullptr;
	// Input�N���X
	Input* input = nullptr;
	// Audio�N���X
	Audio* audio = nullptr;
	// Scene�N���X
	GameScene* gameScene = nullptr;
	// �V�[���Ǘ��p�o�b�t�@
	SceneDataBuffer sceneDateBuffa;
};