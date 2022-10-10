#include "PlayScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"
#include "LoadSprite.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
	safe_delete(lightGroup);
	safe_delete(player);
	safe_delete(stage);
	
	safe_delete(waterField);
	safe_delete(fireField);
	safe_delete(leafField);

	safe_delete(waterOopth);
	safe_delete(fireOopth);
	safe_delete(leafOopth);
	
	safe_delete(pluton);
	safe_delete(poseidon);
	safe_delete(uranus);

	safe_delete(mana);
	safe_delete(sceneWhile);
	safe_delete(playerUI);
	safe_delete(enemyUI);
}

void PlayScene::Initialize(DirectXCommon* dxCommon, SceneDataBuffer* sceneDateBuffer)
{
	this->input = Input::GetInstance();
	this->audio = Audio::GetInstance();

	// nullptrチェック
	assert(dxCommon);
	assert(sceneDateBuffer);

	this->dxCommon = dxCommon;
	this->sceneNo = sceneDateBuffer->sceneNo;
	this->stageNo = sceneDateBuffer->stageNo;
	this->sceneDateBuffer = sceneDateBuffer;

#pragma region 基盤オブジェクト
	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);

	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);
	particleMan->SetDeleteFlag(false);

#pragma endregion

#pragma region 各オブジェクト＆スプライト生成

	//クラスオブジェクト生成
	player = Player::Create(input);

	stage = new Stage();
	stage->Initialize(stageNo);

	mana = Mana::Create();

#pragma region オーディオ読み込み
	//クリアBGM
	audio->LoadWave("clearBGM.wav");
	//オーバーBGM
	audio->LoadWave("overBGM.wav");
	//エンド時SE
	audio->LoadWave("finishSE.wav");
	audio->SetVolume("finishSE.wav", 0.1);
	//ダメージSE１
	audio->LoadWave("damegeSE1.wav");
	audio->SetVolume("damegeSE1.wav", 0.1f);
	//ダメージSE２
	audio->LoadWave("damegeSE2.wav");
	audio->SetVolume("damegeSE2.wav", 0.1f);
	//ダメージSE３
	audio->LoadWave("damegeSE3.wav");
	audio->SetVolume("damegeSE3.wav", 0.1f);
	//回避SE
	audio->LoadWave("evasionSE.wav");
	audio->SetVolume("evasionSE.wav", 0.1f);
	//変身SE(F)
	audio->LoadWave("changeFireSE.wav");
	audio->SetVolume("changeFireSE.wav", 0.1f);
	//変身SE(W)
	audio->LoadWave("changeWaterSE.wav");
	audio->SetVolume("changeWaterSE.wav", 0.1f);
	//変身SE(L)
	audio->LoadWave("changeLeafSE.wav");
	audio->SetVolume("changeLeafSE.wav", 0.1f);
	//攻撃範囲表示SE
	audio->LoadWave("scopeSE.wav");
	audio->SetVolume("scopeSE.wav", 0.1f);
	//オープス出現SE
	audio->LoadWave("arrivalSE1.wav");
	audio->SetVolume("arrivalSE1.wav", 0.1f);
	//ボス出現SE
	audio->LoadWave("arrivalSE2.wav");
	audio->SetVolume("arrivalSE2.wav", 0.1f);
	//チャージSE
	audio->LoadWave("chargeSE.wav");
	audio->SetVolume("chargeSE.wav", 0.1f);
	
#pragma endregion

	switch (stageNo)
	{
	case WATER_ELEMENT:
		waterField = WaterField::Create();
		waterOopth = WaterOopth::Create();
		poseidon = Poseidon::Create();
		//ポセイドンBGM
		audio->LoadWave("poseidonBGM.wav");
		//ポセイドンステージ攻撃SE
		audio->LoadWave("blastSE.wav");
		audio->SetVolume("blastSE.wav", 0.01f);
		break;
	case FIRE_ELEMENT:
		fireField = FireField::Create();
		fireOopth = FireOopth::Create();
		pluton = Pluton::Create();
		//プルトンBGM
		audio->LoadWave("plutonBGM.wav");
		//プルトンステージ攻撃SE
		audio->LoadWave("shotSE.wav");
		audio->SetVolume("shotSE.wav", 0.3f);
		break;
	case LEAF_ELEMENT:
		leafField = LeafField::Create();
		leafOopth = LeafOopth::Create();
		uranus = Uranus::Create();
		//ウラヌスBGM
		audio->LoadWave("uranusBGM.wav");
		//ウラヌスステージ攻撃SE
		audio->LoadWave("windSE.wav");
		audio->SetVolume("windSE.wav", 0.1f);
		break;
	}

	//クラススプライト生成
	sceneWhile = SceneWhile::Create();
	playerUI = PlayerUI::Create();
	enemyUI = EnemyUI::Create();

#pragma endregion



#pragma region カメラ設定
	cameraPos = { 0.0f, 2.7f, 0.0f };
	cameraShake = { 0.0f, 0.0f };
	camera->SetTarget(cameraPos);
	camera->SetEye({ 0.0f, 6.0f, -12.0f });
	shakeTimer = 0.0f;

#pragma endregion

	stageNumber = 0;
}

void PlayScene::Update()
{
	switch (stageNo)
	{
	case WATER_ELEMENT:
		PoseidonStageUpdata();
		break;
	case FIRE_ELEMENT:
		PlutonStageUpdata();
		break;
	case LEAF_ELEMENT:
		UranusStageUpdata();;
		break;
	}
	SceneDateBufferUpdate();
}

void PlayScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	Object3d::PreDraw(cmdList);
	switch (stageNo)
	{
	case WATER_ELEMENT:
		PoseidonStageDraw(cmdList);
		break;
	case FIRE_ELEMENT:
		PlutonStageDraw(cmdList);
		break;
	case LEAF_ELEMENT:
		UranusStageDraw(cmdList);
		break;
	}
	Object3d::PostDraw();

}

void PlayScene::PlutonStageUpdata()
{
	switch (stageNumber)
	{
	case 0:
#pragma region ステージ開始時演出
		if (sceneWhile->StageName_GetCompletion1Flag() == false)
		{
			//ステージ名表示
			sceneWhile->StageName_SetTransUpFlag(true);

			volume[1] += 0.001f;
		}
		else
		{
			//ステージ名及び挿絵の透明度を下げる
			if (sceneWhile->RoundCount_GetMoveFlag() == false)
			{
				//TransDown起動
				sceneWhile->Illustration_SetTransDownFlag(true);
				sceneWhile->StageName_SetTransDownFlag(true);

				//TransDownが終了したら
				if (sceneWhile->Illustration_GetCompletion2Flag() == true &&
					sceneWhile->StageName_GetCompletion2Flag() == true)
				{
					//Moveを起動
					sceneWhile->RoundCount_SetMoveFlag(true);
				}
			}
		}

		//Move終了後
		if (sceneWhile->RoundCount_GetCompletionFlag() == true)
		{
			stageNumber = 1;  
		}


#pragma endregion
		break;
	case 1:
#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (fireOopth->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.3f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			fireOopth->SetShakeFlag(false);
		}

#pragma endregion

#pragma region 攻撃範囲表示
		if (fireOopth->GetAttackPreparationFlag() == true)
		{
			fireField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (fireOopth->GetAttackEndFlag() == true)
		{
			fireField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (fireOopth->GetAttackFlag() == false && fireOopth->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					fireOopth->GetCollsionOopth(), fireOopth->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					fireOopth->Damage(player->GetTypeFlag());
					fireOopth->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyBullet
			for (int i = 0; i < 9; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (Collision::SphereSphere(fireOopth->GetCollisionBullet(i), 1.8f,
					player->GetPos(), 0.3f))
				{
					if (fireOopth->GetHitFlag(i) != true)
					{
						audio->StopWave("damegeSE2.wav");
						audio->PlayWave("damegeSE2.wav", false);
						//衝突した弾を画面外に
						fireOopth->SetHitFlag(true, i);
						player->Damege(stageNo);
						player->SetDamegeShakeTimer(0.5f);
					}
				}
			}
		}

		if (fireOopth->GetResetFlag() == true)
		{
			sceneWhile->RoundCount_SetResetFlag(true);
			fireField->SetResetFlag(true);
			stageNumber = 2;
		}
#pragma endregion
		break;
	case 2:
#pragma region ステージ切り替え演出
		if (fireOopth->GetStartFlag() == false)
		{
			sceneWhile->RoundCount_SetMoveFlag(true);
		}

#pragma endregion

#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (fireOopth->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.3f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			fireOopth->SetShakeFlag(false);
		}
#pragma endregion

#pragma region 攻撃範囲表示
		if (fireOopth->GetAttackPreparationFlag() == true)
		{
			fireField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (fireOopth->GetAttackEndFlag() == true)
		{
			fireField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (fireOopth->GetAttackFlag() == false && fireOopth->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					fireOopth->GetCollsionOopth(), fireOopth->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					fireOopth->Damage(player->GetTypeFlag());
					fireOopth->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyBullet
			for (int i = 0; i < 9; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (Collision::SphereSphere(fireOopth->GetCollisionBullet(i), 1.8f,
					player->GetPos(), 0.3f))
				{
					if (fireOopth->GetHitFlag(i) != true)
					{
						audio->StopWave("damegeSE2.wav");
						audio->PlayWave("damegeSE2.wav", false);
						//衝突した弾を画面外に
						fireOopth->SetHitFlag(true, i);
						player->Damege(stageNo);
						player->SetDamegeShakeTimer(0.5f);
					}
				}
			}
		}

		if (fireOopth->GetResetFlag() == true)
		{
			sceneWhile->RoundCount_SetResetFlag(true);
			fireField->SetResetFlag(true);
			stageNumber = 3;
		}
#pragma endregion

		break;
	case 3:
#pragma region ステージ切り替え演出
		if (fireOopth->GetStartFlag() == false)
		{
			sceneWhile->RoundCount_SetMoveFlag(true);
		}

#pragma endregion

#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (fireOopth->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.3f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			fireOopth->SetShakeFlag(false);
		}
#pragma endregion

#pragma region 攻撃範囲表示
		if (fireOopth->GetAttackPreparationFlag() == true)
		{
			fireField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (fireOopth->GetAttackEndFlag() == true)
		{
			fireField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (fireOopth->GetAttackFlag() == false && fireOopth->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					fireOopth->GetCollsionOopth(), fireOopth->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					fireOopth->Damage(player->GetTypeFlag());
					fireOopth->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyBullet
			for (int i = 0; i < 9; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (Collision::SphereSphere(fireOopth->GetCollisionBullet(i), 1.8f,
					player->GetPos(), 0.3f))
				{
					if (fireOopth->GetHitFlag(i) != true)
					{
						audio->StopWave("damegeSE2.wav");
						audio->PlayWave("damegeSE2.wav", false);
						//衝突した弾を画面外に
						fireOopth->SetHitFlag(true, i);
						player->Damege(stageNo);
						player->SetDamegeShakeTimer(0.5f);
					}
				}
			}
		}

		if (fireOopth->GetResetFlag() == true)
		{
			sceneWhile->RoundCount_SetResetFlag(true);
			fireField->SetResetFlag(true);
			stageNumber = 4;
		}
#pragma endregion

	case 4:
#pragma region ステージ切り替え演出
		if (pluton->GetStartFlag() == false)
		{
			sceneWhile->RoundCount_SetMoveFlag(true);
		}

#pragma endregion

#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (pluton->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.5f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			pluton->SetShakeFlag(false);
		}
#pragma endregion

#pragma region 攻撃範囲表示
		if (pluton->GetAttackPreparationFlag() == true)
		{
			fireField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (pluton->GetAttackEndFlag() == true)
		{
			fireField->SetResetFlag(true);

		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (pluton->GetAttackFlag() == false && pluton->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					pluton->GetCollsionPluton(), pluton->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					pluton->Damage(player->GetTypeFlag());
					pluton->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyBullet
			for (int i = 0; i < 17; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (Collision::SphereSphere(pluton->GetCollisionBullet(i), 1.8f,
					player->GetPos(), 0.3f))
				{
					if (pluton->GetHitFlag(i) != true)
					{
						audio->StopWave("damegeSE2.wav");
						audio->PlayWave("damegeSE2.wav", false);
						//衝突した弾を画面外に
						pluton->SetHitFlag(true, i);
						player->Damege(stageNo);
						player->SetDamegeShakeTimer(0.5f);
					}
				}
			}

			//Player To Flame
			if (Collision::SphereSphere(pluton->GetCollisionFlame(), 3.6f,
				player->GetPos(), 0.3f))
			{
				if (pluton->GetFlameHitFlag() != true)
				{
					audio->StopWave("damegeSE3.wav");
					audio->PlayWave("damegeSE3.wav", false);
					pluton->SetFlameHitFlag(true);
					camera->SetShakeTimer(0.7f);
					player->SpecialAttackDamege(stageNo);
					player->SetDamegeShakeTimer(0.5f);
				}
			}
		}
#pragma endregion
		break;
	}

#pragma region ステージエンド演出
	if (finishFlag == false)
	{
		if (player->GetDeathFlag() == true || pluton->GetDeathFlag() == true)
		{
			volume[1] = 0.0f;
			playerUI->SetStageEndFlag(true);
			enemyUI->SetStageEndFlag(true);
			audio->PlayWave("finishSE.wav", false);
			finishFlag = true;
		}
	}

	if (playerUI->GetCompletionFlag() == true)
	{
		if (endVolume <= 0.1f)
		{
			endVolume += 0.001f;
		}
		else
		{
			endVolume = 0.1f;
		}

		if (player->GetDeathFlag() == true)
		{
			sceneWhile->EndScene_SetOverFlag(true);
		}
		if (pluton->GetDeathFlag() == true)
		{
			sceneDateBuffer->sceneClearFlag[0] = true;
			sceneWhile->EndScene_SetClearFlag(true);
		}
		sceneWhile->EndScene_SetTransUpFlag(true);

	}

	if (sceneWhile->EndScene_GetCompletionFlag1() == true && endVolume == 0.1f)
	{

		if (input->GetJoyButton(JoyAButton) || input->GetJoyButton(JoyBButton) ||
			input->GetJoyButton(JoyYButton) || input->GetJoyButton(JoyXButton))
		{
			sceneWhile->Illustration_SetTransUpFlag(true);
		}
	}

	if (sceneWhile->Illustration_GetCompletion1Flag() == true)
	{
		endVolume = 0.0f;
		audio->StopWave("clearBGM.wav");
		audio->StopWave("overBGM.wav");
		audio->StopWave("plutonBGM.wav");
		sceneNo = SceneNo::SELECT_SCENE;
	}


#pragma endregion

#pragma region ライティング設定
	lightGroup->DefaultLightSetting();
#pragma endregion

#pragma region オーディオ
	audio->SetVolume("plutonBGM.wav", volume[1]);
	if (audio->GetVolume("plutonBGM.wav") != 0.0f)
	{
		audio->PlayWave("plutonBGM.wav", false);
	}
	if (endVolume != 0.0f)
	{
		if (sceneWhile->EndScene_GetClearFlag() == true)
		{
			audio->SetVolume("clearBGM.wav", endVolume);
			audio->PlayWave("clearBGM.wav", true);
		}
		if (sceneWhile->EndScene_GetOverFlag() == true)
		{
			audio->SetVolume("overBGM.wav", endVolume);
			audio->PlayWave("overBGM.wav", true);
		}
	}
#pragma endregion

#pragma region 各更新処理
	lightGroup->Update();
	camera->ShakeCamera(cameraPos);
	camera->Update();
	particleMan->Update();
	sceneWhile->Update(stageNumber, stageNo);
	playerUI->Update(player->GetHp(), player->GetChangeTimer(), player->GetTypeFlag());
	enemyUI->Update(fireOopth->GetHp(), pluton->GetHp(), stageNumber, stageNo);
	if (player->GetDeathFlag() == false && pluton->GetDeathFlag() == false)
	{
		player->Update(fireOopth->GetAttackFlag(), pluton->GetAttackFlag(),
		fireOopth->GetStartFlag(), pluton->GetStartFlag(), stageNumber);
		fireField->Update(stageNumber, pluton->GetModeChangeFlag());
		if (stageNumber == 1 || stageNumber == 2 || stageNumber == 3) { fireOopth->Update(stageNumber, fireField->GetCompletionFlag(), sceneWhile->RoundCount_GetCompletionFlag()); }
		if (stageNumber == 4) { pluton->Update(stageNumber, fireField->GetCompletionFlag(), sceneWhile->RoundCount_GetCompletionFlag()); }
		mana->Update(fireOopth->GetStartFlag(), pluton->GetStartFlag());
		stage->Update();
	}
#pragma endregion
}

void PlayScene::PoseidonStageUpdata()
{
	switch (stageNumber)
	{
	case 0:
#pragma region ステージ開始時演出
		if (sceneWhile->StageName_GetCompletion1Flag() == false)
		{
			//ステージ名表示
			sceneWhile->StageName_SetTransUpFlag(true);

			volume[0] += 0.001f;
		}
		else
		{
			//ステージ名及び挿絵の透明度を下げる
			if (sceneWhile->RoundCount_GetMoveFlag() == false)
			{
				//TransDown起動
				sceneWhile->Illustration_SetTransDownFlag(true);
				sceneWhile->StageName_SetTransDownFlag(true);

				//TransDownが終了したら
				if (sceneWhile->Illustration_GetCompletion2Flag() == true &&
					sceneWhile->StageName_GetCompletion2Flag() == true)
				{
					//Moveを起動
					sceneWhile->RoundCount_SetMoveFlag(true);
				}
			}
		}

		//Move終了後
		if (sceneWhile->RoundCount_GetCompletionFlag() == true)
		{
			stageNumber = 1;
		}


#pragma endregion
		break;

	case 1:
#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (waterOopth->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.3f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			waterOopth->SetShakeFlag(false);
		}


#pragma endregion

#pragma region 攻撃範囲表示
		if (waterOopth->GetAttackPreparationFlag() == true)
		{
			waterField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (waterOopth->GetAttackEndFlag() == true)
		{
			waterField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (waterOopth->GetAttackFlag() == false && waterOopth->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					waterOopth->GetCollsionOopth(), waterOopth->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					waterOopth->Damage(player->GetTypeFlag());
					waterOopth->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyBullet
			for (int i = 0; i < 120; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (Collision::SphereSphere(waterOopth->GetCollisionBullet(i), 1.25f,
					player->GetPos(), 0.3f))
				{
					if (waterOopth->GetHitFlag(i) != true)
					{
						audio->StopWave("damegeSE2.wav");
						audio->PlayWave("damegeSE2.wav", false);
						//衝突した弾を画面外に
						waterOopth->SetHitFlag(true, i);
						player->Damege(stageNo);
						player->SetDamegeShakeTimer(0.5f);
					}
				}
			}
		}


		if (waterOopth->GetResetFlag() == true)
		{
			sceneWhile->RoundCount_SetResetFlag(true);
			waterField->SetResetFlag(true);
			stageNumber = 2;
		}
#pragma endregion
		break;

	case 2:
#pragma region ステージ切り替え演出
		if (waterOopth->GetStartFlag() == false)
		{
			sceneWhile->RoundCount_SetMoveFlag(true);
		}

#pragma endregion

#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (waterOopth->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.3f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			waterOopth->SetShakeFlag(false);
		}


#pragma endregion

#pragma region 攻撃範囲表示
		if (waterOopth->GetAttackPreparationFlag() == true)
		{
			waterField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (waterOopth->GetAttackEndFlag() == true)
		{
			waterField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (waterOopth->GetAttackFlag() == false && waterOopth->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					waterOopth->GetCollsionOopth(), waterOopth->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					waterOopth->Damage(player->GetTypeFlag());
					waterOopth->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyBullet
			for (int i = 0; i < 120; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (Collision::SphereSphere(waterOopth->GetCollisionBullet(i), 1.25f,
					player->GetPos(), 0.3f))
				{
					if (waterOopth->GetHitFlag(i) != true)
					{
						audio->StopWave("damegeSE2.wav");
						audio->PlayWave("damegeSE2.wav", false);
						//衝突した弾を画面外に
						waterOopth->SetHitFlag(true, i);
						player->Damege(stageNo);
						player->SetDamegeShakeTimer(0.5f);
					}
				}
			}
		}

		if (waterOopth->GetResetFlag() == true)
		{
			sceneWhile->RoundCount_SetResetFlag(true);
			waterField->SetResetFlag(true);
			stageNumber = 3;
		}
#pragma endregion
		break;

	case 3:
#pragma region ステージ切り替え演出
		if (waterOopth->GetStartFlag() == false)
		{
			sceneWhile->RoundCount_SetMoveFlag(true);
		}

#pragma endregion

#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (waterOopth->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.3f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			waterOopth->SetShakeFlag(false);
		}

#pragma endregion

#pragma region 攻撃範囲表示
		if (waterOopth->GetAttackPreparationFlag() == true)
		{
			waterField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (waterOopth->GetAttackEndFlag() == true)
		{
			waterField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (waterOopth->GetAttackFlag() == false && waterOopth->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					waterOopth->GetCollsionOopth(), waterOopth->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					waterOopth->Damage(player->GetTypeFlag());
					waterOopth->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyBullet
			for (int i = 0; i < 120; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (Collision::SphereSphere(waterOopth->GetCollisionBullet(i), 1.25f,
					player->GetPos(), 0.3f))
				{
					if (waterOopth->GetHitFlag(i) != true)
					{
						audio->StopWave("damegeSE2.wav");
						audio->PlayWave("damegeSE2.wav", false);
						//衝突した弾を画面外に
						waterOopth->SetHitFlag(true, i);
						player->Damege(stageNo);
						player->SetDamegeShakeTimer(0.5f);
					}
				}
			}
		}

		if (waterOopth->GetResetFlag() == true)
		{
			sceneWhile->RoundCount_SetResetFlag(true);
			waterField->SetResetFlag(true);
			stageNumber = 4;
		}
#pragma endregion
		break;

	case 4:
#pragma region ステージ切り替え演出
		if (poseidon->GetStartFlag() == false)
		{
			sceneWhile->RoundCount_SetMoveFlag(true);
		}

#pragma endregion

#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (poseidon->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.5f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			poseidon->SetShakeFlag(false);
		}


#pragma endregion

#pragma region 攻撃範囲表示
		if (poseidon->GetAttackPreparationFlag() == true)
		{
			waterField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (poseidon->GetAttackEndFlag() == true)
		{
			waterField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (poseidon->GetAttackFlag() == false && poseidon->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					poseidon->GetCollsionPoseidon(), poseidon->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					poseidon->Damage(player->GetTypeFlag());
					poseidon->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyBullet
			for (int i = 0; i < 360; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (Collision::SphereSphere(poseidon->GetCollisionBullet(i), 1.25f,
					player->GetPos(), 0.3f))
				{
					if (poseidon->GetHitFlag(i) != true)
					{
						audio->StopWave("damegeSE2.wav");
						audio->PlayWave("damegeSE2.wav", false);
						//衝突した弾を画面外に
						poseidon->SetHitFlag(true, i);
						player->Damege(stageNo);
						player->SetDamegeShakeTimer(0.5f);
					}
				}
			}
			if (poseidon->GetModeChangeFlag() == true && poseidon->GetFireFlag() == true)
			{
				for (int i = 360; i < 440; i++)
				{
					//エネミー弾とプレイヤーのあたり判定
					//一度当たった弾でないのなら
					if (Collision::SphereSphere(poseidon->GetCollisionBullet(i), 1.25f,
						player->GetPos(), 0.3f))
					{
						if (poseidon->GetHitFlag(i) != true)
						{
							audio->StopWave("damegeSE2.wav");
							audio->PlayWave("damegeSE2.wav", false);
							//衝突した弾を画面外に
							poseidon->SetHitFlag(true, i);
							camera->SetShakeTimer(0.7f);
							player->SpecialAttackDamege(stageNo);
							player->SetDamegeShakeTimer(0.5f);
						}
					}
				}
			}
		}
#pragma endregion
		break;
	}

#pragma region ステージエンド演出
	if (finishFlag == false)
	{
		if (player->GetDeathFlag() == true || poseidon->GetDeathFlag() == true)
		{
			volume[0] = 0.0f;
			playerUI->SetStageEndFlag(true);
			enemyUI->SetStageEndFlag(true);
			audio->PlayWave("finishSE.wav", false);
			finishFlag = true;
		}
	}

	if (playerUI->GetCompletionFlag() == true)
	{
		if (endVolume <= 0.1f)
		{
			endVolume += 0.001f;
		}
		else
		{
			endVolume = 0.1f;
		}

		if (player->GetDeathFlag() == true)
		{
			sceneWhile->EndScene_SetOverFlag(true);
		}
		if (poseidon->GetDeathFlag() == true)
		{
			sceneDateBuffer->sceneClearFlag[1] = true;
			sceneWhile->EndScene_SetClearFlag(true);
		}
		sceneWhile->EndScene_SetTransUpFlag(true);
	}

	if (sceneWhile->EndScene_GetCompletionFlag1() == true && endVolume == 0.1f)
	{
		if (input->GetJoyButton(JoyAButton) || input->GetJoyButton(JoyBButton) ||
			input->GetJoyButton(JoyYButton) || input->GetJoyButton(JoyXButton))
		{
			sceneWhile->Illustration_SetTransUpFlag(true);
		}
	}

	if (sceneWhile->Illustration_GetCompletion1Flag() == true)
	{
		endVolume = 0.0f;
		audio->StopWave("clearBGM.wav");
		audio->StopWave("overBGM.wav");
		audio->StopWave("poseidonBGM.wav");
		sceneNo = SceneNo::SELECT_SCENE;
	}

#pragma endregion

#pragma region ライティング設定
	lightGroup->DefaultLightSetting();
#pragma endregion

#pragma region オーディオ
	audio->SetVolume("poseidonBGM.wav", volume[0]);
	if (audio->GetVolume("poseidonBGM.wav") != 0.0f)
	{
		audio->PlayWave("poseidonBGM.wav", false);
	}
	if (endVolume != 0.0f)
	{
		if (sceneWhile->EndScene_GetClearFlag() == true)
		{
			audio->SetVolume("clearBGM.wav", endVolume);
			audio->PlayWave("clearBGM.wav", true);
		}
		if (sceneWhile->EndScene_GetOverFlag() == true)
		{
			audio->SetVolume("overBGM.wav", endVolume);
			audio->PlayWave("overBGM.wav", true);
		}
	}
#pragma endregion

#pragma region 各種更新処理
	lightGroup->Update();
	camera->ShakeCamera(cameraPos);
	camera->Update();
	particleMan->Update();
	sceneWhile->Update(stageNumber, stageNo);
	playerUI->Update(player->GetHp(), player->GetChangeTimer(), player->GetTypeFlag());
	enemyUI->Update(waterOopth->GetHp(), poseidon->GetHp(), stageNumber, stageNo);
	if (player->GetDeathFlag() == false && poseidon->GetDeathFlag() == false)
	{
		stage->Update();
		player->Update(waterOopth->GetAttackFlag(), poseidon->GetAttackFlag(),
		waterOopth->GetStartFlag(), poseidon->GetStartFlag(), stageNumber);
		waterField->Update(stageNumber, poseidon->GetModeChangeFlag());
		if (stageNumber == 1 || stageNumber == 2 || stageNumber == 3)
		{
			waterOopth->Update(stageNumber, waterField->GetCompletionFlag(), sceneWhile->RoundCount_GetCompletionFlag());
		}
		if (stageNumber == 4)
		{
			poseidon->Update(stageNumber, waterField->GetCompletionFlag(), sceneWhile->RoundCount_GetCompletionFlag());
		}
		mana->Update(waterOopth->GetStartFlag(), poseidon->GetStartFlag());
	}
#pragma endregion
}

void PlayScene::UranusStageUpdata()
{
	switch (stageNumber)
	{
	case 0:
#pragma region ステージ開始時演出
		if (sceneWhile->StageName_GetCompletion1Flag() == false)
		{
			//ステージ名表示
			sceneWhile->StageName_SetTransUpFlag(true);

			volume[2] += 0.001f;
		}
		else
		{
			//ステージ名及び挿絵の透明度を下げる
			if (sceneWhile->RoundCount_GetMoveFlag() == false)
			{
				//TransDown起動
				sceneWhile->Illustration_SetTransDownFlag(true);
				sceneWhile->StageName_SetTransDownFlag(true);

				//TransDownが終了したら
				if (sceneWhile->Illustration_GetCompletion2Flag() == true &&
					sceneWhile->StageName_GetCompletion2Flag() == true)
				{
					//Moveを起動
					sceneWhile->RoundCount_SetMoveFlag(true);
				}
			}
		}

		//Move終了後
		if (sceneWhile->RoundCount_GetCompletionFlag() == true)
		{
			stageNumber = 1;
		}


#pragma endregion
		break;

	case 1:
#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (leafOopth->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.3f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			leafOopth->SetShakeFlag(false);
		}
#pragma endregion

#pragma region 攻撃範囲表示
		if (leafOopth->GetAttackPreparationFlag() == true)
		{
			leafField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (leafOopth->GetAttackEndFlag() == true)
		{
			leafField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (leafOopth->GetAttackFlag() == false && leafOopth->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					leafOopth->GetCollsionOopth(), leafOopth->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					leafOopth->Damage(player->GetTypeFlag());
					leafOopth->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyWind
			for (int i = 0; i < 9; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (leafOopth->GetOutBreakFlag(i) == true && Collision::SphereSphere(leafOopth->GetCollisionWind(i), 1.0f,
					player->GetPos(), 0.3f))
				{
					audio->PlayWave("damegeSE2.wav", false);
					player->Damege(stageNo);
					player->SetDamegeShakeTimer(0.5f);
				}
			}
		}

		if (leafOopth->GetResetFlag() == true)
		{
			sceneWhile->RoundCount_SetResetFlag(true);
			leafField->SetResetFlag(true);
			stageNumber = 2;
		}
#pragma endregion
		break;

	case 2:
#pragma region ステージ切り替え演出
		if (leafOopth->GetStartFlag() == false)
		{
			sceneWhile->RoundCount_SetMoveFlag(true);
		}

#pragma endregion

#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (leafOopth->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.3f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			leafOopth->SetShakeFlag(false);
		}
#pragma endregion

#pragma region 攻撃範囲表示
		if (leafOopth->GetAttackPreparationFlag() == true)
		{
			leafField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (leafOopth->GetAttackEndFlag() == true)
		{
			leafField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (leafOopth->GetAttackFlag() == false && leafOopth->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					leafOopth->GetCollsionOopth(), leafOopth->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					leafOopth->Damage(player->GetTypeFlag());
					leafOopth->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyWind
			for (int i = 0; i < 9; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (leafOopth->GetOutBreakFlag(i) == true && Collision::SphereSphere(leafOopth->GetCollisionWind(i), 1.0f,
					player->GetPos(), 0.3f))
				{
					audio->PlayWave("damegeSE2.wav", false);
					player->Damege(stageNo);
					player->SetDamegeShakeTimer(0.5f);
				}
			}
		}

		if (leafOopth->GetResetFlag() == true)
		{
			sceneWhile->RoundCount_SetResetFlag(true);
			leafField->SetResetFlag(true);
			stageNumber = 3;
		}
#pragma endregion
		break;

	case 3:
#pragma region ステージ切り替え演出
		if (leafOopth->GetStartFlag() == false)
		{
			sceneWhile->RoundCount_SetMoveFlag(true);
		}

#pragma endregion

#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (leafOopth->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.3f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			leafOopth->SetShakeFlag(false);
		}
#pragma endregion

#pragma region 攻撃範囲表示
		if (leafOopth->GetAttackPreparationFlag() == true)
		{
			leafField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (leafOopth->GetAttackEndFlag() == true)
		{
			leafField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (leafOopth->GetAttackFlag() == false && leafOopth->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 50; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					leafOopth->GetCollsionOopth(), leafOopth->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					leafOopth->Damage(player->GetTypeFlag());
					leafOopth->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyWind
			for (int i = 0; i < 9; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (leafOopth->GetOutBreakFlag(i) == true && Collision::SphereSphere(leafOopth->GetCollisionWind(i), 1.0f,
					player->GetPos(), 0.3f))
				{
					audio->PlayWave("damegeSE2.wav", false);
					player->Damege(stageNo);
					player->SetDamegeShakeTimer(0.5f);
				}
			}
		}

		if (leafOopth->GetResetFlag() == true)
		{
			sceneWhile->RoundCount_SetResetFlag(true);
			leafField->SetResetFlag(true);
			stageNumber = 4;
		}
#pragma endregion
		break;

	case 4:
#pragma region ステージ切り替え演出
		if (uranus->GetStartFlag() == false)
		{
			sceneWhile->RoundCount_SetMoveFlag(true);
		}

#pragma endregion

#pragma region 出現演出
		//シェイク演出フラッグがONなら
		if (uranus->GetShakeFlag() == true)
		{
			camera->SetShakeTimer(0.5f);
			//シェイクさせるのは一度だけなので即座にOFFにする
			uranus->SetShakeFlag(false);
		}
#pragma endregion

#pragma region 攻撃範囲表示
		if (uranus->GetAttackPreparationFlag() == true)
		{
			leafField->SetShowRangeFlag(true);
		}
#pragma endregion

#pragma region 攻撃終了時処理
		if (uranus->GetAttackEndFlag() == true)
		{
			leafField->SetResetFlag(true);
		}
#pragma endregion

#pragma region あたり判定
		//Player To Mana
		if (mana->GetMoveFlag() == true)
		{
			if (mana->GetHitFlag() == false && mana->GetCompletionFlag() == false &&
				Collision::SphereSphere(player->GetPos(), 0.3f, mana->GetPos(), 1.0f))
			{
				if (mana->GetTypeFlag() == WATER_ELEMENT)
				{
					player->SetTypeFlag(WATER_TYPE);
				}

				if (mana->GetTypeFlag() == FIRE_ELEMENT)
				{
					player->SetTypeFlag(FIRE_TYPE);
				}

				if (mana->GetTypeFlag() == LEAF_ELEMENT)
				{
					player->SetTypeFlag(LEAF_TYPE);
				}

				mana->SetHitFlag(true);
				player->SetChangeFlag(true);
			}
		}

		if (uranus->GetAttackFlag() == false && uranus->GetAttackPreparationFlag() == false)
		{
			//Enemy To PlayerBullet
			for (int i = 0; i < 9; i++)
			{
				if (Collision::SphereSphere(player->GetCollisionBullet(i), 0.3f,
					uranus->GetCollsionUranus(), uranus->GetSphereRadius())
					&& player->GetHitFlag(i) != true)
				{
					audio->StopWave("damegeSE1.wav");
					audio->PlayWave("damegeSE1.wav", false);
					//衝突した弾のあたり判定をなくす
					player->SetHitFlag(true, i);
					uranus->Damage(player->GetTypeFlag());
					uranus->SetDamegeShakeFlag(true);
				}
			}
		}
		else
		{
			//Player To EnemyWind
			for (int i = 0; i < 9; i++)
			{
				//エネミー弾とプレイヤーのあたり判定
				//一度当たった弾でないのなら
				if (uranus->GetOutBreakFlag(i) == true && Collision::SphereSphere(uranus->GetCollisionWind(i), 1.0f,
					player->GetPos(), 0.3f))
				{
					audio->PlayWave("damegeSE2.wav", false);
					player->Damege(stageNo);
					player->SetDamegeShakeTimer(0.5f);
				}
			}

			if (uranus->GetOutBreakFlag(9) == true)
			{
				if (player->GetPos().x > -2.8f)
				{
					audio->PlayWave("damegeSE2.wav", false);
					camera->SetShakeTimer(0.7f);
					player->SpecialAttackDamege(stageNo);
					player->SetDamegeShakeTimer(0.5f);
				}

			}
		}
#pragma endregion
		break;
	}

#pragma region ステージエンド演出
	if (finishFlag == false)
	{
		if (player->GetDeathFlag() == true || uranus->GetDeathFlag() == true)
		{
			volume[2] = 0.0f;
			audio->SetVolume("damegeSE2.wav", 0.0f);
			audio->SetVolume("windSE.wav", 0.0f);
			playerUI->SetStageEndFlag(true);
			enemyUI->SetStageEndFlag(true);
			audio->PlayWave("finishSE.wav", false);
			finishFlag = true;
		}
	}

	if (playerUI->GetCompletionFlag() == true)
	{
		if (endVolume <= 0.1f)
		{
			endVolume += 0.001f;
		}
		else
		{
			endVolume = 0.1f;
		}

		if (player->GetDeathFlag() == true)
		{
			sceneWhile->EndScene_SetOverFlag(true);
		}
		if (uranus->GetDeathFlag() == true)
		{
			sceneDateBuffer->sceneClearFlag[2] = true;
			sceneWhile->EndScene_SetClearFlag(true);
		}
		sceneWhile->EndScene_SetTransUpFlag(true);
	}

	if (sceneWhile->EndScene_GetCompletionFlag1() == true && endVolume == 0.1f)
	{
		if (input->GetJoyButton(JoyAButton) || input->GetJoyButton(JoyBButton) ||
			input->GetJoyButton(JoyYButton) || input->GetJoyButton(JoyXButton))
		{
			sceneWhile->Illustration_SetTransUpFlag(true);
		}
	}

	if (sceneWhile->Illustration_GetCompletion1Flag() == true)
	{
		endVolume = 0.0f;
		audio->StopWave("clearBGM.wav");
		audio->StopWave("overBGM.wav");
		audio->StopWave("windSE.wav");
		audio->StopWave("damegeSE2.wav");
		audio->StopWave("uranusBGM.wav");
		sceneNo = SceneNo::SELECT_SCENE;
	}


#pragma endregion

#pragma region ライティング設定
	lightGroup->DefaultLightSetting();
#pragma endregion

#pragma region オーディオ
	audio->SetVolume("uranusBGM.wav", volume[2]);
	if (audio->GetVolume("uranusBGM.wav") != 0.0f)
	{
		audio->PlayWave("uranusBGM.wav", false);
	}
	if (endVolume != 0.0f)
	{
		if (sceneWhile->EndScene_GetClearFlag() == true)
		{
			audio->SetVolume("clearBGM.wav", endVolume);
			audio->PlayWave("clearBGM.wav", true);
		}
		if (sceneWhile->EndScene_GetOverFlag() == true)
		{
			audio->SetVolume("overBGM.wav", endVolume);
			audio->PlayWave("overBGM.wav", true);
		}
	}
#pragma endregion

#pragma region 各種更新処理
	lightGroup->Update();
	camera->ShakeCamera(cameraPos);
	camera->Update();
	particleMan->Update();
	sceneWhile->Update(stageNumber, stageNo);
	playerUI->Update(player->GetHp(), player->GetChangeTimer(), player->GetTypeFlag());
	enemyUI->Update(leafOopth->GetHp(), uranus->GetHp(), stageNumber, stageNo);
	if (player->GetDeathFlag() == false && uranus->GetDeathFlag() == false)
	{
		stage->Update();
		leafField->Update(stageNumber, uranus->GetModeChangeFlag());
		player->Update(leafOopth->GetAttackFlag(), uranus->GetAttackFlag(),
		leafOopth->GetStartFlag(), uranus->GetStartFlag(), stageNumber);
		if (stageNumber == 1 || stageNumber == 2 || stageNumber == 3)
		{
			leafOopth->Update(stageNumber, leafField->GetCompletionFlag(), sceneWhile->RoundCount_GetCompletionFlag());
		}
		if (stageNumber == 4)
		{
			uranus->Update(stageNumber, leafField->GetCompletionFlag(), sceneWhile->RoundCount_GetCompletionFlag());
		}
		mana->Update(leafOopth->GetStartFlag(), uranus->GetStartFlag());
	}
#pragma endregion
}

void PlayScene::PlutonStageDraw(ID3D12GraphicsCommandList* cmdList)
{
#pragma region 3D描画
	stage->Draw();
	fireField->Draw();
	fireOopth->Draw();
	pluton->Draw();
	player->Draw();
	mana->Draw();
	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma endregion

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	playerUI->Draw();
	enemyUI->Draw();
	sceneWhile->Draw();
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion
}

void PlayScene::PoseidonStageDraw(ID3D12GraphicsCommandList* cmdList)
{
	waterField->Draw();
	stage->Draw();
	waterOopth->Draw();
	poseidon->Draw();
	player->Draw();
	mana->Draw();
	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	playerUI->Draw();
	enemyUI->Draw();
	sceneWhile->Draw();
	debugText.DrawAll(cmdList);
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion
}

void PlayScene::UranusStageDraw(ID3D12GraphicsCommandList* cmdList)
{
	stage->Draw();
	leafField->Draw();
	leafOopth->Draw();
	uranus->Draw();
	player->Draw();
	mana->Draw();
	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	playerUI->Draw();
	enemyUI->Draw();
	sceneWhile->Draw();
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion
}