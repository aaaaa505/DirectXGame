#pragma once

#include "GameScene.h"

/// <summary>
/// �v���C�V�[��
/// </summary>
class PlayScene : public GameScene
{
public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PlayScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayScene()override;

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

private: // �����o�֐�
	/// <summary>
	/// �v���g���X�e�[�W�X�V
	/// </summary>
	void PlutonStageUpdata();

	/// <summary>
	/// �|�Z�C�h���X�e�[�W�X�V
	/// </summary>
	void PoseidonStageUpdata();

	/// <summary>
	/// �E���k�X�X�e�[�W�X�V
	/// </summary>
	void UranusStageUpdata();

	/// <summary>
	/// �v���g���X�e�[�W�`��
	/// </summary>
	void PlutonStageDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �|�Z�C�h���X�e�[�W�`��
	/// </summary>
	void PoseidonStageDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �E���k�X�X�e�[�W�`��
	/// </summary>
	void UranusStageDraw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
	Camera* camera = nullptr;
	ParticleManager* particleMan = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;

#pragma region �I�u�W�F�N�g�E�X�v���C�g	
	//�N���X�I�u�W�F�N�g
	Player* player = nullptr;
	Mana* mana = nullptr;
	Stage* stage = nullptr;
	FireField* fireField = nullptr;
	WaterField* waterField = nullptr;
	FireOopth* fireOopth = nullptr;
	WaterOopth* waterOopth = nullptr;
	LeafOopth* leafOopth = nullptr;
	LeafField* leafField = nullptr;
	Pluton* pluton = nullptr;
	Poseidon* poseidon = nullptr;
	Uranus* uranus = nullptr;

	//�N���X�X�v���C�g
	SceneWhile* sceneWhile;
	PlayerUI* playerUI;
	EnemyUI* enemyUI;

#pragma endregion

#pragma region �J�����p
	XMFLOAT3 cameraPos{};
	XMFLOAT2 cameraShake{};
	float shakeTimer;

#pragma endregion

#pragma region ���̑��ϐ�
	//�V�[���Ǘ�
	int stageNo = 0;

	//�^�[����
	int stageNumber;

	//�{�����[��
	float volume[3] = { 0.0f };
	float endVolume = 0.0f;
	
	bool finishFlag = false;
#pragma endregion

};