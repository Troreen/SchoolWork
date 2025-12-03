#include "HighScoreManager.h"

#include <algorithm>
#include <fstream>
#include <sstream>

void HighScoreManager::InitFromDefault(int defaultScore)
{
    myEntries.clear();
    myEntries.push_back({ "AAA", defaultScore });
    myEntries.push_back({ "BBB", defaultScore / 2 });
    myEntries.push_back({ "CCC", defaultScore / 4 });
}

void HighScoreManager::LoadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        return;
    }

    myEntries.clear();
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        HighScoreEntry entry;
        ss >> entry.name >> entry.score;
        if (!entry.name.empty())
        {
            myEntries.push_back(entry);
        }
    }

    std::sort(myEntries.begin(), myEntries.end(), [](const HighScoreEntry& a, const HighScoreEntry& b)
    {
        return a.score > b.score;
    });

    while (myEntries.size() < 3)
    {
        myEntries.push_back({ "---", 0 });
    }
    if (myEntries.size() > 3)
    {
        myEntries.resize(3);
    }
}

void HighScoreManager::SaveToFile(const std::string& path) const
{
    std::ofstream file(path, std::ios::trunc);
    if (!file.is_open())
    {
        return;
    }

    for (const auto& entry : myEntries)
    {
        file << entry.name << " " << entry.score << "\n";
    }
}

const std::vector<HighScoreEntry>& HighScoreManager::GetEntries() const
{
    return myEntries;
}

bool HighScoreManager::TryInsert(const HighScoreEntry& entry)
{
    myEntries.push_back(entry);

    std::sort(myEntries.begin(), myEntries.end(), [](const HighScoreEntry& a, const HighScoreEntry& b)
    {
        return a.score > b.score;
    });

    if (myEntries.size() > 3)
    {
        myEntries.resize(3);
    }

    return std::find_if(myEntries.begin(), myEntries.end(), [&](const HighScoreEntry& e)
    {
        return e.name == entry.name && e.score == entry.score;
    }) != myEntries.end();
}
