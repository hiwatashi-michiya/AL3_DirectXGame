#include <Enemy.h>
#include <cassert>
#include <ImGuiManager.h>

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);
	assert(textureHandle);
	//モデルの初期化
	model_ = model;
	textureHandle_ = textureHandle;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//初期座標設定
	worldTransform_.translation_ = {0.0f, 5.0f, 50.0f};
	pPhase = &Enemy::PhaseApproach;

}

void (Enemy::*Enemy::pPhaseTable[])() = {
	&Enemy::PhaseApproach,
	&Enemy::PhaseLeave
};

void Enemy::Update() {

	(this->*pPhaseTable[static_cast<size_t>(phase_)])();

	/*switch (phase_) {
	case Phase::Approach:
	default:
		PhaseApproach();
		break;
	case Phase::Leave:
		PhaseLeave();
		break;
	}*/

	//行列の更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("Enemy state");
	ImGui::Text(
	    "Player Position\n x : %0.2f\n y : %0.2f\n z : %0.2f", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();

}

//接近
void Enemy::PhaseApproach() {

	// 敵の移動速さ
	const float kEnemySpeed = -0.2f;

	// 敵の移動ベクトル
	Vector3 move = {0, 0, kEnemySpeed};

	// 座標の更新
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	//Z座標が0.0f未満になったら離脱フェーズ移行
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
	
}

//離脱
void Enemy::PhaseLeave() {

	// 敵の移動速さ
	const float kEnemySpeed = -0.2f;

	// 敵の移動ベクトル
	Vector3 move = {kEnemySpeed, -kEnemySpeed, kEnemySpeed};

	// 座標の更新。全ての座標が規定値に達したら止める
	if (worldTransform_.translation_.x > -100.0f ||
		worldTransform_.translation_.y < 100.0f ||
		worldTransform_.translation_.z > -100.0f) {
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

}

void Enemy::Draw(ViewProjection viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);	

}
