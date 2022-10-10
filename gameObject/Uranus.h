#pragma once
#include "Object3d.h"
#include "Audio.h"
#include "ParticleManager.h"

////モード変更時の演出最大弾
const int TEMPESTCHARGEBULLET_MAX = 36;

class Uranus
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
	static Uranus* Create();

public: // メンバ関数

	// デストラクタ
	~Uranus();

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

#pragma region ウラヌス
	//座標
	XMFLOAT3 GetPos() { return boss.obj->GetPosition(); }

	//戦闘開始フラッグ
	bool GetStartFlag() { return boss.startFlag; }
	void SetStartflag(bool startFlag) { this->boss.startFlag = startFlag; }

	//あたり判定用
	XMFLOAT3 GetCollsionUranus() { return { boss.pos.x, boss.pos.y, boss.pos.z }; }
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

#pragma region ウィンド
	//あたり判定用
	XMFLOAT3 GetCollisionWind(int i) { return circle.obj[i]->GetPosition(); }
	bool GetOutBreakFlag(int i) { return wind.outBreakFlag[i]; }
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
	/// モード変更
	/// </summary>
	void ModeChange();

	/// <summary>
	/// 状態移行
	/// </summary>
	void StatusChange();

	/// <summary>
	/// 攻撃準備
	/// </summary>
	/// <param name="turnNomber"></param>
	void AttackPreparation();

	/// <summary>
	/// ウィンドの挙動
	/// </summary>
	void WindMove();

	/// <summary>
	/// テンペストの挙動
	/// </summary>
	void TempestMove();

	/// <summary>
	/// オープス１攻撃パターン
	/// </summary>
	/// <param name="field_completionFlag"></param>
	void CrossWind();

	/// <summary>
	/// オープス２攻撃パターン
	/// </summary>
	/// <param name="field_completionFlag"></param>
	void SlantingWind();

	/// <summary>
	/// オープス３攻撃パターン
	/// </summary>
	/// <param name="field_completionFlag"></param>
	void BlockWind();

	/// <summary>
	/// 攻撃パターン1
	/// </summary>
	void TempestWind1(bool field_completionFlag);

	/// <summary>
	/// 攻撃パターン2
	/// </summary>
	void TempestWind2(bool field_completionFlag);

	/// <summary>
	/// 弾のパーティクル演出
	/// </summary>
	void WindEffect();

	/// <summary>
	/// チャージ演出
	/// </summary>
	void ChargeEffect(bool field_completionFlag);

	/// <summary>
	/// テンペスト演出
	/// </summary>
	void TempestEfect();

	/// <summary>
	/// 攻撃終了
	/// </summary>
	void AttackEnd();

	/// <summary>
	/// 死亡演出
	/// </summary>
	void Death(int turnNomber);

	/// <summary>
	/// オープス攻撃時のシェイク演出
	/// </summary>
	void AttackShake();

	/// <summary>
	/// オープス被弾時のシェイク演出
	/// </summary>
	void DamegeShake();

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
		Object3d* obj[9] = { nullptr };
		Model* model = nullptr;

		//設定変数
		XMFLOAT3 pos[9]{};
		XMFLOAT3 rot{};
		XMFLOAT3 scale{};
	};

	Circle circle;
#pragma endregion

#pragma region ウィンド構造体
	typedef struct Wind
	{
		//オブジェクト・モデル
		Object3d* obj[10] = { nullptr };
		Model* model = nullptr;

		//設定変数
		XMFLOAT3 pos[10]{};
		XMFLOAT3 rot{};
		XMFLOAT3 tempestRot{};
		XMFLOAT3 scale[10]{};
		float alpha = 0.0f;

		//状態変化変数
		int outBreakTimer = 0;
		bool outBreakFlag[10] = { false };
		int delayTimer = 0;
		bool delayFlag = false;
	};

	Wind wind;
#pragma endregion

#pragma region チャージ演出用構造体
	typedef struct Charge
	{
		//設定変数
		XMFLOAT3 pos[TEMPESTCHARGEBULLET_MAX]{};
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

