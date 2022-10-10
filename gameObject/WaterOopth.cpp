#include "WaterOopth.h"
#include "SafeDelete.h"

WaterOopth* WaterOopth::Create()
{
	// インスタンスの生成
	WaterOopth* instance = new WaterOopth();

	// インスタンスの初期化
	instance->Initialize();

	// インスタンスを返す
	return instance;
}

WaterOopth::~WaterOopth()
{
	//オープス
	safe_delete(oopth.obj);
	safe_delete(oopth.model_1);
	safe_delete(oopth.model_2);
	safe_delete(oopth.model_3);
	//コリジョン
	safe_delete(oopth.collision);
	safe_delete(model_Collision);
	//サークル
	for (int i = 0; i < 3; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
	//バレット
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
	{
		safe_delete(bullet.obj[i]);
	}
	safe_delete(bullet.model);
}


void WaterOopth::Initialize()
{
#pragma region 複数使用モデル
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region オープス初期化
	//オープス１・２・３モデル読み込み
	oopth.model_1 = Model::CreateFromOBJ("oopth1_W");
	oopth.model_2 = Model::CreateFromOBJ("oopth2_W");
	oopth.model_3 = Model::CreateFromOBJ("oopth3_W");

	//生成
	oopth.obj = Object3d::Create();

	//初期座標
	oopth.pos = { 0.0f, 50.0f, 5.0f };
	oopth.obj->SetPosition(oopth.pos);

	//初期速度
	oopth.vel = { 0.0f, 0.0f, 0.0f };

	//初期加速度
	oopth.acc = 0.01f;

	//初期回転角
	oopth.rot = { 0.0f, 0.0f, 0.0f };

	//サイズ
	oopth.obj->SetScale({ 0.3f, 0.3f, 0.3f });

	//初期透明度
	oopth.alpha1 = 1.0f;
	oopth.alpha2 = 1.0f;
	oopth.alpha3 = 1.0f;

	//初期シェイク値
	oopth.shake = { 0.0f, 0.0f };

	//状態変化変数初期化
	oopth.startFlag = false;
	oopth.attackPreparationFlag = false;
	oopth.attackFlag = false;
	oopth.statusChangeFlag = false;
	oopth.changeTimer = 150;
	oopth.attackShakeTimer = 0.2f;
	oopth.damegeShakeFlag = false;
	oopth.damegeShakeTimer = 0.0f;
	oopth.attackOrder = 0;
	oopth.attackEndFlag = false;
	oopth.fixFlag = false;
	oopth.fixTimer = 20;

	//Hp最大値
	oopth.hp = 300;

	//コリジョン
	oopth.collision = Object3d::Create();
	oopth.collision->SetModel(model_Collision);
	oopth.collision->SetPosition({ oopth.pos.x, oopth.pos.y + 0.5f, oopth.pos.z });
	oopth.radius = 1.0f;
	oopth.collision->SetScale({ oopth.radius, oopth.radius, oopth.radius });

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
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
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

#pragma region オーディオ
	audio = Audio::GetInstance();
#pragma endregion

#pragma region 外部干渉用変数
	bool shakeFlag = false;
#pragma endregion
}

#pragma region オープス関数
void WaterOopth::Reset()
{
	if (oopth.resetFlag == true)
	{
#pragma region オープス再起動
		//座標
		oopth.pos = { 0.0f, 50.0f, 5.0f };
		//速度
		oopth.vel = { 0.0f, 0.0f, 0.0f };
		//回転角
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		//シェイク値
		oopth.shake = { 0.0f, 0.0f };
		//状態変化変数初期化
		oopth.startFlag = false;
		oopth.attackPreparationFlag = false;
		oopth.attackFlag = false;
		oopth.statusChangeFlag = false;
		oopth.changeTimer = 150;
		oopth.attackShakeTimer = 0.2f;
		oopth.damegeShakeFlag = false;
		oopth.damegeShakeTimer = 0.0f;
		oopth.attackOrder = 0;
		oopth.attackEndFlag = false;
		//Hp最大値
		oopth.hp = 300;

#pragma endregion

#pragma region サークル再起動
		for (int i = 0; i < 6; i++)
		{
			//回転角
			circle.rot = { 0.0f, 90.0f, 0.0f };
			//スケール
			circle.scale = { 0.0f, 0.0f, 1.0f };
		}
#pragma endregion

#pragma region バレット再起動
		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			//座標
			bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

			//攻撃判定フラッグ
			bullet.hitFlag[i] = false;

			//発射フラッグ
			bullet.fireFlag[i] = false;
		}
		//状態変化変数初期化
		bullet.delayTimer = 0;
#pragma endregion

		oopth.resetFlag = false;
	}
}

void WaterOopth::Appearance()
{
	if (oopth.pos.y >= 2.0f)
	{
		oopth.pos.y -= oopth.vel.y;
		oopth.vel.y += oopth.acc;
	}
	else
	{
		audio->PlayWave("arrivalSE1.wav", false);
		shakeFlag = true;
		oopth.startFlag = true;
		oopth.statusChangeFlag = true;

	}
}

void WaterOopth::ModelChange(int turnNomber)
{
	//オープス1
	if (turnNomber == 1)
	{
		oopth.obj->SetModel(oopth.model_1);
	}

	//オープス2
	if (turnNomber == 2)
	{
		oopth.obj->SetModel(oopth.model_2);
	}

	//オープス3
	if (turnNomber == 3)
	{
		oopth.obj->SetModel(oopth.model_3);
	}
}

void WaterOopth::StatusChange()
{
	//非攻撃状態の時
	if (oopth.statusChangeFlag == true)
	{
		oopth.changeTimer -= 1;
		if (oopth.changeTimer <= 0)
		{
			oopth.attackPreparationFlag = true;
			oopth.statusChangeFlag = false;
		}
	}
}

void WaterOopth::AttackPreparation()
{
	//攻撃準備フラッグがONなら
	if (oopth.attackPreparationFlag == true)
	{
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
		if (oopth.pos.y <= 5.0f)
		{
			oopth.pos.y += 0.1f;
		}
		//上記の処理が終了
		else
		{
			//攻撃状態へ移行
			oopth.attackFlag = true;
			//攻撃準備状態から外れる
			oopth.attackPreparationFlag = false;
			//サークルの回転角を元に戻す
			circle.rot = { 0.0f, 90.0f, 0.0f };
		}

	}
}

void WaterOopth::AttackShake()
{
	oopth.shake.x = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
	oopth.shake.y = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
}

void WaterOopth::AttackEnd()
{
	if (oopth.attackEndFlag == true)
	{
		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			bullet.hitFlag[i] = false;
			bullet.fireFlag[i] = false;
		}

		if (oopth.pos.y >= 1.0f)
		{
			oopth.pos.y -= 0.1f;
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

		//上記処理が終了したら
		else
		{
			//StatusChangeを再起動
			oopth.statusChangeFlag = true;
			oopth.changeTimer = 150;
			//AttackEndをOFF
			oopth.attackEndFlag = false;
		}
	}
}

void WaterOopth::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 2)
	{
		oopth.hp -= 3;
	}
	if (player_TypeFlag == 3)
	{
		oopth.hp -= 7;
	}
	if (player_TypeFlag == 1)
	{
		oopth.hp -= 2;
	}
}

void WaterOopth::DamegeShake()
{
	if (oopth.damegeShakeFlag == true)
	{
		oopth.damegeShakeTimer = 0.2f;
		oopth.damegeShakeFlag = false;
	}

	if (oopth.damegeShakeTimer > 0)
	{
		oopth.shake.x = (float)rand() / RAND_MAX * oopth.damegeShakeTimer - oopth.damegeShakeTimer / 2.0f;
		oopth.shake.y = (float)rand() / RAND_MAX * oopth.damegeShakeTimer - oopth.damegeShakeTimer / 2.0f;
		oopth.damegeShakeTimer -= 0.005f;
	}
}

void WaterOopth::Death(int turnNomber)
{
	if (oopth.hp <= 10)
	{
		if (turnNomber == 1)
		{
			if (oopth.alpha1 >= 0.0f)
			{
				oopth.alpha1 -= 0.05f;
			}
			//死亡と同時に初期化
			else
			{
				oopth.resetFlag = true;
			}

			oopth.model_1->SetAlpha(oopth.alpha1);
		}

		if (turnNomber == 2)
		{
			if (oopth.alpha2 >= 0.0f)
			{
				oopth.alpha2 -= 0.05f;
			}
			//死亡と同時に初期化
			else
			{
				oopth.resetFlag = true;
			}

			oopth.model_2->SetAlpha(oopth.alpha2);
		}

		if (turnNomber == 3)
		{
			if (oopth.alpha3 >= 0.0f)
			{
				oopth.alpha3 -= 0.05f;
			}
			//死亡と同時に初期化
			else
			{
				oopth.pos = { 0.0f, 50.0f, 5.0f };
				oopth.startFlag = false;
				for (int i = 0; i < 6; i++)
				{
					//回転角
					circle.rot = { 0.0f, 90.0f, 0.0f };
					//スケール
					circle.scale = { 0.0f, 0.0f, 1.0f };
				}
				for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
				{
					//座標
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

					//攻撃判定フラッグ
					bullet.hitFlag[i] = false;

					//発射フラッグ
					bullet.fireFlag[i] = false;
				}
				oopth.resetFlag = true;
			}

			oopth.model_3->SetAlpha(oopth.alpha3);
		}
	}
}
#pragma endregion

#pragma region バレット関数
void WaterOopth::BulletLoading_1()
{
	bullet.delayTimer -= 1;
	for (int i = 0; bullet.delayTimer <= 0 && i < WATEROOPTHBULLET_MAX; i++)
	{
		//オーダー１
		if (i < 20 && bullet.fireFlag[i] == false && oopth.attackOrder == 0)
		{
			bullet.pos[i].x = circle.pos[0].x;
			bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[19] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー２
		else if (i < 40 && bullet.fireFlag[i] == false && oopth.attackOrder == 1)
		{
			bullet.pos[i].x = circle.pos[1].x;
			bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[39] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー３
		else if (i < 60 && bullet.fireFlag[i] == false && oopth.attackOrder == 2)
		{
			bullet.pos[i].x = circle.pos[2].x;
			bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[59] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー４
		else if (i < 80 && bullet.fireFlag[i] == false && oopth.attackOrder == 3)
		{
			bullet.pos[i].x = circle.pos[3].x;
			bullet.pos[i].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[79] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー５
		else if (i < 100 && bullet.fireFlag[i] == false && oopth.attackOrder == 4)
		{
			bullet.pos[i].x = circle.pos[4].x;
			bullet.pos[i].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[99] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー６
		else if (i < 120 && bullet.fireFlag[i] == false && oopth.attackOrder == 5)
		{
			bullet.pos[i].x = circle.pos[5].x;
			bullet.pos[i].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void WaterOopth::BulletLoading_2()
{
	bullet.delayTimer -= 1;
	for (int i = 0; bullet.delayTimer <= 0 && i < WATEROOPTHBULLET_MAX; i++)
	{
		//オーダー１
		if (i < 20 && bullet.fireFlag[i] == false && oopth.attackOrder == 0)
		{
			bullet.pos[i].x = circle.pos[4].x;
			bullet.pos[i].y = (circle.pos[4].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[4].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[19] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー２
		else if (i < 40 && bullet.fireFlag[i] == false && oopth.attackOrder == 1)
		{
			bullet.pos[i].x = circle.pos[2].x;
			bullet.pos[i].y = (circle.pos[2].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[2].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[39] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー３
		else if (i < 60 && bullet.fireFlag[i] == false && oopth.attackOrder == 2)
		{
			bullet.pos[i].x = circle.pos[3].x;
			bullet.pos[i].y = (circle.pos[3].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[3].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[59] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー４
		else if (i < 80 && bullet.fireFlag[i] == false && oopth.attackOrder == 3)
		{
			bullet.pos[i].x = circle.pos[1].x;
			bullet.pos[i].y = (circle.pos[1].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[1].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[79] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー５
		else if (i < 100 && bullet.fireFlag[i] == false && oopth.attackOrder == 4)
		{
			bullet.pos[i].x = circle.pos[0].x;
			bullet.pos[i].y = (circle.pos[0].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[0].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = 90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			if (bullet.fireFlag[99] == true)
			{
				oopth.attackOrder += 1;
				bullet.delayTimer = 18;
			}
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
		//オーダー６
		else if (i < 120 && bullet.fireFlag[i] == false && oopth.attackOrder == 5)
		{
			bullet.pos[i].x = circle.pos[5].x;
			bullet.pos[i].y = (circle.pos[5].y + (float)rand() / RAND_MAX) - 0.5f;
			bullet.pos[i].z = (circle.pos[5].z + (float)rand() / RAND_MAX) - 0.5f;
			bullet.rot[i].z = -90.0f;
			bullet.fireFlag[i] = true;
			bullet.delayTimer = 3;
			audio->StopWave("blastSE.wav");
			audio->PlayWave("blastSE.wav", false);
			break;
		}
	}
}

void WaterOopth::BulletLoading_3()
{
	bullet.delayTimer -= 1;
	for (int i = 0; bullet.delayTimer <= 0 && i < WATEROOPTHBULLET_MAX; i++)
	{
#pragma region オーダー１・オーダー４
		if (oopth.attackOrder == 0 || oopth.attackOrder == 3)
		{
			if (i < 20 && bullet.fireFlag[i] == false)
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
				if (oopth.attackOrder == 3)
				{
					FireFlagReset80_119();
				}
				bullet.delayTimer = 3;
				if (bullet.fireFlag[39] == true)
				{
					oopth.attackOrder += 1;
					bullet.delayTimer = 18;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}
#pragma endregion

#pragma region オーダー２・オーダー５
		if (oopth.attackOrder == 1 || oopth.attackOrder == 4)
		{
			//オーダー２
			if (i >= 40 && i < 60 && bullet.fireFlag[i] == false)
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
				if (bullet.fireFlag[79] == true)
				{
					oopth.attackOrder += 1;
					bullet.delayTimer = 18;
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}
#pragma endregion

#pragma region オーダー３・オーダー６
		if (oopth.attackOrder == 2 || oopth.attackOrder == 5)
		{
			//オーダー３
			if (i >= 80 && i < 100 && bullet.fireFlag[i] == false)
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
				if (oopth.attackOrder == 2)
				{
					if (bullet.fireFlag[119] == true)
					{
						oopth.attackOrder += 1;
						bullet.delayTimer = 18;
					}
					FireFlagReset0_39();
					FireFlagReset40_79();
				}
				audio->StopWave("blastSE.wav");
				audio->PlayWave("blastSE.wav", false);
				break;
			}
		}

#pragma endregion
	}
}

void WaterOopth::BulletMove_1(int i)
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
	else
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

void WaterOopth::BulletMove_2(int i)
{
	if (i >= 0 && i < 20 || i >= 40 && i < 60 || i >= 100 && i < 120)
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
	else if (i >= 20 && i < 40 || i >= 60 && i < 80 || i >= 80 && i < 100)
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

void WaterOopth::BulletMove_3(int i)
{
	if (i >= 0 && i < 20)
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
	else if (i >= 40 && i < 60)
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
	else if (i >= 80 && i < 100)
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

void WaterOopth::FireFlagReset0_39()
{
	for (int i = 0; i < 40; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void WaterOopth::FireFlagReset40_79()
{
	for (int i = 40; i < 80; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void WaterOopth::FireFlagReset80_119()
{
	for (int i = 80; i < 120; i++)
	{
		bullet.fireFlag[i] = false;
		bullet.hitFlag[i] = false;
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
	}
}

void WaterOopth::BulletEffect()
{
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
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

void WaterOopth::OneSideOrderBlast(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		oopth.rot.y += 200.0f;
		circle.rot.x += 200.0f;

		//装填
		BulletLoading_1();

		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			//発射フラッグがONなら
			if (bullet.fireFlag[i] == true)
			{
				//起動
				BulletMove_1(i);

				//あたり判定
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//最後の弾が発射され、画面外に出たら
		if (bullet.fireFlag[119] == true && bullet.pos[119].y == 100.0f)
		{
			oopth.fixFlag = true;
		}

		if (oopth.fixFlag == true)
		{
			oopth.fixTimer -= 1;
			if (oopth.fixTimer <= 0)
			{
				oopth.fixTimer = 20;
				oopth.fixFlag = false;
				oopth.attackFlag = false;
				oopth.attackEndFlag = true;
			}
		}

		//バレットエフェクト起動
		BulletEffect();
		//シェイク演出
		AttackShake();

	}
	else if (oopth.attackFlag == false)
	{
		oopth.shake = { 0.0f, 0.0f };
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		oopth.attackOrder = 0;
	}
}

void WaterOopth::TwinSideOrderBlast(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		oopth.rot.y += 200.0f;
		circle.rot.x += 200.0f;

		//装填
		BulletLoading_2();

		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			//発射フラッグがONなら
			if (bullet.fireFlag[i] == true)
			{
				//起動
				BulletMove_2(i);

				//あたり判定
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//最後の弾が発射され、画面外に出たら
		if (bullet.fireFlag[119] == true && bullet.pos[119].y == 100.0f)
		{
			oopth.fixFlag = true;
		}

		if (oopth.fixFlag == true)
		{
			oopth.fixTimer -= 1;
			if (oopth.fixTimer <= 0)
			{
				oopth.fixTimer = 20;
				oopth.fixFlag = false;
				oopth.attackFlag = false;
				oopth.attackEndFlag = true;
			}
		}

		//バレットエフェクト起動
		BulletEffect();
		//シェイク演出
		AttackShake();
	}
	else if (oopth.attackFlag == false)
	{
		oopth.shake = { 0.0f, 0.0f };
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		oopth.attackOrder = 0;
	}
}

void WaterOopth::TwinLineSideBlast(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		oopth.rot.y += 200.0f;
		circle.rot.x += 200.0f;

		//装填
		BulletLoading_3();

		for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
		{
			//発射フラッグがONなら
			if (bullet.fireFlag[i] == true)
			{
				//起動
				BulletMove_3(i);

				//あたり判定
				if (bullet.hitFlag[i] == true)
				{
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };
				}
			}
		}

		//最後の弾が発射され、画面外に出たら
		if (oopth.attackOrder == 5 && bullet.fireFlag[119] == true && bullet.pos[119].y == 100.0f)
		{
			oopth.fixFlag = true;
		}

		if (oopth.fixFlag == true)
		{
			oopth.fixTimer -= 1;
			if (oopth.fixTimer <= 0)
			{
				oopth.fixTimer = 20;
				oopth.fixFlag = false;
				oopth.attackFlag = false;
				oopth.attackEndFlag = true;
			}
		}

		//バレットエフェクト起動
		BulletEffect();
		//シェイク演出
		AttackShake();
	}
	else if (oopth.attackFlag == false)
	{
		oopth.shake = { 0.0f, 0.0f };
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		oopth.attackOrder = 0;
	}
}
#pragma endregion


void WaterOopth::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
{
	//オープスを再起動
	Reset();
	if (turnNomber != 0 && turnNomber != 4)
	{
		//ステージによってモデルを切り替える
		ModelChange(turnNomber);
		if (oopth.startFlag == false && scene_completionFlag == true)
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
			if (turnNomber == 1)
			{
				//攻撃パターン1
				OneSideOrderBlast(field_completionFlag);
			}
			if (turnNomber == 2)
			{
				//攻撃パターン2
				TwinSideOrderBlast(field_completionFlag);
			}
			if (turnNomber == 3)
			{
				//攻撃パターン3
				TwinLineSideBlast(field_completionFlag);
			}
			//攻撃終了時
			AttackEnd();
			//被弾時のシェイク演出
			DamegeShake();
			//死亡時
			Death(turnNomber);
		}
	}

	oopth.obj->SetPosition({ oopth.pos.x + oopth.shake.x, oopth.pos.y + oopth.shake.y, oopth.pos.z });
	oopth.obj->SetRotation(oopth.rot);
	oopth.obj->Update();
	oopth.collision->SetPosition({ oopth.pos.x, oopth.pos.y + 0.5f, oopth.pos.z });
	oopth.collision->Update();
	for (int i = 0; i < 6; i++)
	{
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();
	}
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
	{
		bullet.obj[i]->SetPosition(bullet.pos[i]);
		bullet.obj[i]->SetRotation(bullet.rot[i]);
		bullet.obj[i]->Update();
	}
}

void WaterOopth::Draw()
{
	oopth.obj->Draw();
	for (int i = 0; i < 6; i++)
	{
		circle.obj[i]->Draw();
	}
	for (int i = 0; i < WATEROOPTHBULLET_MAX; i++)
	{
		if (bullet.fireFlag[i] == true)
		{
			bullet.obj[i]->Draw();
		}

	}
}
