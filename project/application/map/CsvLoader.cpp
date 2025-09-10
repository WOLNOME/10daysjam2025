#include "CsvLoader.h"
#include <algorithm>

namespace csv_detail {
    // 文字列の前後の空白文字の削除
    inline void trim(std::string& s) {
        // std::isspaceで空白文字の判定
        auto isSp = [](unsigned char c) { return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            [&](unsigned char c) { return !isSp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(),
            [&](unsigned char c) { return !isSp(c); }).base(), s.end());
    }
    // 行が空白とカンマだけで構成されていたら ture （空行と判断）
    inline bool isBlankCsvLine(const std::string& line) {
        for (char c : line) if (c != ',' && !std::isspace((unsigned char)c)) return false;
        return true;
    }
    // １行の最初のセル（カンマの部分）を取り出す
    inline std::string firstCell(const std::string& line) {
        auto p = line.find(',');
        return (p == std::string::npos) ? line : line.substr(0, p);
    }

	// UTF-8 BOM を取り除く
    inline std::string stripBOM(std::string s) {
        if (s.size() >= 3 &&
            (unsigned char)s[0] == 0xEF && (unsigned char)s[1] == 0xBB && (unsigned char)s[2] == 0xBF) {
            s.erase(0, 3);
        }
        return s;
    }
    // 文字列を小文字化
    inline std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return (char)std::tolower(c); });
        return s;
    }
    // 行ごとに長さが違った時最大の長さに合わせて足りない部分を Emptyで埋める
    inline void normalize(std::vector<std::vector<MapChipType>>& grid) {
        size_t maxW = 0;
        for (auto& r : grid) maxW = std::max(maxW, r.size());
        for (auto& r : grid) r.resize(maxW, MapChipType::Empty);
    }

    // Vector2 が float でも int でもそのまま書ける汎用代入
    inline void setGrid(Vector2& v, int gx, int gy) {
        v.x = (decltype(v.x))gx;
        v.y = (decltype(v.y))gy;
    }

}

static bool isSpawnHeader(std::string h) {
    h = csv_detail::toLower(h);
    return (h == "spawn" || h == "spawns" || h == "marker" || h == "markers"
        || h == "layer3" || h == "layer_spawn" || h == "spawnlayer");
}




void CsvLoader::Initialize() {


}

CsvMapData CsvLoader::Loader(const std::string& filepath)
{
    // --- パス決定：拡張子あり/なし両対応 ---
    std::string path;
    if (filepath.size() >= 4) {
        const std::string ext = filepath.substr(filepath.size() - 4);
        if (ext == ".csv" || ext == ".CSV") path = "Resources/mapData/" + filepath;
        else                                 path = "Resources/mapData/" + filepath + ".csv";
    } else {
        path = "Resources/mapData/" + filepath + ".csv";
    }

    std::ifstream ifs(path);
    if (!ifs) {
        throw std::runtime_error("CsvLoader: failed to open " + path);
    }

    // ---- ヘルパ：キー正規化（BOM→trim→lower→空白/下線/ハイフン除去→末尾記号除去）----
    auto normalizeKey = [](std::string s) {
        s = csv_detail::stripBOM(s);
        csv_detail::trim(s);
        // 末尾に付く ":" "=" および全角版「：」「＝」を除去
        while (!s.empty()) {
            unsigned char b = (unsigned char)s.back();
            if (b == ':' || b == '=') { s.pop_back(); continue; }
            if (s.size() >= 3 &&
                (unsigned char)s[s.size() - 3] == 0xEF &&
                (unsigned char)s[s.size() - 2] == 0xBC &&
                ((unsigned char)s[s.size() - 1] == 0x9A /*：*/ ||
                    (unsigned char)s[s.size() - 1] == 0x9D /*＝*/)) {
                s.erase(s.end() - 3, s.end());
                continue;
            }
            break;
        }
        csv_detail::trim(s);
        s = csv_detail::toLower(s);
        // 空白/タブ/アンダースコア/ハイフンを除去（"camera_type" や "camera type" も "cameratype" に）
        s.erase(std::remove_if(s.begin(), s.end(),
            [](unsigned char c) { return c == ' ' || c == '\t' || c == '_' || c == '-'; }), s.end());
        return s;
        };

    // ---- ヘルパ：カメラ数値トークンのパース（数字のみ対応：0..4 / 1..5）----
    auto parseCameraCodeToken = [](std::string token) -> int {
        token = csv_detail::stripBOM(token);
        csv_detail::trim(token);
        // 余計な引用符や区切り記号を軽く除去
        token.erase(std::remove_if(token.begin(), token.end(),
            [](unsigned char c) { return c == '"' || c == '\'' || c == ':' || c == '='; }), token.end());
        csv_detail::trim(token);

        long val = 0;
        bool seen = false;
        size_t i = 0;
        while (i < token.size() && std::isspace((unsigned char)token[i])) ++i;
        for (; i < token.size(); ++i) {
            unsigned char ch = (unsigned char)token[i];
            if (std::isdigit(ch)) { seen = true; val = val * 10 + (ch - '0'); } else if (seen) break; // 最初の整数だけ読む
        }
        if (!seen) return 0; // 数字なしは既定A=0

        int n = (int)val;
        if (0 <= n && n <= 4) return n;     // 0..4 をそのまま
        if (1 <= n && n <= 5) return n - 1; // 1..5 を 0..4 に変換
        return 0; // 範囲外は既定A=0
        };

    std::vector<std::vector<MapChipType>> layer1, layer2, layerSpawn;
    int  curLayer = 0;               // ヘッダー無しでも layer1 として読む
    int  parsedCameraCode = 0;       // 既定 A=0
    bool expectCameraValue = false;  // 「次の行で数値を拾う」用

    std::string line;
    while (std::getline(ifs, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // --- 「CameraType の次の行」の値を最優先で拾う（空行はスキップしてよい）---
        if (expectCameraValue) {
            // 空行ならスキップして次の行へ（値が出るまで待つ）
            if (csv_detail::isBlankCsvLine(line)) continue;

            // 先頭の非空セルの数値を採用
            std::stringstream ssv(line);
            std::string cell;
            bool got = false;
            while (std::getline(ssv, cell, ',')) {
                csv_detail::trim(cell);
                if (!cell.empty()) {
                    parsedCameraCode = parseCameraCodeToken(cell);
                    got = true;
                    break;
                }
            }
            expectCameraValue = false;
            if (got) continue; // この行は「値行」として消費。レイヤー扱いしない
            // 値が見つからなければ通常の判定へ落とす
        }

        // 空行はここでスキップ
        if (csv_detail::isBlankCsvLine(line)) continue;

        // ---- 行をセルに分割し、ヘッダーを“行全体”から探す ----
        std::stringstream ssline(line);
        std::string cell;
        std::vector<std::string> cells;
        while (std::getline(ssline, cell, ',')) {
            csv_detail::trim(cell);
            cells.push_back(cell);
        }

        // 1) まず最初のセルで従来通りのレイヤー/スポーン判定
        std::string head = normalizeKey(cells.empty() ? std::string() : cells[0]);

        if (head == "layer1") { curLayer = 0; continue; }
        if (head == "layer2") { curLayer = 1; continue; }
        if (isSpawnHeader(head)) { curLayer = 2; continue; }

        // 2) 次に、行内のどのセルにも "cameratype" / "camera" がないか探す
        int cameraKeyIndex = -1;
        for (int i = 0; i < (int)cells.size(); ++i) {
            std::string key = normalizeKey(cells[i]);
            if (key == "cameratype" || key == "camera") {
                cameraKeyIndex = i;
                break;
            }
        }
        if (cameraKeyIndex >= 0) {
            // 同一行の「次セル」に値があれば採用、無ければ次の非空行で拾う
            if (cameraKeyIndex + 1 < (int)cells.size() && !cells[cameraKeyIndex + 1].empty()) {
                parsedCameraCode = parseCameraCodeToken(cells[cameraKeyIndex + 1]);
            } else {
                expectCameraValue = true; // 次の非空行の先頭セルから取得
            }
            continue; // この行はレイヤーデータではない
        }

        // ---- ここまで来たら、数値行として現在のレイヤーに格納 ----
        std::vector<MapChipType> row;
        for (auto& c : cells) {
            if (c.empty()) { row.push_back(MapChipType::Empty); continue; }
            int v = 0;
            try { v = std::stoi(c); }
            catch (...) { v = 0; }
            if (v < 0 || v >= (int)MapChipType::kMaxNumMapChipType) v = 0;
            row.push_back(static_cast<MapChipType>(v));
        }
        if (curLayer == 0) layer1.push_back(std::move(row));
        else if (curLayer == 1) layer2.push_back(std::move(row));
        else                    layerSpawn.push_back(std::move(row));
    }

    // ---- 行長の正規化 ----
    csv_detail::normalize(layer1);
    csv_detail::normalize(layer2);
    csv_detail::normalize(layerSpawn);

    // ---- 出力構築 ----
    CsvMapData out{};
    out.height = (int)(!layer1.empty() ? layer1.size()
        : !layer2.empty() ? layer2.size()
        : layerSpawn.size());
    out.width = (out.height > 0)
        ? (int)std::max({
            layer1.empty() ? 0 : (int)layer1[0].size(),
            layer2.empty() ? 0 : (int)layer2[0].size(),
            layerSpawn.empty() ? 0 : (int)layerSpawn[0].size()
            })
        : 0;

    out.layer1 = std::move(layer1);
    out.layer2 = std::move(layer2);

    // ---- Spawn から開始位置を検出（無ければ layer1/2 をフォールバック）----
    auto scanSpawn = [&](const std::vector<std::vector<MapChipType>>& G) {
        for (int y = 0; y < (int)G.size(); ++y) {
            for (int x = 0; x < (int)G[y].size(); ++x) {
                switch (G[y][x]) {
                case MapChipType::StartDog:
                    if (out.spawnDog.x < 0) csv_detail::setGrid(out.spawnDog, x, y);
                    break;
                case MapChipType::StartMonkey:
                    if (out.spawnMonkey.x < 0) csv_detail::setGrid(out.spawnMonkey, x, y);
                    break;
                default: break;
                }
            }
        }
        };
    scanSpawn(layerSpawn);

    auto fallbackScanAndClear = [&](std::vector<std::vector<MapChipType>>& G) {
        for (int y = 0; y < (int)G.size(); ++y) {
            for (int x = 0; x < (int)G[y].size(); ++x) {
                MapChipType t = G[y][x];
                if (t == MapChipType::StartDog && out.spawnDog.x < 0) {
                    csv_detail::setGrid(out.spawnDog, x, y);
                    G[y][x] = MapChipType::Empty;
                }
                if (t == MapChipType::StartMonkey && out.spawnMonkey.x < 0) {
                    csv_detail::setGrid(out.spawnMonkey, x, y);
                    G[y][x] = MapChipType::Empty;
                }
            }
        }
        };
    if (out.spawnDog.x < 0 || out.spawnMonkey.x < 0) {
        fallbackScanAndClear(out.layer1);
        fallbackScanAndClear(out.layer2);
    }

    // ---- カメラ数値コードを反映（0..4）----
    out.cameraCode = parsedCameraCode;

    return out;
}
