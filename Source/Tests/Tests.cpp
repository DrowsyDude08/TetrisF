#include <gtest/gtest.h>
#include "../Utils/LeaderBoard.hpp"
#include <filesystem>
#include <fstream>

class LeaderBoardTest : public ::testing::Test {
protected:
    LeaderBoard leaderboard;

    void SetUp() override {
        std::ofstream("test_scores.json");
        leaderboard.setFilename("test_scores.json"); 
    }

    void TearDown() override {
        std::filesystem::remove("test_scores.json");
    }
};

TEST_F(LeaderBoardTest, AddScoreTest) {
    leaderboard.addScore("Player1", 100);
    leaderboard.addScore("Player2", 200);

    auto topScores = leaderboard.getTopScores();
    EXPECT_EQ(topScores.size(), 2);
    EXPECT_EQ(topScores[0].playerName, "Player2");
    EXPECT_EQ(topScores[0].score, 200);
    EXPECT_EQ(topScores[1].playerName, "Player1");
    EXPECT_EQ(topScores[1].score, 100);
}

TEST_F(LeaderBoardTest, AddScoreExceedLimit) {
    for (int i = 0; i < 10; ++i) {
        leaderboard.addScore("Player" + std::to_string(i), 100 - i);
    }

    auto topScores = leaderboard.getTopScores();
    EXPECT_EQ(topScores.size(), 7);
}

TEST_F(LeaderBoardTest, LoadAndSaveTest) {
    leaderboard.addScore("Player1", 100);
    leaderboard.addScore("Player2", 200);
    leaderboard.save();

    LeaderBoard newLeaderboard;
    newLeaderboard.setFilename("test_scores.json");
    newLeaderboard.load();

    auto topScores = newLeaderboard.getTopScores();
    EXPECT_EQ(topScores.size(), 2);
    EXPECT_EQ(topScores[0].playerName, "Player2");
    EXPECT_EQ(topScores[1].playerName, "Player1");
}

TEST_F(LeaderBoardTest, GetNearestScoreTest) {
    leaderboard.addScore("Player1", 100);
    leaderboard.addScore("Player2", 200);
    leaderboard.addScore("Player3", 150);

    ScoreEntry nearest = leaderboard.getNearestScore(175);
    EXPECT_EQ(nearest.playerName, "Player3");
    EXPECT_EQ(nearest.score, 150);
}