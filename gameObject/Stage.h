#pragma once
#include "Object3d.h"

#include "GameScene.h"

class Stage
{
private: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Stage();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(const USHORT& sceneNo);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private: // メンバ変数
	// オブジェクト
	Object3d* obj = nullptr;
	// モデル
	Model* model = nullptr;
};

