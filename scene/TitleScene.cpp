#include "TitleScene.h"
#include "SafeDelete.h"
#include "LoadSprite.h"


TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	safe_delete(title.sprite);
	safe_delete(manual.sprite);
}

void TitleScene::Initialize(DirectXCommon* dxCommon, SceneDataBuffer* sceneDateBuffer)
{
	// �C���X�^���X�̎擾
	this->input = Input::GetInstance();
	this->audio = Audio::GetInstance();

	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(sceneDateBuffer);

	this->dxCommon = dxCommon;
	this->sceneNo = sceneDateBuffer->sceneNo;
	this->stageNo = sceneDateBuffer->stageNo;
	this->sceneDateBuffer = sceneDateBuffer;

	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	// �X�v���C�g����
	title.sprite = Sprite::Create(titleNumber, { 0.0f, 0.0f });
	manual.sprite = Sprite::Create(manualNumber, { 0.0f, 0.0f });

	//�I�[�f�B�I�ǂݍ���
	audio->LoadWave("titleBGM.wav");
	volume = 0.0f;
	audio->SetVolume("titleBGM.wav", volume);

}

void TitleScene::Update()
{
	switch (screenNum)
	{
	case MANUAL_SCREEN: // �}�j���A�����
		ManualUpdate();
		break;
	case TITLEL_SCREEN: // �^�C�g�����
		TitleUpdate();
		break;
	case CHANGE_SCENE: // �V�[���؂�ւ�
		ChangeScene();
		break;
	}

	if (screenNum == TITLEL_SCREEN || screenNum == CHANGE_SCENE)
	{
		audio->SetVolume("titleBGM.wav", volume);
		audio->PlayWave("titleBGM.wav", true);
	}

	title.sprite->SetAlpha(title.alpha);
	manual.sprite->SetAlpha(manual.alpha);
	particleMan->Update();
	SceneDateBufferUpdate();
}

void TitleScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	//�^�C�g���`��
	title.sprite->Draw();
	manual.sprite->Draw();
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�`��
	Object3d::PreDraw(cmdList);
	// �p�[�e�B�N���̕`��
	particleMan->Draw(cmdList);
	Object3d::PostDraw();
#pragma endregion
}

void TitleScene::ManualUpdate()
{
	// �}�j���A���X�v���C�g�̃A���t�@�l��1.0f�łȂ��ꍇ
	if (manual.alpha != 1.0f)
	{
		// �}�j���A���X�v���C�g�̃A���t�@�l��0.01f�グ��
		manual.alpha += 0.01;
		// �}�j���A���X�v���C�g�̃A���t�@�l��1.0f��荂���Ȃ�����
		if (manual.alpha > 1.0f)
		{
			// �}�j���A���X�v���C�g�̃A���t�@�l��1.0f�ɂ���
			manual.alpha = 1.0f;
		}
	}
	// �}�j���A���X�v���C�g�̃A���t�@�l��1.0f�ŃQ�[���p�b�h���ڑ�����Ă���Ȃ�
	if (manual.alpha == 1.0f && input->GetJoyPadSet())
	{
		// �Q�[���p�b�h��A�{�^���ŃV�[���؂�ւ�
		if (input->GetJoyButton(JoyAButton))
		{
			screenNum = TITLEL_SCREEN;
		}
	}
}

void TitleScene::TitleUpdate()
{
	// �}�j���A���X�v���C�g�̃A���t�@�l��0.0f�łȂ��ꍇ
	if (manual.alpha != 0.0f)
	{
		// �}�j���A���X�v���C�g�̃A���t�@�l��0.01f������
		manual.alpha -= 0.01f;
		// �}�j���A���X�v���C�g�̃A���t�@�l��0.0f���Ⴍ�Ȃ�����
		if (manual.alpha < 0.0f)
		{
			// �}�j���A���X�v���C�g�̃A���t�@�l��0.0f�ɂ���
			manual.alpha = 0.0f;
		}
	}

	// �}�j���A���X�v���C�g�̃A���t�@�l��0.0f�̏ꍇ
	if (manual.alpha == 0.0f)
	{
		// �^�C�g���X�v���C�g�̃A���t�@�l��1.0f�łȂ��ꍇ
		if (title.alpha != 1.0f)
		{
			//�{�����[�����グ��
			volume += 0.0005f;
			// �^�C�g���X�v���C�g�̃A���t�@�l��0.01f�グ��
			title.alpha += 0.01;
			// �^�C�g���X�v���C�g�̃A���t�@�l��1.0f��荂���Ȃ�����
			if (title.alpha > 1.0f)
			{
				// �^�C�g���X�v���C�g�̃A���t�@�l��1.0f�ɂ���
				title.alpha = 1.0f;
			}
		}
		// �^�C�g���X�v���C�g�̃A���t�@�l��1.0f�̏ꍇ
		if (title.alpha == 1.0f)
		{
			// �^�C�g���G�t�F�N�g�𔭐�������
			TitleEfect();
			for (USHORT i = 0; i < 4; i++)
			{
				// �Q�[���p�b�h��A,B,X,Y�̂ǂꂩ�̃{�^���������ꂽ�ꍇ
				if (input->GetJoyButton(i))
				{
					screenNum = CHANGE_SCENE;
					break;
				}
			}
		}
	}
}

void TitleScene::ChangeScene()
{
	title.alpha -= 0.01f;
	// �^�C�g���X�v���C�g�̃A���t�@�l��0.0f�ȉ��Ȃ�
	if (title.alpha <= 0.0f)
	{
		// �^�C�g���̃A���t�@�l��0.0�ɂ���
		title.alpha = 0.01;
		// �p�[�e�B�N����S�폜
		particleMan->SetDeleteFlag(true);
		// �Z���N�g�V�[���֐؂�ւ�
		sceneNo = SceneNo::SELECT_SCENE;
	}
}

void TitleScene::TitleEfect()
{
	for (int j = 0; j < 5; j++)
	{
		const float rnd_pos = 100.0f;
		particle.pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
		particle.pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
		particle.pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;

		particle.vel = { 0.0f, 0.01f, 0.00f };

		particle.acc = { 0.0f, 0.0f, 0.0f };

		particleMan->Add(200, particle.pos, particle.vel, particle.acc, 0.2f, 0.1f, { 20.0f, 20.0f, 20.0f, 1.0f });

	}
}
