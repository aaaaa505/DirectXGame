#pragma once
#include "Object3d.h"
#include "Audio.h"

class WaterField
{
private: // エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;

public: // 静的メンバ関数
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static WaterField* Create();

public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~WaterField();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(int turnNomber, bool modeChangeFlag);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

#pragma region setter・getter
	//攻撃範囲表示起動フラッグ
	bool GetShowRangeFlag() { return showRangeFlag; }
	void SetShowRangeFlag(bool showRangeFlag) { this->showRangeFlag = showRangeFlag; }

	//攻撃範囲表示完了フラッグ
	bool GetCompletionFlag() { return completionFlag; }
	void SetCompletionFlag(bool completionFlag) { this->completionFlag = completionFlag; }

	//再起動フラッグ
	bool GetResetFlag() { return resetFlag; }
	void SetResetFlag(bool resetFlag) { this->resetFlag = resetFlag; }

#pragma endregion


private: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 攻撃パターン1範囲表示
	/// </summary>
	void OneSideOrderBlastScope();

	/// <summary>
	/// 攻撃パターン2範囲表示
	/// </summary>
	void TwinSideOrderBlastScope();

	/// <summary>
	/// 攻撃パターン3範囲表示
	/// </summary>
	void TwinLineSideBlastScope();

	/// <summary>
	/// ボス攻撃パターン１範囲表示
	/// </summary>
	void SplashBlast1Scope();

	/// <summary>
	/// ボス攻撃パターン１範囲表示
	/// </summary>
	void SplashBlast2Scope();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private: // メンバ変数
#pragma region オーディオ
	Audio* audio = nullptr;
#pragma endregion


#pragma region オブジェクト・モデル
	Object3d* obj_Field[3][3];
	Model* model_Normal;
	Model* model_Scope;
#pragma endregion

#pragma region 状態変化変数
	bool scopeFlag[3][3];
	bool showRangeFlag;
	bool completionFlag;
	bool resetFlag;
	int processNomber;
	int showTimer;
#pragma endregion


};

