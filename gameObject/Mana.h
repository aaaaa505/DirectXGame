#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "ParticleManager.h"

#include "GameScene.h"

class Mana
{
private: // エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: // 静的メンバ関数
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Mana* Create();

public: // メンバ関数

	// デストラクタ
	~Mana();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool oopth_StartFlag, bool boss_StartFlag);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

#pragma region setter・getter
	//マナ動作フラッグ
	void SetMoveFlag(bool moveFlag) { this->moveFlag = moveFlag; }
	bool GetMoveFlag() { return moveFlag; }
	//マナ動作完了フラッグ
	bool GetCompletionFlag() { return completionFlag; }
	short GetTypeFlag() { return typeFlag; }
	
	//リセットフラッグ
	void SetResetFlag(bool resetFlag) { this->resetFlag = resetFlag; }

	//あたり判定用
	XMFLOAT3 GetPos() { return mana->GetPosition(); }
	bool GetHitFlag() { return hitFlag; }
	void SetHitFlag(bool hitFlag) { this->hitFlag = hitFlag; }
#pragma endregion

private: // メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 属性変更
	/// </summary>
	void TypeChange();

	/// <summary>
	/// 自身のエフェクト
	/// </summary>
	void ManaEffect();

	/// <summary>
	/// ランダム配置
	/// </summary>
	void RandamSetting();

	/// <summary>
	/// マナの動作
	/// </summary>
	void Move();

	/// <summary>
	/// 再起動
	/// </summary>
	void Reset();

private: // メンバ変数

#pragma region 他クラス
	ParticleManager* particleMan = nullptr;
#pragma endregion

#pragma region オブジェクト・モデル
	// オブジェクト
	Object3d* mana = nullptr;
	Object3d* guide = nullptr;

	// モデル
	Model* model_F = nullptr;
	Model* model_W = nullptr;
	Model* model_L = nullptr;
	Model* model_guide = nullptr;
#pragma endregion

#pragma region パーティクル構造体
	typedef struct Particle
	{
		XMFLOAT3 pos{};
		XMFLOAT3 vel{};
		XMFLOAT3 acc{};
		XMFLOAT4 color{};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
	};
	Particle particle;
#pragma endregion

#pragma region 設定変数
	//座標
	XMFLOAT3 pos{};
	//移動量
	XMFLOAT3 vel{};
	//半径
	float radius = 0.0f;
	//アルファ値
	float alpha = 0.0f;

#pragma endregion

#pragma region 状態変化変数
	int placementFlag = 0;
	short typeFlag = 0;
	bool moveFlag = false;
	bool completionFlag = false;
	bool settingFlag = false;
	bool hitFlag = false;
	int moveTimer = 0;
	bool resetFlag = false;
#pragma endregion
};

