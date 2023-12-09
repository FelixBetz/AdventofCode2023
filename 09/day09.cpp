#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define INPUT_FILE "../../input.txt"

//------------------------------------------------------------------------------
//   parseInput
//------------------------------------------------------------------------------
static void parseInput(const char *pFileName, std::vector<std::vector<int>> *pHistoryRows)
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

        std::vector<int> histroyRow;
        std::stringstream iss(line);
        int number;
        while (iss >> number)
        {
            histroyRow.push_back(number);
        }

        pHistoryRows->push_back(histroyRow);
    }

    inputFile.close();
}

//------------------------------------------------------------------------------
//   isArrayNull
//------------------------------------------------------------------------------
bool isArrayNull(std::vector<int> *pArray)
{
    for (auto num : *pArray)
    {
        if (num != 0)
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
//   extrapolateHistory
//------------------------------------------------------------------------------
int extrapolateHistory(std::vector<int> *pHistoryRow, bool isPartOne)
{
    // build difference rows
    std::vector<std::vector<int>> diffRows;
    diffRows.push_back((*pHistoryRow));
    std::vector<int> row;
    do
    {
        row.clear();
        size_t rowIdx = diffRows.size() - 1;
        for (int i = 0; i < diffRows[rowIdx].size() - 1; i++)
        {
            row.push_back(diffRows[rowIdx][i + 1] - diffRows[rowIdx][i]);
        }
        diffRows.push_back(row);
    } while (!isArrayNull(&row));

    // extrapolate
    if (isPartOne)
    {
        diffRows[diffRows.size() - 1].push_back(0);
    }
    else
    {
        diffRows[diffRows.size() - 1].insert(diffRows[diffRows.size() - 1].begin(), 0);
    }

    int element = 0;
    for (int i = 0; i < diffRows.size() - 1; i++)
    {
        size_t rowIndex = diffRows.size() - i - 1;

        size_t lastRowIdx = isPartOne ? diffRows[rowIndex].size() - 1 : 0;
        int lastRowElement = diffRows[rowIndex][lastRowIdx];

        size_t previousLastRowIdx = isPartOne ? diffRows[rowIndex - 1].size() - 1 : 0;
        int previousLastRowElement = diffRows[rowIndex - 1][previousLastRowIdx];

        element = isPartOne
                      ? previousLastRowElement + lastRowElement
                      : previousLastRowElement - lastRowElement;

        if (isPartOne)
        {
            diffRows[rowIndex - 1].push_back(element);
        }
        else
        {
            diffRows[rowIndex - 1].insert(diffRows[rowIndex - 1].begin(), element);
        }
    }

    return element;
}

//------------------------------------------------------------------------------
//   main
//------------------------------------------------------------------------------
int main(int, char **)
{
    std::vector<std::vector<int>> historyRows;
    parseInput(INPUT_FILE, &historyRows);

    int part1Sum = 0;
    int part2Sum = 0;
    for (auto row : historyRows)
    {
        part1Sum += extrapolateHistory(&row, true);
        part2Sum += extrapolateHistory(&row, false);
    }
    std::cout << "PART 1: " << part1Sum << std::endl;
    std::cout << "PART 2: " << part2Sum << std::endl;

    return 0;
}