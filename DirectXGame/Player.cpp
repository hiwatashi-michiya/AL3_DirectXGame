#define _USE_MATH_DEFINES
#include "Player.h"
#include <cassert>
#include <math.h>
#include "GlobalVariables.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

Player::Player() {}

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {

	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	GlobalVariables* globalVariables;
	globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->SetValue(groupName, "Test int", 90);
	globalVariables->SetValue(groupName, "Test float", 12.5f);
	globalVariables->SetValue(groupName, "Test Vector3", Vector3(1.0f,2.0f,3.0f));

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

	//浮遊ギミック初期化
	InitializeFloatingGimmick();

}

void Player::Update() {

	#ifdef _DEBUG

	ImGui::Begin("weapon");
	ImGui::DragFloat3("translation", &worldTransformWeapon_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransformWeapon_.rotation_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Arm");
	ImGui::DragFloat3("L rotation", &worldTransformL_arm_.rotation_.x, 0.01f);
	ImGui::DragFloat3("R rotation", &worldTransformR_arm_.rotation_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Translation", &worldTransformHead_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderInt("cycle", &cycle, 10, 300);
	ImGui::SliderFloat("swing", &swing, 0.1f, 50.0f);
	ImGui::End();

#endif // _DEBUG

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


	//行数を定数バッファに転送
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
	

}

void Player::Draw(const ViewProjection& viewProjection) {

	//3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
	
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

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// 速さ
		const float speed = 0.3f;

		// 移動量。Lスティックの入力を取る
		Vector3 move = {float(joyState.Gamepad.sThumbLX), 0.0f, float(joyState.Gamepad.sThumbLY)};
		// 移動量に速さを反映
		move = Multiply(speed, Normalize(move));

		Matrix4x4 matRotate = MakeRotateYMatrix(viewProjection_->rotation_.y);

		// 移動ベクトルをカメラの角度だけ回転させる
		move = TransformNormal(move, matRotate);

		// 移動
		worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);

		// 回転
		worldTransformBase_.rotation_.y = float(std::atan2(double(move.x), double(move.z)));
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		behaviorRequest_ = Behavior::kAttack;
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
