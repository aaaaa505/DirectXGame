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

	// nullptrチェック
	assert(dxCommon);
	assert(sceneDateBuffer);

	this->dxCommon = dxCommon;
	this->sceneNo = sceneDateBuffer->sceneNo;
	this->stageNo = sceneDateBuffer->stageNo;
	this->sceneDateBuffer = sceneDateBuffer;

	//背景
	sprites["BackGround"] = Sprite::Create(select_BackGround, { 0.0f, 0.0f });
	//マークW
	sprites["MarkW"] = Sprite::Create(select_MarkW, { 0.0f, 0.0f });
	//マークF
	sprites["MarkF"] = Sprite::Create(select_MarkF, { 0.0f, 0.0f });
	//マークL
	sprites["MarkL"] = Sprite::Create(select_MarkL, { 0.0f, 0.0f });
	//コンプリートフォント
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
	// フェードイン
	if (alpha != 1.0f)
	{
		alpha += 0.01f;
		if (alpha > 1.0f)
		{
			alpha = 1.0f;
		}
	}

	// フェードイン終了後
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
		// シーン切り替え
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
	// フェードアウト
	if (alpha != 0.0f)
	{
		alpha -= 0.01f;
		// フェードアウト終了後
		if (alpha <= 0.0f)
		{
			// シーン切り替え
			sceneNo = GAME_SCENE;
		}
	}
}

void SelectScene::Update()
{
	// 現在の画面
	switch (screenNo)
	{
	case SELECT_SCREEN: // 選択画面
		Select();
		break;
	case CHANGE_SCENE: // シーン切り替え
		ChangeScene();
		break;
	}

	// アルファ値を設定
	for (auto sprite : sprites) {
		sprite.second->SetAlpha(alpha);
	}
	// BGM再生
	if (audio->GetVolume("titleBGM.wav") != 0.0f)
	{
		audio->PlayWave("titleBGM.wav", true);
	}
	SceneDateBufferUpdate();
}

void SelectScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	// スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// タイトルスプライト描画
	sprites["BackGround"]->Draw();

	// 選択中のスプライト描画
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

	// コンプリート描画
	if (sceneDateBuffer->sceneClearFlag[0] == true &&
		sceneDateBuffer->sceneClearFlag[1] == true &&
		sceneDateBuffer->sceneClearFlag[2] == true)
	{
		sprites["Complete"]->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
}