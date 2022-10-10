#include "Player.h"
#include "Safedelete.h"

Player* Player::Create(Input* input)
{
	// インスタンスの生成
	Player* instance = new Player();

	// インスタンスの初期化
	instance->Initialize();
	instance->input = input;

	// インスタンスを返す
	return instance;
}

Player::~Player()
{
	safe_delete(obj_Player);
	safe_delete(model_Normal);
	safe_delete(model_Fire);
	safe_delete(model_Leaf);
	safe_delete(model_Water);
}

void Player::Initialize()
{
#pragma region 本体の初期化
	model_Normal = Model::CreateFromOBJ("player_N");
	model_Fire = Model::CreateFromOBJ("player_F");
	model_Water = Model::CreateFromOBJ("player_W");
	model_Leaf = Model::CreateFromOBJ("player_L");
	
	obj_Player = Object3d::Create();
	obj_Player->SetModel(model_Normal);

	pos_Player = { 0.0f, 0.5f, 0.0f };
	vel_Player = { 0.1f, 0.0f, 0.1f };
	rot_Player = { 0.0f, 0.0f, 0.0f };
	shake = { 0.0f, 0.0f };
	acc_Player = 0.1;
	obj_Player->SetRotation({ 0.0f, 0.0f, 0.0f });
	obj_Player->SetScale({ 0.5f, 0.5f, 0.5f });

	moveFlag = 0;
	typeFlag = 0;
	hp = 500;
	changeTimer = 0;
	changeFlag = false;
	attckShakeTimer = 0.0f;
	damegeShakeTimer = 0.0f;
	deathFlag = false;
#pragma endregion

#pragma region 弾の初期化
	for (int i = 0; i < PLAYERBULLET_MAX; i++)
	{
		pos_Bullet[i] = { 100.0f, 0.0f, 0.0f };//0.3
		fireFlag[i] = false;
		hitFlag[i] = false;
	}
	delayFlag = false;
	delayTimer = 0;
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region ガイドオブジェクト初期化
	//モデル読み込み
	model_LB = Model::CreateFromOBJ("guide_LB");
	model_RB = Model::CreateFromOBJ("guide_RB");
	model_LB->SetAlpha(0.8f);
	model_RB->SetAlpha(0.8f);

	//生成
	guide_LB = Object3d::Create(model_LB);
	guide_RB = Object3d::Create(model_RB);

	//回転角
	guide_LB->SetRotation({ +11.75f ,-90.0f, 0.0f });
	guide_RB->SetRotation({ +11.75f ,-90.0f, 0.0f });

	//スケール
	guide_LB->SetScale({ 0.3f, 0.3f, 0.3f });
	guide_RB->SetScale({ 0.3f, 0.3f, 0.3f });

	//アルファ値
	alpha = 0.8f;
#pragma endregion

#pragma region オーディオ
	audio = Audio::GetInstance();
#pragma endregion

}

void Player::PlayerMove()
{
#pragma region 回避方向選択
	if (moveFlag == 0)
	{
		if (input->GetTriggerLeftStic(SticLeft) && pos_Player.x != -2.8f)
		{
			moveFlag = 1;
			audio->StopWave("evasionSE.wav");
			audio->PlayWave("evasionSE.wav", false);
		}
		if (input->GetTriggerLeftStic(SticRight) && pos_Player.x != 2.8f)
		{
			moveFlag = 2;
			audio->StopWave("evasionSE.wav");
			audio->PlayWave("evasionSE.wav", false);
		}
		if (input->GetTriggerLeftStic(SticUp) && pos_Player.z != 2.8f)
		{
			moveFlag = 3;
			audio->StopWave("evasionSE.wav");
			audio->PlayWave("evasionSE.wav", false);
		}
		if (input->GetTriggerLeftStic(SticDown) && pos_Player.z != -2.8f)
		{
			moveFlag = 4;
			audio->StopWave("evasionSE.wav");
			audio->PlayWave("evasionSE.wav", false);
		}
	}

#pragma endregion

#pragma region 回避行動
	if (moveFlag == 1)
	{
		if (vel_Player.x <= 0.8f)
		{
			pos_Player.x -= vel_Player.x;
			vel_Player.x += acc_Player;
			rot_Player.z = +80.0f;
		}
		else
		{
			moveFlag = 0;
			vel_Player.x = 0.1f;
			rot_Player.z = 0.0f;
		}
	}

	if (moveFlag == 2)
	{
		if (vel_Player.x <= 0.8f)
		{
			pos_Player.x += vel_Player.x;
			vel_Player.x += acc_Player;
			rot_Player.z = -80.0f;
		}
		else
		{
			moveFlag = 0;
			vel_Player.x = 0.1f;
			rot_Player.z = 0.0f;
		}
	}

	if (moveFlag == 3)
	{
		if (vel_Player.z <= 0.8f)
		{
			pos_Player.z += vel_Player.z;
			vel_Player.z += acc_Player;
			rot_Player.x = +80.0f;
		}
		else
		{
			moveFlag = 0;
			vel_Player.z = 0.1f;
			rot_Player.x = 0.0f;
		}
	}

	if (moveFlag == 4)
	{
		if (vel_Player.z <= 0.8f)
		{
			pos_Player.z -= vel_Player.z;
			vel_Player.z += acc_Player;
			rot_Player.x = -80.0f;
		}
		else
		{
			moveFlag = 0;
			vel_Player.z = 0.1f;
			rot_Player.x = 0.0f;
		}
	}
#pragma endregion
}

void Player::TypeChange()
{
	if (changeFlag == true)
	{
		//ファイヤータイプ
		if (typeFlag == FIRE_TYPE)
		{
			audio->PlayWave("changeFireSE.wav", false);
			obj_Player->SetModel(model_Fire);
			particle.color = { 2.0f, 0.5f, 0.0f, 1.0f };
		}
		//ウォータータイプ
		if (typeFlag == WATER_TYPE)
		{
			audio->PlayWave("changeWaterSE.wav", false);
			obj_Player->SetModel(model_Water);
			particle.color = { 0.0f, 0.5f, 5.0f, 1.0f };
		}
		//リーフタイプ
		if (typeFlag == LEAF_TYPE)
		{
			audio->PlayWave("changeLeafSE.wav", false);
			obj_Player->SetModel(model_Leaf);
			particle.color = { 0.2f, 2.0f, 0.0f, 1.0f };
		}

		changeTimer = 500;
		changeFlag = false;
	}

	if (changeTimer > 0)
	{
		changeTimer -= 1;
	}
	else
	{
		obj_Player->SetModel(model_Normal);
		typeFlag = 0;
		particle.color = { 2.51f, 2.24f, 0.0f, 1.0f };
	}
}

void Player::Loading()
{
	if (input->GetTriggerJoyButton(JoyL1Button) || input->GetTriggerJoyButton(JoyR1Button))
	{
		for (int i = 0; i < PLAYERBULLET_MAX; i++)
		{
			if (fireFlag[i] == false && delayFlag == false)
			{
				pos_Bullet[i] = { pos_Player.x, pos_Player.y, pos_Player.z };
				fireFlag[i] = true;
				delayFlag = true;
				delayTimer = 5;
				break;
			}
		}
		attckShakeTimer = 0.2f;
	}

	if (delayFlag == true)
	{
		delayTimer -= 1;
		if (delayTimer == 0)
		{
			delayFlag = false;
		}
	}
}

void Player::BulletMove()
{
	for (int i = 0; i < PLAYERBULLET_MAX; i++)
	{
		if (fireFlag[i] == true)
		{
			pos_Bullet[i].z += 1.0f;
			if (pos_Bullet[i].z >= 10.0f)
			{
				pos_Bullet[i] = { 100.0f, 0.0f, 0.0f };
				hitFlag[i] = false;
				fireFlag[i] = false;
			}
		}
	}
}

void Player::FireEffect()
{
	for (int i = 0; i < PLAYERBULLET_MAX; i++)
	{
		if (fireFlag[i] == true)
		{
			for (int j = 0; j < 10; j++)
			{
				const float rnd_pos = 0.1f;
				particle.pos.x = pos_Bullet[i].x + (float)rand() / RAND_MAX * rnd_pos;
				particle.pos.y = pos_Bullet[i].y + (float)rand() / RAND_MAX * rnd_pos + 1.0f;
				particle.pos.z = pos_Bullet[i].z + (float)rand() / RAND_MAX * rnd_pos;

				particle.vel.z = 0.01;

				const float rnd_acc = 0.001f;
				particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

				particleMan->Add(5, particle.pos, particle.vel, particle.acc, 0.5f, 0.1f, particle.color);

			}
		}
	}
}

void Player::Shake()
{
	//アタックシェイク
	if (attckShakeTimer > 0)
	{
		shake.x = (float)rand() / RAND_MAX * attckShakeTimer - attckShakeTimer / 2.0f;
		shake.y = (float)rand() / RAND_MAX * attckShakeTimer - attckShakeTimer / 2.0f;
		attckShakeTimer -= 0.005f;
	}
	else
	{
		attckShakeTimer = 0.0f;
	}

	//ダメージシェイク
	if (damegeShakeTimer > 0)
	{
		shake.x = (float)rand() / RAND_MAX * damegeShakeTimer - damegeShakeTimer / 2.0f;
		shake.y = (float)rand() / RAND_MAX * damegeShakeTimer - damegeShakeTimer / 2.0f;
		damegeShakeTimer -= 0.05f;
	}
	else
	{
		damegeShakeTimer = 0.0f;
	}
}

void Player::Damege(int stageNo)
{
	if (stageNo == POSEIDON_STAGE)
	{
		if (typeFlag == NORMAL_TYPE)
		{
			hp -= 9;
		}

		if (typeFlag == WATER_TYPE)
		{
			hp -= 8;
		}
		if (typeFlag == LEAF_TYPE)
		{
			hp -= 5;
		}
		if (typeFlag == FIRE_TYPE)
		{
			hp -= 19;
		}
	}

	if (stageNo == PLUTON_STAGE)
	{
		if (typeFlag == NORMAL_TYPE)
		{
			hp -= 100;
		}

		if (typeFlag == FIRE_TYPE)
		{
			hp -= 80;
		}
		if (typeFlag == WATER_TYPE)
		{
			hp -= 50;
		}
		if (typeFlag == LEAF_TYPE)
		{
			hp -= 150;
		}
	}

	if (stageNo == URANUS_STAGE)
	{
		if (typeFlag == NORMAL_TYPE)
		{
			hp -= 5;
		}

		if (typeFlag == LEAF_TYPE)
		{
			hp -= 4;
		}
		if (typeFlag == FIRE_TYPE)
		{
			hp -= 1;
		}
		if (typeFlag == WATER_TYPE)
		{
			hp -= 15;
		}
	}
}

void Player::SpecialAttackDamege(int stageNo)
{

	if (stageNo == POSEIDON_STAGE)
	{
		if (typeFlag == NORMAL_TYPE || typeFlag == WATER_TYPE)
		{
			hp -= 35;
		}
		if (typeFlag == LEAF_TYPE)
		{
			hp -= 25;
		}
		if (typeFlag == FIRE_TYPE)
		{
			hp -= 45;
		}
	}

	if (stageNo == PLUTON_STAGE)
	{
		if (typeFlag == NORMAL_TYPE || typeFlag == FIRE_TYPE)
		{
			hp -= 200;
		}
		if (typeFlag == WATER_TYPE)
		{
			hp -= 150;
		}
		if (typeFlag == LEAF_TYPE)
		{
			hp -= 250;
		}
	}

	if (stageNo == URANUS_STAGE)
	{
		if (typeFlag == NORMAL_TYPE || typeFlag == LEAF_TYPE)
		{
			hp -= 4;
		}
		if (typeFlag == FIRE_TYPE)
		{
			hp -= 3;
		}
		if (typeFlag == WATER_TYPE)
		{
			hp -= 5;
		}
	}
}

void Player::Guide_INOUT(bool enemy_AttackFlag)
{
	if (enemy_AttackFlag == false)
	{
		if (alpha < 0.8f)
		{
			alpha += 0.05f;
		}
		else
		{
			alpha = 0.8f;
		}
		model_LB->SetAlpha(alpha);
		model_RB->SetAlpha(alpha);
	}
	else
	{
		if (alpha > 0.0f)
		{
			alpha -= 0.05f;
		}
		else
		{
			alpha = 0.0f;
		}
		model_LB->SetAlpha(alpha);
		model_RB->SetAlpha(alpha);
	}
}

void Player::Death()
{
	if (hp <= 0)
	{
		audio->StopWave("chargeSE.wav");
		rot_Player.z = 80.0f;
		deathFlag = true;
	}
}

void Player::Update(bool oopth_AttackFlag, bool boss_AttackFlag, bool oopth_StartFlag, bool boss_StartFlag, short turnNumber)
{
	//シェイク演出
	Shake();
	//死亡演出
	Death();
	if (oopth_StartFlag == true || boss_StartFlag == true)
	{
		//属性変更
		TypeChange();
		//本体の移動処理
		PlayerMove();

		if (turnNumber != 4)
		{
			//ガイドオブジェクト出現＆消失
			Guide_INOUT(oopth_AttackFlag);
			if (oopth_AttackFlag == false)
			{
				//装填
				Loading();
				//弾の移動処理
				BulletMove();
				//弾のエフェクト
				FireEffect();
			}
		}
		else
		{
			//ガイドオブジェクト出現＆消失
			Guide_INOUT(boss_AttackFlag);
			if (boss_AttackFlag == false)
			{
				//装填
				Loading();
				//弾の移動処理
				BulletMove();
				//弾のエフェクト
				FireEffect();
			}
		}
	}


	//プレイヤー更新
	obj_Player->SetPosition({ pos_Player.x + shake.x, pos_Player.y + shake.y, pos_Player.z });
	obj_Player->SetRotation(rot_Player);
	obj_Player->Update();
	//ガイドオブジェクト更新
	guide_LB->SetPosition({ pos_Player.x - 1.2f, pos_Player.y + 1.2f, pos_Player.z });
	guide_RB->SetPosition({ pos_Player.x + 1.2f, pos_Player.y + 1.2f, pos_Player.z });
	guide_LB->Update();
	guide_RB->Update();
}

void Player::Draw()
{
	//プレイヤー描画
	obj_Player->Draw();

	//ガイドオブジェクト描画
	if (alpha != 0.0f)
	{
		guide_LB->Draw();
		guide_RB->Draw();
	}

}
