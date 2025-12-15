#include "ScoreManager.h"

ScoreManager::ScoreManager()
{
	Reset();
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::Reset()
{
	myScore = 0;
}

void ScoreManager::AddScore(int anAmount)
{
	myScore += anAmount;
}

int ScoreManager::GetScore() const
{
	return myScore;
}

int ScoreManager::GetHighScore() const
{
	return myHighScore;
}

void ScoreManager::SetHighScore(int aHighScore)
{
	myHighScore = aHighScore;
}

