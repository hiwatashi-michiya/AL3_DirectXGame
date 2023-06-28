#include "Player.h"
#include <cassert>

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);
	assert(textureHandle);
	//メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	//ワールド変換の初期化
	worldTransform_.Initialize();

}

void Player::Update() {

	//行数を定数バッファに転送
	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
