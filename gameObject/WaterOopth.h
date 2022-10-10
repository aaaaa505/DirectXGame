#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "ParticleManager.h"

//最大弾数
const int WATEROOPTHBULLET_MAX = 120;

class WaterOopth
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
	static WaterOopth* Create();

public: // メンバ関数

	// デストラクタ
	~WaterOopth();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// エネミーのHp減少
	/// </summary>
	void Damage(short player_TypeFlag);

#pragma region setter・getter

#pragma region オープス
	//座標
	float GetPos() { return bullet.pos[0].z; }

	//戦闘開始フラッグ
	bool GetStartFlag() { return oopth.startFlag; }
	void SetStartflag(bool startFlag) { this->oopth.startFlag = startFlag; }

	//あたり判定用
	XMFLOAT3 GetCollsionOopth() { return { oopth.pos.x, oopth.pos.y + 0.5f, oopth.pos.z }; }
	float GetSphereRadius() { return oopth.radius; }

	//エネミーの体力
	int GetHp() { return oopth.hp; }

	//エネミー再起動フラッグ
	bool GetResetFlag() { return oopth.resetFlag; }
	void SetResetFlag(bool resetFlag) { this->oopth.resetFlag = resetFlag; }

	//攻撃フラッグ
	bool GetAttackFlag() { return oopth.attackFlag; }

	//攻撃準備起動フラッグ
	bool GetAttackPreparationFlag() { return oopth.attackPreparationFlag; }
	void SetAttackPreparationFlag(bool attackPreparation) { this->oopth.attackPreparationFlag = attackPreparation; }

	//攻撃終了フラッグ
	bool GetAttackEndFlag() { return oopth.attackEndFlag; }

	//被弾時のシェイク演出起動
	void SetDamegeShakeFlag(bool damegeShakeflag) { this->oopth.damegeShakeFlag = damegeShakeflag; }
#pragma endregion

#pragma region バレット
	//あたり判定用
	XMFLOAT3 GetCollisionBullet(int i) { return bullet.pos[i]; }

	//攻撃結果フラッグ
	bool GetHitFlag(int i) { return bullet.hitFlag[i]; }
	void SetHitFlag(bool hitFlag, int i) { this->bullet.hitFlag[i] = hitFlag; }
#pragma endregion


#pragma region 外部干渉
	//着地時シェイク起動フラッグ
	bool GetShakeFlag() { return shakeFlag; }
	void SetShakeFlag(bool shakeFlag) { this->shakeFlag = shakeFlag; }
#pragma endregion

#pragma endregion
	bool Getdebug() { return bullet.fireFlag[80]; }

private: // メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 戦闘開始前挙動
	/// </summary>
	void Appearance();

	/// <summary>
	/// 死亡演出
	/// </summary>
	void Death(int turnNomber);

	/// <summary>
	/// エネミーを再起動
	/// </summary>
	void Reset();

	/// <summary>
	/// オープスのモデル変更
	/// </summary>
	/// <param name="turnNomber"></param>
	void ModelChange(int turnNomber);

	/// <summary>
	/// 攻撃準備
	/// </summary>
	/// <param name="turnNomber"></param>
	void AttackPreparation();

	/// <summary>
	/// 状態移行
	/// </summary>
	void StatusChange();

	/// <summary>
	/// 装填パターン１
	/// </summary>
	void BulletLoading_1();

	/// <summary>
	/// 装填パターン２
	/// </summary>
	void BulletLoading_2();

	/// <summary>
	/// 装填パターン３
	/// </summary>
	void BulletLoading_3();

	/// <summary>
	/// 挙動パターン１
	/// </summary>
	void BulletMove_1(int i);

	/// <summary>
	/// 挙動パターン２
	/// </summary>
	void BulletMove_2(int i);

	/// <summary>
	/// 挙動パターン３
	/// </summary>
	void BulletMove_3(int i);

	/// <summary>
	/// 0番～39番の発射フラッグをリセット
	/// </summary>
	void FireFlagReset0_39();

	/// <summary>
	/// 40番～79番の発射フラッグをリセット
	/// </summary>
	void FireFlagReset40_79();

	/// <summary>
	/// 80番～99番の発射フラッグをリセット
	/// </summary>
	void FireFlagReset80_119();

	/// <summary>
	/// 攻撃パターン1
	/// </summary>
	void OneSideOrderBlast(bool field_completionFlag);

	/// <summary>
	/// 攻撃パターン2
	/// </summary>
	/// <param name="completionFlag"></param>
	void TwinSideOrderBlast(bool field_completionFlag);

	/// <summary>
	/// 攻撃パターン3
	/// </summary>
	/// <param name="field_completionFlag"></param>
	void TwinLineSideBlast(bool field_completionFlag);

	/// <summary>
	/// オープス攻撃時のシェイク演出
	/// </summary>
	void AttackShake();

	/// <summary>
	/// オープス被弾時のシェイク演出
	/// </summary>
	void DamegeShake();

	/// <summary>
	/// 弾のパーティクル演出
	/// </summary>
	void BulletEffect();

	/// <summary>
	/// 攻撃終了
	/// </summary>
	void AttackEnd();

private: // メンバ変数

#pragma region 他クラス
	ParticleManager* particleMan = nullptr;
	Audio* audio = nullptr;
#pragma endregion

#pragma region 複数使用モデル
	Model* model_Collision = nullptr;
#pragma endregion

#pragma region オープス構造体
	typedef struct Oopth
	{
		//オブジェクト・モデル・コリジョン
		Object3d* obj = nullptr;
		Model* model_1 = nullptr;
		Model* model_2 = nullptr;
		Model* model_3 = nullptr;
		Object3d* collision = nullptr;

		//設定変数
		XMFLOAT3 pos{};
		XMFLOAT2 shake{};
		XMFLOAT3 rot{};
		XMFLOAT3 vel{};
		float acc = 0.0f;
		float alpha1 = 0.0f;
		float alpha2 = 0.0f;
		float alpha3 = 0.0f;
		float radius = 0.0f;
		int hp = 0;

		//状態変化変数
		bool startFlag = false;
		bool resetFlag = false;
		bool attackPreparationFlag = false;
		bool attackFlag = false;
		bool statusChangeFlag = false;
		int changeTimer = 0;
		float attackShakeTimer = 0.0f;
		bool damegeShakeFlag = false;
		float damegeShakeTimer = 0.0f;
		int attackOrder = 0;
		bool attackEndFlag = false;
		bool fixFlag = false;
		int fixTimer = 0;
	};

	Oopth oopth;
#pragma endregion

#pragma region サークル構造体
	typedef struct Circle
	{
		//オブジェクト・モデル
		Object3d* obj[6] = { nullptr };
		Model* model = nullptr;

		//設定変数
		XMFLOAT3 pos[6]{};
		XMFLOAT3 rot{};
		XMFLOAT3 scale{};
	};

	Circle circle;
#pragma endregion

#pragma region バレット構造体
	typedef struct Bullet
	{
		//オブジェクト・モデル
		Object3d* obj[WATEROOPTHBULLET_MAX] = { nullptr };
		Model* model = nullptr;

		//設定変数
		XMFLOAT3 pos[WATEROOPTHBULLET_MAX];
		XMFLOAT3 rot[WATEROOPTHBULLET_MAX];

		//状態変化変数
		bool hitFlag[WATEROOPTHBULLET_MAX] = { false };
		bool fireFlag[WATEROOPTHBULLET_MAX] = { false };
		int delayTimer = 0;
	};

	Bullet bullet;
#pragma endregion

#pragma region パーティクル構造体
	typedef struct Particle
	{
		XMFLOAT3 pos{};
		XMFLOAT3 vel{};
		XMFLOAT3 acc{};
		XMFLOAT4 color{};
	};

	Particle particle;
#pragma endregion

#pragma region 外部干渉用変数
	bool shakeFlag = false;
#pragma endregion


};

