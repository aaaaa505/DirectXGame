#include "FireOopth.h"
#include "SafeDelete.h"

FireOopth* FireOopth::Create()
{
	// インスタンスの生成
	FireOopth* instance = new FireOopth();

	// インスタンスの初期化
	instance->Initialize();

	// インスタンスを返す
	return instance;
}

FireOopth::~FireOopth()
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
	for (int  i = 0; i < 3; i++)
	{
		safe_delete(circle.obj[i]);
	}
	safe_delete(circle.model);
	

}

void FireOopth::Initialize()
{
#pragma region 複数使用モデル
	model_Collision = Model::CreateFromOBJ("sphere");
#pragma endregion

#pragma region オープス初期化
	//オープス１・２・３モデル読み込み
	oopth.model_1 = Model::CreateFromOBJ("oopth1_F");
	oopth.model_2 = Model::CreateFromOBJ("oopth2_F");
	oopth.model_3 = Model::CreateFromOBJ("oopth3_F");

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
	oopth.damegeShakeFlag = false;
	oopth.damegeShakeTimer = 0.0f;
	oopth.attackShakeTimer = 0.2f;
	for (int i = 0; i < 3; i++)
	{
		oopth.attackOrder[i] = false;
	}
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
	circle.model = Model::CreateFromOBJ("circle_F");

	for (int i = 0; i < 3; i++)
	{
		//生成
		circle.obj[i] = Object3d::Create();
		circle.obj[i]->SetModel(circle.model);

		//初期座標
		circle.pos[0] = { -2.7f, 1.5f, 5.0f };
		circle.pos[1] = {  0.0f, 1.5f, 5.0f };
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

#pragma region バレット・パーティクル
	for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
	{
		//初期座標
		bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

		bullet.hitFlag[i] = false;
	}

	//状態変化変数初期化
	bullet.allFireFlag = false;
	bullet.delayFlag = false;
	bullet.delayTimer = 0;

	//パーティクル
	particleMan = ParticleManager::GetInstance();
#pragma endregion

#pragma region オーディオ
	audio = Audio::GetInstance();
	audio->LoadWave("shotSE.wav");
	audio->SetVolume("shotSE.wav", 0.3f);
#pragma endregion


#pragma region 外部干渉変数
	shakeFlag = false;
#pragma endregion

}

#pragma region オープス関数
void FireOopth::Reset()
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
		oopth.damegeShakeFlag = false;
		oopth.damegeShakeTimer = 0.0f;
		oopth.attackShakeTimer = 0.2f;
		for (int i = 0; i < 3; i++)
		{
			oopth.attackOrder[i] = false;
		}
		oopth.attackEndFlag = false;
		//Hp最大値
		oopth.hp = 300;
		
#pragma endregion

#pragma region サークル再起動
		for (int i = 0; i < 3; i++)
		{
			//回転角
			circle.rot = { 0.0f, 0.0f, 0.0f };
			//スケール
			circle.scale = { 0.0f, 0.0f, 1.0f };
		}
#pragma endregion

#pragma region バレット再起動
		for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
		{
			//座標
			bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

			//攻撃判定フラッグ
			bullet.hitFlag[i] = false;
		}
		//状態変化変数初期化
		bullet.allFireFlag = false;
		bullet.delayFlag = false;
		bullet.delayTimer = 0;
#pragma endregion

		oopth.resetFlag = false;
	}

}

void FireOopth::ModelChange(int turnNomber)
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

void FireOopth::StatusChange()
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

void FireOopth::AttackPreparation()
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
			circle.rot.z -= 100.0f;
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
			//弾を装填
			Loading();
			//攻撃準備状態から外れる
			oopth.attackPreparationFlag = false;
			//サークルの回転角を元に戻す
			circle.rot = { 0.0f, 0.0f, 0.0f };
		}

	}

}

void FireOopth::AttackEnd()
{
	if (oopth.attackEndFlag == true)
	{
		for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
		{
			bullet.hitFlag[i] = false;
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
			circle.rot.z += 100.0f;
		}

		//上記処理が終了したら
		else
		{
			//StatusChangeを再起動
			oopth.statusChangeFlag = true;
			oopth.changeTimer = 150;
			//AllFireFlagをOFF
			bullet.allFireFlag = false;
			//AttackEndをOFF
			oopth.attackEndFlag = false;
		}
	}
}

void FireOopth::Appearance()
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

void FireOopth::Damage(short player_TypeFlag)
{
	if (player_TypeFlag == 0 || player_TypeFlag == 1)
	{
		oopth.hp -= 3;
	}
	if (player_TypeFlag == 2)
	{
		oopth.hp -= 7;
	}
	if (player_TypeFlag == 3)
	{
		oopth.hp -= 2;
	}
}

void FireOopth::Death(int turnNomber)
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
				for (int i = 0; i < 3; i++)
				{
					//回転角
					circle.rot = { 0.0f, 0.0f, 0.0f };
					//スケール
					circle.scale = { 0.0f, 0.0f, 1.0f };
				}
				for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
				{
					//座標
					bullet.pos[i] = { 0.0f, 100.0f, 0.0f };

					//攻撃判定フラッグ
					bullet.hitFlag[i] = false;
				}
				oopth.resetFlag = true;
			}

			oopth.model_3->SetAlpha(oopth.alpha3);
		}
	}

}

void FireOopth::AttackShake()
{
	oopth.shake.x = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
	oopth.shake.y = (float)rand() / RAND_MAX * oopth.attackShakeTimer - oopth.attackShakeTimer / 2.0f;
}

void FireOopth::DamegeShake()
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

#pragma endregion

#pragma region バレット関数
void FireOopth::Loading()
{
	//起動時に各サークルの裏に弾を三個ずつ配置
	for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
	{
		if (i < 3)
		{
			bullet.pos[i] = { circle.pos[0].x, circle.pos[0].y, circle.pos[0].z + 0.6f };
		}
		else if (i < 6)
		{
			bullet.pos[i] = { circle.pos[1].x, circle.pos[1].y, circle.pos[1].z + 0.6f };
		}
		else
		{
			bullet.pos[i] = { circle.pos[2].x, circle.pos[2].y, circle.pos[2].z + 0.6f };
		}
	}
	
}

void FireOopth::FireEffect()
{
	for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
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

void FireOopth::OneLineOrderShot(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		oopth.rot.y += 200.0f;
		circle.rot.z += 200.0f;
		
		//全弾発射していないなら
		if (bullet.allFireFlag == false)
		{
			//ディレイタイマー起動
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 3 && bullet.delayTimer <= 0; i++)
		{
			if (oopth.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				oopth.attackOrder[i] = true;
				bullet.delayTimer = 50;
				break;
			}
		}

		//オーダー１
		if (oopth.attackOrder[0] == true)
		{
			bullet.pos[3].z -= 0.2f;
			if (bullet.hitFlag[3] == true)
			{
				bullet.pos[3].y = 100.0f;
			}
		}

		//オーダー２
		if (oopth.attackOrder[1] == true)
		{
			bullet.pos[0].z -= 0.2f;
			if (bullet.hitFlag[0] == true)
			{
				bullet.pos[0].y = 100.0f;
			}
		}

		//オーダー３
		if (oopth.attackOrder[2] == true)
		{
			bullet.allFireFlag = true;
			bullet.pos[6].z -= 0.2f;
			if (bullet.hitFlag[6] == true)
			{
				bullet.pos[6].y = 100.0f;
			}
			if (bullet.pos[6].z <= -8.0f)
			{
				oopth.fixFlag = true;
			}
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
		//ファイヤーエフェクト起動
		FireEffect();
		//シェイク演出
		AttackShake();
	}
	else if(oopth.attackFlag == false)
	{
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++)
		{
			oopth.attackOrder[i] = false;
		}
	}
}

void FireOopth::TwinLineOrderShot(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		oopth.rot.y += 200.0f;
		circle.rot.z += 200.0f;

		//全弾発射していないなら
		if (bullet.allFireFlag == false)
		{
			//ディレイタイマー起動
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 3 && bullet.delayTimer <= 0; i++)
		{
			if (oopth.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				oopth.attackOrder[i] = true;
				bullet.delayTimer = 50;
				break;
			}
		}

		//オーダー１
		if (oopth.attackOrder[0] == true)
		{
			bullet.pos[0].z -= 0.2f;
			bullet.pos[3].z -= 0.2f;
			if (bullet.hitFlag[0] == true)
			{
				bullet.pos[0].y = 100.0f;
			}

			if (bullet.hitFlag[3] == true)
			{
				bullet.pos[3].y = 100.0f;
			}

		}

		//オーダー２
		if (oopth.attackOrder[1] == true)
		{
			bullet.pos[4].z -= 0.2f;
			bullet.pos[6].z -= 0.2f;
			if (bullet.hitFlag[4] == true)
			{
				bullet.pos[4].y = 100.0f;
			}

			if (bullet.hitFlag[6] == true)
			{
				bullet.pos[6].y = 100.0f;
			}
		}

		//オーダー３
		if (oopth.attackOrder[2] == true)
		{
			bullet.allFireFlag = true;
			bullet.pos[1].z -= 0.2f;
			bullet.pos[5].z -= 0.2f;
			if (bullet.hitFlag[1] == true)
			{
				bullet.pos[1].y = 100.0f;
			}

			if (bullet.hitFlag[5] == true)
			{
				bullet.pos[5].y = 100.0f;
			}
			if (bullet.pos[1].z <= -8.0f)
			{
				oopth.fixFlag = true;
			}
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
		//ファイヤーエフェクト起動
		FireEffect();
		//シェイク演出
		AttackShake();
	}
	else if (oopth.attackFlag == false)
	{
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++)
		{
			oopth.attackOrder[i] = false;
		}
	}
}

void FireOopth::AllLineOrderShot(bool field_completionFlag)
{
	if (oopth.attackFlag == true && field_completionFlag == true)
	{
		//回転演出
		oopth.rot.y += 200.0f;
		circle.rot.z += 200.0f;

		//全弾発射していないなら
		if (bullet.allFireFlag == false)
		{
			//ディレイタイマー起動
			bullet.delayTimer -= 1;
		}
		for (int i = 0; i < 3 && bullet.delayTimer <= 0; i++)
		{

			if (oopth.attackOrder[i] == false)
			{
				audio->StopWave("shotSE.wav");
				audio->PlayWave("shotSE.wav", false);
				oopth.attackOrder[i] = true;
				bullet.delayTimer = 100;
				break;
			}
		}

		for (int i = 0; i < FIREOOPTHBULLET_MAX; i++)
		{
			if (bullet.hitFlag[i] == true)
			{
				bullet.pos[i].y = 100.0f;
			}
		}

		//オーダー１
		if (oopth.attackOrder[0] == true)
		{
			bullet.pos[0].z -= 0.2f;
			bullet.pos[3].z -= 0.2f;
			bullet.pos[6].z -= 0.13f;
		}

		//オーダー２
		if (oopth.attackOrder[1] == true)
		{
			bullet.pos[1].z -= 0.1f;
			bullet.pos[7].z -= 0.1f;
		}

		//オーダー３
		if (oopth.attackOrder[2] == true)
		{
			bullet.allFireFlag = true;
			bullet.pos[2].z -= 0.13f;
			bullet.pos[4].z -= 0.2f;
			bullet.pos[8].z -= 0.2f;
			if (bullet.pos[2].z <= -8.0f)
			{
				oopth.fixFlag = true;
			}
		}

		if (oopth.fixFlag == true)
		{
			oopth.fixTimer -= 1;
			if (oopth.fixTimer <= 0)
			{
				oopth.fixTimer = 40;
				oopth.fixFlag = false;
				oopth.attackFlag = false;
				oopth.attackEndFlag = true;
			}
		}
		//ファイヤーエフェクト起動
		FireEffect();
		//シェイク演出
		AttackShake();
	}
	else if (oopth.attackFlag == false)
	{
		oopth.rot = { 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++)
		{
			oopth.attackOrder[i] = false;
		}
	}
}
#pragma endregion

void FireOopth::Update(int turnNomber, bool field_completionFlag, bool scene_completionFlag)
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
				OneLineOrderShot(field_completionFlag);
			}
			if (turnNomber == 2)
			{
				//攻撃パターン2
				TwinLineOrderShot(field_completionFlag);
			}
			if (turnNomber == 3)
			{
				AllLineOrderShot(field_completionFlag);
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
	for (int i = 0; i < 3; i++)
	{
		circle.obj[i]->SetScale(circle.scale);
		circle.obj[i]->SetRotation(circle.rot);
		circle.obj[i]->Update();
	}

}

void FireOopth::Draw()
{
	oopth.obj->Draw();
	for (int i = 0; i < 3; i++)
	{
		circle.obj[i]->Draw();
	}
}
