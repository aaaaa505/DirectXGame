#pragma once
#include "GameScene.h"

#include "map"

class SelectScene : public GameScene
{
private: // �����o�N���X
	// �Z���N�g�V�[���i���o�[
	enum SelectScreenNo
	{
		SELECT_SCREEN,
		CHANGE_SCENE,
	};

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SelectScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SelectScene()override;

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

private:
	/// <summary>
	/// �X�e�[�W�I��
	/// </summary>
	void Select();

	/// <summary>
	/// �V�[���؂�ւ�
	/// </summary>
	void ChangeScene();
private:
	// Sprite�̘A�z�z��
	std::map<std::string, Sprite*> sprites;
	// ��ʐ؂�ւ��ϐ�
	USHORT screenNo = SELECT_SCREEN;
	// �A���t�@�l
	float alpha = 0.0f;
};

