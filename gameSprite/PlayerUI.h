#pragma once
#include "Sprite.h"

#include "string"
#include "map"

class PlayerUI
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
	static PlayerUI* Create();

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PlayerUI();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayerUI();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(int playerHp, int changeTimer, short typeFlag);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

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
	/// �X�e�[�W�G���h���̓���
	/// </summary>
	void EndMove();

private: // �����o�ϐ�
	// �X�v���C�g
	std::map<std::string, Sprite*> sprites;

	// �ݒ�ϐ�
	XMFLOAT2 pos{};

	// ��ԕω��ϐ�
	short markFlag = 0;
	bool stageEndFlag = false;
	bool completionFlag = false;
};

