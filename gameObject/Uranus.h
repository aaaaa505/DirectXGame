#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "ParticleManager.h"

////���[�h�ύX���̉��o�ő�e
const int TEMPESTCHARGEBULLET_MAX = 36;

class Uranus
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
	static Uranus* Create();

public: // �����o�֐�

	// �f�X�g���N�^
	~Uranus();

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

#pragma region �E���k�X
	//���W
	XMFLOAT3 GetPos() { return boss.obj->GetPosition(); }

	//�퓬�J�n�t���b�O
	bool GetStartFlag() { return boss.startFlag; }
	void SetStartflag(bool startFlag) { this->boss.startFlag = startFlag; }

	//�����蔻��p
	XMFLOAT3 GetCollsionUranus() { return { boss.pos.x, boss.pos.y, boss.pos.z }; }
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

#pragma region �E�B���h
	//�����蔻��p
	XMFLOAT3 GetCollisionWind(int i) { return circle.obj[i]->GetPosition(); }
	bool GetOutBreakFlag(int i) { return wind.outBreakFlag[i]; }
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
	/// ���[�h�ύX
	/// </summary>
	void ModeChange();

	/// <summary>
	/// ��Ԉڍs
	/// </summary>
	void StatusChange();

	/// <summary>
	/// �U������
	/// </summary>
	/// <param name="turnNomber"></param>
	void AttackPreparation();

	/// <summary>
	/// �E�B���h�̋���
	/// </summary>
	void WindMove();

	/// <summary>
	/// �e���y�X�g�̋���
	/// </summary>
	void TempestMove();

	/// <summary>
	/// �I�[�v�X�P�U���p�^�[��
	/// </summary>
	/// <param name="field_completionFlag"></param>
	void CrossWind();

	/// <summary>
	/// �I�[�v�X�Q�U���p�^�[��
	/// </summary>
	/// <param name="field_completionFlag"></param>
	void SlantingWind();

	/// <summary>
	/// �I�[�v�X�R�U���p�^�[��
	/// </summary>
	/// <param name="field_completionFlag"></param>
	void BlockWind();

	/// <summary>
	/// �U���p�^�[��1
	/// </summary>
	void TempestWind1(bool field_completionFlag);

	/// <summary>
	/// �U���p�^�[��2
	/// </summary>
	void TempestWind2(bool field_completionFlag);

	/// <summary>
	/// �e�̃p�[�e�B�N�����o
	/// </summary>
	void WindEffect();

	/// <summary>
	/// �`���[�W���o
	/// </summary>
	void ChargeEffect(bool field_completionFlag);

	/// <summary>
	/// �e���y�X�g���o
	/// </summary>
	void TempestEfect();

	/// <summary>
	/// �U���I��
	/// </summary>
	void AttackEnd();

	/// <summary>
	/// ���S���o
	/// </summary>
	void Death(int turnNomber);

	/// <summary>
	/// �I�[�v�X�U�����̃V�F�C�N���o
	/// </summary>
	void AttackShake();

	/// <summary>
	/// �I�[�v�X��e���̃V�F�C�N���o
	/// </summary>
	void DamegeShake();

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
		Object3d* obj[9] = { nullptr };
		Model* model = nullptr;

		//�ݒ�ϐ�
		XMFLOAT3 pos[9]{};
		XMFLOAT3 rot{};
		XMFLOAT3 scale{};
	};

	Circle circle;
#pragma endregion

#pragma region �E�B���h�\����
	typedef struct Wind
	{
		//�I�u�W�F�N�g�E���f��
		Object3d* obj[10] = { nullptr };
		Model* model = nullptr;

		//�ݒ�ϐ�
		XMFLOAT3 pos[10]{};
		XMFLOAT3 rot{};
		XMFLOAT3 tempestRot{};
		XMFLOAT3 scale[10]{};
		float alpha = 0.0f;

		//��ԕω��ϐ�
		int outBreakTimer = 0;
		bool outBreakFlag[10] = { false };
		int delayTimer = 0;
		bool delayFlag = false;
	};

	Wind wind;
#pragma endregion

#pragma region �`���[�W���o�p�\����
	typedef struct Charge
	{
		//�ݒ�ϐ�
		XMFLOAT3 pos[TEMPESTCHARGEBULLET_MAX]{};
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

