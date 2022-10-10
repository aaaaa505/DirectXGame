#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "ParticleManager.h"

//最大弾数
const int FIREOOPTHBULLET_MAX = 9;

class FireOopth
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
	static FireOopth* Create();

public: // メンバ関数

	// デストラクタ
	~FireOopth();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(int turnNomber, bool field_completionFlag , bool scene_completionFlag);

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
	/// 攻撃パターン1
	/// </summary>
	void OneLineOrderShot(bool field_completionFlag);

	/// <summary>
	/// 攻撃パターン2
	/// </summary>
	/// <param name="completionFlag"></param>
	void TwinLineOrderShot(bool field_completionFlag);

	/// <summary>
	/// 攻撃パターン3
	/// </summary>
	/// <param name="field_completionFlag"></param>
	void AllLineOrderShot(bool field_completionFlag);

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
		bool damegeShakeFlag = false;
		float damegeShakeTimer = 0.0f;
		float attackShakeTimer = 0.0f;
		bool attackOrder[3] = { false };
		bool attackEndFlag = false;
		bool fixFlag = false;
		int fixTimer = 0.0f;
	};

	Oopth oopth;
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
	};

	Circle circle;
#pragma endregion

#pragma region バレット構造体
	typedef struct Bullet
	{
		//設定変数
		XMFLOAT3 pos[FIREOOPTHBULLET_MAX];

		//状態変化変数
		bool hitFlag[FIREOOPTHBULLET_MAX];
		bool allFireFlag;
		bool delayFlag;
		int delayTimer;
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

