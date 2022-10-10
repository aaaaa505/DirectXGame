#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleManager.h"
#include "DebugText.h"
#include "Audio.h"
#include "LightGroup.h"
#include "Player.h"
#include "Stage.h"
#include "FireField.h"
#include "WaterField.h"
#include "LeafField.h"
#include "FireOopth.h"
#include "WaterOopth.h"
#include "LeafOopth.h"
#include "Pluton.h"
#include "Poseidon.h"
#include "Uranus.h"
#include "SceneWhile.h"
#include "PlayerUI.h"
#include "EnemyUI.h"
#include "Mana.h"

// 属性列挙型
enum Element
{
	WATER_ELEMENT,
	FIRE_ELEMENT,
	LEAF_ELEMENT
};

// シーン管理用列挙型
enum SceneNo
{
	TITLE_SCENE,
	SELECT_SCENE,
	GAME_SCENE,
};

// シーン管理用バッファ
struct SceneDataBuffer
{
	int sceneNo = TITLE_SCENE;
	int stageNo = 1;
	bool sceneClearFlag[3] = {};
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon, SceneDataBuffer* sceneDateBuffer) = 0;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

protected: // メンバ関数
	/// <summary>
	/// シーンデータ更新処理
	/// </summary>
	void SceneDateBufferUpdate();

protected: // メンバ変数
#pragma region 汎用クラス
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	int sceneNo;
	int stageNo;
	SceneDataBuffer* sceneDateBuffer = nullptr;
#pragma endregion
};

