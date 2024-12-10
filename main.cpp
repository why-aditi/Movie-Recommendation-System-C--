#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const int NUM_USERS = 400;
const int NUM_MOVIES = 500;
const string INPUT_FILE = "data/rating.csv";

void loadRatings(const string &filePath, vector<std::vector<int>> &ratings)
{
    ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        cerr << "Error: Unable to open file " << filePath << endl;
        exit(1);
    }

    for (int i = 0; i < NUM_USERS; ++i)
    {
        ratings.push_back(std::vector<int>(NUM_MOVIES, 0));
        for (int j = 0; j < NUM_MOVIES; ++j)
        {
            inFile >> ratings[i][j];
            if (inFile.peek() == ',')
                inFile.ignore();
        }
    }
}

double computeSimilarity(const vector<int> &user1, const vector<int> &user2)
{
    double dotProduct = 0.0, magnitude1 = 0.0, magnitude2 = 0.0;
    for (size_t i = 0; i < user1.size(); ++i)
    {
        dotProduct += user1[i] * user2[i];
        magnitude1 += user1[i] * user1[i];
        magnitude2 += user2[i] * user2[i];
    }
    return magnitude1 && magnitude2 ? dotProduct / (sqrt(magnitude1) * sqrt(magnitude2)) : 0.0;
}

vector<double> predictRatings(const vector<vector<int>> &ratings, int targetUser)
{
    vector<double> predictedRatings(NUM_MOVIES, 0.0);
    vector<double> similarityScores(NUM_USERS, 0.0);

    for (int i = 0; i < NUM_USERS; ++i)
    {
        if (i != targetUser)
            similarityScores[i] = computeSimilarity(ratings[targetUser], ratings[i]);
    }

    for (int movie = 0; movie < NUM_MOVIES; ++movie)
    {
        if (ratings[targetUser][movie] == 0)
        {
            double weightedSum = 0.0, similaritySum = 0.0;
            for (int user = 0; user < NUM_USERS; ++user)
            {
                if (ratings[user][movie] > 0)
                {
                    weightedSum += similarityScores[user] * ratings[user][movie];
                    similaritySum += std::abs(similarityScores[user]);
                }
            }
            predictedRatings[movie] = similaritySum ? weightedSum / similaritySum : 0.0;
        }
    }

    return predictedRatings;
}

vector<int> rankMovies(const vector<double> &predictedRatings, int topN)
{
    vector<pair<double, int>> movieRatings;
    for (int i = 0; i < predictedRatings.size(); ++i)
    {
        if (predictedRatings[i] > 0.0)
            movieRatings.emplace_back(predictedRatings[i], i);
    }

    sort(movieRatings.rbegin(), movieRatings.rend());
    vector<int> recommendedMovies;
    for (int i = 0; i < topN && i < movieRatings.size(); ++i)
        recommendedMovies.push_back(movieRatings[i].second);

    return recommendedMovies;
}

double calculateRMSE(const std::vector<std::vector<int>> &ratings, const std::vector<std::vector<double>> &predictions)
{
    double errorSum = 0.0;
    int count = 0;

    for (int i = 0; i < NUM_USERS; ++i)
    {
        for (int j = 0; j < NUM_MOVIES; ++j)
        {
            if (ratings[i][j] > 0)
            {
                double error = ratings[i][j] - predictions[i][j];
                errorSum += error * error;
                count++;
            }
        }
    }

    return count ? sqrt(errorSum / count) : 0.0;
}

int main()
{
    vector<vector<int>> ratings;
    loadRatings(INPUT_FILE, ratings);

    int targetUser = 0;
    vector<double> predictedRatings = predictRatings(ratings, targetUser);

    int topN = 10;
    vector<int> recommendedMovies = rankMovies(predictedRatings, topN);

    cout << "Predicted ratings for user " << targetUser << ":\n";
    for (int movie : recommendedMovies)
        cout << "Movie " << movie << ": " << predictedRatings[movie] << "\n";

    vector<vector<double>> predictions(NUM_USERS, vector<double>(NUM_MOVIES, 0.0));
    for (int user = 0; user < NUM_USERS; ++user)
        predictions[user] = predictRatings(ratings, user);

    double rmse = calculateRMSE(ratings, predictions);
    cout << "RMSE: " << rmse << "\n";

    return 0;
}
