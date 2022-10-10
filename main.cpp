#include "Illegal.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ゲームの宣言
	Illegal* illegal = nullptr;
	// 初期化処理
	illegal = new Illegal();
	illegal->Initialize();
	// ゲームループ
	illegal->Loop();
	// 終了処理
	illegal->Finalize();
	safe_delete(illegal);
	return 0;
}