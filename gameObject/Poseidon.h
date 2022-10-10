#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "ParticleManager.h"

//�ő�e��
const int POSEIDONBULLET_MAX = 440;//360�ʏ� 80/2���[�h�`�F���W

//���[�h�ύX���̉��o�ő�e
const int SPLASHCHARGEBULLET_MAX = 18;

class Poseidon
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
	static Poseidon* Create();

public: // �����o�֐�

	// �f�X�g���N�^
	~Poseidon();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �G�l�~�[��Hp����
	/// </summary>
	void Damage(short player_TypeFlag);

#pragma region setter�Egetter

#pragma region �|�Z�C�h��
	//���W
	float GetPos() { return bullet.pos[0].z; }

	//�퓬�J�n�t���b�O
	bool GetStartFlag() { return boss.startFlag; }
	void SetStartflag(bool startFlag) { this->boss.startFlag = startFlag; }

	//�����蔻��p
	XMFLOAT3 GetCollsionPoseidon() { return { boss.pos.x, boss.pos.y, boss.pos.z }; }
	float GetSphereRadius() { return boss.radius; }

	//�G�l�~�[�̗̑�
	int GetHp() { return boss.hp; }

	//�G�l�~�[�ċN���t���b�O
	bool GetResetFlag() { return boss.resetFlag; }
	void SetResetFlag(bool resetFlag) { this->boss.resetFlag = resetFlag; }

	//�U���t���b�O
	bool GetAttackFlag() { return boss.attackFlag; }

	//�U�������N���t���b�O
	bool GetAttackPreparationFlag() { return boss.attackPreparationFlag; }
	void SetAttackPreparationFlag(bool attackPreparation) { this->boss.attackPreparationFlag = attackPreparation; }

	//�U���I���t���b�O
	bool GetAttackEndFlag() { return boss.attackEndFlag; }

	//��e���̃V�F�C�N���o�N��
	void SetDamegeShakeFlag(bool damegeShakeflag) { this->boss.damegeShakeFlag = damegeShakeflag; }

	//���[�h�ύX�t���b�O
	bool GetModeChangeFlag() { return boss.modeChangeFlag; }

	//�{�X�m�F�t���b�O
	bool GetDeathFlag() { return boss.deathFlag; }
#pragma endregion

#pragma region �o���b�g
	//�����蔻��p
	XMFLOAT3 GetCollisionBullet(int i) { return bullet.pos[i]; }

	//�U�����ʃt���b�O
	bool GetHitFlag(int i) { return bullet.hitFlag[i]; }
	void SetHitFlag(bool hitFlag, int i) { this->bullet.hitFlag[i] = hitFlag; }

	bool GetFireFlag() { return bullet.fireFlag[350]; }
#pragma endregion

#pragma region �O������
	//���n���V�F�C�N�N���t���b�O
	bool GetShakeFlag() { return shakeFlag; }
	void SetShakeFlag(bool shakeFlag) { this->shakeFlag = shakeFlag; }
#pragma endregion

#pragma endregion

private: // �����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �퓬�J�n�O����
	/// </summary>
	void Appearance();

	/// <summary>
	/// ���S���o
	/// </summary>
	void Death(int turnNomber);

	/// <summary>
	/// �U������
	/// </summary>
	/// <param name="turnNomber"></param>
	void AttackPreparation();

	/// <summary>
	/// ��Ԉڍs
	/// </summary>
	void StatusChange();

	/// <summary>
	/// ���[�h�ύX
	/// </summary>
	void ModeChange();
	
	/// <summary>
	/// ���U
	/// </summary>
	void BulletLoading_1();

	/// <summary>
	/// ���U�p�^�[���Q
	/// </summary>
	void BulletLoading_2();

	/// <summary>
	/// ���U�p�^�[���R
	/// </summary>
	void BulletLoading_3();

	/// <summary>
	/// ���U�p�^�[���S
	/// </summary>
	void BulletLoading_4();

	/// <summary>
	/// �����p�^�[���P
	/// </summary>
	void BulletMove_1(int i);

	/// <summary>
	/// �����p�^�[���Q
	/// </summary>
	void BulletMove_2(int i);

	/// <summary>
	/// �����p�^�[���R
	/// </summary>
	void BulletMove_3(int i);

	/// <summary>
	/// �����p�^�[���S
	/// </summary>
	/// <param name="i"></param>
	void BulletMove_4(int i);

	/// <summary>
	/// �U���p�^�[��1
	/// </summary>
	void SplashBlast1(bool field_completionFlag);

	/// <summary>
	/// �U���p�^�[��2
	/// </summary>
	void SplashBlast2(bool field_completionFlag);

	/// <summary>
	/// 0�ԁ`39�Ԃ̔��˃t���b�O�����Z�b�g
	/// </summary>
	void FireFlagReset240_279();

	/// <summary>
	/// 40�ԁ`79�Ԃ̔��˃t���b�O�����Z�b�g
	/// </summary>
	void FireFlagReset280_319();

	/// <summary>
	/// 80�ԁ`99�Ԃ̔��˃t���b�O�����Z�b�g
	/// </summary>
	void FireFlagReset320_359();

	/// <summary>
	/// �I�[�v�X�U�����̃V�F�C�N���o
	/// </summary>
	void AttackShake();

	/// <summary>
	/// �I�[�v�X��e���̃V�F�C�N���o
	/// </summary>
	void DamegeShake();

	/// <summary>
	/// �`���[�W���o
	/// </summary>
	void ChargeEffect(bool field_completionFlag);

	/// <summary>
	/// �t���C���G�t�F�N�g
	/// </summary>
	void BulletEffect();

	/// <summary>
	/// �U���I��
	/// </summary>
	void AttackEnd();

private: // �����o�ϐ�

#pragma region ���N���X
	ParticleManager* particleMan = nullptr;
	Audio* audio = nullptr;
#pragma endregion

#pragma region �����g�p���f��
	Model* model_Collision = nullptr;
#pragma endregion

#pragma region �{�X�\����
	typedef struct Boss
	{
		//�I�u�W�F�N�g�E���f���E�R���W����
		Object3d* obj = nullptr;
		Model* model = nullptr;
		Object3d* collision = nullptr;

		//�ݒ�ϐ�
		XMFLOAT3 pos{};
		XMFLOAT2 shake{};
		XMFLOAT3 rot{};
		XMFLOAT3 vel{};
		float acc = 0.0f;
		float alpha = 0.0f;
		float radius = 0.0f;
		int hp = 0;

		//��ԕω��ϐ�
		bool startFlag = false;
		bool resetFlag = false;
		bool attackPreparationFlag = false;
		bool attackFlag = false;
		bool statusChangeFlag = false;
		int changeTimer = 0;
		float attackShakeTimer = 0.0f;
		bool damegeShakeFlag = false;
		float damegeShakeTimer = 0.0f;
		int attackOrder = 0;
		bool attackEndFlag = false;
		bool fixFlag = false;
		int fixTimer = 0.0f;
		bool modeChangeFlag = false;
		bool deathFlag = false;
	};

	Boss boss;
#pragma endregion

#pragma region �T�[�N���\����
	typedef struct Circle
	{
		//�I�u�W�F�N�g�E���f��
		Object3d* obj[6] = { nullptr };
		Model* model = nullptr;

		//�ݒ�ϐ�
		XMFLOAT3 pos[6]{};
		XMFLOAT3 rot{};
		XMFLOAT3 scale{};
		float alpha = 0.0f;
	};

	Circle circle;
#pragma endregion

#pragma region �o���b�g�\����
	typedef struct Bullet
	{
		//�R���W����
		Object3d* obj[POSEIDONBULLET_MAX];
		Model* model = nullptr;

		//�ݒ�ϐ�
		XMFLOAT3 pos[POSEIDONBULLET_MAX];
		XMFLOAT3 rot[POSEIDONBULLET_MAX];

		//��ԕω��ϐ�
		bool hitFlag[POSEIDONBULLET_MAX] = { false };
		bool fireFlag[POSEIDONBULLET_MAX] = { false };
		int delayTimer;
	};

	Bullet bullet;
#pragma endregion

#pragma region �`���[�W���o�p�\����
	typedef struct Charge
	{
		//�ݒ�ϐ�
		XMFLOAT3 pos[SPLASHCHARGEBULLET_MAX]{};
		float angle = 0.0f;
		float radius = 0.0f;
	};

	Charge charge;
#pragma endregion

#pragma region �p�[�e�B�N���\����
	typedef struct Particle
	{
		XMFLOAT3 pos{};
		XMFLOAT3 vel{};
		XMFLOAT3 acc{};
		XMFLOAT4 color{};
	};

	Particle particle;
#pragma endregion

#pragma region �O�����p�ϐ�
	bool shakeFlag = false;
#pragma endregion
};

