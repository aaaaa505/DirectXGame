#pragma once
#include "Sprite.h"

class EnemyUI
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
	static EnemyUI* Create();

public: // メンバ関数

	// デストラクタ
	~EnemyUI();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(int enemyHp,int bossHp, int turnNumber, int sceneNo);

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
	/// マークを変更する
	/// </summary>
	void MarkChange(int turnNumber, int sceneNo);

	/// <summary>
	/// ゲージ変更
	/// </summary>
	/// <param name="sceneNo"></param>
	void GageChange(int sceneNo);

	/// <summary>
	/// ステージエンド時の動作
	/// </summary>
	void EndMove();

private: // メンバ変数

	//Hpバー
	Sprite* bar = nullptr;
	//Hpゲージ
	Sprite* gage = nullptr;
	//マーク
	Sprite* mark = nullptr;

	//設定変数
	XMFLOAT2 pos{};

	//状態変化変数
	int markFlag = 0;
	bool stageEndFlag = false;
	bool completionFlag = false;
};

