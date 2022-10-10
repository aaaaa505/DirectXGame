#include "Pluton.h"
#include "SafeDelete.h"
#define _USE_MATH_DEFINES
#include <math.h>


Pluton* Pluton::Create()
{
	// インスタンスの生成
	Pluton* instance = new Pluton();

	// インスタンスの初期化
	instance->Initialize();

	// インスタンスを返す
	return instance;
}

Pluton::~Pluton()
{
	//本体
	safe_delete(boss.obj);
	safe_delete(boss.model);
	//コリジョン
	safe_delete(boss.collision);
	safe_delete(model_Collision);
	//サークル
	for (int i = 0; i < 3; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
}

void Pluton::Initialize()
{
#pragma region 複数使用モデル
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region ボス初期化
	//オープス１・２・３モデル読み込み
	boss.model = Model::CreateFromOBJ("pluton");

	//生成
	boss.obj = Object3d::Create();
	boss.obj->SetModel(boss.model);

	//初期座標
	boss.pos = { 0.0f, 50.0f, 11.0f };//変形攻撃時 z=17 攻撃時 z = 17 通常時 z = 11
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
	boss.deathFlag = false;
	boss.attackPreparationFlag = false;
	boss.attackFlag = false;
	boss.statusChangeFlag = false;
	boss.changeTimer = 150;
	boss.attackShakeTimer = 0.2f;
	boss.damegeShakeFlag = false;
	boss.damegeShakeTimer = 0.0f;
	for (int i = 0; i < 10; i++)
	{
		boss.attackOrder[i] = false;
	}
	boss.attackEndFlag = false;
	boss.fixFlag = false;
	boss.fixTimer = 20;
	boss.modeChangeFlag = false;

	//Hp最大値
	boss.hp = 400;

	//コリジョン
	boss.collision = Object3d::Create();
	boss.radius = 5.0f;
	boss.collision->SetScale({ boss.radius, boss.radius, boss.radius });

#pragma endregion

#pragma region サークル初期化
	//モデル読み込み
	circle.model = Model::CreateFromOBJ("circle_F");

	for (int i = 0; i < 3; i++)
	{
		//生成
		circle.obj[i] = Object3d::Create();
		circle.obj[i]->SetModel(circle.model);

		//初期座標
		circle.pos[0] = { -2.7f, 1.5f, 5.0f };
		circle.pos[1] = { 0.0f, 1.5f, 5.0f };
		circle.pos[2] = { +2.7f, 1.5f, 5.0f };
		circle.obj[i]->SetPosition(circle.pos[i]);

		//初期回転角
		circle.rot = { 0.0f, 0.0f, 0.0f };
		circle.obj[i]->SetRotation(circle.rot);

		//初期スケール
		circle.scale = { 0.0f, 0.0f, 1.0f };
		circle.obj[i]->SetScale(circle.scale);
	}
#pragma endregion

#pragma region バレット・パーティクル初期化
	for (int i = 0; i < PLUTONBULLET_MAX; i++)
	{
		//初期座標
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

		bullet.hitFlag[i] = false;
	}

	//半径
	bullet.radius = 1.0f;

	//状態変化変数初期化
	bullet.allFireFlag = false;
	bullet.delayFlag = false;
	bullet.delayTimer = 0;

	//パーティクル
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region チャージ初期化
	charge.angle = 0.0f;
	charge.radius = 15.0f;
	for (int i = 0; i < FLAMECHARGEBULLET_MAX; i++)
	{
		//初期座標
		charge.pos[i] = { boss.pos.x + cos(charge.angle + i * 0.628f) * charge.radius, 
						  boss.pos.y + sin(charge.angle + i * 0.628f) * charge.radius,
						  boss.pos.z - 5.0f };
	}
#pragma endregion

#pragma region フレイム初期化
	flame.pos = { 0.0f, 100.0f, 0.0f};

	flame.radius = 5.0f;

	//状態変化変数
	flame.hitFlag = false;
#pragma endregion

#pragma region オーディオ
	audio = Audio::GetInstance();
#pragma endregion


#pragma region 外部干渉変数
	shakeFlag = false;
#pragma endregion
}

#pragma region プルトン関数
void Pluton::Appearance()
{
	if (boss.pos.y >= 3.0f)
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

void Pluton::AttackPreparation()
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
			circle.rot.z -= 100.0f;
		}
		//座標を変更
		if (boss.pos.z <= 20.0f)//50
		{
			boss.pos.z += 0.1f;
		}
		//上記の処理が終了
		else
		{
			//攻撃状態へ移行
			boss.attackFlag = true;
			//弾を装填
			Loading();
			//攻撃準備状態から外れる
			boss.attackPreparationFlag = false;
			//サークルの回転角を元に戻す
			circle.rot = { 0.0f, 0.0f, 0.0f };
		}
	}
}

void Pluton::StatusChange()
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

void Pluton::ModeChange()
{
	if (boss.modeChangeFlag == true)
	{
		if (boss.rot.y < 180.0f)
		{
			boss.rot.y += 3.0f;
		}
		else
		{
			boss.rot.y = 180.0f;
		}
	}
}

void Pluton::AttackShake()
{
	boss.shake.x = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
	boss.shake.y = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
}

void Pluton::AttackEnd()
{
	if (boss.attackEndFlag == true)
	{
		for (int i = 0; i < PLUTONBULLET_MAX; i++)
		{
			bullet.hitFlag[i] = false;
		}

		flame.hitFlag = false;

		//設定値までサークルのスケールを縮小、同時に回転する
		if (circle.scale.x >= 0.0f && circle.scale.y >= 0.0f)
		{
			//拡大
			circle.scale.x -= 0.0005f;
			circle.scale.y -= 0.0005f;
			//回転
			circle.rot.z += 100.0f;

			circle.pos[0].y = 1.5f;
			circle.pos[1].y = 1.5f;
			circle.pos[2].y = 1.5f;
		}
		if(boss.pos.z >= 11.0f)
		{
			boss.pos.x = 0.0f;
			boss.pos.z -= 0.1f;
		}
		//上記処理が終了したら
		else
		{
			//StatusChangeを再起動
			boss.statusChangeFlag = true;
			boss.changeTimer = 150;
			//AllFireFlagをOFF
			bullet.allFireFlag = false;
			//AttackEndをOFF
			boss.attackEndFlag = false;
		}

		if (boss.modeChangeFlag == true)
		{
			if (boss.rot.y > 0.0f)
			{
				boss.rot.y -= 3.0f;
			}
			else
			{
				boss.rot.y = 0.0f;
			}
		}
	}
}

void Pluton::DamegeShake()
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

void Pluton::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 1)
	{
		boss.hp -= 3;
	}
	if (player_TypeFlag == 2)
	{
		boss.hp -= 7;
	}
	if (player_TypeFlag == 3)
	{
		boss.hp -= 2;
	}
	if (boss.hp < 200)
	{
		boss.modeChangeFlag = true;
	}
}

void Pluton::Death(int turnNomber)
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
void Pluton::Loading()
{
	//起動時に各サークルの裏に弾を三個ずつ配置
	for (int i = 0; i < PLUTONBULLET_MAX; i++)
	{
		if (i < 6)
		{
			bullet.pos[i] = { circle.pos[0].x, circle.pos[0].y, circle.pos[0].z + 0.6f };
		}
		else if (i < 12)
		{
			bullet.pos[i] = { circle.pos[1].x, circle.pos[1].y, circle.pos[1].z + 0.6f };
		}
		else
		{
			bullet.pos[i] = { circle.pos[2].x, circle.pos[2].y, circle.pos[2].z + 0.6f };
		}
	}
}

void Pluton::FireEffect()
{
	for (int i = 0; i < PLUTONBULLET_MAX; i++)
	{
		const float rnd_pos = 0.1f;
		particle.pos.x = bullet.pos[i].x + (float)rand() / RAND_MAX * rnd_pos;
		particle.pos.y = bullet.pos[i].y + (float)rand() / RAND_MAX * rnd_pos;
		particle.pos.z = bullet.pos[i].z + (float)rand() / RAND_MAX * rnd_pos;

		particle.vel.z = 0.01f;

		const float rnd_acc = 0.001f;
		particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

		particleMan->Add(18, particle.pos, particle.vel, particle.acc, 1.8f, 0.1f, { 2.0f, 1.0f, 0.0f, 1.0f });
	}
}

void Pluton::ChargeEffect(bool field_completionFlag)
{
	if (boss.modeChangeFlag == true && field_completionFlag == true && boss.attackOrder[9] == false)
	{
		audio->PlayWave("chargeSE.wav", false);
		flame.pos = { boss.pos.x, 1.7f, boss.pos.z - 0.5f };
		boss.pos.x = -1.25f;

		if (charge.radius >= 0.0f)
		{
			charge.radius -= 0.3f;
			charge.angle += 1.54f;
		}
		else
		{
			charge.radius = 15.0f;
		}

		for (int i = 0; i < FLAMECHARGEBULLET_MAX; i++)
		{
			charge.pos[i].x = boss.pos.x + cos(charge.angle + i * 0.628f) * charge.radius;
			charge.pos[i].y = 1.7f + sin(charge.angle + i * 0.628f) * charge.radius;

			const float rnd_pos = 0.01f;
			particle.pos.x = charge.pos[i].x + (float)rand() / RAND_MAX * rnd_pos;
			particle.pos.y = charge.pos[i].y + (float)rand() / RAND_MAX * rnd_pos;
			particle.pos.z = charge.pos[i].z + (float)rand() / RAND_MAX * rnd_pos;

			particle.vel.x = 0.01f;

			const float rnd_acc = 0.0f;
			particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

			particleMan->Add(18, particle.pos, particle.vel, particle.acc, 1.8f, 0.1f, { 2.0f, 0.5f, 0.0f, 1.0f });
		}
	}
}

void Pluton::FlameEffect()
{

	const float rnd_pos = 0.1f;
	particle.pos.x = flame.pos.x + (float)rand() / RAND_MAX * rnd_pos;
	particle.pos.y = flame.pos.y + (float)rand() / RAND_MAX * rnd_pos;
	particle.pos.z = flame.pos.z + (float)rand() / RAND_MAX * rnd_pos;

	particle.vel.z = 0.01f;

	const float rnd_acc = 0.001f;
	particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

	particleMan->Add(18, particle.pos, particle.vel, particle.acc, 5.0f, 4.0f, { 2.0f, 1.0f, 0.0f, 1.0f });
}

void Pluton::OneLineOrderShot()
{
	//オーダー１
	if (boss.attackOrder[0] == true)
	{
		bullet.pos[6].z -= 0.4f;
	}

	//オーダー２
	if (boss.attackOrder[1] == true)
	{
		bullet.pos[0].z -= 0.25f;
	}

	//オーダー３
	if (boss.attackOrder[2] == true)
	{
		bullet.pos[12].z -= 0.25;
	}
}

void Pluton::TwinLineOrderShot()
{
	//オーダー４
	if (boss.attackOrder[3] == true)
	{
		bullet.pos[1].z -= 0.25f;
		bullet.pos[7].z -= 0.25f;
		
	}

	//オーダー５
	if (boss.attackOrder[4] == true)
	{
		bullet.pos[8].z -= 0.25f;
		bullet.pos[13].z -= 0.25f;
	}

	//オーダー６
	if (boss.attackOrder[5] == true)
	{
		bullet.pos[9].z -= 0.25f;
		bullet.pos[2].z -= 0.25f;
	}
}

void Pluton::AllLineOrderShot()
{
	//オーダー７
	if (boss.attackOrder[6] == true)
	{
		bullet.pos[3].z -= 0.25f;
		bullet.pos[10].z -= 0.25f;
		bullet.pos[14].z -= 0.15f;
	}

	//オーダー８
	if (boss.attackOrder[7] == true)
	{
		bullet.pos[4].z -= 0.15f;
		bullet.pos[15].z -= 0.15f;
	}

	//オーダー９
	if (boss.attackOrder[8] == true)
	{
		bullet.pos[5].z -= 0.15f;
		bullet.pos[11].z -= 0.25f;
		bullet.pos[16].z -= 0.25f;
	}
}

void Pluton::TwinLineFlame()
{
	if (boss.attackOrder[9] == true)
	{
		audio->StopWave("chargeSE.wav");
		flame.pos.z -= 0.4f;
		FlameEffect();
		bullet.allFireFlag = true;
		if (flame.pos.z <= -8.0f)
		{
			boss.fixFlag = true;
		}
		
	}
}

void Pluton::FlameShot1(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		circle.rot.z += 200.0f;

		//全弾発射していないなら
		if (bullet.allFireFlag == false)
		{
			//ディレイタイマー起動
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 10 && bullet.delayTimer <= 0; i++)
		{
			if (boss.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				boss.attackOrder[i] = true;
				bullet.delayTimer = 50;
				break;
			}
		}

		for (int i = 0; i < PLUTONBULLET_MAX; i++)
		{
			if (bullet.hitFlag[i] == true)
			{
				bullet.pos[i].y = 100.0f;
			}
		}

		OneLineOrderShot();
		
		TwinLineOrderShot();
		
		AllLineOrderShot();

		if (boss.attackOrder[8] == true)
		{
			bullet.allFireFlag = true;
			if (bullet.pos[5].z <= -8.0f)
			{
				boss.fixFlag = true;
			}
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
		//ファイヤーエフェクト起動
		FireEffect();
		//シェイク起動
		AttackShake();
	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		for (int i = 0; i < 9; i++)
		{
			boss.attackOrder[i] = false;
		}
	}
}

void Pluton::FlameShot2(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		circle.rot.z += 200.0f;

		//全弾発射していないなら
		if (bullet.allFireFlag == false)
		{
			//ディレイタイマー起動
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 10 && bullet.delayTimer <= 0; i++)
		{
			if (boss.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				boss.attackOrder[i] = true;
				bullet.delayTimer = 50;
				break;
			}
		}

		for (int i = 0; i < PLUTONBULLET_MAX; i++)
		{
			if (bullet.hitFlag[i] == true)
			{
				bullet.pos[i].y = 100.0f;
			}
		}

		OneLineOrderShot();

		TwinLineOrderShot();

		AllLineOrderShot();

		if (boss.attackOrder[8] == true)
		{
			circle.pos[0].y = 100.0f;
			circle.pos[1].y = 100.0f;
			circle.pos[2].y = 100.0f;
		}

		TwinLineFlame();
			
		if (boss.fixFlag == true)
		{
			boss.fixTimer -= 1;
			if (boss.fixTimer <= 0)
			{
				flame.pos = { 0.0f, 100.0f, 0.0f };
				boss.fixTimer = 20;
				boss.fixFlag = false;
				boss.attackFlag = false;
				boss.attackEndFlag = true;
			}
		}
		//ファイヤーエフェクト起動
		FireEffect();
		//シェイク起動
		AttackShake();
	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		for (int i = 0; i < 10; i++)
		{
			boss.attackOrder[i] = false;
		}
	}
}
#pragma endregion

void Pluton::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
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
				FlameShot1(field_completionFlag);
			}
			else
			{
				//攻撃パターン２
				FlameShot2(field_completionFlag);
			}
			//チャージ演出
			ChargeEffect(field_completionFlag);
			//攻撃終了時
			AttackEnd();
			//被弾時のシェイク演出
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
	for (int i = 0; i < 3; i++)
	{
		circle.obj[i]->SetPosition(circle.pos[i]);
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();
	}
}

void Pluton::Draw()
{
	boss.obj->Draw();
	for (int i = 0; i < 3; i++)
	{
		circle.obj[i]->Draw();
	}
}
