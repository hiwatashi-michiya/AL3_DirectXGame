#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models) {

	BaseCharacter::Initialize(models);

	worldTransform_.Initialize();
	worldTransform_.translation_.y = 1.0f;
	worldTransformBody_.Initialize();
	worldTransformBody_.translation_.z = 20.0f;

	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_.x = -1.5f;
	worldTransformL_arm_.translation_.y = 1.0f;
	worldTransformL_arm_.parent_ = &worldTransformBody_;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_.x = 1.5f;
	worldTransformR_arm_.translation_.y = 1.0f;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

}

void Enemy::Update() {

	{

		float rad = 3.14f / 60.0f;
		worldTransformBody_.rotation_.y += rad;

		if (worldTransformBody_.rotation_.y >= 6.28f) {
			worldTransformBody_.rotation_.y = 0.0f;
		}

		Vector3 move = {0.0f, 0.0f, 1.0f};

		move = TransformNormal(move, worldTransformBody_.matWorld_);
		move /= 3.0f;
		worldTransformBody_.translation_ += move;

	}

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

}
