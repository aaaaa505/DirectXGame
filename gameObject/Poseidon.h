#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "ParticleManager.h"

//最大弾数
const int POSEIDONBULLET_MAX = 440;//360通常 80/2モードチェンジ

//モード変更時の演出最大弾
const int SPLASHCHARGEBULLET_MAX = 18;

class Poseidon
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
	static Poseidon* Create();

public: // メンバ関数

	// デストラクタ
	~Poseidon();

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

#pragma region ポセイドン
	//座標
	float GetPos() { return bullet.pos[0].z; }

	//戦闘開始フラッグ
	bool GetStartFlag() { return boss.startFlag; }
	void SetStartflag(bool startFlag) { this->boss.startFlag = startFlag; }

	//あたり判定用
	XMFLOAT3 GetCollsionPoseidon() { return { boss.pos.x, boss.pos.y, boss.pos.z }; }
	float GetSphereRadius() { return boss.radius; }

	//エネミーの体力
	int GetHp() { return boss.hp; }

	//エネミー再起動フラッグ
	bool GetResetFlag() { return boss.resetFlag; }
	void SetResetFlag(bool resetFlag) { this->boss.resetFlag = resetFlag; }

	//攻撃フラッグ
	bool GetAttackFlag() { return boss.attackFlag; }

	//攻撃準備起動フラッグ
	bool GetAttackPreparationFlag() { return boss.attackPreparationFlag; }
	void SetAttackPreparationFlag(bool attackPreparation) { this->boss.attackPreparationFlag = attackPreparation; }

	//攻撃終了フラッグ
	bool GetAttackEndFlag() { return boss.attackEndFlag; }

	//被弾時のシェイク演出起動
	void SetDamegeShakeFlag(bool damegeShakeflag) { this->boss.damegeShakeFlag = damegeShakeflag; }

	//モード変更フラッグ
	bool GetModeChangeFlag() { return boss.modeChangeFlag; }

	//ボス確認フラッグ
	bool GetDeathFlag() { return boss.deathFlag; }
#pragma endregion

#pragma region バレット
	//あたり判定用
	XMFLOAT3 GetCollisionBullet(int i) { return bullet.pos[i]; }

	//攻撃結果フラッグ
	bool GetHitFlag(int i) { return bullet.hitFlag[i]; }
	void SetHitFlag(bool hitFlag, int i) { this->bullet.hitFlag[i] = hitFlag; }

	bool GetFireFlag() { return bullet.fireFlag[350]; }
#pragma endregion

#pragma region 外部干渉
	//着地時シェイク起動フラッグ
	bool GetShakeFlag() { return shakeFlag; }
	void SetShakeFlag(bool shakeFlag) { this->shakeFlag = shakeFlag; }
#pragma endregion

#pragma endregion

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
	/// 攻撃準備
	/// </summary>
	/// <param name="turnNomber"></param>
	void AttackPreparation();

	/// <summary>
	/// 状態移行
	/// </summary>
	void StatusChange();

	/// <summary>
	/// モード変更
	/// </summary>
	void ModeChange();
	
	/// <summary>
	/// 装填
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
	/// 装填パターン４
	/// </summary>
	void BulletLoading_4();

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
	/// 挙動パターン４
	/// </summary>
	/// <param name="i"></param>
	void BulletMove_4(int i);

	/// <summary>
	/// 攻撃パターン1
	/// </summary>
	void SplashBlast1(bool field_completionFlag);

	/// <summary>
	/// 攻撃パターン2
	/// </summary>
	void SplashBlast2(bool field_completionFlag);

	/// <summary>
	/// 0番～39番の発射フラッグをリセット
	/// </summary>
	void FireFlagReset240_279();

	/// <summary>
	/// 40番～79番の発射フラッグをリセット
	/// </summary>
	void FireFlagReset280_319();

	/// <summary>
	/// 80番～99番の発射フラッグをリセット
	/// </summary>
	void FireFlagReset320_359();

	/// <summary>
	/// オープス攻撃時のシェイク演出
	/// </summary>
	void AttackShake();

	/// <summary>
	/// オープス被弾時のシェイク演出
	/// </summary>
	void DamegeShake();

	/// <summary>
	/// チャージ演出
	/// </summary>
	void ChargeEffect(bool field_completionFlag);

	/// <summary>
	/// フレイムエフェクト
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

#pragma region ボス構造体
	typedef struct Boss
	{
		//オブジェクト・モデル・コリジョン
		Object3d* obj = nullptr;
		Model* model = nullptr;
		Object3d* collision = nullptr;

		//設定変数
		XMFLOAT3 pos{};
		XMFLOAT2 shake{};
		XMFLOAT3 rot{};
		XMFLOAT3 vel{};
		float acc = 0.0f;
		float alpha = 0.0f;
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
		int fixTimer = 0.0f;
		bool modeChangeFlag = false;
		bool deathFlag = false;
	};

	Boss boss;
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
		float alpha = 0.0f;
	};

	Circle circle;
#pragma endregion

#pragma region バレット構造体
	typedef struct Bullet
	{
		//コリジョン
		Object3d* obj[POSEIDONBULLET_MAX];
		Model* model = nullptr;

		//設定変数
		XMFLOAT3 pos[POSEIDONBULLET_MAX];
		XMFLOAT3 rot[POSEIDONBULLET_MAX];

		//状態変化変数
		bool hitFlag[POSEIDONBULLET_MAX] = { false };
		bool fireFlag[POSEIDONBULLET_MAX] = { false };
		int delayTimer;
	};

	Bullet bullet;
#pragma endregion

#pragma region チャージ演出用構造体
	typedef struct Charge
	{
		//設定変数
		XMFLOAT3 pos[SPLASHCHARGEBULLET_MAX]{};
		float angle = 0.0f;
		float radius = 0.0f;
	};

	Charge charge;
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

