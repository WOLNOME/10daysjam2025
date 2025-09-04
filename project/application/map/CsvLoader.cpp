#include "CsvLoader.h"
#include <algorithm>

namespace csv_detail {

    inline void trim(std::string& s) {
        auto isSp = [](unsigned char c) { return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            [&](unsigned char c) { return !isSp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(),
            [&](unsigned char c) { return !isSp(c); }).base(), s.end());
    }

    inline bool isBlankCsvLine(const std::string& line) {
        for (char c : line) if (c != ',' && !std::isspace((unsigned char)c)) return false;
        return true;
    }

    inline std::string firstCell(const std::string& line) {
        auto p = line.find(',');
        return (p == std::string::npos) ? line : line.substr(0, p);
    }

   
    inline std::string stripBOM(std::string s) {
        if (s.size() >= 3 &&
            (unsigned char)s[0] == 0xEF && (unsigned char)s[1] == 0xBB && (unsigned char)s[2] == 0xBF) {
            s.erase(0, 3);
        }
        return s;
    }

    inline std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return (char)std::tolower(c); });
        return s;
    }

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
	std::string path = "Resources/mapData/" + filepath;
    std::ifstream ifs(path);
    if (!ifs) {
        throw std::runtime_error("CsvLoader: failed to open " + filepath);
    }

    // レイヤーごとの一次バッファ
    std::vector<std::vector<MapChipType>> layer1, layer2, layerSpawn;
    int curLayer = -1; // -1: 未決定, 0: Layer1, 1: Layer2, 2: Spawn

    std::string line;
    while (std::getline(ifs, line)) {
       
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (csv_detail::isBlankCsvLine(line)) continue;

        
        std::string head = csv_detail::toLower(csv_detail::stripBOM(csv_detail::firstCell(line)));
        if (head == "layer1") { curLayer = 0; continue; }
        if (head == "layer2") { curLayer = 1; continue; }
        if (isSpawnHeader(head)) { curLayer = 2; continue; }
        if (curLayer < 0) continue;   

        std::stringstream ss(line);
        std::string cell;
        std::vector<MapChipType> row;
        while (std::getline(ss, cell, ',')) {
            csv_detail::trim(cell);
            if (cell.empty()) { row.push_back(MapChipType::Empty); continue; }
            int v = 0;
            try { v = std::stoi(cell); }
            catch (...) { v = 0; }
            if (v < 0 || v >= (int)MapChipType::kMaxNumMapChipType) v = 0;
            row.push_back(static_cast<MapChipType>(v));
        }

        // 今のレイヤーに追加
        if (curLayer == 0) layer1.push_back(std::move(row));
        else if (curLayer == 1) layer2.push_back(std::move(row));
        else                    layerSpawn.push_back(std::move(row));
    }

    // 行の長さをそろえる
    csv_detail::normalize(layer1);
    csv_detail::normalize(layer2);
    csv_detail::normalize(layerSpawn);

    CsvMapData out{};

    
    out.height = (int)(!layer1.empty() ? layer1.size() : !layer2.empty() ? layer2.size() : layerSpawn.size());
    out.width = (out.height > 0)
        ? (int)std::max({ layer1.empty() ? 0 : layer1[0].size(),
                          layer2.empty() ? 0 : layer2[0].size(),
                          layerSpawn.empty() ? 0 : layerSpawn[0].size() })
        : 0;

    
    out.layer1 = std::move(layer1);
    out.layer2 = std::move(layer2);

auto scanSpawn = [&](const std::vector<std::vector<MapChipType>>& G) {
    for (int y = 0; y < (int)G.size(); ++y) {
        for (int x = 0; x < (int)G[y].size(); ++x) {
            switch (G[y][x]) {
            case MapChipType::StartDog:
                if (out.spawnDog.x < 0)    csv_detail::setGrid(out.spawnDog, x, y);
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
            if (t == MapChipType::StartDog && out.spawnDog.x < 0) { csv_detail::setGrid(out.spawnDog, x, y); G[y][x] = MapChipType::Empty; }
            if (t == MapChipType::StartMonkey && out.spawnMonkey.x < 0) { csv_detail::setGrid(out.spawnMonkey, x, y); G[y][x] = MapChipType::Empty; }
            // GoalDog / GoalMonkey は触らない（そのまま残す）
        }
    }
    };
if (out.spawnDog.x < 0 || out.spawnMonkey.x < 0) {
    fallbackScanAndClear(out.layer1);
    fallbackScanAndClear(out.layer2);
}

    return out;
}
