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
	// インスタンスの取得
	this->input = Input::GetInstance();
	this->audio = Audio::GetInstance();

	// nullptrチェック
	assert(dxCommon);
	assert(sceneDateBuffer);

	this->dxCommon = dxCommon;
	this->sceneNo = sceneDateBuffer->sceneNo;
	this->stageNo = sceneDateBuffer->stageNo;
	this->sceneDateBuffer = sceneDateBuffer;

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	// スプライト生成
	title.sprite = Sprite::Create(titleNumber, { 0.0f, 0.0f });
	manual.sprite = Sprite::Create(manualNumber, { 0.0f, 0.0f });

	//オーディオ読み込み
	audio->LoadWave("titleBGM.wav");
	volume = 0.0f;
	audio->SetVolume("titleBGM.wav", volume);

}

void TitleScene::Update()
{
	switch (screenNum)
	{
	case MANUAL_SCREEN: // マニュアル画面
		ManualUpdate();
		break;
	case TITLEL_SCREEN: // タイトル画面
		TitleUpdate();
		break;
	case CHANGE_SCENE: // シーン切り替え
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
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	//タイトル描画
	title.sprite->Draw();
	manual.sprite->Draw();
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	Object3d::PreDraw(cmdList);
	// パーティクルの描画
	particleMan->Draw(cmdList);
	Object3d::PostDraw();
#pragma endregion
}

void TitleScene::ManualUpdate()
{
	// マニュアルスプライトのアルファ値が1.0fでない場合
	if (manual.alpha != 1.0f)
	{
		// マニュアルスプライトのアルファ値を0.01f上げる
		manual.alpha += 0.01;
		// マニュアルスプライトのアルファ値が1.0fより高くなったら
		if (manual.alpha > 1.0f)
		{
			// マニュアルスプライトのアルファ値を1.0fにする
			manual.alpha = 1.0f;
		}
	}
	// マニュアルスプライトのアルファ値が1.0fでゲームパッドが接続されているなら
	if (manual.alpha == 1.0f && input->GetJoyPadSet())
	{
		// ゲームパッドのAボタンでシーン切り替え
		if (input->GetJoyButton(JoyAButton))
		{
			screenNum = TITLEL_SCREEN;
		}
	}
}

void TitleScene::TitleUpdate()
{
	// マニュアルスプライトのアルファ値が0.0fでない場合
	if (manual.alpha != 0.0f)
	{
		// マニュアルスプライトのアルファ値を0.01f下げる
		manual.alpha -= 0.01f;
		// マニュアルスプライトのアルファ値が0.0fより低くなったら
		if (manual.alpha < 0.0f)
		{
			// マニュアルスプライトのアルファ値を0.0fにする
			manual.alpha = 0.0f;
		}
	}

	// マニュアルスプライトのアルファ値が0.0fの場合
	if (manual.alpha == 0.0f)
	{
		// タイトルスプライトのアルファ値が1.0fでない場合
		if (title.alpha != 1.0f)
		{
			//ボリュームを上げる
			volume += 0.0005f;
			// タイトルスプライトのアルファ値を0.01f上げる
			title.alpha += 0.01;
			// タイトルスプライトのアルファ値が1.0fより高くなったら
			if (title.alpha > 1.0f)
			{
				// タイトルスプライトのアルファ値を1.0fにする
				title.alpha = 1.0f;
			}
		}
		// タイトルスプライトのアルファ値が1.0fの場合
		if (title.alpha == 1.0f)
		{
			// タイトルエフェクトを発生させる
			TitleEfect();
			for (USHORT i = 0; i < 4; i++)
			{
				// ゲームパッドのA,B,X,Yのどれかのボタンが押された場合
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
	// タイトルスプライトのアルファ値が0.0f以下なら
	if (title.alpha <= 0.0f)
	{
		// タイトルのアルファ値を0.0にする
		title.alpha = 0.01;
		// パーティクルを全削除
		particleMan->SetDeleteFlag(true);
		// セレクトシーンへ切り替え
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
