#include "LoadSprite.h"
#include "Sprite.h"

void LoadSprite::StaticInitialize()
{
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// スプライトのロード
	if (!Sprite::LoadTexture(titleNumber, L"Resources/titleScene/title.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(manualNumber, L"Resources/titleScene/manual.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(clearNumber, L"Resources/endScene/clear.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(overNumber, L"Resources/endScene/over.png")) {
		assert(0);
		return;
	}
	//
	if (!Sprite::LoadTexture(stageF_Name, L"Resources/stageName/F.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(stageW_Name, L"Resources/stageName/W.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(stageL_Name, L"Resources/stageName/L.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(IllustrationNumber, L"Resources/Illustration.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(round1Number, L"Resources/RoundCount/round1.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(round2Number, L"Resources/RoundCount/round2.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(round3Number, L"Resources/RoundCount/round3.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(round4Number, L"Resources/RoundCount/round4.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Back, L"Resources/PlayerUI/parts_Back.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Front, L"Resources/PlayerUI/parts_Front.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Hp, L"Resources/PlayerUI/parts_Hp.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark_F, L"Resources/PlayerUI/parts_Mark_F.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark_W, L"Resources/PlayerUI/parts_Mark_W.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark_L, L"Resources/PlayerUI/parts_Mark_L.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_ChangeTimer, L"Resources/PlayerUI/parts_ChangeTimer.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Bar, L"Resources/EnemyUI/parts_Bar.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Hp_F, L"Resources/EnemyUI/parts_Hp_F.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Hp_W, L"Resources/EnemyUI/parts_Hp_W.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Hp_L, L"Resources/EnemyUI/parts_Hp_L.png")) {
		assert(0);
		return;
	}
	//
	if (!Sprite::LoadTexture(parts_Mark_N, L"Resources/EnemyUI/parts_Mark_N.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark1_F, L"Resources/EnemyUI/parts_Mark1_F.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark2_F, L"Resources/EnemyUI/parts_Mark2_F.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark3_F, L"Resources/EnemyUI/parts_Mark3_F.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark4_F, L"Resources/EnemyUI/parts_Mark4_F.png")) {
		assert(0);
		return;
	}
	//
	if (!Sprite::LoadTexture(parts_Mark1_W, L"Resources/EnemyUI/parts_Mark1_W.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark2_W, L"Resources/EnemyUI/parts_Mark2_W.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark3_W, L"Resources/EnemyUI/parts_Mark3_W.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark4_W, L"Resources/EnemyUI/parts_Mark4_W.png")) {
		assert(0);
		return;
	}
	//
	if (!Sprite::LoadTexture(parts_Mark1_L, L"Resources/EnemyUI/parts_Mark1_L.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark2_L, L"Resources/EnemyUI/parts_Mark2_L.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark3_L, L"Resources/EnemyUI/parts_Mark3_L.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(parts_Mark4_L, L"Resources/EnemyUI/parts_Mark4_L.png")) {
		assert(0);
		return;
	}
	//
	if (!Sprite::LoadTexture(select_BackGround, L"Resources/SelectScene/parts_BackGround.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(select_MarkF, L"Resources/SelectScene/parts_MarkF.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(select_MarkW, L"Resources/SelectScene/parts_MarkW.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(select_MarkL, L"Resources/SelectScene/parts_MarkL.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(select_Complete, L"Resources/SelectScene/parts_Complete.png")) {
		assert(0);
		return;
	}

}
