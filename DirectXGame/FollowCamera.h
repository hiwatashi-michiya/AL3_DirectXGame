#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Input.h>

class FollowCamera {
public:
	FollowCamera();
	~FollowCamera();

	void Initialize();

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void Reset();

private:

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//追従対象
	const WorldTransform* target_ = nullptr;

};


