#pragma once
#include <string>
#include <vector>



struct HighScoreEntry {
    std::string name;
    int score;
};

class HighScoreManager {
public:
    void InitFromDefault(int defaultScore);
    void LoadFromFile(const std::string& path);
    void SaveToFile(const std::string& path) const;

    const std::vector<HighScoreEntry>& GetEntries() const;
    bool TryInsert(const HighScoreEntry& entry); // returns true if made top 3
private:
    std::vector<HighScoreEntry> myEntries; // size 3
};
