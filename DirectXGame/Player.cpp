#define _USE_MATH_DEFINES
#include "Player.h"
#include <cassert>
#include <math.h>
#include "GlobalVariables.h"
#include "Score.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

Player::Player() {}

Player::~Player() {

	//弾の解放
	for (Bullet* bullet : bullets_) {
		delete bullet;
	}

}

void Player::Initialize(const std::vector<Model*>& models) {

	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	modelBullet_.reset(Model::CreateFromOBJ("playerBullet", true));
	modelBurst_.reset(Model::CreateFromOBJ("burst", true));

	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//GlobalVariables* globalVariables;
	//globalVariables = GlobalVariables::GetInstance();
	//const char* groupName = "Player";
	////グループを追加
	//GlobalVariables::GetInstance()->CreateGroup(groupName);	

	//ワールド変換の初期化
	worldTransformBase_.Initialize();
	worldTransformBase_.translation_.y = 1.0f;
	//体
	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransformBase_;
	//頭
	worldTransformHead_.Initialize();
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformHead_.translation_.y = 3.0f;
	//左腕
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.translation_.x = -1.0f;
	worldTransformL_arm_.translation_.y = 2.5f;
	//右腕
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.translation_.x = 1.0f;
	worldTransformR_arm_.translation_.y = 2.5f;

	//武器
	worldTransformWeapon_.Initialize();
	worldTransformWeapon_.parent_ = &worldTransformBody_;
	worldTransformWeapon_.translation_.y = 3.0f;

	//範囲攻撃
	worldTransformBurst_.Initialize();
	worldTransformBurst_.parent_ = &worldTransformBase_;
	worldTransformBurst_.scale_ *= kBurstRadius;
	worldTransformBurst_.UpdateMatrix();

	//globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	//globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_arm_.translation_);
	//globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_arm_.translation_);

	//浮遊ギミック初期化
	InitializeFloatingGimmick();

	colorTex_[kColorRed] = TextureManager::Load("burst/alphared.png");
	colorTex_[kColorGreen] = TextureManager::Load("burst/alphagreen.png");
	colorTex_[kColorBlue] = TextureManager::Load("burst/alphablue.png");

	playerColorTex_[kColorRed] = TextureManager::Load("player_body/player_red.png");
	playerColorTex_[kColorGreen] = TextureManager::Load("player_body/player_green.png");
	playerColorTex_[kColorBlue] = TextureManager::Load("player_body/player_blue.png");

	burstTex_ = colorTex_[kColorRed];

	playerTex_ = playerColorTex_[kColorRed];

	changeColorSE_ = audio_->LoadWave("audio/changecolor.wav");

	SetCollisionAttribute(0x00000001);
	SetCollisionMask(0xfffffffe);

}

void Player::Update() {

//	#ifdef _DEBUG
//
//	ImGui::Begin("weapon");
//	ImGui::DragFloat3("translation", &worldTransformWeapon_.translation_.x, 0.01f);
//	ImGui::DragFloat3("rotation", &worldTransformWeapon_.rotation_.x, 0.01f);
//	ImGui::End();
//
//	ImGui::Begin("Arm");
//	ImGui::DragFloat3("L rotation", &worldTransformL_arm_.rotation_.x, 0.01f);
//	ImGui::DragFloat3("R rotation", &worldTransformR_arm_.rotation_.x, 0.01f);
//	ImGui::End();
//
//	ImGui::Begin("Player");
//	ImGui::SliderFloat3("Head Translation", &worldTransformHead_.translation_.x, -100.0f, 100.0f);
//	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, -100.0f, 100.0f);
//	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, -100.0f, 100.0f);
//	ImGui::SliderInt("cycle", &cycle, 10, 300);
//	ImGui::SliderFloat("swing", &swing, 0.1f, 50.0f);
//	ImGui::End();
//
//#endif // _DEBUG

	/*ApplyGlobalVariables();*/

	bullets_.remove_if([](Bullet* bullet) {
		
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}

		return false;
	});

	if (isInvincible_) {

		if (--invincibleTimer_ <= 0) {
			isInvincible_ = false;
		}

	}

	if (burstCoolTimer_ > 0) {
		burstCoolTimer_--;
	}

	if (behaviorRequest_) {
		//振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		//各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		
		}
		//振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	
	}

	for (Bullet* bullet : bullets_) {
		bullet->Update();
	}

	//行数を定数バッファに転送
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
	worldTransformBurst_.UpdateMatrix();

}

void Player::Draw(const ViewProjection& viewProjection) {

	//3Dモデルを描画
	if (invincibleTimer_ % 2 == 0) {
		models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection, playerTex_);
		models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection, playerTex_);
	}
	/*models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);*/
	
	for (Bullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	if (burstCoolTimer_ == 0) {
		modelBurst_->Draw(worldTransformBurst_, viewProjection, burstTex_);
	}

	if (behavior_ == Behavior::kAttack) {
		models_[kModelIndexWeapon]->Draw(worldTransformWeapon_, viewProjection);
	}
	
}

void Player::InitializeFloatingGimmick() {
	
	floatingParameter_ = 0.0f;

}

void Player::UpdateFloatingGimmick() {



	//1フレームでのパラメータ加算値
	float step = float(2.0f * M_PI / cycle);

	//パラメータを1ステップ分加算
	floatingParameter_ += step;

	//2πを超えたら0に戻す
	floatingParameter_ = float(std::fmod(floatingParameter_, 2.0f * M_PI));

	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sinf(floatingParameter_) * swing;

	worldTransformL_arm_.rotation_.x = std::cosf(floatingParameter_) * swing;
	worldTransformR_arm_.rotation_.x = std::cosf(floatingParameter_) * swing;

}

void Player::BehaviorRootUpdate() {

	if (coolTimer_ > 0) {
		coolTimer_--;
	}

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// 速さ
		const float speed = 0.7f;

		// 移動量。Lスティックの入力を取る
		Vector3 move = {float(joyState.Gamepad.sThumbLX), 0.0f, float(joyState.Gamepad.sThumbLY)};
		// 移動量に速さを反映
		move = Multiply(speed, Normalize(move));

		Matrix4x4 matRotate = MakeRotateYMatrix(viewProjection_->rotation_.y);

		// 移動ベクトルをカメラの角度だけ回転させる
		move = TransformNormal(move, matRotate);

		// 移動
		worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);

		if (worldTransformBase_.translation_.x > 100.0f) {
			worldTransformBase_.translation_.x = 100.0f;
		}
		else if (worldTransformBase_.translation_.x < -100.0f) {
			worldTransformBase_.translation_.x = -100.0f;
		}
		
		if (worldTransformBase_.translation_.z > 100.0f) {
			worldTransformBase_.translation_.z = 100.0f;
		} else if (worldTransformBase_.translation_.z < -100.0f) {
			worldTransformBase_.translation_.z = -100.0f;
		}

		// 回転
		if (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0) {
			worldTransformBase_.rotation_.y = float(std::atan2(double(move.x), double(move.z)));
		}
		
	}

	if ((input_->TriggerKey(DIK_1) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B))) {

		if (GetColorType() != Type::C_RED) {
			SetColorType(kColorRed);
			bulletColor_ = Type::C_RED;
			burstTex_ = colorTex_[Type::C_RED];
			playerTex_ = playerColorTex_[Type::C_RED];
			audio_->PlayWave(changeColorSE_);
		}

		
	}

	else if ((input_->TriggerKey(DIK_2) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A))) {

		if (GetColorType() != Type::C_GREEN) {
			SetColorType(kColorGreen);
			bulletColor_ = Type::C_GREEN;
			burstTex_ = colorTex_[Type::C_GREEN];
			playerTex_ = playerColorTex_[Type::C_GREEN];
			audio_->PlayWave(changeColorSE_);
		}

	}

	else if ((input_->TriggerKey(DIK_3) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X))) {

		if (GetColorType() != Type::C_BLUE) {
			SetColorType(kColorBlue);
			bulletColor_ = Type::C_BLUE;
			burstTex_ = colorTex_[Type::C_BLUE];
			playerTex_ = playerColorTex_[Type::C_BLUE];
			audio_->PlayWave(changeColorSE_);
		}

	}

	if ((input_->TriggerKey(DIK_SPACE) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))&& coolTimer_ == 0) {
		Attack();
		coolTimer_ = kCoolTime;
	}

	if ((input_->TriggerKey(DIK_Q) || (joyState.Gamepad.bRightTrigger != 0)) && burstCoolTimer_ == 0) {
		BurstAttack();
	}

	// 浮遊ギミック更新
	UpdateFloatingGimmick();

}

void Player::BehaviorRootInitialize() {
	worldTransformL_arm_.rotation_.x = 0.0f;
	worldTransformR_arm_.rotation_.x = 0.0f;
	worldTransformWeapon_.rotation_.x = 3.14f;
	worldTransformWeapon_.translation_.y = -1.0f;
	InitializeFloatingGimmick();
}

void Player::BehaviorAttackUpdate() {

	if (attackFrame == 70) {
		behaviorRequest_ = Behavior::kRoot;
	}

	float rad = 3.14f / 30.0f;

	if (attackFrame < 15) {
		worldTransformL_arm_.rotation_.x -= rad;
		worldTransformR_arm_.rotation_.x -= rad;
		worldTransformWeapon_.rotation_.x -= rad;
	} else if (attackFrame >= 25 && attackFrame < 40) {
		worldTransformL_arm_.rotation_.x += rad;
		worldTransformR_arm_.rotation_.x += rad;
		worldTransformWeapon_.rotation_.x += rad;
	}

	// フレーム更新
	attackFrame++;

}

void Player::BehaviorAttackInitialize() {
	attackFrame = 0;
	worldTransformL_arm_.rotation_.x = -1.57f;
	worldTransformR_arm_.rotation_.x = -1.57f;
	worldTransformWeapon_.rotation_.x = 1.57f;
	worldTransformWeapon_.translation_.y = 3.0f;
}

void Player::ApplyGlobalVariables() {

	GlobalVariables* globalVariables;
	globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformHead_.translation_ = globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransformL_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransformR_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmR Translation");

}

void Player::Attack() {

	const float kBulletSpeed = 1.0f;

	Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

	velocity = TransformNormal(velocity, worldTransformBase_.matWorld_);

	Bullet* newBullet = new Bullet();

	std::vector<Model*> modelBullets{modelBullet_.get()};

	newBullet->Initialize(modelBullets, worldTransformBase_.translation_, velocity, bulletColor_);

	bullets_.push_back(newBullet);

}

void Player::BurstAttack() {

	//範囲攻撃開始
	if (burstCoolTimer_ == 0) {

		isBurst_ = true;

		burstCoolTimer_ = kBurstCoolTime;

	}

}

void Player::OnCollision(Collider* collider) {

	SetColorType(collider->GetColorType());

	isInvincible_ = true;
	invincibleTimer_ = kInvincibleTime;

}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];

	return worldPos;

}
