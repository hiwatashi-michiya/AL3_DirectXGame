#include "Player.h"
#include <cassert>

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model) {

	//NULLポインタチェック
	assert(model);
	//メンバ変数に記録
	model_ = model;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 1.0f;

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
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);

		//回転
		worldTransform_.rotation_.y = float(std::atan2(double(move.x), double(move.z)));

	}

	//行数を定数バッファに転送
	worldTransform_.UpdateMatrix();

}

void Player::Draw(ViewProjection viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);

}
