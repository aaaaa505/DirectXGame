#pragma once
#include "GameScene.h"

#include "map"

class SelectScene : public GameScene
{
private: // メンバクラス
	// セレクトシーンナンバー
	enum SelectScreenNo
	{
		SELECT_SCREEN,
		CHANGE_SCENE,
	};

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectScene()override;

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

private:
	/// <summary>
	/// ステージ選択
	/// </summary>
	void Select();

	/// <summary>
	/// シーン切り替え
	/// </summary>
	void ChangeScene();
private:
	// Spriteの連想配列
	std::map<std::string, Sprite*> sprites;
	// 画面切り替え変数
	USHORT screenNo = SELECT_SCREEN;
	// アルファ値
	float alpha = 0.0f;
};

