#pragma once
#include "GameScene.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public GameScene
{
public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TitleScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TitleScene()override;

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SceneDataBuffer* sceneDateBuffer) override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

private://�����o�֐�
	/// <summary>
	/// �}�j���A����ʍX�V����
	/// </summary>
	void ManualUpdate();

	/// <summary>
	/// �^�C�g����ʍX�V����
	/// </summary>
	void TitleUpdate();

	/// <summary>
	/// �V�[���؂�ւ�����
	/// </summary>
	void ChangeScene();

	/// <summary>
	/// �^�C�g���G�t�F�N�g����
	/// </summary>
	void TitleEfect();
	
private: // �����o�N���X
	/// <summary>
	/// ��ʂ��Ƃ̕K�v�ϐ�
	/// </summary>
	struct Screen
	{
		Sprite* sprite = nullptr;		// �X�v���C�g
		float alpha = 0.0f;				// �A���t�@�l
	};

	/// <summary>
	/// �p�[�e�B�N���p�\����
	/// </summary>
	struct Particle
	{
		XMFLOAT3 pos{};
		XMFLOAT3 vel{};
		XMFLOAT3 acc{};
		XMFLOAT4 color{};
		int life = 0;
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
	};

	/// <summary>
	/// �X�N���[���i���o�[
	/// </summary>
	enum ScreenNum
	{
		MANUAL_SCREEN,
		TITLEL_SCREEN,
		CHANGE_SCENE,
	};

private: // �����o�ϐ�
	Camera* camera = nullptr;				// �J����
	ParticleManager* particleMan = nullptr;	// �p�[�e�B�N���}�l�[�W���[
	Screen title;							// �^�C�g����ʗp�\����
	Screen manual;							// �}�j���A����ʗp�\����
	Particle particle;						// �p�[�e�B�N��
	USHORT screenNum = MANUAL_SCREEN;		// ��ʐ؂�ւ��p
	float volume = 0.0f;					// �{�����[��
};