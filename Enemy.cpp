#include <Enemy.h>
#include <cassert>
#include <ImGuiManager.h>

Enemy::Enemy() {}

Enemy::~Enemy() { delete state_; }

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);
	assert(textureHandle);
	//モデルの初期化
	model_ = model;
	textureHandle_ = textureHandle;
	state_ = new EnemyStateApproach();
	state_->SetEnemy(this);
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//初期座標設定
	worldTransform_.translation_ = {0.0f, 5.0f, 50.0f};

}

void Enemy::Update() {

	state_->Update();

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

void Enemy::ChangeState(BaseEnemyState* state) {

	delete state_;
	state_ = state;
	state_->SetEnemy(this);

}

void Enemy::Move(const Vector3& velocity) {

	// 座標の更新
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);

}

void Enemy::Draw(ViewProjection viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);	

}

Vector3 Enemy::GetPosition() { return worldTransform_.translation_; }

BaseEnemyState::BaseEnemyState() {}

BaseEnemyState::~BaseEnemyState() {}

void BaseEnemyState::Update() {

}

void EnemyStateApproach::Update() {

	// 敵の移動速さ
	const float kEnemySpeed = -0.2f;

	// 敵の移動ベクトル
	Vector3 move = {0, 0, kEnemySpeed};

	//座標の更新
	enemy_->Move(move);

	// Z座標が0.0f未満になったら離脱フェーズ移行
	if (enemy_->GetPosition().z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave());
	}

}

void EnemyStateLeave::Update() {

	// 敵の移動速さ
	const float kEnemySpeed = -0.2f;

	// 敵の移動ベクトル
	Vector3 move = {kEnemySpeed, -kEnemySpeed, kEnemySpeed};

	// 座標の更新。全ての座標が規定値に達したら止める
	if (enemy_->GetPosition().x > -100.0f ||
		enemy_->GetPosition().y < 100.0f ||
	    enemy_->GetPosition().z > -100.0f) {
		enemy_->Move(move);
	}

}
