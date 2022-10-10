#pragma once
#include "GameScene.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public GameScene
{
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene()override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SceneDataBuffer* sceneDateBuffer) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private://メンバ関数
	/// <summary>
	/// マニュアル画面更新処理
	/// </summary>
	void ManualUpdate();

	/// <summary>
	/// タイトル画面更新処理
	/// </summary>
	void TitleUpdate();

	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	void ChangeScene();

	/// <summary>
	/// タイトルエフェクト処理
	/// </summary>
	void TitleEfect();
	
private: // メンバクラス
	/// <summary>
	/// 画面ごとの必要変数
	/// </summary>
	struct Screen
	{
		Sprite* sprite = nullptr;		// スプライト
		float alpha = 0.0f;				// アルファ値
	};

	/// <summary>
	/// パーティクル用構造体
	/// </summary>
	struct Particle
	{
		XMFLOAT3 pos{};
		XMFLOAT3 vel{};
		XMFLOAT3 acc{};
		XMFLOAT4 color{};
		int life = 0;
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
	};

	/// <summary>
	/// スクリーンナンバー
	/// </summary>
	enum ScreenNum
	{
		MANUAL_SCREEN,
		TITLEL_SCREEN,
		CHANGE_SCENE,
	};

private: // メンバ変数
	Camera* camera = nullptr;				// カメラ
	ParticleManager* particleMan = nullptr;	// パーティクルマネージャー
	Screen title;							// タイトル画面用構造体
	Screen manual;							// マニュアル画面用構造体
	Particle particle;						// パーティクル
	USHORT screenNum = MANUAL_SCREEN;		// 画面切り替え用
	float volume = 0.0f;					// ボリューム
};