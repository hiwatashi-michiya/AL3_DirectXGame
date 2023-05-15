#include "EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet() {}

EnemyBullet::~EnemyBullet() {}

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


}

void EnemyBullet::Update() {

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
