#include "DecoratorNodeBase.h"

DecoratorNodeBase::~DecoratorNodeBase() {
}

void DecoratorNodeBase::Initialize() {
	//基底クラスの初期化
	NodeBase::Initialize();
	//子ノードの初期化
	mChildNode->Initialize();
}

void DecoratorNodeBase::Finalize() {
	//基底クラスの終了
	NodeBase::Finalize();
	//子ノードの終了
	mChildNode->Finalize();
}
