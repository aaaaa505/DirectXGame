#include "SelectScene.h"
#include "SafeDelete.h"
#include "LoadSprite.h"

SelectScene::SelectScene()
{
}

SelectScene::~SelectScene()
{
	for (auto sprite : sprites) {
		safe_delete(sprite.second);
	}
}

void SelectScene::Initialize(DirectXCommon* dxCommon, SceneDataBuffer* sceneDateBuffer)
{
	this->input = Input::GetInstance();
	this->audio = Audio::GetInstance();

	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(sceneDateBuffer);

	this->dxCommon = dxCommon;
	this->sceneNo = sceneDateBuffer->sceneNo;
	this->stageNo = sceneDateBuffer->stageNo;
	this->sceneDateBuffer = sceneDateBuffer;

	//�w�i
	sprites["BackGround"] = Sprite::Create(select_BackGround, { 0.0f, 0.0f });
	//�}�[�NW
	sprites["MarkW"] = Sprite::Create(select_MarkW, { 0.0f, 0.0f });
	//�}�[�NF
	sprites["MarkF"] = Sprite::Create(select_MarkF, { 0.0f, 0.0f });
	//�}�[�NL
	sprites["MarkL"] = Sprite::Create(select_MarkL, { 0.0f, 0.0f });
	//�R���v���[�g�t�H���g
	sprites["Complete"] = Sprite::Create(select_Complete, { 0.0f, 0.0f });

	alpha = 0.0f;
	for (auto sprite : sprites) {
		sprite.second->SetAlpha(alpha);
	}
	audio->LoadWave("decisionSE.wav");
	audio->LoadWave("choiceSE.wav");
	audio->SetVolume("titleBGM.wav", 0.05f);
	audio->SetVolume("decisionSE.wav", 0.1f);
	audio->SetVolume("choiceSE.wav", 0.1f);

	stageNo = FIRE_ELEMENT;
}

void SelectScene::Select()
{
	// �t�F�[�h�C��
	if (alpha != 1.0f)
	{
		alpha += 0.01f;
		if (alpha > 1.0f)
		{
			alpha = 1.0f;
		}
	}

	// �t�F�[�h�C���I����
	if (alpha == 1.0f)
	{
		if (input->GetTriggerLeftStic(SticLeft) && stageNo > WATER_ELEMENT)
		{
			audio->StopWave("choiceSE.wav");
			audio->PlayWave("choiceSE.wav", false);
			stageNo -= 1; 
		}
		if (input->GetTriggerLeftStic(SticRight) && stageNo < LEAF_ELEMENT)
		{
			audio->StopWave("choiceSE.wav");
			audio->PlayWave("choiceSE.wav", false);
			stageNo += 1; 
		}
		// �V�[���؂�ւ�
		if (input->GetTriggerJoyButton(JoyAButton))
		{
			audio->SetVolume("titleBGM.wav", 0.0f);
			audio->StopWave("titleBGM.wav");
			audio->PlayWave("decisionSE.wav", false);
			screenNo = CHANGE_SCENE;
		}
	}
}

void SelectScene::ChangeScene()
{
	// �t�F�[�h�A�E�g
	if (alpha != 0.0f)
	{
		alpha -= 0.01f;
		// �t�F�[�h�A�E�g�I����
		if (alpha <= 0.0f)
		{
			// �V�[���؂�ւ�
			sceneNo = GAME_SCENE;
		}
	}
}

void SelectScene::Update()
{
	// ���݂̉��
	switch (screenNo)
	{
	case SELECT_SCREEN: // �I�����
		Select();
		break;
	case CHANGE_SCENE: // �V�[���؂�ւ�
		ChangeScene();
		break;
	}

	// �A���t�@�l��ݒ�
	for (auto sprite : sprites) {
		sprite.second->SetAlpha(alpha);
	}
	// BGM�Đ�
	if (audio->GetVolume("titleBGM.wav") != 0.0f)
	{
		audio->PlayWave("titleBGM.wav", true);
	}
	SceneDateBufferUpdate();
}

void SelectScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	// �X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �^�C�g���X�v���C�g�`��
	sprites["BackGround"]->Draw();

	// �I�𒆂̃X�v���C�g�`��
	switch (stageNo)
	{
	case WATER_ELEMENT:
		sprites["MarkW"]->Draw();
		break;
	case FIRE_ELEMENT:
		sprites["MarkF"]->Draw();
		break;
	case LEAF_ELEMENT:
		sprites["MarkL"]->Draw();
		break;
	}

	// �R���v���[�g�`��
	if (sceneDateBuffer->sceneClearFlag[0] == true &&
		sceneDateBuffer->sceneClearFlag[1] == true &&
		sceneDateBuffer->sceneClearFlag[2] == true)
	{
		sprites["Complete"]->Draw();
	}

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
}