#pragma once
#include <vector>

//アプリケーション
#include <application/map/CsvLoader.h>

class RedoUndoSystem {
public:
	//初期マップ読み込み
	void Initialize(const CsvMapData& initialMap);

	//Redo呼び出し(成功ならtrue,失敗ならfalseを返す)
	bool Redo();
	//Undo呼び出し(成功ならtrue,失敗ならfalseを返す)
	bool Undo();
	//マップ状態反映用関数(Redo,Undo処理後に行う)
	CsvMapData reflectionMapState();

	//プレイヤーの行動後新たにリストに履歴を追加する関数
	void AddNewHistory(const CsvMapData& addMap);

private:
	// 現在の変更履歴位置
	int currentHistoryIndex_ = 0;
	// 変更履歴リスト
	std::vector<CsvMapData> redoUndoList_;
};

