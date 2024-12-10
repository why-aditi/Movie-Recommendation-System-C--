#include <iostream>
#include <fstream>
#include <cstdlib>

const int NUM_USERS = 400;
const int NUM_MOVIES = 500;
const std::string OUTPUT_FILE = "train.csv";

int generateRating()
{
    int r = rand() % 10;
    if (r < 4)
        return 0;
    return rand() % 5 + 1;
}

int main()
{
    std::ofstream outFile(OUTPUT_FILE);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Unable to open file " << OUTPUT_FILE << std::endl;
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

    std::cout << "Matrix successfully saved to " << OUTPUT_FILE << std::endl;

    return 0;
}
