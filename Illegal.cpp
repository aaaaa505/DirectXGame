#include "Illegal.h"

Illegal::Illegal()
{
}

Illegal::~Illegal()
{
}

void Illegal::Initialize()
{
	// �Q�[���E�B���h�E�̍쐬
	win = new WinApp();
	win->CreateGameWindow();

	//DirectX����������
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(win);

	// Input�̏���������
	input = Input::GetInstance();
	if (!input->Initialize(win)) {
		assert(0);
	}

	// Audio�̏���������
	audio = Audio::GetInstance();
	if (!audio->Initialize()) {
		assert(0);
	}

	// �X�v���C�g�ÓI����������
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
	}

	// 3D�I�u�W�F�N�g�ÓI����������
	Object3d::StaticInitialize(dxCommon->GetDevice());

	// ���C�g�ÓI����������
	LightGroup::StaticInitialize(dxCommon->GetDevice());

	// �p�[�e�B�N���}�l�[�W������������
	ParticleManager::GetInstance()->Initialize(dxCommon->GetDevice());

	// �X�v���C�g�̃��[�h����
	LoadSprite::StaticInitialize();

	// �V�[���N���X�̐�������
	gameScene = new TitleScene();
	gameScene->Initialize(dxCommon, &sceneDateBuffa);
}

void Illegal::Finalize()
{
	// �e����
	safe_delete(gameScene);
	safe_delete(dxCommon);

	// �Q�[���E�B���h�E�̔j��
	win->TerminateGameWindow();
	safe_delete(win);
}

void Illegal::Loop()
{
	while (true)
	{
		// ���b�Z�[�W����
		if (win->ProcessMessage()) { break; }
		// ���݂̃V�[�����擾
		USHORT sceneNo = sceneDateBuffa.sceneNo;
		// �X�V����
		Update();
		// �`�揈��
		Draw();
		// �V�[���؂�ւ�
		if (sceneDateBuffa.sceneNo != sceneNo) { ChangeScene(); }
		// 
		if (input->TriggerKey(DIK_ESCAPE)) { break; }
	}
}

void Illegal::Update()
{
	// Input�̍X�V����
	input->Update();
	// Scene�̍X�V����
	gameScene->Update();
}

void Illegal::Draw()
{
	// �`��O����
	dxCommon->PreDraw();
	// �V�[���̕`��
	gameScene->Draw();
	// �`��㏈��
	dxCommon->PostDraw();
}

void Illegal::ChangeScene()
{
	// �V�[���؂�ւ�
	safe_delete(gameScene);
	switch (sceneDateBuffa.sceneNo)
	{
	case TITLE_SCENE: // �^�C�g���V�[���̐���
		gameScene = new TitleScene();
		gameScene->Initialize(dxCommon, &sceneDateBuffa);
		break;
	case SELECT_SCENE: // �Z���N�g�V�[���̐���
		gameScene = new SelectScene();
		gameScene->Initialize(dxCommon, &sceneDateBuffa);
		break;
	case GAME_SCENE: // �Q�[���V�[���̐���
		gameScene = new PlayScene();
		gameScene->Initialize(dxCommon, &sceneDateBuffa);
		break;
	}
}
