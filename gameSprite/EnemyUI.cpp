#include "EnemyUI.h"
#include "LoadSprite.h"
#include "GameScene.h"
#include "SafeDelete.h"

EnemyUI* EnemyUI::Create()
{
	//インスタンスの生成
	EnemyUI* instance = new EnemyUI();

	//インスタンスの初期化
	instance->Initialize();

	//インスタンスを返す
	return instance;
}

EnemyUI::~EnemyUI()
{
	safe_delete(bar);
	safe_delete(gage);
	safe_delete(mark);
}

void EnemyUI::Initialize()
{
	//初期座標
	pos = { 0.0f, 0.0f };

	//バー
	bar = Sprite::Create(parts_Bar, pos);

	//ゲージ
	gage = Sprite::Create(parts_Hp_F, {pos.x + 140.0f,pos.y + 50.0f });

	//マーク
	mark = Sprite::Create(parts_Mark_N, pos);

	//状態変化変数
	markFlag = 0;
	stageEndFlag = false;
	completionFlag = false;
}

void EnemyUI::GageChange(int sceneNo)
{
	if (sceneNo == WATER_ELEMENT)
	{
		gage->SetTextureNumber(parts_Hp_W);
	}
	if (sceneNo == FIRE_ELEMENT)
	{
		gage->SetTextureNumber(parts_Hp_F);
	}
	if (sceneNo == LEAF_ELEMENT)
	{
		gage->SetTextureNumber(parts_Hp_L);
	}
}

void EnemyUI::MarkChange(int turnNumber, int sceneNo)
{
	if (sceneNo == WATER_ELEMENT)
	{
		if (turnNumber == 1)
		{
			mark->SetTextureNumber(parts_Mark1_W);
		}
		if (turnNumber == 2)
		{
			mark->SetTextureNumber(parts_Mark2_W);
		}

		if (turnNumber == 3)
		{
			mark->SetTextureNumber(parts_Mark3_W);
		}

		if (turnNumber == 4)
		{
			mark->SetTextureNumber(parts_Mark4_W);
		}
	}
	if (sceneNo == FIRE_ELEMENT)
	{
		if (turnNumber == 1)
		{
			mark->SetTextureNumber(parts_Mark1_F);
		}
		if (turnNumber == 2)
		{
			mark->SetTextureNumber(parts_Mark2_F);
		}

		if (turnNumber == 3)
		{
			mark->SetTextureNumber(parts_Mark3_F);
		}

		if (turnNumber == 4)
		{
			mark->SetTextureNumber(parts_Mark4_F);
		}
	}
	if (sceneNo == LEAF_ELEMENT)
	{
		if (turnNumber == 1)
		{
			mark->SetTextureNumber(parts_Mark1_L);
		}
		if (turnNumber == 2)
		{
			mark->SetTextureNumber(parts_Mark2_L);
		}

		if (turnNumber == 3)
		{
			mark->SetTextureNumber(parts_Mark3_L);
		}

		if (turnNumber == 4)
		{
			mark->SetTextureNumber(parts_Mark4_L);
		}
	}
}

void EnemyUI::EndMove()
{
	if (stageEndFlag == true)
	{
		if (pos.y > -210)
		{
			pos.y -= 1.0f;
		}
		else
		{
			completionFlag = true;
		}

		bar->SetPosition(pos);
		gage->SetPosition({ pos.x + 140.0f,pos.y + 51.0f });
		mark->SetPosition(pos);
	}
}

void EnemyUI::Update(int oopthHp,int bossHp, int turnNumber, int sceneNo)
{
	if (turnNumber != 4)
	{
		gage->SetSize({ (float)oopthHp * 3.34f, 46.0f });
	}
	else
	{
		gage->SetSize({ (float)bossHp * 2.5f, 46.0f });
	}
	

	MarkChange(turnNumber, sceneNo);

	GageChange(sceneNo);

	EndMove();
}

void EnemyUI::Draw()
{
	gage->Draw();
	bar->Draw();
	mark->Draw();

}