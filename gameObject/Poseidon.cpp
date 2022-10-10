#include "Poseidon.h"
#include "SafeDelete.h"
#define _USE_MATH_DEFINES
#include <math.h>

Poseidon* Poseidon::Create()
{
	// インスタンスの生成
	Poseidon* instance = new Poseidon();

	// インスタンスの初期化
	instance->Initialize();

	// インスタンスを返す
	return instance;
}

Poseidon::~Poseidon()
{
	//本体
	safe_delete(boss.obj);
	safe_delete(boss.model);
	//コリジョン
	safe_delete(boss.collision);
	safe_delete(model_Collision);
	//サークル
	for (int i = 0; i < 6; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
	//弾
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
	{
		safe_delete(bullet.obj[i]);
	}
	safe_delete(bullet.model);
}

void Poseidon::Initialize()
{
#pragma region 複数使用モデル
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region ボス初期化

	//オープス１・２・３モデル読み込み
	boss.model = Model::CreateFromOBJ("poseidon");

	//生成
	boss.obj = Object3d::Create();
	boss.obj->SetModel(boss.model);

	//初期座標
	boss.pos = { 0.0f, 50.0f, 10.0f };//変形攻撃時 z=17 攻撃時 z = 17 通常時 z = 10
	boss.obj->SetPosition(boss.pos);

	//初期速度
	boss.vel = { 0.0f, 0.0f, 0.0f };

	//初期加速度
	boss.acc = 0.01f;

	//初期回転角
	boss.rot = { 0.0f, 0.0f, 0.0f };

	//初期透明度
	boss.alpha = 1.0f;

	//初期シェイク値
	boss.shake = { 0.0f, 0.0f };

	//状態変化変数初期化
	boss.startFlag = false;
	boss.attackPreparationFlag = false;
	boss.attackFlag = false;
	boss.statusChangeFlag = false;
	boss.changeTimer = 150;
	boss.attackShakeTimer = 0.2f;
	boss.damegeShakeFlag = false;
	boss.damegeShakeTimer = 0.0f;
	boss.attackOrder = 0;
	boss.attackEndFlag = false;
	boss.fixFlag = false;
	boss.fixTimer = 20;
	boss.modeChangeFlag = false;
	boss.deathFlag = false;

	//Hp最大値
	boss.hp = 400;

	//コリジョン
	boss.collision = Object3d::Create();
	boss.radius = 5.0f;
	boss.collision->SetScale({ boss.radius, boss.radius, boss.radius });

#pragma endregion

#pragma region サークル初期化
	//モデル読み込み
	circle.model = Model::CreateFromOBJ("circle_W");

	for (int i = 0; i < 6; i++)
	{
		//生成
		circle.obj[i] = Object3d::Create();
		circle.obj[i]->SetModel(circle.model);

		//初期座標
		//左側
		circle.pos[0] = { -4.5f, 1.5f, +2.7f };
		circle.pos[1] = { -4.5f, 1.5f,  0.0f };
		circle.pos[2] = { -4.5f, 1.5f, -2.7f };
		//右側
		circle.pos[3] = { +4.5f, 1.5f, +2.7f };
		circle.pos[4] = { +4.5f, 1.5f,  0.0f };
		circle.pos[5] = { +4.5f, 1.5f, -2.7f };
		circle.obj[i]->SetPosition(circle.pos[i]);

		//初期回転角
		circle.rot = { 0.0f, 90.0f, 0.0f };

		//初期スケール
		circle.scale = { 0.0f, 0.0f, 1.0f };
	}
#pragma endregion

#pragma region バレット
	//モデル
	bullet.model = Model::CreateFromOBJ("drop");
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
	{
		//生成
		bullet.obj[i] = Object3d::Create();
		bullet.obj[i]->SetModel(bullet.model);

		//初期座標
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

		//初期回転角
		if (i < 60)
		{
			//左側
			bullet.rot[i] = { 0.0f, 0.0f, +90.0f };
		}
		else
		{
			//右側
			bullet.rot[i] = { 0.0f, 0.0f, -90.0f };
		}

		//サイズ
		bullet.obj[i]->SetScale({ 0.25f, 0.25f, 0.25f });

		//状態変化変数
		bullet.hitFlag[i] = { false };
		bullet.fireFlag[i] = { false };
	}


	//状態変化変数初期化
	bullet.delayTimer = 3;

	//パーティクル
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region チャージ初期化
	charge.angle = 0.0f;
	charge.radius = 5.0f;
	for (int i = 0; i < SPLASHCHARGEBULLET_MAX; i++)
	{
		//初期座標
		charge.pos[i] = { boss.pos.x + cos(charge.angle + i * 0.628f) * charge.radius,
						  boss.pos.y + sin(charge.angle + i * 0.628f) * charge.radius,
						  boss.pos.z - 5.0f };
	}
#pragma endregion

#pragma region オーディオ
	audio = Audio::GetInstance();
#pragma endregion


#pragma region 外部干渉用変数
	shakeFlag = false;
#pragma endregion
}

#pragma region ポセイドン関数
void Poseidon::Appearance()
{
	if (boss.pos.y >= 2.0f)
	{
		boss.pos.y -= boss.vel.y;
		boss.vel.y += boss.acc;
	}
	else
	{
		audio->PlayWave("arrivalSE2.wav", false);
		shakeFlag = true;
		boss.startFlag = true;
		boss.statusChangeFlag = true;

	}
}

void Poseidon::AttackPreparation()
{
	//攻撃準備フラッグがONなら
	if (boss.attackPreparationFlag == true)
	{
		ModeChange();
		//設定値までサークルのスケールを拡大、同時に回転する
		if (circle.scale.x <= 0.02f && circle.scale.y <= 0.02f)
		{
			//拡大
			circle.scale.x += 0.0005f;
			circle.scale.y += 0.0005f;
			//回転
			circle.rot.x -= 100.0f;
		}
		//座標を変更
		if (boss.pos.z <= 15.0f)
		{
			boss.pos.z += 0.1f;
		}
		//上記の処理が終了
		else
		{
			//攻撃状態へ移行
			boss.attackFlag = true;
			//攻撃準備状態から外れる
			boss.attackPreparationFlag = false;
			//サークルの回転角を元に戻す
			circle.rot = { 0.0f, 90.0f, 0.0f };
		}
	}
}

void Poseidon::StatusChange()
{
	//非攻撃状態の時
	if (boss.statusChangeFlag == true)
	{
		boss.changeTimer -= 1;
		if (boss.changeTimer <= 0)
		{
			boss.attackPreparationFlag = true;
			boss.statusChangeFlag = false;
		}
	}
}

void Poseidon::ModeChange()
{
	if (boss.modeChangeFlag == true)
	{
		if (boss.rot.y < 180.0f)
		{
			boss.rot.y += 5.0f;
		}
		else
		{
			boss.rot.y = 180.0f;
		}
	}
}

void Poseidon::AttackShake()
{
	boss.shake.x = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
	boss.shake.y = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
}

void Poseidon::DamegeShake()
{
	if (boss.damegeShakeFlag == true)
	{
		boss.damegeShakeTimer = 0.2f;
		boss.damegeShakeFlag = false;
	}

	if (boss.damegeShakeTimer > 0)
	{
		boss.shake.x = (float)rand() / RAND_MAX * boss.damegeShakeTimer - boss.damegeShakeTimer / 2.0f;
		boss.shake.y = (float)rand() / RAND_MAX * boss.damegeShakeTimer - boss.damegeShakeTimer / 2.0f;
		boss.damegeShakeTimer -= 0.005f;
	}
}

void Poseidon::AttackEnd()
{
	if (boss.attackEndFlag == true)
	{
		for (int i = 0; i < POSEIDONBULLET_MAX; i++)
		{
			bullet.hitFlag[i] = false;
			bullet.fireFlag[i] = false;
		}

		//設定値までサークルのスケールを縮小、同時に回転する
		if (circle.scale.x >= 0.0f && circle.scale.y >= 0.0f)
		{
			//拡大
			circle.scale.x -= 0.0005f;
			circle.scale.y -= 0.0005f;
			//回転
			circle.rot.x += 100.0f;

		}
		if (boss.pos.z >= 10.0f)
		{
			boss.pos.x = 0.0f;
			boss.pos.z -= 0.1f;
		}
		//上記処理が終了したら
		else
		{
			for (int i = 0; i < 6; i++)
			{
				circle.pos[i].y = 1.5f;
			}
			//StatusChangeを再起動
			boss.statusChangeFlag = true;
			boss.changeTimer = 150;
			//AttackEndをOFF
			boss.attackEndFlag = false;
		}

		if (boss.modeChangeFlag == true)
		{
			if (boss.rot.y > 0.0f)
			{
				boss.rot.y -= 5.0f;
			}
			else
			{
				boss.rot.y = 0.0f;
			}
		}
	}
}

void Poseidon::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 2)
	{
		boss.hp -= 3;
	}
	if (player_TypeFlag == 3)
	{
		boss.hp -= 7;
	}
	if (player_TypeFlag == 1)
	{
		boss.hp -= 2;
	}
	if (boss.hp < 200)
	{
		boss.modeChangeFlag = true;
	}
}

void Poseidon::Death(int turnNomber)
{
	if (boss.hp <= 0)
	{
		if (boss.alpha >= 0.0f)
		{
			boss.alpha -= 0.05f;
		}
		//死亡と同時に初期化
		else
		{
			boss.deathFlag = true;
		}

		boss.model->SetAlpha(boss.alpha);
	}
}
#pragma endregion

#pragma region バレット関数
void Poseidon::BulletEffect()
{
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
	{
		const float rnd_pos = 0.1f;
		particle.pos.x = bullet.pos[i].x + (float)rand() / RAND_MAX * rnd_pos;
		particle.pos.y = bullet.pos[i].y + (float)rand() / RAND_MAX * rnd_pos;
		particle.pos.z = bullet.pos[i].z + (float)rand() / RAND_MAX * rnd_pos;

		particle.vel.x = 0.01f;

		const float rnd_acc = 0.001f;
		particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

		particleMan->Add(10, particle.pos, particle.vel, particle.acc, 1.0f, 0.1f, { 0.0f, 0.5f, 5.0f, 1.0f });
	}
}

void Poseidon::ChargeEffect(bool field_completionFlag)
{
	if (boss.modeChangeFlag == true && field_completionFlag == true && boss.attackOrder != 18)
	{
		audio->PlayWave("chargeSE.wav", false);
		if (charge.radius >= 0.0f)
		{
			charge.radius -= 0.05f;
			charge.angle += 0.1f;
		}
		else
		{
			charge.radius = 5.0f;
		}

		for (int i = 0; i < SPLASHCHARGEBULLET_MAX; i++)
		{
			if (i < SPLASHCHARGEBULLET_MAX / 2)
			{
				charge.pos[i].x = boss.pos.x - 2.2f + cos(charge.angle + i * 0.628f) * charge.radius;
			}
			else
			{
				charge.pos[i].x = boss.pos.x + 2.2f + cos(charge.angle + i * 0.628f) * charge.radius;
			}
			charge.pos[i].y = 1.7f + sin(charge.angle + i * 0.628f) * charge.radius;

			const float rnd_pos = 0.01f;
			particle.pos.x = charge.pos[i].x + (float)rand() / RAND_MAX * rnd_pos;
			particle.pos.y = charge.pos[i].y + (float)rand() / RAND_MAX * rnd_pos;
			particle.pos.z = charge.pos[i].z + (float)rand() / RAND_MAX * rnd_pos;

			particle.vel.x = 0.01f;

			const float rnd_acc = 0.0f;
			particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

			particleMan->Add(18, particle.pos, particle.vel, particle.acc, 1.8f, 0.1f, { 0.0f, 0.05f, 2.0f, 0.1f });
		}
	}
}

void Poseidon::BulletLoading_1()
{
	bullet.delayTimer -= 1;
	for (int i = 0; bullet.delayTimer <= 0 && i < 120; i++)
	{
		//オーダー１
		if (i < 20 && bullet.fireFlag[i] == false && boss.attackOrder == 0)
		{
			bullet.pos[i].x = circle.pos[0].x;
			bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[19] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー２
		else if (i < 40 && bullet.fireFlag[i] == false && boss.attackOrder == 1)
		{
			bullet.pos[i].x = circle.pos[1].x;
			bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[39] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー３
		else if (i < 60 && bullet.fireFlag[i] == false && boss.attackOrder == 2)
		{
			bullet.pos[i].x = circle.pos[2].x;
			bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[59] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー４
		else if (i < 80 && bullet.fireFlag[i] == false && boss.attackOrder == 3)
		{
			bullet.pos[i].x = circle.pos[3].x;
			bullet.pos[i].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[79] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー５
		else if (i < 100 && bullet.fireFlag[i] == false && boss.attackOrder == 4)
		{
			bullet.pos[i].x = circle.pos[4].x;
			bullet.pos[i].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[99] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー６
		else if (i < 120 && bullet.fireFlag[i] == false && boss.attackOrder == 5)
		{
			bullet.pos[i].x = circle.pos[5].x;
			bullet.pos[i].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[119] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void Poseidon::BulletLoading_2()
{
	bullet.delayTimer -= 1;
	for (int i = 120; bullet.delayTimer <= 0 && i < 240; i++)
	{
		//オーダー１
		if (i < 140 && bullet.fireFlag[i] == false && boss.attackOrder == 6)
		{
			bullet.pos[i].x = circle.pos[4].x;
			bullet.pos[i].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[139] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー２
		else if (i < 160 && bullet.fireFlag[i] == false && boss.attackOrder == 7)
		{
			bullet.pos[i].x = circle.pos[2].x;
			bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[159] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー３
		else if (i < 180 && bullet.fireFlag[i] == false && boss.attackOrder == 8)
		{
			bullet.pos[i].x = circle.pos[3].x;
			bullet.pos[i].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[179] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー４
		else if (i < 200 && bullet.fireFlag[i] == false && boss.attackOrder == 9)
		{
			bullet.pos[i].x = circle.pos[1].x;
			bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[199] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー５
		else if (i < 220 && bullet.fireFlag[i] == false && boss.attackOrder == 10)
		{
			bullet.pos[i].x = circle.pos[0].x;
			bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[219] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー６
		else if (i < 240 && bullet.fireFlag[i] == false && boss.attackOrder == 11)
		{
			bullet.pos[i].x = circle.pos[5].x;
			bullet.pos[i].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[239] == true)
			{
				boss.attackOrder += 1;
				bullet.delayTimer = 10;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void Poseidon::BulletLoading_3()
{
	bullet.delayTimer -= 1;
	for (int i = 240; bullet.delayTimer <= 0 && i < 360; i++)
	{
#pragma region オーダー１２・オーダー４
		if (boss.attackOrder == 12 || boss.attackOrder == 15)
		{
			if (i < 260 && bullet.fireFlag[i] == false)
			{
				bullet.pos[i].x = circle.pos[0].x;
				bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i].z = 90.0f;
				bullet.fireFlag[i] = true;

				bullet.pos[i + 20].x = circle.pos[3].x;
				bullet.pos[i + 20].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i + 20].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i + 20].z = -90.0f;
				bullet.fireFlag[i + 20] = true;
				if (boss.attackOrder == 15)
				{
					FireFlagReset320_359();
				}
				bullet.delayTimer = 3;
				if (bullet.fireFlag[259] == true)
				{
					boss.attackOrder += 1;
					bullet.delayTimer = 10;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}
#pragma endregion

#pragma region オーダー１３・オーダー５
		if (boss.attackOrder == 13 || boss.attackOrder == 16)
		{
			if (i >= 280 && i < 300 && bullet.fireFlag[i] == false)
			{
				bullet.pos[i].x = circle.pos[1].x;
				bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i].z = 90.0f;
				bullet.fireFlag[i] = true;

				bullet.pos[i + 20].x = circle.pos[4].x;
				bullet.pos[i + 20].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i + 20].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i + 20].z = -90.0f;
				bullet.fireFlag[i + 20] = true;
				bullet.delayTimer = 3;
				if (bullet.fireFlag[299] == true)
				{
					boss.attackOrder += 1;
					bullet.delayTimer = 10;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}
#pragma endregion

#pragma region オーダー１４・オーダー６
		if (boss.attackOrder == 14 || boss.attackOrder == 17)
		{
			//オーダー３
			if (i >= 320 && i < 340 && bullet.fireFlag[i] == false)
			{
				bullet.pos[i].x = circle.pos[2].x;
				bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i].z = 90.0f;
				bullet.fireFlag[i] = true;

				bullet.pos[i + 20].x = circle.pos[5].x;
				bullet.pos[i + 20].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
				bullet.pos[i + 20].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
				bullet.rot[i + 20].z = -90.0f;
				bullet.fireFlag[i + 20] = true;
				bullet.delayTimer = 3;
				if (boss.attackOrder == 14)
				{
					if (bullet.fireFlag[339] == true)
					{
						boss.attackOrder += 1;
						bullet.delayTimer = 10;
					}
					FireFlagReset240_279();
					FireFlagReset280_319();
				}
				if (boss.attackOrder == 17 && bullet.fireFlag[339] == true && boss.modeChangeFlag == true)
				{
					boss.attackOrder += 1;
					bullet.delayTimer = 20;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}

#pragma endregion
	}
}

void Poseidon::BulletLoading_4()
{
	bullet.delayTimer -= 1;
	//左側
	circle.pos[0].y = 100.0f;
	circle.pos[1].y = 100.0f;
	circle.pos[2].y = 100.0f;
	//右側		
	circle.pos[3].y = 100.0f;
	circle.pos[4].y = 100.0f;
	circle.pos[5].y = 100.0f;
	audio->StopWave("chargeSE.wav");
	for (int i = 360; bullet.delayTimer <= 0 && i < 400; i++)
	{
		if (bullet.fireFlag[i] == false && boss.attackOrder == 18)
		{
			bullet.pos[i].x = ((boss.pos.x - 2.7f) + (float)rand() / RAND_MAX) - 0.5;
			bullet.pos[i].y = (1.5f + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = boss.pos.z - 8.0f;
			bullet.rot[i].x = 90.0f;
			bullet.fireFlag[i] = true;

			bullet.pos[i + 40].x = ((boss.pos.x + 2.7f) + (float)rand() / RAND_MAX) - 0.5;
			bullet.pos[i + 40].y = (1.5f + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i + 40].z = boss.pos.z - 8.0f;
			bullet.rot[i + 40].x = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void Poseidon::BulletMove_1(int i)
{
	//レフト
	if (i < 60)
	{
		//画面外に出ていないら
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//画面外
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
	//ライト
	else if (i >= 60 && i < 120)
	{
		//画面外に出ていないら
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x -= 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i].x <= circle.pos[0].x)
			{
				//画面外
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
}

void Poseidon::BulletMove_2(int i)
{
	if (i >= 120 && i < 140 || i >= 160 && i < 180 || i >= 220 && i < 240)
	{
		//画面外に出ていないら
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x -= 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i].x <= circle.pos[0].x)
			{
				//画面外
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
	else if (i >= 140 && i < 160 || i >= 180 && i < 200 || i >= 200 && i < 220)
	{
		//画面外に出ていないら
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//画面外
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
}

void Poseidon::BulletMove_3(int i)
{
	if (i >= 240 && i < 260)
	{
		//画面外に出ていないら
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//画面外
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}

		//画面外に出ていないら
		if (bullet.pos[i + 20].y != 100.0f)
		{
			bullet.pos[i + 20].x -= 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i + 20].x <= circle.pos[0].x)
			{
				//画面外
				bullet.pos[i + 20] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
	else if (i >= 280 && i < 300)
	{
		//画面外に出ていないら
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//画面外
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}

		//画面外に出ていないら
		if (bullet.pos[i + 20].y != 100.0f)
		{
			bullet.pos[i + 20].x -= 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i + 20].x <= circle.pos[0].x)
			{
				//画面外
				bullet.pos[i + 20] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
	else if (i >= 320 && i < 340)
	{
		//画面外に出ていないら
		if (bullet.pos[i].y != 100.0f)
		{
			bullet.pos[i].x += 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i].x >= circle.pos[4].x)
			{
				//画面外
				bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
			}
		}

		//画面外に出ていないら
		if (bullet.pos[i + 20].y != 100.0f)
		{
			bullet.pos[i + 20].x -= 0.5f;
			//設定座標以上になったら
			if (bullet.pos[i + 20].x <= circle.pos[0].x)
			{
				//画面外
				bullet.pos[i + 20] = { 0.0f, 100.0f, 0.0f };
			}
		}
	}
}

void Poseidon::BulletMove_4(int i)
{
	if (bullet.fireFlag[i] == true)
	{
		if (i >= 360 && i < 400)
		{
			//画面外に出ていないら
			if (bullet.pos[i].y != 100.0f)
			{
				bullet.pos[i].z -= 0.5f;
				//設定座標以上になったら
				if (bullet.pos[i].z <= -8.0f)
				{
					//画面外
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}

			//画面外に出ていないら
			if (bullet.pos[i + 40].y != 100.0f)
			{
				bullet.pos[i + 40].z -= 0.5f;
				//設定座標以上になったら
				if (bullet.pos[i + 40].z <= -8.0f)
				{
					//画面外
					bullet.pos[i + 40] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}
	}
}

void Poseidon::FireFlagReset240_279()
{
	for (int i = 240; i < 280; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void Poseidon::FireFlagReset280_319()
{
	for (int i = 280; i < 320; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void Poseidon::FireFlagReset320_359()
{
	for (int i = 320; i < 360; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void Poseidon::SplashBlast1(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		circle.rot.x += 200.0f;

		//装填
		if (boss.attackOrder < 6)
		{
			BulletLoading_1();
		}
		else if (boss.attackOrder >= 6 && boss.attackOrder < 12)
		{
			BulletLoading_2();
		}
		else
		{
			BulletLoading_3();
		}

		for (int i = 0; i < POSEIDONBULLET_MAX; i++)
		{
			//発射フラッグがONなら
			if (bullet.fireFlag[i] == true)
			{
				//起動
				BulletMove_1(i);
				BulletMove_2(i);
				BulletMove_3(i);

				//あたり判定
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//最後の弾が発射され、画面外に出たら
		if (boss.attackOrder == 17 && bullet.fireFlag[359] == true && bullet.pos[359].y == 100.0f)
		{
			boss.fixFlag = true;

		}

		if (boss.fixFlag == true)
		{
			boss.fixTimer -= 1;
			if (boss.fixTimer <= 0)
			{
				boss.fixTimer = 20;
				boss.fixFlag = false;
				boss.attackFlag = false;
				boss.attackEndFlag = true;
			}
		}

		//バレットエフェクト起動
		BulletEffect();
		//シェイク演出
		AttackShake();

	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		boss.rot = { 0.0f, 0.0f, 0.0f };
		boss.attackOrder = 0;
	}
}

void Poseidon::SplashBlast2(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		circle.rot.x += 200.0f;

		//装填
		if (boss.attackOrder < 6)
		{
			BulletLoading_1();
		}
		else if (boss.attackOrder >= 6 && boss.attackOrder < 12)
		{
			BulletLoading_2();
		}
		else if (boss.attackOrder >= 12 && boss.attackOrder < 18)
		{
			BulletLoading_3();
		}
		else
		{
			BulletLoading_4();
		}

		for (int i = 0; i < POSEIDONBULLET_MAX; i++)
		{
			//発射フラッグがONなら
			if (bullet.fireFlag[i] == true)
			{
				//起動
				BulletMove_1(i);
				BulletMove_2(i);
				BulletMove_3(i);
				BulletMove_4(i);

				//あたり判定
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//最後の弾が発射され、画面外に出たら
		if (boss.attackOrder == 18 && bullet.fireFlag[399] == true && bullet.pos[399].y == 100.0f)
		{
			boss.fixFlag = true;
		}

		if (boss.fixFlag == true)
		{
			boss.fixTimer -= 1;
			if (boss.fixTimer <= 0)
			{
				boss.fixTimer = 20;
				boss.fixFlag = false;
				boss.attackFlag = false;
				boss.attackEndFlag = true;
			}
		}
		//バレットエフェクト起動
		BulletEffect();
		//シェイク演出
		AttackShake();

	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		boss.attackOrder = 0;
	}
}
#pragma endregion


void Poseidon::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
{
	//スタート時
	if (turnNomber == 4)
	{
		if (boss.startFlag == false && scene_completionFlag == true)
		{
			//スタート時
			Appearance();
		}
		else
		{
			//非攻撃状態から攻撃状態への移行
			StatusChange();
			//攻撃準備状態から攻撃状態へ
			AttackPreparation();
			if (boss.modeChangeFlag == false)
			{
				//攻撃パターン１
				SplashBlast1(field_completionFlag);
			}
			else
			{
				//攻撃パターン２
				SplashBlast2(field_completionFlag);
			}
			//チャージ演出
			ChargeEffect(field_completionFlag);
			//攻撃終了時
			AttackEnd();
			////被弾時のシェイク演出
			DamegeShake();
			//死亡時
			Death(turnNomber);
		}
	}

	boss.obj->SetPosition({ boss.pos.x + boss.shake.x, boss.pos.y + boss.shake.y, boss.pos.z });
	boss.obj->SetRotation(boss.rot);
	boss.obj->Update();
	boss.collision->SetPosition(boss.pos);
	boss.collision->Update();
	for (int i = 0; i < 6; i++)
	{
		circle.obj[i]->SetPosition(circle.pos[i]);
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();
	}
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
	{
		bullet.obj[i]->SetPosition(bullet.pos[i]);
		bullet.obj[i]->SetRotation(bullet.rot[i]);
		bullet.obj[i]->Update();
	}
}

void Poseidon::Draw()
{
	boss.obj->Draw();
	for (int i = 0; i < 6; i++)
	{
		circle.obj[i]->Draw();
	}
	for (int i = 0; i < POSEIDONBULLET_MAX; i++)
	{
		if (bullet.fireFlag[i] == true)
		{
			bullet.obj[i]->Draw();
		}
	}
}
