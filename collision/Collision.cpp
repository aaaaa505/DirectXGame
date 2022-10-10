#include "Collision.h"

bool Collision::SphereSphere(XMFLOAT3 targetA, float radiusA, XMFLOAT3 targetB, float radiusB)
{
    //2つの球の中心座標を結ぶベクトルを求める
    XMFLOAT3 vec{};
    vec.x = targetA.x - targetB.x;
    vec.y = targetA.y - targetB.y;
    vec.z = targetA.z - targetB.z;

    //ベクトルの長さの2乗(2点間の距離の 2乗)を求める
    float sqLength = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;

    //2つの球の半径の合計を求める
    float r = radiusA + radiusB;

    //2点間の距離の2乗 < 半径の合計の2乗なら2つの球は交差している
    return sqLength < r* r;
}
