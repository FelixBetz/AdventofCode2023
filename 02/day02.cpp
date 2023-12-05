#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>

#define INPUT_FILE "../../input.txt"

#define NUM_RED_CUBES 12
#define NUM_GREEN_CUBES 13
#define NUM_BLUE_CUBES 14

//------------------------------------------------------------------------------
//   main
//------------------------------------------------------------------------------
int main(int, char **)
{
    // Open the file
    std::ifstream inputFile(INPUT_FILE);

    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return 1;
    }

    std::string line;

    int gameIdSum = 0;
    int powerSum = 0;
    int gameId = 1;
    while (std::getline(inputFile, line))
    {

        int cntRedCubes = 0;
        int cntGreenCubes = 0;
        int cntBlueCubes = 0;

        std::istringstream inputStream(line);

        std::string removedGameId;

        // remove leading game id "Game X:"
        std::getline(inputStream, removedGameId, ':');
        std::getline(inputStream, removedGameId, ':');
        std::replace(removedGameId.begin(), removedGameId.end(), ';', ',');

        std::istringstream cubes(removedGameId);
        std::string test;

        while (std::getline(cubes, test, ','))
        {
            std::istringstream iss(test);
            int extractedNumber;
            iss >> extractedNumber;
            if (std::string::npos != test.find("blue"))
            {
                cntBlueCubes = std::max(cntBlueCubes, extractedNumber);
            }
            else if (std::string::npos != test.find("green"))
            {
                cntGreenCubes = std::max(cntGreenCubes, extractedNumber);
            }
            else if (std::string::npos != test.find("red"))
            {
                cntRedCubes = std::max(cntRedCubes, extractedNumber);
            }
        }

        // Part1
        if (cntRedCubes <= NUM_RED_CUBES && cntGreenCubes <= NUM_GREEN_CUBES && cntBlueCubes <= NUM_BLUE_CUBES)
        {
            gameIdSum += gameId;
        }

        // Part2
        powerSum += cntRedCubes * cntBlueCubes * cntGreenCubes;

        gameId++;
    }
    // Close the file
    inputFile.close();
    std::cout << "PART1: sum of the IDs of those games: " << gameIdSum << std::endl;
    std::cout << "PART2: sum of the power: " << powerSum << std::endl;
    return 0;
}
