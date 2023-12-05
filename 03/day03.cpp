#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <list>
#include <tuple>
#include <vector>
#include <cctype>
#include <set>
#define INPUT_FILE "../../input.txt"

//------------------------------------------------------------------------------
//   isSymbol
//------------------------------------------------------------------------------
static bool isSymbol(char pCh)
{
    if (pCh == '.' || std::isdigit(pCh))
    {
        return false;
    }

    return true;
}

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

    std::list<std::tuple<int, int, int, int>> numbersList;
    std::list<std::tuple<int, int>> asteriskList;

    std::vector<std::string> inputLines;

    std::string line;

    int lineIndex = 0;
    while (std::getline(inputFile, line))
    {
        inputLines.push_back(line);

        // parse numbers with regex
        std::regex pattern("\\d+");
        std::sregex_iterator iterator(line.begin(), line.end(), pattern);
        std::sregex_iterator end;
        while (iterator != end)
        {
            std::smatch match = *iterator;

            numbersList.push_back(std::make_tuple(std::stoi(match.str()), lineIndex, match.position(), (int)match.str().length()));
            ++iterator;
        }

        // parse *
        int linePos = 0;
        for (char ch : line)
        {
            if (ch == '*')
            {
                asteriskList.push_back(std::make_tuple(lineIndex, linePos));
            }
            linePos++;
        }

        lineIndex++;
    }

    inputFile.close();

    // solve part 1
    int part1Sum = 0;
    for (auto it = numbersList.begin(); it != numbersList.end(); ++it)
    {
        int value = std::get<0>(*it);
        int lineIndex = std::get<1>(*it);
        int linePos = std::get<2>(*it);
        int valueLen = std::get<3>(*it);

        int lineLength = (int)inputLines.at(0).length();

        int leftIndex = std::max(0, linePos - 1);
        int rightIndex = std::min(linePos + valueLen, lineLength - 1);

        bool isValid = false;
        // check left
        if (leftIndex < linePos)
        {
            char ch = inputLines.at(lineIndex).at(leftIndex);

            if (isSymbol(ch))
            {
                isValid = true;
            }
        }

        // check right
        if (rightIndex > linePos)
        {
            char ch = inputLines.at(lineIndex).at(rightIndex);
            if (isSymbol(ch))
            {
                isValid = true;
            }
        }

        // check top
        if (lineIndex > 0)
        {
            for (int i = leftIndex; i <= rightIndex; i++)
            {
                char ch = inputLines.at(lineIndex - 1).at(i);
                if (isSymbol(ch))
                {
                    isValid = true;
                }
            }
        }

        // check bot
        if (lineIndex < inputLines.size() - 1)
        {
            for (int i = leftIndex; i <= rightIndex; i++)
            {
                char ch = inputLines.at(lineIndex + 1).at(i);
                if (isSymbol(ch))
                {
                    isValid = true;
                }
            }
        }

        if (isValid)
        {
            part1Sum += value;
        }
    }

    // solve part 2
    int part2sum = 0;
    int cntLines = (int)inputLines.size() - 1;
    for (auto it = asteriskList.begin(); it != asteriskList.end(); ++it)
    {

        int asterikLineIndex = std::get<0>(*it);
        int asterikLinePos = std::get<1>(*it);

        int lineLength = (int)inputLines.at(0).length();

        int leftIndex = std::max(0, asterikLinePos - 1);
        int rightIndex = std::min(asterikLinePos + 1, lineLength - 1);

        int startLineIndex = std::max(0, asterikLineIndex - 1);
        int endLineIndex = std::min(cntLines, asterikLineIndex + 1);

        std::set<int> adjacentNumbers;
        for (int lineIdx = startLineIndex; lineIdx <= endLineIndex; lineIdx++)
        {
            for (int posIdx = leftIndex; posIdx <= rightIndex; posIdx++)
            {

                // check if number is adjacent with asterik
                for (auto numbersIt = numbersList.begin(); numbersIt != numbersList.end(); ++numbersIt)
                {
                    int numValue = std::get<0>(*numbersIt);
                    int numLineIndex = std::get<1>(*numbersIt);
                    int numLinePos = std::get<2>(*numbersIt);
                    int numValueLen = std::get<3>(*numbersIt);

                    if ((numLineIndex == lineIdx) && (numLinePos <= posIdx) && (posIdx <= numLinePos + numValueLen - 1))
                    {
                        adjacentNumbers.insert(numValue);
                    }
                }
            }
        }

        if (adjacentNumbers.size() == 2)
        {

            int gearRatio = 1;
            for (auto num : adjacentNumbers)
            {
                gearRatio *= num;
            }
            part2sum += gearRatio;
        }
    }

    std::cout << "PART1: sum of all of the part numbers in the engine schematic: " << part1Sum << std::endl;
    std::cout << "PART2: sum of all of the gear ratios in your engine schematic: " << part2sum << std::endl;
    return 0;
}
