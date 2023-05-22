#include "EnemyBullet.h"
#include <cassert>
#include "Player.h"

EnemyBullet::EnemyBullet() {}

EnemyBullet::~EnemyBullet() {}

static int number = 0;

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("red.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	//Y軸周り角度
	worldTransform_.rotation_.y = float(std::atan2(double(velocity_.x), double(velocity_.z)));
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = TransformNormal(velocity_, rotateYMatrix);
	//X軸周り角度
	worldTransform_.rotation_.x = float(std::atan2(double(-velocityZ.y), double(velocityZ.z)));

	
	number_ = number++;
}

void EnemyBullet::Update() {

	//敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);

	if (number_ == 2) {
		float length = Length(toPlayer);
		if (length <= 14.0f) {
		static int a = 0;
		a+= (int)length;
		}
	}

	//ベクトルを正規化する
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	//球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_ = Slerp(velocity_, toPlayer, 0.05f);
	velocity_.x *= 1.0f;
	velocity_.y *= 1.0f;
	velocity_.z *= 1.0f;
	// Y軸周り角度
	worldTransform_.rotation_.y = float(std::atan2(double(velocity_.x), double(velocity_.z)));
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = TransformNormal(velocity_, rotateYMatrix);
	// X軸周り角度
	worldTransform_.rotation_.x = float(std::atan2(double(-velocityZ.y), double(velocityZ.z)));
	
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
