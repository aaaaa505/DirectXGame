#pragma once
#include <DirectXMath.h>

class Collision
{
private://エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	//単体と単体
	static bool SphereSphere(XMFLOAT3 targetA, float radiusA, XMFLOAT3 targetB, float radiusB);

};

