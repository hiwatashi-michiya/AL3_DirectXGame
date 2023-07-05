#include "Player.h"
#include <cassert>

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model) {

	//NULLポインタチェック
	assert(model);
	//メンバ変数に記録
	model_ = model;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 1.0f;

}

void Player::Update() {

	//行数を定数バッファに転送
	worldTransform_.UpdateMatrix();

}

void Player::Draw(ViewProjection viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);

}
