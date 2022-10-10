#include "Uranus.h"
#include "SafeDelete.h"
#define _USE_MATH_DEFINES
#include <math.h>

Uranus* Uranus::Create()
{
	// インスタンスの生成
	Uranus* instance = new Uranus();

	// インスタンスの初期化
	instance->Initialize();

	// インスタンスを返す
	return instance;
}

Uranus::~Uranus()
{
	//本体
	safe_delete(boss.obj);
	safe_delete(boss.model);
	//コリジョン
	safe_delete(boss.collision);
	safe_delete(model_Collision);
	//サークル
	for (int i = 0; i < 9; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
	//ウィンド
	for (int i = 0; i < 10; i++)
	{
		safe_delete(wind.obj[i]);
	}
	safe_delete(wind.model);

}

void Uranus::Initialize()
{
#pragma region 複数使用モデル
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region ボス初期化

	//オープス１・２・３モデル読み込み
	boss.model = Model::CreateFromOBJ("uranus");

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
	circle.model = Model::CreateFromOBJ("circle_L");

	for (int i = 0; i < 9; i++)
	{
		//生成
		circle.obj[i] = Object3d::Create();
		circle.obj[i]->SetModel(circle.model);

		//初期座標
		//前列
		circle.pos[0] = { -2.7f, 0.1f, +2.7f };
		circle.pos[1] = { 0.0f, 0.1f, +2.7f };
		circle.pos[2] = { +2.7f, 0.1f, +2.7f };
		//中央列
		circle.pos[3] = { -2.7f, 0.1f, 0.0f };
		circle.pos[4] = { 0.0f, 0.1f, 0.0f };
		circle.pos[5] = { +2.7f, 0.1f, 0.0f };
		//後列
		circle.pos[6] = { -2.7f, 0.1f, -2.7f };
		circle.pos[7] = { 0.0f, 0.1f, -2.7f };
		circle.pos[8] = { +2.7f, 0.1f, -2.7f };
		circle.obj[i]->SetPosition(circle.pos[i]);

		//初期回転角
		circle.rot = { 0.0f, 0.0f, 0.0f };

		//初期スケール
		circle.scale = { 0.0f, 1.0f, 0.0f };
	}
#pragma endregion

#pragma region ウィンド関数
	//モデル
	wind.model = Model::CreateFromOBJ("wind");
	for (int i = 0; i < 9; i++)
	{
		//生成
		wind.obj[i] = Object3d::Create();
		wind.obj[i]->SetModel(wind.model);

		//初期座標
		wind.pos[i] = { circle.pos[i].x, circle.pos[i].y - 1.0f, circle.pos[i].z };
		wind.obj[i]->SetPosition(wind.pos[i]);

		//初期回転角
		wind.rot = { 0.0f, 0.0f, 0.0f };

		//サイズ
		wind.scale[i] = { 0.0f, 0.0f, 0.0f };

		//状態変化変数
		wind.outBreakFlag[i] = false;
	}
	//初期アルファ値
	wind.alpha = 0.8f;
	wind.outBreakTimer = 10;
	wind.delayTimer = 0;
	wind.delayFlag = false;

#pragma region テンペスト用
	//生成
	wind.obj[9] = Object3d::Create();
	wind.obj[9]->SetModel(wind.model);

	//初期座標
	wind.pos[9] = {1.25f, 1.5f, 7.0f};
	wind.obj[9]->SetPosition(wind.pos[9]);

	//初期回転角
	wind.tempestRot = { -90.0f, 0.0f, 0.0f };

	//サイズ
	wind.scale[9] = { 0.0f, 0.0f, 0.0f };

	//状態変化変数
	wind.outBreakFlag[9] = false;
#pragma endregion

	//状態変化変数初期化
	wind.outBreakTimer = 10;
	wind.delayTimer = 5;

	//パーティクル
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region チャージ初期化
	charge.angle = 0.0f;
	charge.radius = 5.0f;
	for (int i = 0; i < TEMPESTCHARGEBULLET_MAX; i++)
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

#pragma region ウラヌス関数
void Uranus::Appearance()
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

void Uranus::ModeChange()
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

void Uranus::StatusChange()
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

void Uranus::AttackPreparation()
{
	//攻撃準備フラッグがONなら
	if (boss.attackPreparationFlag == true)
	{
		ModeChange();
		//設定値までサークルのスケールを拡大、同時に回転する
		if (circle.scale.x <= 0.02f && circle.scale.z <= 0.02f)
		{
			//拡大
			circle.scale.x += 0.0005f;
			circle.scale.z += 0.0005f;
			//回転
			circle.rot.y -= 100.0f;
		}
		//座標を変更
		if (boss.pos.z <= 20.0f)
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
			circle.rot = { 0.0f, 0.0f, 0.0f };
		}
	}
}

void Uranus::AttackEnd()
{
	if (boss.attackEndFlag == true)
	{
		audio->StopWave("windSE.wav");
		for (int i = 0; i < 10; i++)
		{
			wind.outBreakTimer = 10;
			wind.outBreakFlag[i] = false;
			wind.scale[i] = { 0.0f, 0.0f, 0.0f };
		}

		if (boss.pos.z >= 11.0f)
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
			//AttackEndをOFF
			boss.attackEndFlag = false;
		}

		//設定値までサークルのスケールを縮小、同時に回転する
		if (circle.scale.x >= 0.0f && circle.scale.z >= 0.0f)
		{
			//拡大
			circle.scale.x -= 0.0005f;
			circle.scale.z -= 0.0005f;

			//回転
			circle.rot.y += 100.0f;

			for (int i = 0; i < 9; i++)
			{
				circle.pos[i].y = 0.1f;
			}
		}

		if (boss.modeChangeFlag == true)
		{
			if (boss.rot.y > 0.0f)
			{
				boss.rot.y -= 4.0f;
			}
			else
			{
				boss.rot.y = 0.0f;
			}
		}
	}
}

void Uranus::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 3)
	{
		boss.hp -= 3;
	}
	if (player_TypeFlag == 1)
	{
		boss.hp -= 7;
	}
	if (player_TypeFlag == 2)
	{
		boss.hp -= 2;
	}
	if (boss.hp < 200)
	{
		boss.modeChangeFlag = true;
	}
}

void Uranus::Death(int turnNomber)
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

void Uranus::AttackShake()
{
	boss.shake.x = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
	boss.shake.y = (float)rand() / RAND_MAX * boss.attackShakeTimer - boss.attackShakeTimer / 2.0f;
}

void Uranus::DamegeShake()
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
#pragma endregion

#pragma region ウィンド関数
void Uranus::WindMove()
{
	for (int i = 0; i < 9; i++)
	{
		if (wind.outBreakFlag[i] == true)
		{
			wind.alpha = 0.8f;
			if (wind.scale[i].x <= 1.5f)
			{
				wind.scale[i].x += 0.05f;
				wind.scale[i].z += 0.05f;
			}

			if (wind.scale[i].y <= 1.5f)
			{
				wind.scale[i].y += 0.1f;
			}
		}
		else
		{
			if (wind.alpha >= 0.0f)
			{
				wind.alpha -= 0.05f;
			}
			else
			{
				wind.scale[i] = { 0.0f, 0.0f, 0.0f };
			}
		}
	}
}

void Uranus::TempestMove()
{
	if (wind.outBreakFlag[9] == true)
	{
		TempestEfect();
		wind.tempestRot.y += 20.0f;
		wind.alpha = 1.0f;
		if (wind.scale[9].x <= 2.0f)
		{
			wind.scale[9].x += 0.1f;
			wind.scale[9].z += 0.1f;
		}

		if (wind.scale[9].y <= 3.0f)
		{
			wind.scale[9].y += 0.1f;
		}
	}
	else
	{
		if (wind.alpha >= 0.0f)
		{
			wind.alpha -= 0.01f;
		}
		else
		{
			wind.scale[9] = { 0.0f, 0.0f, 0.0f };
		}
	}

	if (boss.attackOrder == 13)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[6] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[9] = true;
		}
	}
}

void Uranus::WindEffect()
{
	for (int i = 0; i < 10; i++)
	{
		if (wind.outBreakFlag[i] == true)
		{
			const XMFLOAT3 rnd_pos = { 3.0f, 5.0f, 1.0f };
			particle.pos.x = wind.pos[i].x + (float)rand() / RAND_MAX * rnd_pos.x - rnd_pos.x / 2.0f;
			particle.pos.y = wind.pos[i].y + 3.0f + (float)rand() / RAND_MAX * rnd_pos.y - rnd_pos.y / 2.0f;
			particle.pos.z = wind.pos[i].z + (float)rand() / RAND_MAX * rnd_pos.z - rnd_pos.z / 2.0f;

			const float rnd_vel = 0.01f;
			particle.vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			particle.vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			particle.vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			const float rnd_acc = 0.001f;
			particle.acc.y = (float)rand() / RAND_MAX * rnd_acc;

			particleMan->Add(20, particle.pos, particle.vel, particle.acc, 0.5f, 0.1f, { 0.5f, 2.0f, 0.0f, 1.0f });
		}

	}
}

void Uranus::ChargeEffect(bool field_completionFlag)
{
	if (boss.modeChangeFlag == true && field_completionFlag == true && boss.attackOrder != 13)
	{
		audio->PlayWave("chargeSE.wav", false);
		boss.pos.x = 1.25f;

		if (charge.radius >= 0.0f)
		{
			charge.radius -= 0.3f;
			charge.angle += 0.7f;
		}
		else
		{
			charge.radius = 15.0f;
		}

		for (int i = 0; i < TEMPESTCHARGEBULLET_MAX; i++)
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

			particleMan->Add(18, particle.pos, particle.vel, particle.acc, 1.8f, 0.1f, { 0.1f, 2.0f, 0.0f, 1.0f });
		}
	}
}

void Uranus::TempestEfect()
{
	if (wind.outBreakFlag[9] == true)
	{
		audio->StopWave("chargeSE.wav");
		const XMFLOAT3 rnd_pos = { 3.0f, 1.0f, 5.0f };
		particle.pos.x = wind.pos[9].x + (float)rand() / RAND_MAX * rnd_pos.x - rnd_pos.x / 2.0f;
		particle.pos.y = wind.pos[9].y + (float)rand() / RAND_MAX * rnd_pos.y - rnd_pos.y / 2.0f;
		particle.pos.z = wind.pos[9].z - 10.0f + (float)rand() / RAND_MAX * rnd_pos.z - rnd_pos.z / 2.0f;

		const float rnd_vel = 0.01f;
		particle.vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		particle.vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		particle.vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		const float rnd_acc = 0.001f;
		particle.acc.z = (float)rand() / RAND_MAX * rnd_acc;

		particleMan->Add(20, particle.pos, particle.vel, particle.acc, 1.0f, 0.1f, { 0.5f, 2.0f, 0.0f, 1.0f });
	}

	
}

void Uranus::CrossWind()
{
	//オーダー１
	if (boss.attackOrder == 1)
	{
		wind.outBreakFlag[0] = true;
		wind.outBreakFlag[1] = true;
		wind.outBreakFlag[2] = true;
		wind.outBreakFlag[5] = true;
		wind.outBreakFlag[8] = true;
	}

	//オーダー２
	if (boss.attackOrder == 2)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[2] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
		}

	}

	//オーダー３
	if (boss.attackOrder == 3)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[6] = true;
			wind.outBreakFlag[7] = true;
			wind.outBreakFlag[8] = true;
		}
	}

	//オーダー４
	if (boss.attackOrder == 4)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[6] = false;
			wind.outBreakFlag[7] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
		}
	}
}

void Uranus::SlantingWind()
{
	//オーダー１
	if (boss.attackOrder == 5)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[2] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[6] = true;
		}
	}

	//オーダー２
	if (boss.attackOrder == 6)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[2] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[6] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
		}

	}

	//オーダー３
	if (boss.attackOrder == 7)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[8] = true;
		}
	}

	//オーダー４
	if (boss.attackOrder == 8)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
		}
	}
}

void Uranus::BlockWind()
{
	//オーダー１
	if (boss.attackOrder == 9)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[2] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
		}
	}

	//オーダー２
	if (boss.attackOrder == 10)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[0] = false;
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[2] = false;
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[6] = true;
			wind.outBreakFlag[7] = true;
			wind.outBreakFlag[8] = true;
		}

	}

	//オーダー３
	if (boss.attackOrder == 11)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[3] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[6] = false;
			wind.outBreakFlag[7] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[2] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[5] = true;
			wind.outBreakFlag[7] = true;
			wind.outBreakFlag[8] = true;
		}
	}

	//オーダー４
	if (boss.attackOrder == 12)
	{
		if (wind.delayFlag == true)
		{
			wind.outBreakFlag[1] = false;
			wind.outBreakFlag[2] = false;
			wind.outBreakFlag[4] = false;
			wind.outBreakFlag[5] = false;
			wind.outBreakFlag[7] = false;
			wind.outBreakFlag[8] = false;
		}
		if (wind.delayTimer <= 0)
		{
			wind.delayFlag = false;
			wind.outBreakFlag[0] = true;
			wind.outBreakFlag[1] = true;
			wind.outBreakFlag[3] = true;
			wind.outBreakFlag[4] = true;
			wind.outBreakFlag[6] = true;
			wind.outBreakFlag[7] = true;
		}
	}
}

void Uranus::TempestWind1(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		audio->PlayWave("windSE.wav", true);
		//回転演出
		circle.rot.y += 200.0f;
		wind.rot.y += 20.0f;


		if (wind.delayFlag == false)
		{
			wind.outBreakTimer -= 1;
		}
		else
		{
			audio->StopWave("windSE.wav");
			wind.delayTimer -= 1;
		}
		if (wind.outBreakTimer <= 0)
		{
			if (boss.attackOrder <= 11)
			{
				boss.attackOrder += 1;
				wind.outBreakTimer = 150;
				wind.delayFlag = true;
				wind.delayTimer = 30;
				if (boss.attackOrder == 1)
				{
					wind.delayFlag = false;
				}
			}
			else
			{
				//攻撃可能フラッグOFF
				boss.attackFlag = false;
				//攻撃終了フラッグON
				boss.attackEndFlag = true;
			}
		}

		CrossWind();
		SlantingWind();
		BlockWind();

		

		//ウィンド挙動
		WindMove();
		//バレットエフェクト起動
		WindEffect();
		//シェイク演出
		AttackShake();

	}
	else if (boss.attackFlag == false)
	{
		boss.shake = { 0.0f, 0.0f };
		boss.attackOrder = 0;
	}
}

void Uranus::TempestWind2(bool field_completionFlag)
{
	if (boss.attackFlag == true && field_completionFlag == true)
	{
		audio->PlayWave("windSE.wav", true);
		//回転演出
		circle.rot.y += 200.0f;
		wind.rot.y += 20.0f;


		if (wind.delayFlag == false)
		{
			wind.outBreakTimer -= 1;
		}
		else
		{
			audio->StopWave("windSE.wav");
			wind.delayTimer -= 1;
		}
		if (wind.outBreakTimer <= 0)
		{
			if (boss.attackOrder <= 12)
			{
				boss.attackOrder += 1;
				wind.outBreakTimer = 150;
				wind.delayFlag = true;
				wind.delayTimer = 30;
				if (boss.attackOrder == 1)
				{
					wind.delayFlag = false;
				}
			}
			else
			{
				//攻撃可能フラッグOFF
				boss.attackFlag = false;
				//攻撃終了フラッグON
				boss.attackEndFlag = true;
			}
		}

		CrossWind();
		SlantingWind();
		BlockWind();

		//ウィンド挙動
		WindMove();
		//テンペスト挙動
		TempestMove();
		//バレットエフェクト起動
		WindEffect();
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


void Uranus::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
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
				TempestWind1(field_completionFlag);
			}
			else
			{
				//攻撃パターン２
				TempestWind2(field_completionFlag);
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
	wind.model->SetAlpha(wind.alpha);
	for (int i = 0; i < 9; i++)
	{
		circle.obj[i]->SetPosition(circle.pos[i]);
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();

		wind.obj[i]->SetScale(wind.scale[i]);
		wind.obj[i]->SetRotation(wind.rot);
		wind.obj[i]->Update();

	}
	wind.obj[9]->SetScale(wind.scale[9]);
	wind.obj[9]->SetRotation(wind.tempestRot);
	wind.obj[9]->Update();
}

void Uranus::Draw()
{
	boss.obj->Draw();
	for (int i = 0; i < 9; i++)
	{
		circle.obj[i]->Draw();

		wind.obj[i]->Draw();
	}

	wind.obj[9]->Draw();
}
