#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "Input.h"
#include "ParticleManager.h"

enum TYEP
{
	NORMAL_TYPE,
	FIRE_TYPE,
	WATER_TYPE,
	LEAF_TYPE,
};

enum STAGE
{
	POSEIDON_STAGE,
	PLUTON_STAGE,
	URANUS_STAGE,
};

const int PLAYERBULLET_MAX = 50;

class Player
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
	static Player* Create(Input* input);

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool oopth_AttackFlag, bool boss_AttackFlag, bool oopth_StartFlag,bool boss_StartFlag, short turnNumber);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーのHp減少
	/// </summary>
	void Damege(int stageNo);

	/// <summary>
	/// ボスの必殺技被弾時のHp減少
	/// </summary>
	void SpecialAttackDamege(int stageNo);

	//プレイヤーの座標取得
	XMFLOAT3 GetPos() { return pos_Player; }

	//プレイヤーのHp
	int GetHp() { return hp; }
	void SetHp(int Hp) { this->hp = Hp; }

	//変身時間取得
	int GetChangeTimer() { return changeTimer; }

	//属性変更フラッグ
	void SetTypeFlag(short typeFlag) { this->typeFlag = typeFlag; }
	short GetTypeFlag() { return typeFlag; }

	//変身フラッグ
	void SetChangeFlag(bool changeFlag) { this->changeFlag = changeFlag; }

	//被弾時のシェイク演出起動
	void SetDamegeShakeTimer(float damegeShakeTimer) { this->damegeShakeTimer = damegeShakeTimer; }

	//プレイヤー死亡確認フラッグ
	bool GetDeathFlag() { return deathFlag; }

	//あたり判定用
	XMFLOAT3 GetCollisionBullet(int i) { return { pos_Bullet[i].x, pos_Bullet[i].y + 1.0f, pos_Bullet[i].z }; }
	bool GetHitFlag(int i) { return hitFlag[i]; }
	void SetHitFlag(bool hitFlag, int i) { this->hitFlag[i] = hitFlag; }

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
	/// 本体の移動処理
	/// </summary>
	void PlayerMove();

	/// <summary>
	/// 弾の移動処理
	/// </summary>
	void BulletMove();

	/// <summary>
	/// 弾のエフェクト
	/// </summary>
	void FireEffect();

	/// <summary>
	/// 装填
	/// /// </summary>
	/// <param name="playerPos"></param>
	void Loading();

	/// <summary>
	/// 攻撃時のシェイク演出
	/// </summary>
	void Shake();

	/// <summary>
	/// ガイドオブジェクトの出現消失
	/// </summary>
	void Guide_INOUT(bool enemy_AttackFlag);

	/// <summary>
	/// 死亡演出
	/// </summary>
	void Death();

private: // メンバ変数

#pragma region 他クラス
	//パーティクル
	ParticleManager* particleMan;
	Input* input;
	Audio* audio;
#pragma endregion

#pragma region オブジェクト・モデル
	//本体
	Object3d* obj_Player = nullptr;
	Model* model_Normal = nullptr;
	Model* model_Fire = nullptr;
	Model* model_Water = nullptr;
	Model* model_Leaf = nullptr;

	//ガイドオブジェクト
	Model* model_LB = nullptr;
	Object3d* guide_LB = nullptr;
	Model* model_RB = nullptr;
	Object3d* guide_RB = nullptr;
#pragma endregion

#pragma region 各オブジェクトの設定変数
	//本体
	XMFLOAT3 pos_Player{};
	XMFLOAT3 vel_Player{};
	XMFLOAT3 rot_Player{};
	XMFLOAT2 shake{};
	float acc_Player = 0.0f;

	/// <summary>
	/// 弾の変数群
	XMFLOAT3 pos_Bullet[PLAYERBULLET_MAX] = {};
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

	//ガイドオブジェクト
	float alpha = 0.0f;
#pragma endregion

#pragma region 状態変化変数
	//本体
	short moveFlag = 0;
	short typeFlag = 0;
	int hp = 0;
	int changeTimer = 0;
	bool changeFlag = false;
	int justCount = 0;
	float attckShakeTimer = 0.0f;
	bool attckShakeFlag = false;
	float damegeShakeTimer = 0.0f;
	bool damegeShakeFlag = false;
	bool deathFlag = false;
	

	//弾
	bool fireFlag[PLAYERBULLET_MAX] = { false };
	bool delayFlag = false;
	bool hitFlag[PLAYERBULLET_MAX] = { false };
	int delayTimer = 0;
#pragma endregion

};