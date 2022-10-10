#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "Input.h"
#include "ParticleManager.h"

enum TYEP
{
	NORMAL_TYPE,
	FIRE_TYPE,
	WATER_TYPE,
	LEAF_TYPE,
};

enum STAGE
{
	POSEIDON_STAGE,
	PLUTON_STAGE,
	URANUS_STAGE,
};

const int PLAYERBULLET_MAX = 50;

class Player
{
private: // �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Player* Create(Input* input);

public: // �����o�֐�

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(bool oopth_AttackFlag, bool boss_AttackFlag, bool oopth_StartFlag,bool boss_StartFlag, short turnNumber);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �v���C���[��Hp����
	/// </summary>
	void Damege(int stageNo);

	/// <summary>
	/// �{�X�̕K�E�Z��e����Hp����
	/// </summary>
	void SpecialAttackDamege(int stageNo);

	//�v���C���[�̍��W�擾
	XMFLOAT3 GetPos() { return pos_Player; }

	//�v���C���[��Hp
	int GetHp() { return hp; }
	void SetHp(int Hp) { this->hp = Hp; }

	//�ϐg���Ԏ擾
	int GetChangeTimer() { return changeTimer; }

	//�����ύX�t���b�O
	void SetTypeFlag(short typeFlag) { this->typeFlag = typeFlag; }
	short GetTypeFlag() { return typeFlag; }

	//�ϐg�t���b�O
	void SetChangeFlag(bool changeFlag) { this->changeFlag = changeFlag; }

	//��e���̃V�F�C�N���o�N��
	void SetDamegeShakeTimer(float damegeShakeTimer) { this->damegeShakeTimer = damegeShakeTimer; }

	//�v���C���[���S�m�F�t���b�O
	bool GetDeathFlag() { return deathFlag; }

	//�����蔻��p
	XMFLOAT3 GetCollisionBullet(int i) { return { pos_Bullet[i].x, pos_Bullet[i].y + 1.0f, pos_Bullet[i].z }; }
	bool GetHitFlag(int i) { return hitFlag[i]; }
	void SetHitFlag(bool hitFlag, int i) { this->hitFlag[i] = hitFlag; }

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
	/// �{�̂̈ړ�����
	/// </summary>
	void PlayerMove();

	/// <summary>
	/// �e�̈ړ�����
	/// </summary>
	void BulletMove();

	/// <summary>
	/// �e�̃G�t�F�N�g
	/// </summary>
	void FireEffect();

	/// <summary>
	/// ���U
	/// /// </summary>
	/// <param name="playerPos"></param>
	void Loading();

	/// <summary>
	/// �U�����̃V�F�C�N���o
	/// </summary>
	void Shake();

	/// <summary>
	/// �K�C�h�I�u�W�F�N�g�̏o������
	/// </summary>
	void Guide_INOUT(bool enemy_AttackFlag);

	/// <summary>
	/// ���S���o
	/// </summary>
	void Death();

private: // �����o�ϐ�

#pragma region ���N���X
	//�p�[�e�B�N��
	ParticleManager* particleMan;
	Input* input;
	Audio* audio;
#pragma endregion

#pragma region �I�u�W�F�N�g�E���f��
	//�{��
	Object3d* obj_Player = nullptr;
	Model* model_Normal = nullptr;
	Model* model_Fire = nullptr;
	Model* model_Water = nullptr;
	Model* model_Leaf = nullptr;

	//�K�C�h�I�u�W�F�N�g
	Model* model_LB = nullptr;
	Object3d* guide_LB = nullptr;
	Model* model_RB = nullptr;
	Object3d* guide_RB = nullptr;
#pragma endregion

#pragma region �e�I�u�W�F�N�g�̐ݒ�ϐ�
	//�{��
	XMFLOAT3 pos_Player{};
	XMFLOAT3 vel_Player{};
	XMFLOAT3 rot_Player{};
	XMFLOAT2 shake{};
	float acc_Player = 0.0f;

	/// <summary>
	/// �e�̕ϐ��Q
	XMFLOAT3 pos_Bullet[PLAYERBULLET_MAX] = {};
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

	//�K�C�h�I�u�W�F�N�g
	float alpha = 0.0f;
#pragma endregion

#pragma region ��ԕω��ϐ�
	//�{��
	short moveFlag = 0;
	short typeFlag = 0;
	int hp = 0;
	int changeTimer = 0;
	bool changeFlag = false;
	int justCount = 0;
	float attckShakeTimer = 0.0f;
	bool attckShakeFlag = false;
	float damegeShakeTimer = 0.0f;
	bool damegeShakeFlag = false;
	bool deathFlag = false;
	

	//�e
	bool fireFlag[PLAYERBULLET_MAX] = { false };
	bool delayFlag = false;
	bool hitFlag[PLAYERBULLET_MAX] = { false };
	int delayTimer = 0;
#pragma endregion

};