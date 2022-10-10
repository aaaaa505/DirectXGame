#pragma once
#include "Sprite.h"

#include "string"
#include "map"

class PlayerUI
{
private: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: // 静的メンバ関数
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static PlayerUI* Create();

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerUI();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerUI();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(int playerHp, int changeTimer, short typeFlag);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

#pragma region setter・getter
	void SetStageEndFlag(bool stageEndFlag) { this->stageEndFlag = stageEndFlag; }

	bool GetCompletionFlag() { return completionFlag; }
#pragma endregion


private: // メンバ関数
	// <summary>
	// 生成処理
	// </summary>
	void Initialize();

	/// <summary>
	/// ステージエンド時の動作
	/// </summary>
	void EndMove();

private: // メンバ変数
	// スプライト
	std::map<std::string, Sprite*> sprites;

	// 設定変数
	XMFLOAT2 pos{};

	// 状態変化変数
	short markFlag = 0;
	bool stageEndFlag = false;
	bool completionFlag = false;
};

