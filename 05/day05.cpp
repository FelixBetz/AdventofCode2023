#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include <algorithm>

#define INPUT_FILE "../../input.txt"

#define NUM_SLICE_SEED 6 // 8
//------------------------------------------------------------------------------
//   mapSeed
//------------------------------------------------------------------------------
static long long mapSeed(const long long pSeed, const std::vector<std::vector<long long>> &pMap)
{
    for (auto &el : pMap)
    {
        if (pSeed >= el[1] && pSeed < el[1] + el[2])
        {
            return el[0] + (pSeed - el[1]);
        }
    }

    return pSeed;
}

//------------------------------------------------------------------------------
//   parseInput
//------------------------------------------------------------------------------
static void parseInput(const char *pFileName, std::vector<long long> *pSeeds,
                       std::vector<std::vector<std::vector<long long>>> *pMaps)
{
    // Open the file
    std::ifstream inputFile(INPUT_FILE);

    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return;
    }

    std::vector<int> indices;
    std::vector<std::string> inputLines;

    std::string line;
    int idx = 0;
    while (std::getline(inputFile, line))
    {
        inputLines.push_back(line);

        if (std::string::npos != line.find("seeds"))
        {
            line.erase(0, NUM_SLICE_SEED);

            std::stringstream iss(line);
            long long number;
            while (iss >> number)
            {
                pSeeds->push_back(number);
            }
        }

        if (std::string::npos != line.find(":"))
        {
            indices.push_back(idx + 1);
        }

        idx++;
    }
    inputFile.close();

    // add last index
    indices.push_back(idx + 2);

    // parse maps

    for (int mapIdx = 0; mapIdx < indices.size() - 1; mapIdx++)
    {
        int startIdx = indices.at(mapIdx);
        int endIdx = indices.at(mapIdx + 1) - 2;
        std::vector<std::vector<long long>> map;
        for (int lineIdx = startIdx; lineIdx < endIdx; lineIdx++)
        {
            std::stringstream iss(inputLines.at(lineIdx));
            std::vector<long long> numbers;

            long long number;
            while (iss >> number)
            {
                numbers.push_back(number);
            }

            std::vector<long long> range;
            range.push_back(numbers.at(0));
            range.push_back(numbers.at(1));
            range.push_back(numbers.at(2));

            map.push_back(range);
        }
        pMaps->push_back(map);
    }
}

//------------------------------------------------------------------------------
//   solvePart1
//------------------------------------------------------------------------------
static void solvePart1(std::vector<long long> &pSeeds,
                       std::vector<std::vector<std::vector<long long>>> &pMaps)
{
    // solve part1
    long long part1MinLocation = std::numeric_limits<long long>::max();
    for (auto seed : pSeeds)
    {
        for (auto map : pMaps)
        {
            seed = mapSeed(seed, map);
        }
        if (seed < part1MinLocation)
        {
            part1MinLocation = seed;
        }
    }
    std::cout << "PART1:lowest location number : " << part1MinLocation << std::endl;
}

//------------------------------------------------------------------------------
//   solvePart2
//------------------------------------------------------------------------------
static void solvePart2(std::vector<long long> &pSeeds,
                       std::vector<std::vector<std::vector<long long>>> &pMaps)
{
    long long part2MinLocation = std::numeric_limits<long long>::max();
    for (int i = 0; i < pSeeds.size(); i += 2)
    {
        for (long long j = pSeeds[i]; j < pSeeds[i] + pSeeds[i + 1]; j += 1)
        {
            long long seed = j;
            for (auto &map : pMaps)
            {
                seed = mapSeed(seed, map);
            }

            if (seed < part2MinLocation)
            {
                part2MinLocation = seed;
            }
        }
    }

    std::cout << "PART2: lowest location number : " << part2MinLocation << std::endl;
}

//------------------------------------------------------------------------------
//   main
//------------------------------------------------------------------------------
int main(int, char **)
{
    std::vector<long long> seeds;
    std::vector<std::vector<std::vector<long long>>> maps;
    parseInput(INPUT_FILE, &seeds, &maps);
    solvePart1(seeds, maps);
    solvePart2(seeds, maps);

    return 0;
}
