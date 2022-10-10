#include "SceneWhile.h"
#include "LoadSprite.h"
#include "SafeDelete.h"

#include "GameScene.h"

SceneWhile* SceneWhile::Create()
{
	//インスタンスの生成
	SceneWhile* instance = new SceneWhile();

	//インスタンスの初期化
	instance->Initialize();
	
	//インスタンスを返す
	return instance;
}

SceneWhile::~SceneWhile()
{
	safe_delete(stageName.sprite);
	safe_delete(illustration.sprite);
	safe_delete(roundCount.sprite);
}

void SceneWhile::Initialize()
{
#pragma region 挿絵構造体初期化及び生成
	//alpha値
	illustration.alpha = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	//スプライト生成
	illustration.sprite = Sprite::Create(IllustrationNumber, { 0.0f, 0.0f }, illustration.alpha);

	//状態変化変数
	illustration.transDownFlag = false;
	illustration.completionFlag2 = false;
	illustration.resetFlag = false;

#pragma endregion

#pragma region ステージ名構造体初期化及び生成
	//alpha値
	stageName.alpha = { 1.0f, 1.0f, 1.0f, 0.0f };

	//スプライト生成
	stageName.sprite = Sprite::Create(stageF_Name, { 0.0f, 0.0f }, stageName.alpha);

	//状態変化変数
	//状態変化変数
	stageName.transUpFlag = false;
	stageName.transDownFlag = false;
	stageName.completionFlag1 = false;
	stageName.completionFlag2 = false;
	stageName.resetFlag = false;

#pragma endregion

#pragma region ラウンドカウント構造体の初期化及び生成
	//座標
	roundCount.pos = { 1280.0f, 0.0f };

	//スプライト生成
	roundCount.sprite = Sprite::Create(round1Number, roundCount.pos);

	//状態変化変数
	roundCount.moveFlag = false;
	roundCount.completionFlag = false;
	roundCount.resetFlag = false;

	roundCount.stopFlag = false;
	roundCount.stopTimer = 70;
#pragma endregion

#pragma region エンドシーン構造体初期化及び生成
	//alpha値
	endScene.alpha = { 1.0f, 1.0f, 1.0f, 0.0f };

	//スプライト生成
	endScene.sprite = Sprite::Create(clearNumber, { 0.0f, 0.0f }, endScene.alpha);

	//状態変化変数
	endScene.transUpFlag = false;
	endScene.transDownFlag = false;
	endScene.completionFlag1 = false;
	endScene.completionFlag2 = false;
	endScene.clearFlag = false;
	endScene.overFlag = false;
	endScene.resetFlag = false;
#pragma endregion
}

#pragma region 挿絵関数

void SceneWhile::Illustration::TransUp()
{
	//起動フラッグがONなら
	if (transUpFlag == true)
	{
		//規定値までalpha値を下げる
		if (alpha.w <= 1.0f)
		{
			alpha.w += 0.01f;
		}
		//規定値になったら
		else
		{
			//誤差修正
			alpha.w = 1.0f;
			//起動フラッグがOFF&起動完了フラッグをON
			transUpFlag = false;
			completionFlag1 = true;
		}
		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::Illustration::TransDown()
{
	//起動フラッグがONなら
	if (transDownFlag == true)
	{
		//規定値までalpha値を下げる
		if (alpha.w >= 0.0f)
		{
			alpha.w -= 0.01f;
		}
		//規定値になったら
		else
		{
			//誤差修正
			alpha.w = 0.0f;
			//起動フラッグがOFF&起動完了フラッグをON
			transDownFlag = false;
			completionFlag2 = true;
		}
		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::Illustration::Reset()
{
	if (resetFlag == true)
	{
		alpha.w = 0.0f;
		transDownFlag = false;
		completionFlag2 = false;

		resetFlag = false;
	}
}
#pragma endregion

#pragma region ステージ名関数
void SceneWhile::StageName::TransUp()
{
	//起動フラッグがONなら
	if (transUpFlag == true)
	{
		//規定値までalpha値を上げる
		if (alpha.w <= 1.0f)
		{
			alpha.w += 0.01f;
		}
		//規定値になったら
		else
		{
			//誤差修正
			alpha.w = 1.0f;
			//起動フラッグがOFF&起動完了フラッグをON
			transUpFlag = false;
			completionFlag1 = true;
		}
		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::StageName::TransDown()
{
	//起動フラッグがONなら
	if (transDownFlag == true)
	{
		//規定値までalpha値を下げる
		if (alpha.w >= 0.0f)
		{
			alpha.w -= 0.01f;
		}
		//規定値になったら
		else
		{
			//誤差修正
			alpha.w = 0.0f;
			//起動フラッグがOFF&起動完了フラッグをON
			transDownFlag = false;
			completionFlag2 = true;
		}
		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::StageName::Reset()
{
	if (resetFlag == true)
	{
		alpha.w = 0.0f;
		transUpFlag = false;
		transDownFlag = false;
		completionFlag1 = false;
		completionFlag2 = false;

		resetFlag = false;
	}
}

void SceneWhile::StageName::NameChange(int sceneNo)
{
	if (sceneNo == WATER_ELEMENT)
	{
		sprite->SetTextureNumber(stageW_Name);
	}
	if (sceneNo == FIRE_ELEMENT)
	{
		sprite->SetTextureNumber(stageF_Name);
	}
	if (sceneNo == LEAF_ELEMENT)
	{
		sprite->SetTextureNumber(stageL_Name);
	}
}

#pragma endregion

#pragma region ラウンドカウント関数
void SceneWhile::RoundCount::NomberChange(int roundNomber)
{
	//現在のラウンド数によってスプライトを変更
	if (roundNomber == 1)
	{
		sprite->SetTextureNumber(round1Number);
	}
	if (roundNomber == 2)
	{
		sprite->SetTextureNumber(round2Number);
	}
	if (roundNomber == 3)
	{
		sprite->SetTextureNumber(round3Number);
	}
	if (roundNomber == 4)
	{
		sprite->SetTextureNumber(round4Number);
	}
}

void SceneWhile::RoundCount::Move(Illustration illustration)
{
	//起動フラッグがONなら
	if (moveFlag == true)
	{
		illustration.alpha.w = 0.5f;
		//ストップフラッグOFF
		if (stopFlag == false)
		{
			pos.x -= 64.0f;
			//中間地点に到達したら
			if (pos.x == 0.0f)
			{
				stopFlag = true;
			}

			else if (pos.x <= -1280.0f)
			{
				illustration.alpha.w = 0.0f;
				completionFlag = true;
				moveFlag = false;
			}
		}
		//ONなら
		else
		{
			//停止時間をカウント
			stopTimer -= 1;
			//0になったらストップフラッグをOFF
			if (stopTimer <= 0)
			{
				stopFlag = false;
			}
		}

		illustration.sprite->SetAlpha(illustration.alpha.w);
		sprite->SetPosition(pos);
	}
}

void SceneWhile::RoundCount::Reset()
{
	if (resetFlag == true)
	{
		pos = { 1280.0f, 0.0f };
		moveFlag = false;
		completionFlag = false;
		stopFlag = false;
		stopTimer = 70;
	}
	
	resetFlag = false;
}
#pragma endregion

#pragma region エンドシーン関数
void SceneWhile::EndScene::SpriteChange()
{
	if (clearFlag == true)
	{
		sprite->SetTextureNumber(clearNumber);
	}
	if (overFlag == true)
	{
		sprite->SetTextureNumber(overNumber);
	}
}
void SceneWhile::EndScene::TransUp()
{
	if (transUpFlag == true && completionFlag1 == false)
	{
		if (alpha.w <= 1.0f)
		{
			alpha.w += 0.01f;
		}
		else
		{
			alpha.w = 1.0f;
			transUpFlag = false;
			completionFlag1 = true;
		}

		sprite->SetAlpha(alpha.w);
	}
}

void SceneWhile::EndScene::TransDown()
{
	if (transDownFlag == true && completionFlag2 == false)
	{
		if (alpha.w >= 0.0f)
		{
			alpha.w -= 0.01f;
		}
		else
		{
			alpha.w = 0.0f;
			transDownFlag = false;
			completionFlag2 = true;
		}

		sprite->SetAlpha(alpha.w);
	}
}
#pragma endregion


void SceneWhile::Update(int roundNo, int sceneNo)
{
	illustration.TransUp();
	illustration.TransDown();
	illustration.Reset();

	stageName.TransUp();
	stageName.TransDown();
	stageName.Reset();
	stageName.NameChange(sceneNo);

	roundCount.NomberChange(roundNo);
	roundCount.Move(illustration);
	roundCount.Reset();

	endScene.SpriteChange();
	endScene.TransUp();
	endScene.TransDown();
}

void SceneWhile::Draw()
{
	if (endScene.clearFlag == true || endScene.overFlag == true)
	{
		endScene.sprite->Draw();
	}
	illustration.sprite->Draw();
	stageName.sprite->Draw();
	roundCount.sprite->Draw();
}