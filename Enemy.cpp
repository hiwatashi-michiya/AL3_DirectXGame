#include <Enemy.h>
#include <cassert>
#include <ImGuiManager.h>
#include "TimedCall.h"

Enemy::Enemy() {}

Enemy::~Enemy() {

	//timedCallの解放
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}

	//bulletの解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);
	assert(textureHandle);
	//モデルの初期化
	model_ = model;
	textureHandle_ = textureHandle;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//初期座標設定
	worldTransform_.translation_ = {20.0f, 5.0f, 50.0f};

	//接近フェーズ初期化
	InitPhaseApproach();

}

void Enemy::Update() {

	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->isFinished()) {
			delete timedCall;
			return true;
		}
		return false;
	});

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	switch (phase_) {
	case Phase::Approach:
	default:
		PhaseApproach();
		break;
	case Phase::Leave:
		PhaseLeave();
		break;
	}

	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	//行列の更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("Enemy state");
	ImGui::Text(
	    "Enemy Position\n x : %0.2f\n y : %0.2f\n z : %0.2f\nFireTimer\n %d", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z, fireTimer_);
	ImGui::End();

}

void Enemy::Fire() {

	//弾速
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0.0f, 0.0f, -kBulletSpeed);

	//弾生成、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);

}

void Enemy::LoopFire() {

	//弾発射
	Fire();

	//発射タイマーをセットする
	timedCalls_.push_back(
		new TimedCall(std::bind(&Enemy::LoopFire, this), fireTimer_));

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

void Enemy::InitPhaseApproach() {
	//発射タイマーを初期化
	fireTimer_ = kFireInterval;
	//発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::LoopFire, this), fireTimer_));
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

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}
