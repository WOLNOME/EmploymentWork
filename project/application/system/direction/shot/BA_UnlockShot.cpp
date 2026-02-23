#include "BA_UnlockShot.h"

BA_UnlockShot::BA_UnlockShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
}

BA_UnlockShot::~BA_UnlockShot() {
}

void BA_UnlockShot::Initialize() {
}

void BA_UnlockShot::Update() {
}

void BA_UnlockShot::Finalize() {
}

void BA_UnlockShot::Debug() {
}
