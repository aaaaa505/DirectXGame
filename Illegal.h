#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "GameScene.h"
#include "TitleScene.h" 
#include "SelectScene.h"
#include "PlayScene.h"
#include "LightGroup.h"
#include "ParticleManager.h"
#include "LoadSprite.h"

/// <summary>
/// ゲーム固有のクラス
/// </summary>
class Illegal
{
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Illegal();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual~Illegal();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ゲームループ
	/// </summary>
	void Loop();

protected: // メンバ関数
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	void ChangeScene();

protected: // メンバ関数
	// WindowsAPI基盤クラス
	WinApp* win = nullptr;
	// DirectX12基盤クラス
	DirectXCommon* dxCommon = nullptr;
	// Inputクラス
	Input* input = nullptr;
	// Audioクラス
	Audio* audio = nullptr;
	// Sceneクラス
	GameScene* gameScene = nullptr;
	// シーン管理用バッファ
	SceneDataBuffer sceneDateBuffa;
};