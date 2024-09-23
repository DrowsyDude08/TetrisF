#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

struct ScoreEntry {
    std::string playerName;
    int score;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ScoreEntry, playerName, score)
};

class LeaderBoard {
private: 
    std::vector<ScoreEntry> scores;
   
    std::string filename = "scores.json";

public:
    LeaderBoard();
    void load();
    void save();
    void addScore(const std::string& playerName, int score);
    void print() const;
    std::vector<ScoreEntry> getTopScores() const;
    ScoreEntry getNearestScore(int currentScore) const;
    void setFilename(const std::string& newFilename);
};

#endif // LEADERBOARD_HPP
