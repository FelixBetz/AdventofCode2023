#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define INPUT_FILE "../../input.txt"

//------------------------------------------------------------------------------
//   getSpaceOffset
//------------------------------------------------------------------------------
static long long getSpaceOffset(std::vector<int> *pEmpty, int idxA, int idxB, long long pGalaxyDistance)
{

    if (idxB < idxA)
    {
        int tmp = idxA;
        idxA = idxB;
        idxB = tmp;
    }

    long long distance = idxB - idxA;
    long long retOffset = 0;
    for (auto empty : *pEmpty)
    {
        if (empty >= idxA && empty <= idxB)
        {
            distance += pGalaxyDistance;
        }
    }

    return distance;
}

//------------------------------------------------------------------------------
//   calcShortestPathesSum
//------------------------------------------------------------------------------
static long long calcShortestPathesSum(std::vector<std::vector<char>> *pSpaceRows, int pGalaxyDistance)
{

    // parse galaxies
    int cntGalaxys = 1;
    std::vector<std::pair<int, int>> galaxies;
    for (int row = 0; row < pSpaceRows->size(); row++)
    {
        for (int col = 0; col < (*pSpaceRows)[row].size(); col++)
        {

            bool isEmptyCol = true;
            {

                if ((*pSpaceRows)[row][col] == '#')
                {
                    galaxies.push_back(std::make_pair(col, row));
                    cntGalaxys++;
                }
            }
        }
    }

    // calc empty rows
    std::vector<int> emptyRows;
    for (int row = 0; row < pSpaceRows->size(); row++)
    {
        bool isEmptyRow = true;
        for (auto ch : (*pSpaceRows)[row])
        {
            if (ch != '.')
            {
                isEmptyRow = false;
                break;
            }
        }
        if (isEmptyRow)
        {
            emptyRows.push_back(row);
        }
    }

    // calc empty col
    std::vector<int> emptyCols;
    for (int col = 0; col < (*pSpaceRows)[0].size(); col++)
    {
        bool isEmptyCol = true;
        for (auto row : *pSpaceRows)
        {
            if (row[col] != '.')
            {
                isEmptyCol = false;
                break;
            }
        }
        if (isEmptyCol)
        {
            emptyCols.push_back(col);
        }
    }

    // create pairs
    std::vector<std::pair<int, int>> pairs;

    for (int i = 0; i < galaxies.size() - 1; ++i)
    {
        for (int j = i + 1; j < galaxies.size(); ++j)
        {
            // Check if the pair is already present in the vector
            bool isDuplicate = false;
            for (const auto &pair : pairs)
            {
                if ((pair.first == i && pair.second == j) ||
                    (pair.first == j && pair.second == i))
                {
                    isDuplicate = true;
                    break;
                }
            }

            // If not a duplicate, add the pair to the vector
            if (!isDuplicate)
            {
                pairs.push_back(std::make_pair(i, j));
            }
        }
    }

    // create shortest pathes sum
    long long retSum = 0;
    for (auto pair : pairs)
    {

        long long xDistance = getSpaceOffset(&emptyCols, galaxies[pair.first].first, galaxies[pair.second].first, pGalaxyDistance);
        long long yDistance = getSpaceOffset(&emptyRows, galaxies[pair.first].second, galaxies[pair.second].second, pGalaxyDistance);
        retSum += xDistance + yDistance;
    }

    return retSum;
}

//------------------------------------------------------------------------------
//   parseInput
//------------------------------------------------------------------------------
static void parseInput(const char *pFileName, std::vector<std::vector<char>> *pSpaceRows)
{
    // Open the file
    std::ifstream inputFile(pFileName);

    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return;
    }

    std::string line;

    while (std::getline(inputFile, line))
    {
        std::vector<char> row;

        for (auto ch : line)
        {
            row.push_back(ch);
        }

        pSpaceRows->push_back(row);
    }

    inputFile.close();
}

//------------------------------------------------------------------------------
//   main
//------------------------------------------------------------------------------
int main(int, char **)
{
    std::vector<std::vector<char>> spaceRows;

    parseInput(INPUT_FILE, &spaceRows);

    long long part1Sum = calcShortestPathesSum(&spaceRows, 1);
    long long part2Sum = calcShortestPathesSum(&spaceRows, 1'000'000 - 1);

    std::cout << "PART 1: " << part1Sum << std::endl;
    std::cout << "PART 2: " << part2Sum << std::endl;

    return 0;
}