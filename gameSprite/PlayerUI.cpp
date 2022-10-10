#include "PlayerUI.h"
#include "LoadSprite.h"

#include "GameScene.h"
#include "SafeDelete.h"

PlayerUI* PlayerUI::Create()
{
	//インスタンスの生成
	PlayerUI* instance = new PlayerUI();

	//インスタンスの初期化
	instance->Initialize();

	//インスタンスを返す
	return instance;
}

PlayerUI::PlayerUI()
{
}

PlayerUI::~PlayerUI()
{
	for (auto sprite : sprites) {
		safe_delete(sprite.second);
	}
}

void PlayerUI::Initialize()
{
	// 初期座標
	pos = { 0.0f, 0.0f };

	// 下地
	sprites["back"] = Sprite::Create(parts_Back, pos);

	// HPゲージ
	sprites["hpGage"] = Sprite::Create(parts_Hp, { pos.x + 138.0f,pos.y + 630.0f });

	// タイマー
	sprites["timer"] = Sprite::Create(parts_ChangeTimer, { pos.x + 116.0f,pos.y + 670.0f });

	// カバー
	sprites["front"] = Sprite::Create(parts_Front, pos);

	// マークF
	sprites["mark_F"] = Sprite::Create(parts_Mark_F, pos);

	// マークW
	sprites["mark_W"] = Sprite::Create(parts_Mark_W, pos);

	// マークL
	sprites["mark_L"] = Sprite::Create(parts_Mark_L, pos);

	//状態変化変数
	markFlag = NORMAL_TYPE;
	stageEndFlag = false;
	completionFlag = false;
}

void PlayerUI::EndMove()
{
	if (stageEndFlag == true)
	{
		if (pos.y < 210)
		{
			pos.y += 1.0f;
		}
		else
		{
			completionFlag = true;
		}
		sprites["back"]->SetPosition(pos);
		sprites["hpGage"]->SetPosition({pos.x + 138.0f,pos.y + 630.0f});
		sprites["timer"]->SetPosition({ pos.x + 116.0f,pos.y + 670.0f });
		sprites["front"]->SetPosition(pos);
		sprites["mark_F"]->SetPosition(pos);
		sprites["mark_W"]->SetPosition(pos);
		sprites["mark_L"]->SetPosition(pos);
	}
}

void PlayerUI::Update(int playerHp, int changeTimer, short typeFlag)
{
	//プレイヤーの属性によってマークを変更
	markFlag = typeFlag;
	//プレイヤーのHpをゲージ化
	sprites["hpGage"]->SetSize({(float)playerHp * 0.94f, 30.0f});
	//プレイヤーの変身時間をゲージ化
	sprites["timer"]->SetSize({ (float)changeTimer * 0.83f, 20.0f });

	EndMove();
}

void PlayerUI::Draw()
{
	sprites["back"]->Draw();
	sprites["hpGage"]->Draw();
	sprites["timer"]->Draw();
	sprites["front"]->Draw();

	switch (markFlag)
	{
	case WATER_TYPE:
		sprites["mark_W"]->Draw();
		break;
	case FIRE_TYPE:
		sprites["mark_F"]->Draw();
		break;
	case LEAF_TYPE:
		sprites["mark_L"]->Draw();
		break;
	}
}
