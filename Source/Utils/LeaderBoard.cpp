
#include "LeaderBoard.hpp"
#include <fstream>
#include <iostream>

LeaderBoard::LeaderBoard() {
    load();
}

void LeaderBoard::load() {
    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        nlohmann::json jsonData;
        inputFile >> jsonData;
        scores = jsonData.get<std::vector<ScoreEntry>>();
        inputFile.close();
    }
}

void LeaderBoard::save() {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        nlohmann::json jsonData = scores;
        outputFile << jsonData.dump(4);
        outputFile.close();
    }
}

void LeaderBoard::addScore(const std::string& playerName, int score) {
    ScoreEntry newEntry{ playerName, score };
    scores.push_back(newEntry);
    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });
    if (scores.size() > 7) {
        scores.pop_back();
    }
    save();
}

void LeaderBoard::print() const {
    std::cout << "LeaderBoard:\n";
    for (const auto& entry : scores) {
        std::cout << entry.playerName << ": " << entry.score << "\n";
    }
}

std::vector<ScoreEntry> LeaderBoard::getTopScores() const {
    return scores;
}

void LeaderBoard::setFilename(const std::string& newFilename) {
    filename = newFilename;
}

ScoreEntry LeaderBoard::getNearestScore(int currentScore) const {
    ScoreEntry nearest = scores.front();
    for (const auto& entry : scores) {
        if (entry.score < currentScore) {
            nearest = entry;
            break;
        }
    }
    return nearest;
}
