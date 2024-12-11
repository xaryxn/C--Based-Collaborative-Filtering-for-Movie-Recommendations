#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;


double cosineSimilarity(const vector<int>& user1, const vector<int>& user2) {
    double dotProduct = 0, norm1 = 0, norm2 = 0;
    for (size_t i = 0; i < user1.size(); ++i) {
        dotProduct += user1[i] * user2[i];
        norm1 += user1[i] * user1[i];
        norm2 += user2[i] * user2[i];
    }
    return dotProduct / (sqrt(norm1) * sqrt(norm2) + 1e-9); 
}
double predictRating(int user, int movie, const vector<vector<int>>& ratings, const vector<vector<double>>& similarities) {
    double weightedSum = 0, similaritySum = 0;
    for (size_t i = 0; i < ratings.size(); ++i) {
        if (i != user && ratings[i][movie] != 0) { 
            weightedSum += similarities[user][i] * ratings[i][movie];
            similaritySum += fabs(similarities[user][i]);
        }
    }
    return (similaritySum > 0) ? weightedSum / similaritySum : 0.0;
}

vector<pair<int, double>> recommendMovies(int user, const vector<vector<int>>& ratings, const vector<vector<double>>& similarities) {
    vector<pair<int, double>> recommendations;
    for (size_t movie = 0; movie < ratings[user].size(); ++movie) {
        if (ratings[user][movie] == 0) { 
            double predictedRating = predictRating(user, movie, ratings, similarities);
            recommendations.emplace_back(movie, predictedRating);
        }
    }
    
    sort(recommendations.begin(), recommendations.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });
    return recommendations;
}

int main() {
    vector<vector<int>> ratings = {
        {5, 3, 0, 1},
        {4, 0, 0, 1},
        {1, 1, 0, 5},
        {0, 0, 5, 4},
        {0, 3, 4, 0}
    };

    int numUsers = ratings.size();
    int numMovies = ratings[0].size();

    vector<vector<double>> similarities(numUsers, vector<double>(numUsers, 0));
    for (int i = 0; i < numUsers; ++i) {
        for (int j = 0; j < numUsers; ++j) {
            if (i != j) {
                similarities[i][j] = cosineSimilarity(ratings[i], ratings[j]);
            }
        }
    }

    int targetUser = 0; 
    vector<pair<int, double>> recommendations = recommendMovies(targetUser, ratings, similarities);

    cout << "Recommended movies for user " << targetUser << ":\n";
    for (const auto& rec : recommendations) {
        cout << "Movie " << rec.first << " with predicted rating: " << rec.second << endl;
    }

    return 0;
}
