#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
};

struct Matrix4x4;

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(float scalar, const Vector3& v1);

float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);

Vector3 Normalize(const Vector3& v);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//ベクトル変換(スケールと回転)
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

//2点間の距離を求める関数
float Distance(const Vector3& v1, const Vector3& v2);

//二つの物体の半径から当たり判定を計算する関数
float HitRadius(float r1, float r2);
