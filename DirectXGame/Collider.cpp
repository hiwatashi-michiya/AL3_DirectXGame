#include "Collider.h"

Collider::Collider() {}

Collider::~Collider() {}

void Collider::OnCollision(Collider* collider) {

	SetColorType(collider->GetColorType());

}
