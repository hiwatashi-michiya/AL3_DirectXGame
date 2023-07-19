#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <list>
#include "Enemy.h"
#include <DebugCamera.h>
#include <Input.h>
#include <vector>

//敵情報の構造体
struct EnemyData {

	WorldTransform worldTransform;
	int type;

};

class ScriptEditor {
public:
	ScriptEditor();
	~ScriptEditor();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Draw(ViewProjection viewProjection);

	//エディター開いているかどうかの判定
	bool GetIsEdit() { return isEdit_; }

	Matrix4x4 GetMatView() const { return matView_; }

	Matrix4x4 GetMatProjection() const { return matProjection_; }

private:

	bool isEdit_;

	std::list<EnemyData> enemyData_;

	Input* input_ = nullptr;

	Model* model_ = nullptr;

	//スクリプト編集モード用のカメラ
	Matrix4x4 matCamera_;
	
	//ビュー行列
	Matrix4x4 matView_;

	//プロジェクション行列
	Matrix4x4 matProjection_;

	//スケール
	Vector3 scale_;

	//回転
	Vector3 rotate_;

	//平行移動
	Vector3 translate_;

	//3Dレティクルのワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//カメラからレティクルへの距離
	float distance_;

	//3Dレティクルモデル
	Model* model3DReticle_ = nullptr;

	//テクスチャハンドル
	uint32_t enemyEasyTex_ = 0u;
	uint32_t enemyNormalTex_ = 0u;
	uint32_t enemyHardTex_ = 0u;

	//敵タイプの文字列格納リスト
	std::vector<const char*> typeString_ = {"EASY", "NORMAL", "HARD"};
	

};


