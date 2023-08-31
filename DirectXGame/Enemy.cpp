#include "Enemy.h"
#include "GameScene.h"
#include "Score.h"
#include "Rand.h"

Enemy::Enemy() {

}

Enemy::~Enemy() {

}

void Enemy::Initialize(const std::vector<Model*>& models) {

	audio_ = Audio::GetInstance();

	BaseCharacter::Initialize(models);

	worldTransform_.Initialize();
	worldTransform_.translation_.y = 1.0f;
	worldTransformBody_.Initialize();
	worldTransformBody_.translation_.z = 20.0f;
	worldTransformBody_.scale_ = Vector3(0.0f, 0.0f, 0.0f);
	worldTransformBody_.UpdateMatrix();

	effect_ = std::make_unique<Effect>();
	effect_->Initialize(30);
	effect_->SetColor(GetColorType());

	/*worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_.x = -1.5f;
	worldTransformL_arm_.translation_.y = 1.0f;
	worldTransformL_arm_.parent_ = &worldTransformBody_;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_.x = 1.5f;
	worldTransformR_arm_.translation_.y = 1.0f;
	worldTransformR_arm_.parent_ = &worldTransformBody_;*/

	colorTex_[kColorRed] = TextureManager::Load("enemy/enemy_red.png");
	colorTex_[kColorGreen] = TextureManager::Load("enemy/enemy_green.png");
	colorTex_[kColorBlue] = TextureManager::Load("enemy/enemy_blue.png");

	enemyTex_ = colorTex_[kColorRed];

	SetCollisionAttribute(0x00000002);
	SetCollisionMask(0xfffffffd);
	SetRadius(1.1f);

	switch (GetColorType()) {
	case C_RED:
		attackTimer_ = kAttackTime / 3;
		break;
	case C_GREEN:
		attackTimer_ = kAttackTime;
		break;
	case C_BLUE:
		attackTimer_ = kAttackTime / 2;
		break;
	default:
		attackTimer_ = kAttackTime;
		break;
	}

	deadSE_ = audio_->LoadWave("audio/enemydead.mp3");
	hitSE_ = audio_->LoadWave("audio/enemyhit.mp3");

}

void Enemy::Update() {

	if (isDead_) {

		effect_->Update();

	}
	else if (isSpawnTime_) {

		if (--spawnTimer_ > 0) {

			float rad = 3.14f / 5.0f;

			worldTransformBody_.rotation_.y += rad;

			worldTransformBody_.scale_ += (Vector3(1.0f, 1.0f, 1.0f) /= 30);

		}
		else {
			isSpawnTime_ = false;
		}

	}
	else {

		switch (GetColorType()) {
		default:
		case C_RED:
		
		{

			Vector3 toPlayer = Subtract(
			    gameScene_->GetPlayer()->GetWorldPosition(), worldTransformBody_.translation_);

			toPlayer = Normalize(toPlayer);

			// Y軸周り角度
			worldTransformBody_.rotation_.y =
			    float(std::atan2(double(toPlayer.x), double(toPlayer.z)));
			
		}

			break;

		case C_GREEN:

			{

			float rad = 3.14f / 120.0f;
			worldTransformBody_.rotation_.y += rad;

			if (worldTransformBody_.rotation_.y >= 6.28f) {
				worldTransformBody_.rotation_.y = 0.0f;
			}

			Vector3 move = {0.0f, 0.0f, 0.5f};

			move = TransformNormal(move, worldTransformBody_.matWorld_);
			move /= 3.0f;
			worldTransformBody_.translation_ += move;
			}

			break;

		case C_BLUE:

			{

			float rad = -3.14f / 120.0f;
			worldTransformBody_.rotation_.y += rad;

			if (worldTransformBody_.rotation_.y <= -6.28f) {
				worldTransformBody_.rotation_.y = 0.0f;
			}

			Vector3 move = {0.0f, 0.0f, 0.5f};

			move = TransformNormal(move, worldTransformBody_.matWorld_);
			move /= 2.0f;
			worldTransformBody_.translation_ += move;
		    }

			break;

		}

		Attack();

	}

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	/*worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();*/

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	if (isDead_) {
		effect_->Draw(viewProjection);
	}
	else {

		models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection, enemyTex_);
		/*models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
		models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);*/

	}

}

void Enemy::OnCollision(Collider* collider) {

	Score* score;
	score = Score::GetInstance();

	SetColorType(collider->GetColorType());

	enemyTex_ = colorTex_[GetColorType()];

	if (--life_ <= 0) {

		score->AddScore(100, score->GetMagScore());

		StartEffect();

		gameScene_->SetMagScoreTimer();

		gameScene_->AddKillCount();

		audio_->PlayWave(deadSE_);

		score->SetMagScore(score->GetMagScore() + 0.05f);

		isDead_ = true;
	}
	else {

		score->AddScore(10);

		audio_->PlayWave(hitSE_);

		worldTransformBody_.scale_ *= 1.2f;
		SetRadius(GetRadius() * 1.2f);

	}

}

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransformBody_.matWorld_.m[3][0];
	worldPos.y = worldTransformBody_.matWorld_.m[3][1];
	worldPos.z = worldTransformBody_.matWorld_.m[3][2];

	return worldPos;

}

void Enemy::Attack() {

	switch (GetColorType()) {
	default:
	case C_RED:

		// 一定間隔毎に弾を発射
		if (--attackTimer_ <= 0 && !isDead_) {

			const float kBulletSpeed = 1.0f;

			Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

			velocity = TransformNormal(velocity, worldTransformBody_.matWorld_);

			EnemyBullet* newBullet = new EnemyBullet();

			newBullet->Initialize(worldTransformBody_.translation_, velocity);

			gameScene_->AddEnemyBullet(newBullet);

			// タイマーリセット
			attackTimer_ = kAttackTime;
		}

		break;

	case C_GREEN:

		// 一定間隔毎に弾を発射
		if (--attackTimer_ <= 0 && !isDead_) {

			const float kBulletSpeed = 1.0f;

			Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

			velocity = TransformNormal(velocity, worldTransformBody_.matWorld_);

			EnemyBullet* newBullet = new EnemyBullet();

			newBullet->Initialize(worldTransformBody_.translation_, velocity);

			gameScene_->AddEnemyBullet(newBullet);

			// タイマーリセット
			attackTimer_ = kAttackTime;
		}

		break;

	case C_BLUE:

		// 一定間隔毎に弾を発射
		if (--attackTimer_ <= 0 && !isDead_) {

			const float kBulletSpeed = 1.0f;

			Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

			velocity = TransformNormal(velocity, worldTransformBody_.matWorld_);

			EnemyBullet* newBullet = new EnemyBullet();

			newBullet->Initialize(worldTransformBody_.translation_, velocity);

			gameScene_->AddEnemyBullet(newBullet);

			// タイマーリセット
			attackTimer_ = kAttackTime / 2;
		}

		break;
	}

}

void Enemy::Burst() {

	Score* score;
	score = Score::GetInstance();

	score->AddScore(100, score->GetMagScore());

	StartEffect();

	gameScene_->AddKillCount();
	gameScene_->SetMagScoreTimer();

	audio_->PlayWave(deadSE_);

	isDead_ = true;

	score->SetMagScore(score->GetMagScore() * 1.2f);

}

void Enemy::SetStartPosition(Vector3 position) {

	worldTransformBody_.translation_ = position;

}

void Enemy::UpdateColor() {

	enemyTex_ = colorTex_[GetColorType()];

}

void Enemy::StartEffect() {

	effect_->SetEffect(worldTransformBody_.translation_);
	effect_->SetColor(GetColorType());

}
