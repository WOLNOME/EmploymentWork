#include "BA_ShakeShot.h"

BA_ShakeShot::BA_ShakeShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
}

BA_ShakeShot::~BA_ShakeShot() {
}

void BA_ShakeShot::Initialize() {
}

void BA_ShakeShot::Update() {
}

void BA_ShakeShot::Finalize() {
}

void BA_ShakeShot::Debug() {
}
