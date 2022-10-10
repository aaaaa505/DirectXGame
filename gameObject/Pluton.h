#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "ParticleManager.h"

//最大弾数
const int PLUTONBULLET_MAX = 17;

//モード変更時の演出最大弾
const int FLAMECHARGEBULLET_MAX = 36;

class Pluton
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
	static Pluton* Create();

public: // メンバ関数

	// デストラクタ
	~Pluton();

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

#pragma region プルトン
	//座標
	float GetPos() { return bullet.pos[0].z; }

	//戦闘開始フラッグ
	bool GetStartFlag() { return boss.startFlag; }
	void SetStartflag(bool startFlag) { this->boss.startFlag = startFlag; }

	//あたり判定用
	XMFLOAT3 GetCollsionPluton() { return { boss.pos.x, boss.pos.y, boss.pos.z }; }
	float GetSphereRadius() { return boss.radius; }

	//エネミーの体力
	int GetHp() { return boss.hp; }

	//ボス死亡確認フラッグ
	bool GetDeathFlag() { return boss.deathFlag; }

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
#pragma endregion

#pragma region バレット
	//あたり判定用
	XMFLOAT3 GetCollisionBullet(int i) { return bullet.pos[i]; }

	//攻撃結果フラッグ
	bool GetHitFlag(int i) { return bullet.hitFlag[i]; }
	void SetHitFlag(bool hitFlag, int i) { this->bullet.hitFlag[i] = hitFlag; }
#pragma endregion

#pragma region フレイム
	//あたり判定用
	XMFLOAT3 GetCollisionFlame() { return flame.pos; }

	//攻撃結果フラッグ
	bool GetFlameHitFlag() { return flame.hitFlag; }
	void SetFlameHitFlag(bool hitFlag) { this->flame.hitFlag = hitFlag; }
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
	/// オープス１攻撃パターン
	/// </summary>
	void OneLineOrderShot();

	/// <summary>
	/// オープス２攻撃パターン
	/// </summary>
	/// <param name="completionFlag"></param>
	void TwinLineOrderShot();

	/// <summary>
	/// オープス３攻撃パターン
	/// </summary>
	void AllLineOrderShot();

	/// <summary>
	/// 攻撃スロット10番目
	/// </summary>
	void TwinLineFlame();

	/// <summary>
	/// 攻撃パターン1
	/// </summary>
	void FlameShot1(bool field_completionFlag);

	/// <summary>
	/// 攻撃パターン2
	/// </summary>
	void FlameShot2(bool field_completionFlag);

	/// <summary>
	/// オープス攻撃時のシェイク演出
	/// </summary>
	void AttackShake();

	/// <summary>
	/// オープス被弾時のシェイク演出
	/// </summary>
	void DamegeShake();

	/// <summary>
	/// 装填
	/// </summary>
	void Loading();

	/// <summary>
	/// 弾のパーティクル演出
	/// </summary>
	void FireEffect();

	/// <summary>
	/// チャージ演出
	/// </summary>
	void ChargeEffect(bool field_completionFlag);

	/// <summary>
	/// フレイムエフェクト
	/// </summary>
	void FlameEffect();

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
		bool deathFlag = false;
		bool attackPreparationFlag = false;
		bool attackFlag = false;
		bool statusChangeFlag = false;
		int changeTimer = 0;
		float attackShakeTimer = 0.0f;
		bool damegeShakeFlag = false;
		float damegeShakeTimer = 0.0f;
		bool attackOrder[10] = { false };
		bool attackEndFlag = false;
		bool fixFlag = false;
		int fixTimer = 0.0f;
		bool modeChangeFlag = false;
	};

	Boss boss;
#pragma endregion

#pragma region サークル構造体
	typedef struct Circle
	{
		//オブジェクト・モデル
		Object3d* obj[3] = { nullptr };
		Model* model = nullptr;

		//設定変数
		XMFLOAT3 pos[3]{};
		XMFLOAT3 rot{};
		XMFLOAT3 scale{};
		float alpha = 0.0f;
	};

	Circle circle;
#pragma endregion

#pragma region バレット構造体
	typedef struct Bullet
	{
		//設定変数
		XMFLOAT3 pos[PLUTONBULLET_MAX];
		float radius;

		//状態変化変数
		bool hitFlag[PLUTONBULLET_MAX];
		bool allFireFlag;
		bool delayFlag;
		int delayTimer;
	};

	Bullet bullet;
#pragma endregion

#pragma region チャージ演出用構造体
	typedef struct Charge
	{
		//設定変数
		XMFLOAT3 pos[FLAMECHARGEBULLET_MAX]{};
		float angle = 0.0f;
		float radius = 0.0f;
	};

	Charge charge;
#pragma endregion

#pragma region フレイム用構造体
	typedef struct Flame
	{
		//設定変数
		XMFLOAT3 pos{};
		float radius;

		//状態変化変数
		bool hitFlag = false;
	};

	Flame flame;
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

