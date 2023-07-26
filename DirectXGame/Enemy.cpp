#include <Enemy.h>
#include <cassert>
#include <ImGuiManager.h>
#include "Player.h"
#include "GameScene.h"
#include <TextureManager.h>

Enemy::Enemy() {}

Enemy::~Enemy() {

}

void Enemy::Initialize(Model* model, uint32_t textureHandle, GameScene* gameScene) {

	assert(model);
	assert(textureHandle);
	//モデルの初期化
	model_ = model;
	textureHandle_ = textureHandle;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//ゲームシーンのセット
	SetGameScene(gameScene);
	//接近フェーズ初期化
	InitPhaseApproach();

	enemyEasyTex_ = TextureManager::Load("easy.png");
	enemyNormalTex_ = TextureManager::Load("normal.png");
	enemyHardTex_ = TextureManager::Load("hard.png");

}

void Enemy::Update() {

	switch (phase_) {
	case Phase::Approach:
	default:
		PhaseApproach();
		break;
	case Phase::Leave:
		PhaseLeave();
		break;
	}

	////発射タイマーカウントダウン
	//fireTimer_--;
	////指定時間に達した
	//if (fireTimer_ == 0) {
	//	//弾発射
	//	Fire();
	//	//発射タイマー初期化
	//	fireTimer_ = kFireInterval;
	//}

	//行列の更新
	worldTransform_.UpdateMatrix();

	/*ImGui::Begin("Enemy state");
	ImGui::Text(
	    "Enemy Position\n x : %0.2f\n y : %0.2f\n z : %0.2f\nFireTimer\n %d", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z, fireTimer_);
	ImGui::End();*/

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

	gameScene_->AddEnemyBullet(newBullet);

}

//接近
void Enemy::PhaseApproach() {

	// 敵の移動速さ
	const float kEnemySpeed = -0.05f;

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
	const float kEnemySpeed = -0.05f;

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

	switch (type_) {
	
	default:
	case EASY:

		// 3Dモデルを描画
		model_->Draw(worldTransform_, viewProjection, enemyEasyTex_);	

		break;

	case NORMAL:

		// 3Dモデルを描画
		model_->Draw(worldTransform_, viewProjection, enemyNormalTex_);	

		break;

	case HARD:

		// 3Dモデルを描画
		model_->Draw(worldTransform_, viewProjection, enemyHardTex_);	

		break;

	}

	

}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {

}

void Enemy::SetStartPosition(Vector3 position) {
	worldTransform_.translation_ = position;
}
