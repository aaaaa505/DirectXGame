#pragma once
#include "Sprite.h"

class EnemyUI
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
	static EnemyUI* Create();

public: // �����o�֐�

	// �f�X�g���N�^
	~EnemyUI();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(int enemyHp,int bossHp, int turnNumber, int sceneNo);

#pragma region setter�Egetter
	void SetStageEndFlag(bool stageEndFlag) { this->stageEndFlag = stageEndFlag; }

	bool GetCompletionFlag() { return completionFlag; }
#pragma endregion

private: // �����o�֐�
	// <summary>
	// ��������
	// </summary>
	void Initialize();

	/// <summary>
	/// �}�[�N��ύX����
	/// </summary>
	void MarkChange(int turnNumber, int sceneNo);

	/// <summary>
	/// �Q�[�W�ύX
	/// </summary>
	/// <param name="sceneNo"></param>
	void GageChange(int sceneNo);

	/// <summary>
	/// �X�e�[�W�G���h���̓���
	/// </summary>
	void EndMove();

private: // �����o�ϐ�

	//Hp�o�[
	Sprite* bar = nullptr;
	//Hp�Q�[�W
	Sprite* gage = nullptr;
	//�}�[�N
	Sprite* mark = nullptr;

	//�ݒ�ϐ�
	XMFLOAT2 pos{};

	//��ԕω��ϐ�
	int markFlag = 0;
	bool stageEndFlag = false;
	bool completionFlag = false;
};

