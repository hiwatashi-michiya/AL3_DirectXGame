#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>

class RailCamera {
public:
	RailCamera();
	~RailCamera();

	void Initialize(const Vector3& transform, const Vector3& radian);

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

};

