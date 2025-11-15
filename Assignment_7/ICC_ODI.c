#include <stdio.h>
#include <stdlib.h>
#include "Players_data.h"

struct player
{
    int playerId;
    char Name[50];
    char teamName[50];
    char Role[20];
};

#define NAME_LENGTH 51
#define NUMBER_OF_PLAYER_ID 1500
#define NUMBER_OF_TEAMS 10

typedef struct playerNode
{
    int playerID;
    char *name;
    char *teamName;
    char *role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct playerNode *next;
} playerNode;

typedef struct
{
    int teamID;
    char *name;
    int totalPlayers;
    int totalBatsmenAllRounders;
    float battingAverageStrikeRate;
    float batterAllRounderAvgStrikeRate;
    playerNode *batsmen;
    playerNode *bowlers;
    playerNode *allRounders;
} teamNode;

teamNode Teams[NUMBER_OF_TEAMS];

void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidRole()
{
    int role;
    while (scanf("%d", &role) != 1 || getchar() != '\n' || role < 1 || role > 3)
    {
        printf("Invalid Role. Enter again: ");
        clearBuffer();
    }
    return role;
}

int getValidInteger()
{
    int num;
    while (scanf("%d", &num) != 1 || getchar() != '\n')
    {
        printf("Invalid input. Please enter again: ");
        clearBuffer();
    }
    return num;
}

float getValidFloat()
{
    float num;
    while (scanf("%f", &num) != 1 || getchar() != '\n')
    {
        printf("Invalid input. Please enter again: ");
        clearBuffer();
    }
    return num;
}

int getValidTeamId()
{
    int id;
    while ((id = getValidInteger()) < 1 || id > teamCount)
        printf("Enter ID in range 1-%d: ", teamCount);
    return id;
}

int ifPlayerIdExists(int id)
{
    for (int i = 0; i < teamCount; i++)
    {
        playerNode *lists[3] = {Teams[i].batsmen, Teams[i].allRounders, Teams[i].bowlers};
        for (int j = 0; j < 3; j++)
            for (playerNode *p = lists[j]; p; p = p->next)
                if (p->playerID == id)
                    return 1;
    }
    return 0;
}

int getValidPlayerId()
{
    int id;
    while (1)
    {
        id = getValidInteger();
        if (id < 1 || id > NUMBER_OF_PLAYER_ID)
            printf("Enter ID in range 1-%d: ", NUMBER_OF_PLAYER_ID);
        else if (ifPlayerIdExists(id))
            printf("ID already exists! Enter again: ");
        else
            return id;
    }
}

int getStringLength(const char *string)
{
    int length = 0;
    while (*string++)
    {
        length++;
    }
    return length;
}

char *strnCpy(const char *string)
{
    char *resultString = (char *)malloc(sizeof(char) * getStringLength(string) + 1);
    char *copyPtr = resultString;
    while (*string)
    {
        *copyPtr = *string;
        copyPtr++;
        string++;
    }
    *copyPtr = '\0';
    return resultString;
}

void initializeTeams()
{
    for (int i = 0; i < teamCount; i++)
    {
        Teams[i].teamID = i + 1;
        Teams[i].name = strnCpy(teams[i]);
        Teams[i].totalPlayers = 0;
        Teams[i].totalBatsmenAllRounders = 0;
        Teams[i].battingAverageStrikeRate = 0;
        Teams[i].batterAllRounderAvgStrikeRate = 0;
        Teams[i].batsmen = NULL;
        Teams[i].bowlers = NULL;
        Teams[i].allRounders = NULL;
    }
}

int comparingString(const char *string1, const char *string2)
{
    while (*string1 && *string2)
    {
        if (*string1 != *string2)
            return (*string1 - *string2);
        string1++;
        string2++;
    }
    return (*string1 - *string2);
}

float PerformanceIndex(playerNode *newPlayer)
{
    float performanceIndex = 0;
    if (comparingString(newPlayer->role, "Batsman") == 0)
    {
        performanceIndex = (newPlayer->battingAverage * newPlayer->strikeRate) / 100.0f;
    }
    else if (comparingString(newPlayer->role, "Bowler") == 0)
    {
        performanceIndex = (newPlayer->wickets * 2.0f) + (100.0f - newPlayer->economyRate);
    }
    else
    {
        performanceIndex = ((newPlayer->battingAverage * newPlayer->strikeRate) / 100.0f) + (newPlayer->wickets * 2.0f);
    }
    return performanceIndex;
}

void insertInOrder(playerNode **head, playerNode *newPlayer)
{
    if (*head == NULL || newPlayer->performanceIndex > (*head)->performanceIndex)
    {
        newPlayer->next = *head;
        *head = newPlayer;
        return;
    }
    playerNode *current = *head;
    while (current->next != NULL && current->next->performanceIndex > newPlayer->performanceIndex)
    {
        current = current->next;
    }
    newPlayer->next = current->next;
    current->next = newPlayer;
}

int binarySearchOfTeam(const char *teamName)
{
    int left = 0, right = teamCount - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int comparision = comparingString(Teams[mid].name, teamName);
        if (comparision == 0)
            return mid;
        else if (comparision < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

void sortTeam(playerNode *newPlayer)
{
    int index = binarySearchOfTeam(newPlayer->teamName);
    if (index == -1)
    {
        printf("There is NO team like this! \n");
        return;
    }

    if (comparingString(newPlayer->role, "Batsman") == 0)
    {
        insertInOrder(&Teams[index].batsmen, newPlayer);
        Teams[index].batterAllRounderAvgStrikeRate =
            ((Teams[index].batterAllRounderAvgStrikeRate * Teams[index].totalBatsmenAllRounders)
             + newPlayer->strikeRate) /
            (Teams[index].totalBatsmenAllRounders + 1);
        Teams[index].totalBatsmenAllRounders++;
    }
    else if (comparingString(newPlayer->role, "All-rounder") == 0)
    {
        insertInOrder(&Teams[index].allRounders, newPlayer);
        Teams[index].batterAllRounderAvgStrikeRate =
            ((Teams[index].batterAllRounderAvgStrikeRate * Teams[index].totalBatsmenAllRounders)
             + newPlayer->strikeRate) /
            (Teams[index].totalBatsmenAllRounders + 1);
        Teams[index].totalBatsmenAllRounders++;
    }
    else
    {
        insertInOrder(&Teams[index].bowlers, newPlayer);
    }
    Teams[index].battingAverageStrikeRate =
        ((Teams[index].battingAverageStrikeRate * Teams[index].totalPlayers) + newPlayer->strikeRate)
        / (Teams[index].totalPlayers + 1);
    Teams[index].totalPlayers++;
}

void intializePlayers()
{
    for (int i = 0; i < playerCount; i++)
    {
        playerNode *newPlayer = (playerNode *)malloc(sizeof(playerNode));
        newPlayer->playerID = players[i].id;
        newPlayer->name = strnCpy(players[i].name);
        newPlayer->teamName = strnCpy(players[i].team);
        newPlayer->role = strnCpy(players[i].role);
        newPlayer->totalRuns = players[i].totalRuns;
        newPlayer->battingAverage = players[i].battingAverage;
        newPlayer->strikeRate = players[i].strikeRate;
        newPlayer->wickets = players[i].wickets;
        newPlayer->economyRate = players[i].economyRate;
        newPlayer->performanceIndex = PerformanceIndex(newPlayer);
        newPlayer->next = NULL;
        sortTeam(newPlayer);
    }
}

void createAndAddPlayer()
{
    playerNode *newPlayer = (playerNode *)malloc(sizeof(playerNode));
    if (!newPlayer)
    {
        printf("Failed to allocate memory!\n");
        return;
    }

    printf("Enter Team ID to add player: ");
    int teamID = getValidTeamId();
    newPlayer->teamName = strnCpy(Teams[teamID - 1].name);
    printf("Please enter Player Details: \n");

    printf("Enter Player ID: ");
    newPlayer->playerID = getValidPlayerId();

    printf("Enter Player Name: ");
    char *name = (char *)malloc(NAME_LENGTH);
    fgets(name, NAME_LENGTH, stdin);
    if (*(name + getStringLength(name) - 1) == '\n')
    {
        *(name + getStringLength(name) - 1) = '\0';
    }
    newPlayer->name = strnCpy(name);
    free(name);

    printf("Choose Role:\n  1.Batsman\n 2.Bowler\n 3.All-rounder\n ");
    int role = getValidRole();
    if (role == 1)
        newPlayer->role = strnCpy("Batsman");
    else if (role == 2)
        newPlayer->role = strnCpy("Bowler");
    else
        newPlayer->role = strnCpy("All-rounder");

    printf("Total Runs: ");
    newPlayer->totalRuns = getValidInteger();

    printf("Batting Average: ");
    newPlayer->battingAverage = getValidFloat();

    printf("Strike Rate: ");
    newPlayer->strikeRate = getValidFloat();

    printf("Wickets: ");
    newPlayer->wickets = getValidInteger();

    printf("Economy Rate: ");
    newPlayer->economyRate = getValidFloat();

    newPlayer->performanceIndex = PerformanceIndex(newPlayer);
    newPlayer->next = NULL;
    sortTeam(newPlayer);

    printf("Player added successfully to %s!\n", newPlayer->teamName);
}

void playerDetails(playerNode *newPlayer)
{
    printf("%-5d %-20s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n",
           newPlayer->playerID, newPlayer->name, newPlayer->role, newPlayer->totalRuns,
           newPlayer->battingAverage, newPlayer->strikeRate,
           newPlayer->wickets, newPlayer->economyRate, newPlayer->performanceIndex);
}

void showTeamPlayers()
{
    printf("Enter Team ID: ");
    int teamID = getValidTeamId();
    printf("Players of Team %s:\n", Teams[teamID - 1].name);
    printf("\n------------------------------------------------------------------------------------------------------\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("\n------------------------------------------------------------------------------------------------------\n");
    playerNode *temp = Teams[teamID - 1].batsmen;
    while (temp != NULL)
    {
        playerDetails(temp);
        temp = temp->next;
    }
    temp = Teams[teamID - 1].allRounders;
    while (temp != NULL)
    {
        playerDetails(temp);
        temp = temp->next;
    }
    temp = Teams[teamID - 1].bowlers;
    while (temp != NULL)
    {
        playerDetails(temp);
        temp = temp->next;
    }
    printf("\n------------------------------------------------------------------------------------------------------\n");
    printf("\nTotal Players: %d\n", Teams[teamID - 1].totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", Teams[teamID - 1].battingAverageStrikeRate);
}

void displayTeamsBySR()
{
    int teamIndices[teamCount];
    for (int i = 0; i < teamCount; i++)
        teamIndices[i] = i;

    for (int i = 0; i < teamCount; i++)
    {
        for (int j = 0; j < teamCount - i - 1; j++)
        {
            if (Teams[teamIndices[j]].batterAllRounderAvgStrikeRate <
                Teams[teamIndices[j + 1]].batterAllRounderAvgStrikeRate)
            {
                int temp = teamIndices[j];
                teamIndices[j] = teamIndices[j + 1];
                teamIndices[j + 1] = temp;
            }
        }
    }

    printf("\nTeams sorted by Average Strike Rate of Batsmen + All-rounders:\n\n");
    for (int i = 0; i < teamCount; i++)
    {
        int t = teamIndices[i];
        printf("%d. %s (Avg SR: %.2f)\n",
               i + 1, Teams[t].name, Teams[t].batterAllRounderAvgStrikeRate);
    }
}

void showPlayersByRole()
{
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int role = getValidRole();
    playerNode *playerByRole[teamCount];

    for (int i = 0; i < teamCount; i++)
    {
        if (role == 1)
            playerByRole[i] = Teams[i].batsmen;
        else if (role == 2)
            playerByRole[i] = Teams[i].bowlers;
        else
            playerByRole[i] = Teams[i].allRounders;
    }

    printf("\n------------------------------------------------------------------------------------------------------\n");
    printf("%-5s %-20s %-15s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf  Index");
    printf("\n------------------------------------------------------------------------------------------------------\n");

    while (1)
    {
        int bestIndex = -1;
        float bestPI = -1;

        for (int i = 0; i < teamCount; i++)
        {
            if (playerByRole[i] != NULL)
            {
                if (playerByRole[i]->performanceIndex > bestPI)
                {
                    bestPI = playerByRole[i]->performanceIndex;
                    bestIndex = i;
                }
            }
        }
        if (bestIndex == -1)
            break;

        playerNode *temp = playerByRole[bestIndex];
        printf("%-5d %-20s %-15s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n",
               temp->playerID, temp->name, temp->teamName, temp->role,
               temp->totalRuns, temp->battingAverage, temp->strikeRate,
               temp->wickets, temp->economyRate, temp->performanceIndex);

        playerByRole[bestIndex] = playerByRole[bestIndex]->next;
    }

    printf("\n------------------------------------------------------------------------------------------------------\n");
}

void showTopKPlayers()
{
    printf("Enter Team ID: ");
    int teamID = getValidTeamId();
    printf("Enter Role:\n 1.Batsman\n 2.Bowler\n 3.All-rounder\n ");
    int role = getValidRole();
    printf("Enter number of players: ");
    int k = getValidInteger();

    printf("Top %d %s of Team %s\n",
           k,
           (role == 1 ? "Batsmen" : (role == 2 ? "Bowlers" : "All-rounders")),
           Teams[teamID - 1].name);

    printf("\n------------------------------------------------------------------------------------------------------\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("\n------------------------------------------------------------------------------------------------------\n");

    playerNode *temp;
    if (role == 1)
        temp = Teams[teamID - 1].batsmen;
    else if (role == 2)
        temp = Teams[teamID - 1].bowlers;
    else
        temp = Teams[teamID - 1].allRounders;

    while (k-- && temp != NULL)
    {
        playerDetails(temp);
        temp = temp->next;
    }

    printf("\n------------------------------------------------------------------------------------------------------\n");
}

void Menu()
{
    printf("\n-----------------------------------------------------------\n");
    printf("           ICC ODI Player Performance Analyzer\n");
    printf("-------------------------------------------------------------\n");
    printf("1. Add a new player to a team\n");
    printf("2. Show all players in a team\n");
    printf("3. List teams by average batting strike rate\n");
    printf("4. Top K players of a team by role\n");
    printf("5. List all players by role across all teams\n");
    printf("6. Exit the program\n");
    printf("--------------------------------------------------------------\n");
}

void freeMemory()
{
    for (int i = 0; i < teamCount; i++)
    {
        playerNode *lists[3] = {Teams[i].batsmen, Teams[i].bowlers, Teams[i].allRounders};
        for (int role = 0; role < 3; role++)
        {
            playerNode *current = lists[role];
            while (current != NULL)
            {
                playerNode *temp = current;
                current = current->next;
                free(temp->name);
                free(temp->teamName);
                free(temp->role);
                free(temp);
            }
        }
        free(Teams[i].name);
    }
}

int main()
{
    initializeTeams();
    intializePlayers();

    int choice = 0;
    while (choice != 6)
    {
        Menu();
        printf("Enter your choice:\n ");
        choice = getValidInteger();
        switch (choice)
        {
        case 1:
            createAndAddPlayer();
            break;
        case 2:
            showTeamPlayers();
            break;
        case 3:
            displayTeamsBySR();
            break;
        case 4:
            showTopKPlayers();
            break;
        case 5:
            showPlayersByRole();
            break;
        case 6:
            freeMemory();
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
