#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

#define INPUT_FILE "../../input.txt"

typedef enum
{
    LEFT = 'L',
    RIGHT = 'R'
} e_Instruction;

//------------------------------------------------------------------------------
//   parseInput
//------------------------------------------------------------------------------
static void parseInput(const char *pFileName, std::vector<char> *pInstructions,
                       std::vector<int> *pLefts, std::vector<int> *pRights,
                       int *pStartIndex, int *pEndIndex,
                       std::vector<int> *pPart2StartIndices, std::vector<int> *pPart2endIndices)
{

    // Open the file
    std::ifstream inputFile(pFileName);

    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return;
    }

    std::vector<std::string> leftStrings;
    std::vector<std::string> rightStrings;
    std::vector<std::string> keyStrings;

    std::string line;
    int lineIdx = 0;

    while (std::getline(inputFile, line))
    {

        if (lineIdx == 0)
        {
            for (auto ch : line)
            {
                if (ch == e_Instruction::LEFT)
                {
                    pInstructions->push_back(LEFT);
                }
                else if (ch == e_Instruction::RIGHT)
                {
                    pInstructions->push_back(RIGHT);
                }
            }
        }
        if (lineIdx > 1)
        {
            keyStrings.push_back(line.substr(0, 3));
            leftStrings.push_back(line.substr(7, 3));
            rightStrings.push_back(line.substr(12, 3));
        }

        lineIdx++;
    }

    inputFile.close();

    int idx = 0;
    for (int i = 0; i < leftStrings.size(); i++)
    {
        for (int k = 0; k < keyStrings.size(); k++)
        {
            if (keyStrings[k] == leftStrings[i])
            {
                pLefts->push_back(k);
            }
            if (keyStrings[k] == rightStrings[i])
            {
                pRights->push_back(k);
            }
        }
        if (keyStrings[i] == "ZZZ")
        {
            *pEndIndex = i;
        }
        if (keyStrings[i] == "AAA")
        {
            *pStartIndex = i;
        }

        if (keyStrings[i][2] == 'A')
        {
            pPart2StartIndices->push_back(i);
        }

        if (keyStrings[i][2] == 'Z')
        {
            pPart2endIndices->push_back(i);
        }
    }
}

//------------------------------------------------------------------------------
//   getSteps
//------------------------------------------------------------------------------
static int getSteps(std::vector<char> *pInstructions, std::vector<int> *pLefts, std::vector<int> *pRights, int pStartIndex, std::vector<int> *pEndIndices)
{
    int cntPart1Steps = 0;
    int instructionIdx = 0;

    bool isEnd = false;
    while (!isEnd)
    {
        pStartIndex = ((*pInstructions)[instructionIdx] == LEFT)
                          ? (*pLefts)[pStartIndex]
                          : (*pRights)[pStartIndex];
        cntPart1Steps++;
        instructionIdx++;
        if (instructionIdx >= pInstructions->size())
        {
            instructionIdx = 0;
        }

        for (auto idx : *pEndIndices)
        {
            if (idx == pStartIndex)
            {
                isEnd = true;
                break;
            }
        }
    }
    return cntPart1Steps;
}

//------------------------------------------------------------------------------
//   main
//------------------------------------------------------------------------------
int main(int, char **)
{

    std::vector<char> instructions;

    std::vector<int> lefts;
    std::vector<int> rights;

    int part1EndIndex, part1StartIndex;

    std::vector<int> part2startIndices, part2EndIndices;

    parseInput(INPUT_FILE, &instructions, &lefts, &rights, &part1StartIndex, &part1EndIndex, &part2startIndices, &part2EndIndices);

    // part1
    std::vector<int> part1EndIndices = {part1EndIndex};
    int cntPart1Steps = getSteps(&instructions, &lefts, &rights, part1StartIndex, &part1EndIndices);
    std::cout << "PART 1: " << cntPart1Steps << std::endl;

    // part2

    std::vector<int> steps;
    for (auto startIndex : part2startIndices)
    {
        int step = getSteps(&instructions, &lefts, &rights, startIndex, &part2EndIndices);
        steps.push_back(step);
    }

    // calc lcm
    long long cntPart2Steps = 1;
    for (int i = 0; i < steps.size(); i++)
    {
        cntPart2Steps = std::lcm(steps[i], cntPart2Steps);
    }

    std::cout << "PART 2: " << cntPart2Steps << std::endl;

    return 0;
}