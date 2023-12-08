#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define INPUT_FILE "../../input.txt"

#define NUM_SPLICE_TIME 5
#define NUM_SPLICE_DISTANCE 9

#define JOKER_VAL_PART_2 1

typedef enum
{
    FIVE_OF_A_KIND = 7,
    FOUR_OF_A_KIND = 6,
    FULL_HOUSE = 5,
    THREE_OF_A_KIND = 4,
    TWO_PAIR = 3,
    ONE_PAIR = 2,
    HIGH_CARD = 1,
} e_CARD_TYPE;

//------------------------------------------------------------------------------
//   getCardsType
//------------------------------------------------------------------------------
static e_CARD_TYPE getCardsType(std::vector<int> *cards)
{
    std::unordered_map<int, int> elementCount;

    // Iterate through the vector and update the count in the map
    int idx = 0;
    for (int element : *cards)
    {
        elementCount[element]++;
    }

    int cntFives = 0;
    int cntFours = 0;
    int cntThrees = 0;
    int cntPairs = 0;

    // Print the count of each element
    for (const auto &pair : elementCount)
    {
        switch (pair.second)
        {
        case 5:
            cntFives++;
            break;
        case 4:
            cntFours++;
            break;
        case 3:
            cntThrees++;
            break;
        case 2:
            cntPairs++;
            break;
        }
    }

    if (cntFives == 1)
    {
        return e_CARD_TYPE::FIVE_OF_A_KIND;
    }
    if (cntFours == 1)
    {
        return e_CARD_TYPE::FOUR_OF_A_KIND;
    }
    if (cntThrees == 1 && cntPairs == 1)
    {
        return e_CARD_TYPE::FULL_HOUSE;
    }
    if (cntThrees == 1)
    {
        return e_CARD_TYPE::THREE_OF_A_KIND;
    }
    if (cntPairs == 2)
    {
        return e_CARD_TYPE::TWO_PAIR;
    }
    if (cntPairs == 1)
    {
        return e_CARD_TYPE::ONE_PAIR;
    }

    return e_CARD_TYPE::HIGH_CARD;
}

//------------------------------------------------------------------------------
//   getTypeWithJokers
//------------------------------------------------------------------------------
static e_CARD_TYPE getCardsTypeWithJokers(e_CARD_TYPE pType, int pNumJokers)
{

    if (pNumJokers == 5)
    {
        return FIVE_OF_A_KIND;
    }

    if (pNumJokers == 4)
    {
        return FIVE_OF_A_KIND;
    }

    if (pNumJokers == 3)
    {
        switch (pType)
        {
        case ONE_PAIR:
            return FIVE_OF_A_KIND;

        case HIGH_CARD:
            return FOUR_OF_A_KIND;
        }
    }
    if (pNumJokers == 2)
    {
        switch (pType)
        {
        case THREE_OF_A_KIND:
            return FIVE_OF_A_KIND;
        case ONE_PAIR:
            return FOUR_OF_A_KIND;
        case HIGH_CARD:
            return THREE_OF_A_KIND;
        }
    }

    if (pNumJokers == 1)
    {
        switch (pType)
        {
        case FOUR_OF_A_KIND:
            return FIVE_OF_A_KIND;
        case THREE_OF_A_KIND:
            return FOUR_OF_A_KIND;
        case TWO_PAIR:
            return FULL_HOUSE;
        case ONE_PAIR:
            return THREE_OF_A_KIND;
        case HIGH_CARD:
            return ONE_PAIR;
        }
    }

    return pType;
}

//------------------------------------------------------------------------------
//   getCardsTypePart2
//------------------------------------------------------------------------------
static e_CARD_TYPE getCardsTypePart2(std::vector<int> *pCards)
{
    std::vector<int> cardsRemovedJokers;
    int cntJokers = 0;
    for (auto card : *pCards)
    {
        if (card != JOKER_VAL_PART_2)
        {
            cardsRemovedJokers.push_back(card);
        }
        else
        {
            cntJokers++;
        }
    }
    e_CARD_TYPE typeRemovedJokers = getCardsType(&cardsRemovedJokers);
    return getCardsTypeWithJokers(typeRemovedJokers, cntJokers);
}

//------------------------------------------------------------------------------
//   sortCardsPart1
//------------------------------------------------------------------------------
static bool sortCardsPart1(std::tuple<std::vector<int>, int> &a, std::tuple<std::vector<int>, int> &b)
{
    e_CARD_TYPE aType = getCardsType(&std::get<0>(a));
    e_CARD_TYPE bType = getCardsType(&std::get<0>(b));

    // different type
    if (aType != bType)
    {
        return aType < bType;
    }

    // same type => strongest leading card
    for (int i = 0; i < std::get<0>(a).size(); i++)
    {
        if (std::get<0>(a)[i] != std::get<0>(b)[i])
        {
            return std::get<0>(a)[i] < std::get<0>(b)[i];
        }
    }

    // diffrent type
    return aType > bType;
}

//------------------------------------------------------------------------------
//   sortCardsPart2
//------------------------------------------------------------------------------
static bool sortCardsPart2(std::tuple<std::vector<int>, int> &a, std::tuple<std::vector<int>, int> &b)
{
    e_CARD_TYPE aType = getCardsTypePart2(&(std::get<0>(a)));
    e_CARD_TYPE bType = getCardsTypePart2(&(std::get<0>(b)));

    if (aType)

        // different type
        if (aType != bType)
        {
            return aType < bType;
        }

    // same type => strongest leading card
    for (int i = 0; i < std::get<0>(a).size(); i++)
    {
        if (std::get<0>(a)[i] != std::get<0>(b)[i])
        {
            return std::get<0>(a)[i] < std::get<0>(b)[i];
        }
    }

    // diffrent type
    return aType > bType;
}

//------------------------------------------------------------------------------
//   getCardStrenght
//------------------------------------------------------------------------------
static int getCardStrenght(const char cardCh, bool pIsPart1)
{
    switch (cardCh)
    {
    case 'A':
        return 14;
    case 'K':
        return 13;
    case 'Q':
        return 12;
    case 'J':
        return pIsPart1 ? 11 : JOKER_VAL_PART_2;
    case 'T':
        return 10;
    }
    return cardCh - '0';
}

//------------------------------------------------------------------------------
//   parseInput
//------------------------------------------------------------------------------
static void parseInput(const char *pFileName, std::vector<std::tuple<std::vector<int>, int>> *games, bool isPart1)
{
    games->clear();
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
        std::vector<int> cards;

        std::string bidStr;
        int posIdx = 0;
        for (auto ch : line)
        {
            if (posIdx < 5)
            {
                cards.push_back(getCardStrenght(ch, isPart1));
            }
            else
            {
                bidStr += ch;
            }
            posIdx++;
        }

        games->push_back(std::make_pair(cards, std::stoi(bidStr)));
    }
    inputFile.close();
}

//------------------------------------------------------------------------------
//   calcWeight
//------------------------------------------------------------------------------
int calcWeight(std::vector<std::tuple<std::vector<int>, int>> *pGames)
{
    int weight = 1;
    int sum = 0;
    for (auto game : *pGames)
    {
        sum += weight * (int)std::get<1>(game);
        weight++;
    }
    return sum;
}

//------------------------------------------------------------------------------
//   main
//------------------------------------------------------------------------------
int main(int, char **)
{

    std::vector<std::tuple<std::vector<int>, int>> games;

    // part1
    parseInput(INPUT_FILE, &games, true);
    std::sort(games.begin(), games.end(), sortCardsPart1);
    std::cout << "PART 1: multiplied race wins: " << calcWeight(&games) << std::endl;

    // part 2
    parseInput(INPUT_FILE, &games, false);
    std::sort(games.begin(), games.end(), sortCardsPart2);
    std::cout << "PART 2: multiplied race wins: " << calcWeight(&games) << std::endl;

    return 0;
}