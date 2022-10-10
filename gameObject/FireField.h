#pragma once
#include "Object3d.h"
#include "Audio.h"

class FireField
{
private: // �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FireField* Create();

public: // �����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FireField();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(int turnNomber, bool modeChangeFlag);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

#pragma region setter�Egetter
	//�U���͈͕\���N���t���b�O
	bool GetShowRangeFlag() { return showRangeFlag; }
	void SetShowRangeFlag(bool showRangeFlag) { this->showRangeFlag = showRangeFlag; }

	//�U���͈͕\�������t���b�O
	bool GetCompletionFlag() { return completionFlag; }
	void SetCompletionFlag(bool completionFlag) { this->completionFlag = completionFlag; }

	//�ċN���t���b�O
	bool GetResetFlag() { return resetFlag; }
	void SetResetFlag(bool resetFlag) { this->resetFlag = resetFlag; }

#pragma endregion


private: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �U���p�^�[��1�͈͕\��
	/// </summary>
	void OneLineOrderShotScope();

	/// <summary>
	/// �U���p�^�[��2�͈͕\��
	/// </summary>
	void TwinLineOrderShotScope();

	/// <summary>
	/// �U���p�^�[��3�͈͕\��
	/// </summary>
	void AllLineOrderShotScope();

	/// <summary>
	/// �{�X�U���p�^�[���P�͈͕\��
	/// </summary>
	void FlameShot1Scope();

	/// <summary>
	/// �{�X�U���p�^�[���P�͈͕\��
	/// </summary>
	void FlameShot2Scope();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

private: // �����o�ϐ�
#pragma region ���N���X
	Audio* audio = nullptr;
#pragma endregion


#pragma region �I�u�W�F�N�g�E���f��
	Object3d* obj_Field[3][3];
	Model* model_Normal;
	Model* model_Scope;
#pragma endregion

#pragma region ��ԕω��ϐ�
	bool scopeFlag[3][3];
	bool showRangeFlag;
	bool completionFlag;
	bool resetFlag;
	int processNomber;
	int showTimer;
#pragma endregion


};

