#include <Enemy.h>
#include <cassert>
#include <ImGuiManager.h>
#include "Player.h"

Enemy::Enemy() {}

Enemy::~Enemy() {

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

	//発射タイマーカウントダウン
	fireTimer_--;
	//指定時間に達した
	if (fireTimer_ == 0) {
		//弾発射
		Fire();
		//発射タイマー初期化
		fireTimer_ = kFireInterval;
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

	assert(player_);

	//弾速
	const float kBulletSpeed = 1.0f;
	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = worldTransform_.translation_;
	Vector3 length = Subtract(enemyPos, playerPos);
	Vector3 speed = Normalize(length);

	Vector3 velocity(speed.x * -kBulletSpeed,
		speed.y * -kBulletSpeed,
		speed.z * -kBulletSpeed);

	//弾生成、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);

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

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
