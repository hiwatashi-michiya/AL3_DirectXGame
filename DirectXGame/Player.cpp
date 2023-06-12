#include "Player.h"
#include <cassert>
#include <ImGuiManager.h>

Player::Player() {}

Player::~Player() {
	//bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	//NULLポインタチェック
	assert(model);
	assert(textureHandle);
	//メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = position;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("reticle.png");
	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f,0.5f});

}

void Player::Update(ViewProjection viewProjection) {

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.5f;

	//移動限界座標
	const float kMoveLimitX = 33.0f;
	const float kMoveLimitY = 18.0f;

	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//押した方向で移動ベクトルを変更(回転)
	if (input_->PushKey(DIK_A)) {

		if (worldTransform_.rotation_.y > -10.0f) {
			worldTransform_.rotation_.y -= kRotSpeed;
		}
		
	}
	else if (input_->PushKey(DIK_D)) {
		
		if (worldTransform_.rotation_.y < 10.0f) {
			worldTransform_.rotation_.y += kRotSpeed;
		}

	}

	//押した方向で移動ベクトルを変更(左右)
	/*if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}*/

	// 押した方向で移動ベクトルを変更(上下)
	/*if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}*/

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}



	//座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 3Dレティクル
	Set3DReticle(viewProjection);

	//キャラクター攻撃処理
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		//Rトリガーを押していたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			Attack();
		}

	}
	

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	//{
	//	//自機から3Dレティクルへの距離
	//	const float kDistancePlayerTo3DReticle = 50.0f;
	//	//自機から3Dレティクルへのオフセット(Z+向き)
	//	Vector3 offset = {0, 0, 1.0f};
	//	//自機のワールド座標の回転を反映
	//	offset = TransformNormal(offset, worldTransform_.matWorld_);
	//	//ベクトルの長さを整える
	//	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	//	//3Dレティクルの座標を設定
	//	worldTransform3DReticle_.translation_ = Add(worldTransform_.translation_, offset);
	//	//行列の更新と転送
	//	worldTransform3DReticle_.UpdateMatrix();
	//}

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	//{
	//	Vector3 positionReticle = Vector3(
	//	worldTransform3DReticle_.matWorld_.m[3][0],
	//	worldTransform3DReticle_.matWorld_.m[3][1],
	//    worldTransform3DReticle_.matWorld_.m[3][2]);

	//	//ビューポート行列
	//	Matrix4x4 matViewport = 
	//		MakeViewportMatrix(0,0,WinApp::kWindowWidth, WinApp::kWindowHeight,0,1);

	//	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//	Matrix4x4 matViewProjectionViewport =
	//	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	//	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	//	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	//	//スプライトのレティクルに座標設定
	//	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	//}

	

}

void Player::Draw(ViewProjection viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);

	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::DrawUI() {

	//2Dレティクル描画
	sprite2DReticle_->Draw();

}

void Player::Attack() {

	if (coolTimeAttack_ <= 0) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity{};
		// 自機から照準オブジェクトへのベクトル
		velocity = Subtract(
		    Vector3(
		        worldTransform3DReticle_.matWorld_.m[3][0],
		        worldTransform3DReticle_.matWorld_.m[3][1],
		        worldTransform3DReticle_.matWorld_.m[3][2]),
		    Vector3(
				worldTransform_.matWorld_.m[3][0],
				worldTransform_.matWorld_.m[3][1],
		        worldTransform_.matWorld_.m[3][2]));
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		Vector3 worldPos(
		    worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
		    worldTransform_.matWorld_.m[3][2]);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldPos, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);

		// クールタイムの設定
		coolTimeAttack_ = 5;

	}
	else {
		coolTimeAttack_--;
	}

	

}

Vector3 Player::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

void Player::OnCollision() {

}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
	/*worldTransform3DReticle_.parent_ = parent;*/
}

void Player::Set3DReticle(ViewProjection viewProjection) {

	//POINT mousePosition;
	////マウス座標(スクリーン座標)を取得する
	//GetCursorPos(&mousePosition);

	////クライアントエリア座標に変換する
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);

	////マウス座標を2Dレティクルのスプライトに代入する
	//sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));

	//スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyStateReticle;

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyStateReticle)) {
		spritePosition.x += (float)joyStateReticle.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyStateReticle.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		// スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	}

	//ビューポート行列作成
	Matrix4x4 matViewport = MakeViewportMatrix(0,0,WinApp::kWindowWidth, WinApp::kWindowHeight,0,1);

	//ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3(spritePosition.x, spritePosition.y, 0);
	Vector3 posFar = Vector3(spritePosition.x, spritePosition.y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	//レイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	//カメラから標準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_ = Add(posNear, Multiply(kDistanceTestObject,mouseDirection));
	
	worldTransform3DReticle_.matWorld_ = MakeAffineMatrix(
		worldTransform3DReticle_.scale_,
		worldTransform3DReticle_.rotation_,
		worldTransform3DReticle_.translation_);

	worldTransform3DReticle_.UpdateMatrix();

	// 値確認用
	ImGui::Begin("Player");
	ImGui::Text("Player WorldPos:(%+.2f,%+.2f,%+.2f)", worldTransform_.matWorld_.m[3][0],
	    worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	ImGui::Text(
	    "2DReticle:(%1.3f,%1.3f)", sprite2DReticle_->GetPosition().x,
	    sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f, %+.2f, %+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
		worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();

}
