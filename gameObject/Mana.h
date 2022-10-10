#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "ParticleManager.h"

#include "GameScene.h"

class Mana
{
private: // �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Mana* Create();

public: // �����o�֐�

	// �f�X�g���N�^
	~Mana();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(bool oopth_StartFlag, bool boss_StartFlag);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

#pragma region setter�Egetter
	//�}�i����t���b�O
	void SetMoveFlag(bool moveFlag) { this->moveFlag = moveFlag; }
	bool GetMoveFlag() { return moveFlag; }
	//�}�i���슮���t���b�O
	bool GetCompletionFlag() { return completionFlag; }
	short GetTypeFlag() { return typeFlag; }
	
	//���Z�b�g�t���b�O
	void SetResetFlag(bool resetFlag) { this->resetFlag = resetFlag; }

	//�����蔻��p
	XMFLOAT3 GetPos() { return mana->GetPosition(); }
	bool GetHitFlag() { return hitFlag; }
	void SetHitFlag(bool hitFlag) { this->hitFlag = hitFlag; }
#pragma endregion

private: // �����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �����ύX
	/// </summary>
	void TypeChange();

	/// <summary>
	/// ���g�̃G�t�F�N�g
	/// </summary>
	void ManaEffect();

	/// <summary>
	/// �����_���z�u
	/// </summary>
	void RandamSetting();

	/// <summary>
	/// �}�i�̓���
	/// </summary>
	void Move();

	/// <summary>
	/// �ċN��
	/// </summary>
	void Reset();

private: // �����o�ϐ�

#pragma region ���N���X
	ParticleManager* particleMan = nullptr;
#pragma endregion

#pragma region �I�u�W�F�N�g�E���f��
	// �I�u�W�F�N�g
	Object3d* mana = nullptr;
	Object3d* guide = nullptr;

	// ���f��
	Model* model_F = nullptr;
	Model* model_W = nullptr;
	Model* model_L = nullptr;
	Model* model_guide = nullptr;
#pragma endregion

#pragma region �p�[�e�B�N���\����
	typedef struct Particle
	{
		XMFLOAT3 pos{};
		XMFLOAT3 vel{};
		XMFLOAT3 acc{};
		XMFLOAT4 color{};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
	};
	Particle particle;
#pragma endregion

#pragma region �ݒ�ϐ�
	//���W
	XMFLOAT3 pos{};
	//�ړ���
	XMFLOAT3 vel{};
	//���a
	float radius = 0.0f;
	//�A���t�@�l
	float alpha = 0.0f;

#pragma endregion

#pragma region ��ԕω��ϐ�
	int placementFlag = 0;
	short typeFlag = 0;
	bool moveFlag = false;
	bool completionFlag = false;
	bool settingFlag = false;
	bool hitFlag = false;
	int moveTimer = 0;
	bool resetFlag = false;
#pragma endregion
};

