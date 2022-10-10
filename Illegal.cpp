#include "Illegal.h"

Illegal::Illegal()
{
}

Illegal::~Illegal()
{
}

void Illegal::Initialize()
{
	// ゲームウィンドウの作成
	win = new WinApp();
	win->CreateGameWindow();

	//DirectX初期化処理
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(win);

	// Inputの初期化処理
	input = Input::GetInstance();
	if (!input->Initialize(win)) {
		assert(0);
	}

	// Audioの初期化処理
	audio = Audio::GetInstance();
	if (!audio->Initialize()) {
		assert(0);
	}

	// スプライト静的初期化処理
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
	}

	// 3Dオブジェクト静的初期化処理
	Object3d::StaticInitialize(dxCommon->GetDevice());

	// ライト静的初期化処理
	LightGroup::StaticInitialize(dxCommon->GetDevice());

	// パーティクルマネージャ初期化処理
	ParticleManager::GetInstance()->Initialize(dxCommon->GetDevice());

	// スプライトのロード処理
	LoadSprite::StaticInitialize();

	// シーンクラスの生成処理
	gameScene = new TitleScene();
	gameScene->Initialize(dxCommon, &sceneDateBuffa);
}

void Illegal::Finalize()
{
	// 各種解放
	safe_delete(gameScene);
	safe_delete(dxCommon);

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
	safe_delete(win);
}

void Illegal::Loop()
{
	while (true)
	{
		// メッセージ処理
		if (win->ProcessMessage()) { break; }
		// 現在のシーンを取得
		USHORT sceneNo = sceneDateBuffa.sceneNo;
		// 更新処理
		Update();
		// 描画処理
		Draw();
		// シーン切り替え
		if (sceneDateBuffa.sceneNo != sceneNo) { ChangeScene(); }
		// 
		if (input->TriggerKey(DIK_ESCAPE)) { break; }
	}
}

void Illegal::Update()
{
	// Inputの更新処理
	input->Update();
	// Sceneの更新処理
	gameScene->Update();
}

void Illegal::Draw()
{
	// 描画前処理
	dxCommon->PreDraw();
	// シーンの描画
	gameScene->Draw();
	// 描画後処理
	dxCommon->PostDraw();
}

void Illegal::ChangeScene()
{
	// シーン切り替え
	safe_delete(gameScene);
	switch (sceneDateBuffa.sceneNo)
	{
	case TITLE_SCENE: // タイトルシーンの生成
		gameScene = new TitleScene();
		gameScene->Initialize(dxCommon, &sceneDateBuffa);
		break;
	case SELECT_SCENE: // セレクトシーンの生成
		gameScene = new SelectScene();
		gameScene->Initialize(dxCommon, &sceneDateBuffa);
		break;
	case GAME_SCENE: // ゲームシーンの生成
		gameScene = new PlayScene();
		gameScene->Initialize(dxCommon, &sceneDateBuffa);
		break;
	}
}
