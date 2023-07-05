#include "Ground.h"
#include <cassert>

Ground::Ground() {}

Ground::~Ground() {}

// 初期化
void Ground::Initialize(Model* model) {

	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 1000.0f;
	worldTransform_.scale_.y = 1000.0f;
	worldTransform_.scale_.z = 1000.0f;
	worldTransform_.UpdateMatrix();
}

// 更新
void Ground::Update() {

}

// 描画
void Ground::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}
