#pragma once

#include "GameScene.h"

/// <summary>
/// プレイシーン
/// </summary>
class PlayScene : public GameScene
{
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayScene()override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SceneDataBuffer* sceneDateBuffer) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private: // メンバ関数
	/// <summary>
	/// プルトンステージ更新
	/// </summary>
	void PlutonStageUpdata();

	/// <summary>
	/// ポセイドンステージ更新
	/// </summary>
	void PoseidonStageUpdata();

	/// <summary>
	/// ウラヌスステージ更新
	/// </summary>
	void UranusStageUpdata();

	/// <summary>
	/// プルトンステージ描画
	/// </summary>
	void PlutonStageDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ポセイドンステージ描画
	/// </summary>
	void PoseidonStageDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ウラヌスステージ描画
	/// </summary>
	void UranusStageDraw(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数
	Camera* camera = nullptr;
	ParticleManager* particleMan = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;

#pragma region オブジェクト・スプライト	
	//クラスオブジェクト
	Player* player = nullptr;
	Mana* mana = nullptr;
	Stage* stage = nullptr;
	FireField* fireField = nullptr;
	WaterField* waterField = nullptr;
	FireOopth* fireOopth = nullptr;
	WaterOopth* waterOopth = nullptr;
	LeafOopth* leafOopth = nullptr;
	LeafField* leafField = nullptr;
	Pluton* pluton = nullptr;
	Poseidon* poseidon = nullptr;
	Uranus* uranus = nullptr;

	//クラススプライト
	SceneWhile* sceneWhile;
	PlayerUI* playerUI;
	EnemyUI* enemyUI;

#pragma endregion

#pragma region カメラ用
	XMFLOAT3 cameraPos{};
	XMFLOAT2 cameraShake{};
	float shakeTimer;

#pragma endregion

#pragma region その他変数
	//シーン管理
	int stageNo = 0;

	//ターン数
	int stageNumber;

	//ボリューム
	float volume[3] = { 0.0f };
	float endVolume = 0.0f;
	
	bool finishFlag = false;
#pragma endregion

};