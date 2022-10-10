#pragma once
#include "Sprite.h"

class SceneWhile
{
private: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: // 静的メンバ関数
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static SceneWhile* Create();

public: // メンバ関数

	// デストラクタ
	~SceneWhile();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(int roundNo, int sceneNo);

#pragma region 挿絵のsetterとgetter
	bool Illustration_GetTransUpFlag() { return illustration.transUpFlag; }
	void Illustration_SetTransUpFlag(bool transUpFlag) { this->illustration.transUpFlag = transUpFlag; }

	bool Illustration_GetTransDownFlag() { return illustration.transDownFlag; }
	void Illustration_SetTransDownFlag(bool transDownFlag) { this->illustration.transDownFlag = transDownFlag; }

	bool Illustration_GetCompletion1Flag() { return illustration.completionFlag1; }
	void Illustration_SetCompletion1Flag(bool  completionFlag) { this->illustration.completionFlag1 = completionFlag; }

	bool Illustration_GetCompletion2Flag() { return illustration.completionFlag2; }
	void Illustration_SetCompletion2Flag(bool  completionFlag) { this->illustration.completionFlag2 = completionFlag; }

	bool Illustration_GetResetFlag() { return illustration.resetFlag; }
	void Illustration_SetResetFlag(bool resetFlag) { this->illustration.resetFlag = resetFlag; }
#pragma endregion

#pragma region ステージ名のsetterとgetter
	bool StageName_GetTransUpFlag() { return stageName.transUpFlag; }
	void StageName_SetTransUpFlag(bool transUpFlag) { this->stageName.transUpFlag = transUpFlag; }
		 
	bool StageName_GetTransDownFlag() { return stageName.transDownFlag; }
	void StageName_SetTransDownFlag(bool transDownFlag) { this->stageName.transDownFlag = transDownFlag; }
		 
	bool StageName_GetCompletion1Flag() { return stageName.completionFlag1; }
	void StageName_SetCompletion1Flag(bool  completionFlag) { this->stageName.completionFlag1 = completionFlag; }

	bool StageName_GetCompletion2Flag() { return stageName.completionFlag2; }
	void StageName_SetCompletion2Flag(bool  completionFlag) { this->stageName.completionFlag2 = completionFlag; }
		 
	bool StageName_GetResetFlag() { return stageName.resetFlag; }
	void StageName_SetResetFlag(bool resetFlag) { this->stageName.resetFlag = resetFlag; }
#pragma endregion

#pragma region ラウンドカウントのsetterとgetter
	bool RoundCount_GetMoveFlag() { return roundCount.moveFlag; }
	void RoundCount_SetMoveFlag(bool moveFlag) { this->roundCount.moveFlag = moveFlag; }

	bool RoundCount_GetCompletionFlag() { return roundCount.completionFlag; }
	void RoundCount_SetCompletionFlag(bool  completionFlag) { this->roundCount.completionFlag = completionFlag; }

	bool RoundCount_GetResetFlag() { return roundCount.resetFlag; }
	void RoundCount_SetResetFlag(bool resetFlag) { this->roundCount.resetFlag = resetFlag; }
#pragma endregion

#pragma region エンドシーンのsetterとgetter
	void EndScene_SetClearFlag(bool clearFlag) { this->endScene.clearFlag = clearFlag; }
	bool EndScene_GetClearFlag() { return endScene.clearFlag; }

	void EndScene_SetOverFlag(bool overFlag) { this->endScene.overFlag = overFlag; }
	bool EndScene_GetOverFlag() { return endScene.overFlag; }

	void EndScene_SetTransUpFlag(bool transUpFlag) { this->endScene.transUpFlag = transUpFlag; }

	void EndScene_SetTransDownFlag(bool transDownFlag) { this->endScene.transDownFlag = transDownFlag; }

	bool EndScene_GetCompletionFlag1() { return endScene.completionFlag1; }
	bool EndScene_GetCompletionFlag2() { return endScene.completionFlag2; }
#pragma endregion


private: // メンバ関数
	// <summary>
	// 生成処理
	// </summary>
	void Initialize();

private: // メンバ変数

#pragma region 挿絵構造体
	typedef struct Illustration
	{
		Sprite* sprite = nullptr;
		XMFLOAT4 alpha = {0.0f, 0.0f, 0.0f, 0.0f};

		void TransUp();
		void TransDown();
		void Reset();

		bool transUpFlag = false;
		bool completionFlag1 = false;

		bool transDownFlag = false;
		bool completionFlag2 = false;

		bool resetFlag = false;
	};


	Illustration illustration;
#pragma endregion

#pragma region ステージ名構造体
	typedef struct StageName
	{
		Sprite* sprite = nullptr;
		XMFLOAT4 alpha = {0.0f, 0.0f, 0.0f, 0.0f};

		void TransUp();
		void TransDown();
		void Reset();
		void NameChange(int sceneNo);

		//1
		bool transUpFlag = false;
		//2
		bool transDownFlag = false;
		bool completionFlag1 = false;
		bool completionFlag2 = false;
		bool resetFlag = false;
	};

	StageName stageName;
#pragma endregion

#pragma region ラウンドカウント構造体
	typedef struct RoundCount
	{
		Sprite* sprite = nullptr;
		XMFLOAT2 pos{};

		void Move(Illustration illustration);
		void NomberChange(int roundNomber);
		void Reset();

		bool moveFlag = false;
		bool completionFlag = false;
		bool resetFlag = false;

		bool stopFlag = false;
		int stopTimer = 0;
	};

	RoundCount roundCount;
#pragma endregion

#pragma region エンドシーン
	typedef struct EndScene
	{
		Sprite* sprite = nullptr;
		XMFLOAT4 alpha = { 0.0f, 0.0f, 0.0f, 0.0f };

		void SpriteChange();
		void TransUp();
		void TransDown();
		
		//1
		bool transUpFlag = false;
		//2
		bool transDownFlag = false;
		bool completionFlag1 = false;
		bool completionFlag2 = false;
		bool clearFlag = false;
		bool overFlag = false;

		bool resetFlag = false;

	};

	EndScene endScene;
#pragma endregion

};