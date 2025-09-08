#include "RedoUndoSystem.h"
#include <SpriteManager.h>
#include <TextTextureManager.h>

void RedoUndoSystem::Initialize(const CsvMapData& initialMap) {
	//初期マップの読み込み
	redoUndoList_.push_back(initialMap);

	//テキスト
	TextParam textParam;
	textParam.text = L"[Z]・(LB):Redo [Y]・(RB):Undo";
	textParam.font = Font::UDDegitalN_B;
	textParam.fontStyle = FontStyle::Normal;
	textParam.size = 24.0f;
	textParam.color = { 1.0f,1.0f,1.0f,1.0f };
	textTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
	textSprite_ = std::make_unique<Sprite>();
	textSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("redoUndoText"), Sprite::Order::Front2);
	textSprite_->SetTexture(textTexture_);
	textSprite_->SetPosition({ 20,20 });
	textSprite_->SetAnchorPoint({ 0.0f,0.0f });
	textSprite_->SetIsDisplay(true);

}

bool RedoUndoSystem::Redo() {
	//現在の変更履歴を下げる
	if (currentHistoryIndex_ > 0) {
		currentHistoryIndex_--;
		return true;
	}
	return false;
}

bool RedoUndoSystem::Undo() {
	//現在の変更履歴を上げる
	if (currentHistoryIndex_ < redoUndoList_.size() - 1) {
		currentHistoryIndex_++;
		return true;
	}
	return false;
}

CsvMapData RedoUndoSystem::reflectionMapState() {
	return redoUndoList_[currentHistoryIndex_];
}

void RedoUndoSystem::AddNewHistory(const CsvMapData& addMap) {
	//現在の変更履歴に合わせてサイズをそろえる
	redoUndoList_.resize(currentHistoryIndex_ + 1);
	//新しいマップ情報を入力
	redoUndoList_.push_back(addMap);
	//現在の変更履歴位置を更新
	currentHistoryIndex_++;
}
