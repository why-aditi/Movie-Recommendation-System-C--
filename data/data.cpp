#include <iostream>
#include <fstream>

using namespace std;

const int NUM_USERS = 400;
const int NUM_MOVIES = 500;
const string OUTPUT_FILE = "rating.csv";

int generateRating()
{
    int r = rand() % 10;
    if (r < 4)
        return 0;
    return rand() % 5 + 1;
}

int main()
{
    ofstream outFile(OUTPUT_FILE);
    if (!outFile.is_open())
    {
        cerr << "Error: Unable to open file " << OUTPUT_FILE << std::endl;
        return 1;
    }

    for (int i = 0; i < NUM_USERS; ++i)
    {
        for (int j = 0; j < NUM_MOVIES; ++j)
        {
            int rating = generateRating();
            outFile << rating;
            if (j < NUM_MOVIES - 1)
                outFile << ",";
        }
        outFile << "\n";
    }

    outFile.close();

    cout << "Matrix successfully saved to " << OUTPUT_FILE << endl;

    return 0;
}
