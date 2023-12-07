#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#define INPUT_FILE "../../input.txt"

#define NUM_SPLICE_TIME 5
#define NUM_SPLICE_DISTANCE 9

//------------------------------------------------------------------------------
//   parseInput
//------------------------------------------------------------------------------
static void parseInput(const char *pFileName, std::vector<int> *pTimes, std::vector<int> *pDistances)
{
    // Open the file
    std::ifstream inputFile(pFileName);

    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return;
    }

    std::string line;
    int idx = 0;
    while (std::getline(inputFile, line))
    {

        // parse Time row
        if (idx == 0)
        {
            line.erase(0, NUM_SPLICE_TIME);

            std::stringstream iss(line);
            int time;
            while (iss >> time)
            {
                pTimes->push_back(time);
            }
        } // parse Distance row
        else if (idx == 1)
        {
            line.erase(0, NUM_SPLICE_DISTANCE);

            std::stringstream iss(line);
            int distance;
            while (iss >> distance)
            {
                pDistances->push_back(distance);
            }
        }
        idx++;
    }
    inputFile.close();
}

//------------------------------------------------------------------------------
//   main
//------------------------------------------------------------------------------
int main(int, char **)
{

    std::vector<int> times, distances;
    parseInput(INPUT_FILE, &times, &distances);

    int raceWinsMultiply = 1;

    for (int r = 0; r < times.size(); r++)
    {

        int cntWins = 0;
        for (int i = 0; i <= times[r]; i++)
        {
            int chargeTime = i;
            int distance = (times[r] - chargeTime) * chargeTime;

            if (distance > distances[r])
            {
                cntWins++;
            }
        }
        raceWinsMultiply *= cntWins;
    }
    std::cout << "PART 1: multiplied race wins: " << raceWinsMultiply << std::endl;

    // part 2
    std::string timesStr;
    for (int i : times)
    {
        timesStr += std::to_string(i);
    }

    std::string distancesStr;
    for (int i : distances)
    {
        distancesStr += std::to_string(i);
    }

    long long part2Time = std::stoi(timesStr);
    long long part2Distance = std::stoll(distancesStr);

    raceWinsMultiply = 1;
    int cntWins = 0;
    for (long long i = 0; i <= part2Time; i++)
    {
        long long chargeTime = i;
        long long distance = (part2Time - chargeTime) * chargeTime;

        if (distance > part2Distance)
        {
            cntWins++;
        }
    }
    raceWinsMultiply *= cntWins;

    std::cout << "PART 2: multiplied race wins: " << raceWinsMultiply << std::endl;

    return 0;
}
