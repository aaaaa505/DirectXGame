#pragma once

/// <summary>
/// スプライトナンバー
/// </summary>
enum SpriteNumbr
{
	// デバックテキスト
	debugTextTexNumber,
	// タイトルスプライト
	titleNumber,
	// マニュアルスプライト
	manualNumber,
	// クリアスプライト
	clearNumber,
	// オーバースプライト
	overNumber,
	// ステージF
	stageF_Name,
	// ステージW
	stageW_Name,
	// ステージL
	stageL_Name,
	// イリュージョンスプライト
	IllustrationNumber,
	// ステージ1スプライト
	round1Number,
	// ステージ2スプライト
	round2Number,
	// ステージ3スプライト
	round3Number,
	//ステージ4スプライト
	round4Number,
	//プレイヤーUI
	parts_Back,
	parts_Front,
	parts_Hp,
	parts_Mark_F,
	parts_Mark_W,
	parts_Mark_L,
	parts_ChangeTimer,
	//エネミーUI
	parts_Bar,
	parts_Hp_F,
	parts_Hp_W,
	parts_Hp_L,
	parts_Mark_N,
	parts_Mark1_F,
	parts_Mark2_F,
	parts_Mark3_F,
	parts_Mark4_F,
	parts_Mark1_W,
	parts_Mark2_W,
	parts_Mark3_W,
	parts_Mark4_W,
	parts_Mark1_L,
	parts_Mark2_L,
	parts_Mark3_L,
	parts_Mark4_L,
	//ステージセレクト
	select_BackGround,
	select_MarkF,
	select_MarkW,
	select_MarkL,
	select_Complete

};


/// <summary>
/// スプライトロードクラス
/// </summary>
class LoadSprite
{
public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

};