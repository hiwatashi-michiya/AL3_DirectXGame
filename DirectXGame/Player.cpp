#define _USE_MATH_DEFINES
#include "Player.h"
#include <cassert>
#include <math.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

Player::Player() {}

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {

	//基底クラスの初期化
	BaseCharacter::Initialize(models);

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

	//浮遊ギミック初期化
	InitializeFloatingGimmick();

}

void Player::Update() {

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		//速さ
		const float speed = 0.3f;

		//移動量。Lスティックの入力を取る
		Vector3 move = {float(joyState.Gamepad.sThumbLX), 0.0f, float(joyState.Gamepad.sThumbLY)};
		//移動量に速さを反映
		move = Multiply(speed, Normalize(move));

		Matrix4x4 matRotate = MakeRotateYMatrix(viewProjection_->rotation_.y);

		//移動ベクトルをカメラの角度だけ回転させる
		move = TransformNormal(move, matRotate);

		//移動
		worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);

		//回転
		worldTransformBase_.rotation_.y = float(std::atan2(double(move.x), double(move.z)));

	}

	//行数を定数バッファに転送
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	//浮遊ギミック更新
	UpdateFloatingGimmick();

}

void Player::Draw(const ViewProjection& viewProjection) {

	//3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

}

void Player::InitializeFloatingGimmick() {
	
	floatingParameter_ = 0.0f;

}

void Player::UpdateFloatingGimmick() {

#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Translation", &worldTransformHead_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderInt("cycle", &cycle, 10, 300);
	ImGui::SliderFloat("swing", &swing, 0.1f, 50.0f);
	ImGui::End();

#endif // _DEBUG

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
