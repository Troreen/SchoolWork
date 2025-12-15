#pragma once

class ScoreManager
{
public:
	ScoreManager();
	~ScoreManager();

	void Reset();
	void AddScore(int anAmount);
	int GetScore() const;

	int GetHighScore() const;
	void SetHighScore(int aHighScore);

private:
	int myScore = 0;
	int myHighScore = 0;
};