#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>

#define MAX_CHARACTERS 200
#define MAX_SCENERY 40
#define TOT_MAX_GRID 40
#define MAX_TEXTS 100
#define MAX_LETTERS 2000
#define TEXT_SPEED 1 // 55 is reading speed
#define pi 3.14159

enum color{black, darkBlue, darkGreen, darkAqua, darkRed, darkPurple, darkYellow, grey, darkGrey, blue, green, aqua, red, purple, yellow, white};
enum charName{null1, player, palfore, adrea, monsterLv1, decayedAdrea};
enum scenIcon{null2, tree, fallenlog};
enum weapon{fist, logWeap};
enum armour{abominal};
enum direction{west, east, north, south, all};
enum modifier{pAttMod, pBlockMod, oAttMod, oBlockMod};
enum fightingIcons{standing, heavy, light, defend, stun, rest, potion, attack, youmiss, theymiss};

typedef struct entities_struct{
    char name[20];
    char icon[3];
    int color;
    int x;
    int y;
    int maxHealth;
    int health;
    int attack;
    int strength;
    int defence;
    int potions;
    int coins;
    int weaponID;
    int armourID;
    char weapon[8];
    char armour[8];
    int interact;
    int stunTurns;
} entities;

typedef struct bonus_struct{
    int hpBonus;
    int attBonus;
    int strBonus;
    int defBonus;
} bonus;

void createDoors(int doors, int gridSize, int scenery[MAX_SCENERY][MAX_SCENERY]);
void showBoard(int gridSize, entities* characters, int scenery[MAX_SCENERY][MAX_SCENERY]);
void printChar(entities thing);
void updateScreen(entities* characters, int name,  int c);
void col(int color);
void initialize(entities* characters, int scenery[MAX_SCENERY][MAX_SCENERY]);
void movePlayer(int move, entities* characters, int gridSize, int scenery[MAX_SCENERY][MAX_SCENERY]);
int playersTurn(entities* characters, int gridSize, int settings[20], int scenery[MAX_SCENERY][MAX_SCENERY]);
void createRoom(int inRoom, int* gridSize, int scenery[MAX_SCENERY][MAX_SCENERY], entities* characters);
void pressEnter(int printScreen, int gridsize);
void entityFollow(entities* characters, int gridSize, int settings[20], int scenery[MAX_SCENERY][MAX_SCENERY]);
void fight(entities* characters, int opponent, int scenery[MAX_SCENERY][MAX_SCENERY], int gridsize);
char opponentsMove(int* mods);
char action(char oppMove, entities* characters, int opponent, int turn);
void winConditions(entities* characters, int opponent);
void outcome(int* mods, entities* characters, int opponent, char move, char oppMove);
void upperHalf(entities* characters, int opponent, int turn);
void showChances(int* mods);
void giveOptions();
void fightIcon(int who, int picture);
void moveCursor(int a, int b);
void colorText(int speed, int printTextNum, int gridSize, entities* characters);
void readyText(int gridSize, int a);
void interactionEvent(int eventNum, int gridSize, entities* characters, int scenery[MAX_SCENERY][MAX_SCENERY]);
void guide(int showScreen);
void settingsMenu(int settings[20]);
void placeScen(int x, int y, int character, int scenery[MAX_SCENERY][MAX_SCENERY]);
void showStats(entities* characters, int gridSize);
void printScen(int print);
int startMenu(int settings[20]);
void timedEvents(entities* characters, int gridSize, int scenery[MAX_SCENERY][MAX_SCENERY]);
void RoomChanging(entities* characters, int gridSize, int* inRoom, int scenery[MAX_SCENERY][MAX_SCENERY]);
void intro(entities* characters);

int defaultIcons = 2;
int turnDiff = -1;

int load(entities* characters, int scenery[MAX_SCENERY][MAX_SCENERY], int* inRoom, int* turn)
{
    system("cls");

    FILE* inFile = NULL;

    inFile = fopen("newSave.txt", "r");

    if (inFile == NULL)
    {
        printf("ERROR 003: NO SAVE FILE \"save.txt\" FOUND TO OPEN\n");
//        pressEnter(0);
        return -1;
    }
/* characters */
    for (int k = 0; k < MAX_CHARACTERS; k++)
    {
        fscanf(inFile, "%s ", characters[k].name);
        fscanf(inFile, "%d ", &characters[k].color);
        fscanf(inFile, "%d ", &characters[k].x);
        fscanf(inFile, "%d ", &characters[k].y);
        fscanf(inFile, "%d ", &characters[k].maxHealth);
        fscanf(inFile, "%d ", &characters[k].health);
        fscanf(inFile, "%d ", &characters[k].attack);
        fscanf(inFile, "%d ", &characters[k].strength);
        fscanf(inFile, "%d ", &characters[k].defence);
        fscanf(inFile, "%d ", &characters[k].potions);
        fscanf(inFile, "%d ", &characters[k].coins);
        fscanf(inFile, "%d ", &characters[k].weaponID);
        fscanf(inFile, "%d ", &characters[k].armourID);
        fscanf(inFile, "%s ", characters[k].weapon);
        fscanf(inFile, "%s ", characters[k].armour);
        fscanf(inFile, "%d ", &characters[k].interact);
        fscanf(inFile, "%d ", &characters[k].stunTurns);
        fscanf(inFile, "\n");
    }

/* Scenery */
    for (int k = 0; k < MAX_SCENERY; k++)
    {
        for (int j = 0; j < MAX_SCENERY; j++)
        {
            fscanf(inFile, "%d ", &scenery[j][k]);
            if (scenery[j][k] == 2)
            {
                printf("(%d,%d)", j, k);
            }
        }
        fscanf(inFile, "\n");
    }
  //  fscanf(inFile, "\n");
    fscanf(inFile, "%d\n", inRoom);
    fscanf(inFile, "%d\n", turn);




    printf("Opening file newSave.txt.\n");
    printf("Your save file has been read correctly.\n");
    fclose(inFile);
    system("cls");
    return 0 ;
}

int save(entities* characters, int scenery[MAX_SCENERY][MAX_SCENERY], int inRoom, int turn)
{
    FILE* outFile = NULL;

    outFile = fopen("newSave.txt", "w");

    if (outFile == NULL)
    {
        printf("ERROR 002: NO FILE \"save.txt\" FOUND TO SAVE TO\n");
        return -1;
    }
/* characters */
    for (int k = 0; k < MAX_CHARACTERS; k++)
    {
        fprintf(outFile, "%s ", characters[k].name);
        fprintf(outFile, "%d ", characters[k].color);
        fprintf(outFile, "%d ", characters[k].x);
        fprintf(outFile, "%d ", characters[k].y);
        fprintf(outFile, "%d ", characters[k].maxHealth);
        fprintf(outFile, "%d ", characters[k].health);
        fprintf(outFile, "%d ", characters[k].attack);
        fprintf(outFile, "%d ", characters[k].strength);
        fprintf(outFile, "%d ", characters[k].defence);
        fprintf(outFile, "%d ", characters[k].potions);
        fprintf(outFile, "%d ", characters[k].coins);
        fprintf(outFile, "%d ", characters[k].weaponID);
        fprintf(outFile, "%d ", characters[k].armourID);
        fprintf(outFile, "%s ", characters[k].weapon);
        fprintf(outFile, "%s ", characters[k].armour);
        fprintf(outFile, "%d ", characters[k].interact);
        fprintf(outFile, "%d ", characters[k].stunTurns);
        fprintf(outFile, "\n");
    }

/* Scenery */
    for (int k = 0; k < MAX_SCENERY; k++)
    {
        for (int j = 0; j < MAX_SCENERY; j++)
            fprintf(outFile, "%d ", scenery[j][k]);
        fprintf(outFile, "\n");
    }

/* InRoom */
    fprintf(outFile, "%d\n", inRoom);
    fprintf(outFile, "%d\n", turn);


    fclose(outFile);
    printf("Your game state has been saved succesfully.\n");
    return 0;
}

int main()
{
    srand(time(NULL));
    entities characters[MAX_CHARACTERS];
    int scenery[TOT_MAX_GRID][TOT_MAX_GRID];
    int gridSize = 18;
    int inRoom = 0;
    int settings[20] = {-1,-1};
    int turn = 0;

    int option;
    do {option = startMenu(settings);} while (option != 1 && option != 2);


    if (option == 2) // load
    {
        initialize(characters, scenery);
        load(characters, scenery, &inRoom, &turn);
    } else
    {
        initialize(characters, scenery);
        intro(characters);
        createRoom(inRoom, &gridSize, scenery, characters);
    }

    system("cls");
    showBoard(gridSize, characters, scenery);

    while (2)
    {
        if (turnDiff >= 0)
            turnDiff++;
        timedEvents(characters, gridSize, scenery);
        if (turn++ == 4 && inRoom == 0)
        {
            characters[adrea].x = 6;
            characters[adrea].y = 9;
            updateScreen(characters, adrea, 1);
            colorText(TEXT_SPEED, 3, gridSize, characters);
        }
        if (playersTurn(characters, gridSize, settings, scenery))
            save(characters, scenery, inRoom, turn);
        entityFollow(characters, gridSize, settings, scenery);
        RoomChanging(characters, gridSize, &inRoom, scenery);

    }

    return 0;
}

void newRoomPos(entities* characters, enum direction cameFrom, int gridSize, int inRoom)
{
    if (!(inRoom == 4 || (inRoom == 3 && cameFrom == west))) // Stops weirdness when leaving oracles house
     //   configureMap(cameFrom);

    switch(cameFrom)
    {
    case west: // Moves you to east
        {
            characters[player].x = 2;
            characters[player].y = gridSize / 2;
            break;
        }
    case east: // Moves you to west
        {
            characters[player].x = gridSize - 1;
            characters[player].y = gridSize / 2;
            break;
        }
    case north: // Moves you to south
        {
            characters[player].x = gridSize / 2 - 1;
            characters[player].y = gridSize - 2;
            break;
        }
    case south: // Moves you to north
        {
            characters[player].x = gridSize / 2 - 1;
            characters[player].y = 1;
            break;
        }
    case all:
        exit(1000);
     }
     return;
}

void moveToRoom(int toRoom, entities*characters, int* gridSize, int scenery[MAX_SCENERY][MAX_SCENERY], int* inRoom, int atDoor)
{
    *inRoom = toRoom;
    createRoom(*inRoom, gridSize, scenery, characters);
    newRoomPos(characters, atDoor, *gridSize, *inRoom);
    system("cls");
    showBoard(*gridSize, characters, scenery);

    return;
}

void RoomChanging(entities* characters, int gridSize, int* inRoom, int scenery[MAX_SCENERY][MAX_SCENERY]) // Character's will only be updated here if the room changes
{
    int atDoor;

    if (characters[player].x == gridSize / 2 && characters[player].y == 0           ) atDoor = north; else
    if (characters[player].x == 1            && characters[player].y == gridSize / 2) atDoor = east ; else
    if (characters[player].x == gridSize / 2 && characters[player].y == gridSize    ) atDoor = south; else
    if (characters[player].x == gridSize     && characters[player].y == gridSize / 2) atDoor = west ;

    switch(*inRoom)
    {
    case 0:
        if (atDoor == north) ; else
        if (atDoor == east ) ; else
        if (atDoor == south) ; else
        if (atDoor == west ) moveToRoom(1, characters, &gridSize, scenery, inRoom, atDoor);
    case 1:
        if (atDoor == north) ; else
        if (atDoor == east ) moveToRoom(0, characters, &gridSize, scenery, inRoom, atDoor); else
        if (atDoor == south) ; else
        if (atDoor == west ) ;
    }


}

void timedEvents(entities* characters, int gridSize, int scenery[MAX_SCENERY][MAX_SCENERY])
{
    if (turnDiff == 5 && characters[palfore].interact == 1)
    {
        interactionEvent(palfore, gridSize, characters, scenery);
    }
}

void bonuses(entities* characters, int a)
{
    bonus advantage;

    switch(characters[player].weaponID)
    {
    case fist:
        advantage.attBonus = 0;
        advantage.strBonus = 0;
        break;
    case logWeap:
        advantage.attBonus = 1;
        advantage.strBonus = 2;
        break;
    }

    switch(characters[player].armourID)
    {
    case abominal:
        advantage.hpBonus = 0;
        advantage.defBonus = 0;
        break;
    }

    switch(a)
    {
    case 0:
        characters[player].attack   += advantage.attBonus;
        characters[player].strength += advantage.strBonus;
        characters[player].defence  += advantage.defBonus;
        characters[player].health   += advantage.hpBonus;
        characters[player].maxHealth+= advantage.hpBonus;
        break;
    case 1:
        characters[player].attack   -= advantage.attBonus;
        characters[player].strength -= advantage.strBonus;
        characters[player].defence  -= advantage.defBonus;
        characters[player].health   -= advantage.hpBonus;
        characters[player].maxHealth-= advantage.hpBonus;
        break;
    }

}

void fight(entities* characters, int opponent, int scenery[MAX_SCENERY][MAX_SCENERY], int gridsize)
{
    int mods[4];
    char move;
    char oppMove;
    int turn = 0;
    characters[opponent].stunTurns = 0;

    bonuses(characters, 0);

    while (characters[player].health > 0 && characters[opponent].health > 0)
    {
        turn++;

        upperHalf(characters, opponent, turn);
        fightIcon(1, standing);
        fightIcon(0, standing);
        showChances(mods);
        giveOptions();
        oppMove = opponentsMove(mods);
        move = action(oppMove, characters, opponent, turn);
        outcome(mods, characters, opponent, move, oppMove);
        _getch();
    }
    winConditions(characters, opponent);
    bonuses(characters, 1);
    showBoard(gridsize, characters, scenery);

    return;
}

void winConditions(entities* characters, int opponent)
{
    char choice;

    system("cls");
    col(purple);
    printf("The Battle is Over....\n");

    if (characters[player].health > 0)
    {
        col(green);   printf("And you are the victor!\n\n");
        col(purple);  printf("After fighting this battle you much more able to:\n");
        col(green);   printf("\tTake more of a beating (h)\n");
        col(yellow);  printf("\tHit more accurately    (a)\n");
        col(red);     printf("\tDeploy bigger blows    (s)\n");
        col(darkAqua);printf("\tBetter block your foe  (d)\n");
        do
        {
            choice = _getch();
            switch(choice)
            {
            case 'h':
                characters[player].maxHealth += 3;
                characters[player].health += 3;
                break;
            case 'a':
                characters[player].attack ++;
                break;
            case 's':
                characters[player].strength ++;
                break;
            case 'd':
                characters[player].defence ++;
                break;
            default:
                choice = 0;
            }

        } while (choice == 0);
    }
    else
    {
        col(red);
        printf("Oh dear... You have been defeated...\n");
        printf("You gain no bonuses and your opponent takes some of your gold and flees.\n");
        _getch();
    }
    characters[opponent].health = characters[opponent].maxHealth;
    characters[opponent].x = -1;
    characters[opponent].y = -1;
    system("cls");


    return;
}

void outcome(int* mods, entities* characters, int opponent, char move, char oppMove)
{
    float pSuccess;
    float oSuccess;
    float pDMG = (float) characters[player].strength;
    float oDMG = (float) characters[opponent].strength;
    char pHit;
    char oHit;
    int heal;

    switch(move)
    {
    case 'h':
        pSuccess = (2.0/pi) * atan((float)(characters[player].attack * mods[pAttMod] * 0.8) / (float)(characters[opponent].defence * mods[oBlockMod]));
        pDMG *= 1.4;
        break;
    case 'l':
        pSuccess = (2.0/pi) * atan((float)(characters[player].attack * mods[pAttMod] * 1.2) / (float)(characters[opponent].defence * mods[oBlockMod]));
        pDMG *= 0.9;
        break;
    case 'd':
        pSuccess = (2.0/pi) * atan((float)(characters[player].defence * mods[pBlockMod]) / (float)(characters[opponent].attack * mods[oAttMod]));
        break;
    case 'r':
        pSuccess = 0.9;
        break;
    case 's':
        pSuccess = 0.5;
        break;
    case 'p':
        pSuccess = 100;
        break;
    }

    switch(oppMove)
    {
    case 'a':
        oSuccess = (2.0/pi) * atan((float)(characters[opponent].attack * mods[pAttMod]) / (float)(characters[player].defence * mods[oBlockMod]));
        break;
    case 'd':
        oSuccess = (2.0/pi) * atan((float)(characters[opponent].defence * mods[pBlockMod]) / (float)(characters[player].attack * mods[oAttMod]));
        break;
    }

    pHit = move;
    oHit = oppMove;

    if (rand() % 100 > (int)(100 * pSuccess)) // If you fail
            pHit = 0;
    if (rand() % 100 > (int)(100 * oSuccess))
            oHit = 0;

    if (oHit == 0) fightIcon(0, theymiss);
    else if (oppMove == 'a') fightIcon(0, attack);
    else if (oppMove == 'd') fightIcon(0, defend);

    if (pHit == 0)
    {
        fightIcon(1, youmiss);
        col(red);
        switch(move)
        {
        case 'h':
            printf("Your HEAVY attack misses");
            break;
        case 'l':
            printf("Your LIGHT attack misses");
            break;
        case 'd':
            printf("You fail to set up a defence");
            break;
        case 's':
            printf("Your STUN attack misses");
            break;
        case 'p':
            printf("The battle is too tense to drink now");
            break;
        case 'r':
            printf("The battle is too tense to rest now");
            break;
        }
    }
    else
    {
        switch(pHit)
        {
        case 'h':
        case 'l':
            if (oHit == 'd')
            {
                col(red);
                if (pHit == 'h') printf("Your HEAVY");
                else printf("Your LIGHT");
                printf(" attack HITS for 0 damage");
                characters[opponent].health += (int) pDMG;
            }
            else
            {
                col(green);
                if (pHit == 'h') printf("Your HEAVY");
                else printf("Your LIGHT");
                printf(" attack deals %d Damage", (int) pDMG);
            }
            characters[opponent].health -= (int) pDMG;
            break;
        case 'd':
            col(green);
            printf("You DEFEND and 1)2)3)");
            break;
        case 's':
            col(green);
            characters[opponent].stunTurns = (rand() % 2) + 1;
            printf("You STUN your opponent for %d turn(s)", characters[opponent].stunTurns);
            break;
        case 'p':

            heal = (int) (0.30 * (float) characters[player].maxHealth);
            if (characters[player].potions > 0)
                characters[player].potions --;
            else
            {
                col(red);
                printf("You are out of POTIONS and cannot heal");
                break;
            }
        case 'r':
            col(yellow);
            if (pHit != 'p')
                heal = (int) (0.05 * (float) characters[player].maxHealth);
            if (characters[player].health == characters[player].maxHealth)
                printf("You realize you are already at MAX HEALTH");
            else if (characters[player].health + heal >= characters[player].maxHealth)
            {
                    col(green);
                    printf("You HEAL to MAX HEALTH");
                    characters[player].health = characters[player].maxHealth;
            }
            else
            {
                col(green);
                printf("You HEAL %d Damage", heal);
                characters[player].health += heal;
            }
            break;
        }
    }
    printf(". \n");

    if (oHit == 0)
    {
        col(red);
        printf("And your opponent FAILS ");
        if (oppMove == 'a') printf("to HIT you.");
        else if (oppMove == 'd') printf("to DEFEND himself.");
    }
    else
    {
        switch(oHit)
        {
        case 'a':
            if (pHit == 'd')
            {
                col(darkRed);
                printf("And your opponent HITS for 0 damage.");
                characters[player].health += (int) oDMG;
            }
            else
            {
                col(green);
                printf("And your opponent HITS for %d damage.", (int) oDMG);
            }
            characters[player].health -= (int) oDMG;
            break;
        case 'd':
            col(green);
            printf("And your opponent BLOCKS.");
            break;
        }
    }
    return;
}

char opponentsMove(int* mods)
{
    int randNum = rand() % 100;
    char move;

    if (randNum < 50)
        move = 'a';
    else move = 'd';

    return move;
}

void upperHalf(entities* characters, int opponent, int turn)
{
    system("cls");
    col(aqua); printf("----------------- It's Time To -----------------\n");
    col(red);  printf("==================== FIGHT =====================\n");
    col(aqua); printf("Your Stats------------|||--------Opponents Stats\n");

    printf("Health: %d/%d : %3d%%    |||    Health: %d/%d : %3d%%\n", characters[player].health, characters[player].maxHealth,\
           100 * characters[player].health / characters[player].maxHealth,\
           characters[opponent].health, characters[opponent].maxHealth,\
           100 * characters[opponent].health / characters[opponent].maxHealth);
    printf("Att: %d                |||                Att: %d\n", characters[player].attack, characters[opponent].attack);
    printf("Str: %d                |||                Str: %d\n", characters[player].strength, characters[opponent].strength);
    printf("Def: %d                |||                Def: %d\n", characters[player].defence, characters[opponent].defence);
    printf("Potions: %d            |||            Potions: %d\n", characters[player].potions, characters[opponent].potions);
    printf("----------------------%2d -----------------------\n", turn);

    return;
}

void showChances(int* mods)
{
    int i;
    int j;

    for (i = 0; i < 4; i++)
    {
        mods[i] = rand() % 101 + 40;
        switch(i)
        {
        case pAttMod:
            col(yellow);
            printf("||Player  || Attack Mod : ");
            break;
        case oAttMod:
            col(red);
            printf("||Opponent|| Attack Mod : ");
            break;
        case pBlockMod:
            col(yellow);
            printf("Defend Mod : ");
            break;
        case oBlockMod:
            col(red);
            printf("Defend Mod : ");
            break;
        }
        printf("%3d :: ", mods[i]);
        for (j = 0; j < mods[i] / 10; j++)
        printf("=");
        for (j = j - 1; j < 13; j++)
        printf(" ");
        if ((i + 1)% 2  == 0) printf("\n");
    }
    return;
}

void giveOptions()
{
    col(aqua);
    printf("You try to...\n");
    col(white);
    printf("Heavy Attack (h) | Light Attack (l)\n");
    printf("Defend       (d) | Stun         (s)\n");
    printf("Rest         (r) | Use Potion   (p)\n");
    return;
}

void fightIcon(int who, int picture)
{
    int indent;

    if (who == 1)
    {
        indent = 7;
        col(yellow);
    }
    else
    {
        indent = 12;
        col(red);
    }
    switch(picture)
    {
    case standing:
                           moveCursor(indent, 9);
        printf("    0     "); moveCursor(indent, 10);
        printf("   /|\\   "); moveCursor(indent, 11);
        printf("  | | |   "); moveCursor(indent, 12);
        printf("  o | o   "); moveCursor(indent, 13);
        printf("   / \\   "); moveCursor(indent, 14);
        printf("  |   | \n");
        break;
    case heavy:
                           moveCursor(indent, 9);
        printf("    0     "); moveCursor(indent, 10);
        printf("    |---o "); moveCursor(indent, 11);
        printf("    |\\--o"); moveCursor(indent, 12);
        printf("    |     "); moveCursor(indent, 13);
        printf("   / \\   "); moveCursor(indent, 14);
        printf("  |   | \n");
        break;
    case light:
                           moveCursor(indent, 9);
        printf("    0     "); moveCursor(indent, 10);
        printf("   /|---o "); moveCursor(indent, 11);
        printf("  | |     "); moveCursor(indent, 12);
        printf("  o |     "); moveCursor(indent, 13);
        printf("   / \\   "); moveCursor(indent, 14);
        printf("  |   | \n");
        break;
    case defend:
                           moveCursor(indent, 9);
        printf(" |  0  |  "); moveCursor(indent, 10);
        printf(" |\\-|-/| "); moveCursor(indent, 11);
        printf("    |     "); moveCursor(indent, 12);
        printf("    |     "); moveCursor(indent, 13);
        printf("   / \\   "); moveCursor(indent, 14);
        printf("  |   | \n");
        break;
    case stun:
                           moveCursor(indent, 9);
        printf("    0   * "); moveCursor(indent, 10);
        printf("    |--/  "); moveCursor(indent, 11);
        printf("    |\\--*"); moveCursor(indent, 12);
        printf("    |     "); moveCursor(indent, 13);
        printf("   / \\   "); moveCursor(indent, 14);
        printf("  |   | \n");
        break;
    case rest:
                           moveCursor(indent, 9);
        printf("      zZZZ"); moveCursor(indent, 10);
        printf("    0     "); moveCursor(indent, 11);
        printf("   /|\\   "); moveCursor(indent, 12);
        printf("  / |  \\ "); moveCursor(indent, 13);
        printf("   / \\   "); moveCursor(indent, 14);
        printf("   \\ / \n");
        break;
    case potion:
                           moveCursor(indent, 9);
        printf("    0  <**>"); moveCursor(indent, 10);
        printf("    |-/ \\/"); moveCursor(indent, 11);
        printf("    |\\--/ "); moveCursor(indent, 12);
        printf("    |      "); moveCursor(indent, 13);
        printf("   / \\    "); moveCursor(indent, 14);
        printf("  |   | \n ");// moveCursor(0, 17);
        break;
    case attack:
                           moveCursor(indent, 9);
        printf("    0      "); moveCursor(indent, 10);
        printf("o---|\\    "); moveCursor(indent, 11);
        printf("    | |    "); moveCursor(indent, 12);
        printf("    | o    "); moveCursor(indent, 13);
        printf("   / \\    "); moveCursor(indent, 14);
        printf("  |   | \n "); //moveCursor(0, 17);
        break;
    case youmiss:
                           moveCursor(indent, 9);
        printf("    0      "); moveCursor(indent, 10);
        printf("o---|\\    "); moveCursor(indent, 11);
        printf("    | |    "); moveCursor(indent, 12);
        printf("    | o    "); moveCursor(indent, 13);
        printf("   / \\    "); moveCursor(indent, 14);
        printf("  |   | \n "); //moveCursor(0, 17);
        break;
    case theymiss:
                           moveCursor(indent, 9);
        printf("    0      "); moveCursor(indent, 10);
        printf("   /|---o  "); moveCursor(indent, 11);
        printf("  | |      "); moveCursor(indent, 12);
        printf("  o |      "); moveCursor(indent, 13);
        printf("   / \\    "); moveCursor(indent, 14);
        printf("  |   | \n ");// moveCursor(0, 17);
        break;
    }
    col(aqua);
    printf("------------------------------------------------\n");
    return;
}

void moveCursor(int a, int b)
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 2 * a;
    Position.Y = b;
    SetConsoleCursorPosition(hOut, Position);

    return;
}

char action(char oppMove, entities* characters, int opponent, int turn)
{
    int tryAgain;
    char moveType[50];
    char move;

    if (oppMove == 'a')
    {
        strcpy(moveType, "ATTACK");
        fightIcon(0, standing);
    }
    else if (oppMove == 'd')
    {
        strcpy(moveType, "DEFEND");
        fightIcon(0, standing);
    }

    do
    {
        move = _getch();
        tryAgain = 0;
        switch(move)
        {
        case 'h': // Heavy
            upperHalf(characters, opponent, turn);
            fightIcon(1, heavy);
         //   printf("------------------------------------------------\n");
            col(darkRed);
         //   printf("You try a HEAVY attack,");
            break;
        case 'l': // Light
            upperHalf(characters, opponent, turn);
            fightIcon(1, light);
         //   printf("------------------------------------------------\n");
            col(darkRed);
         //   printf("You try a LIGHT attack,");
            break;
        case 'd': // Defend
            upperHalf(characters, opponent, turn);
            fightIcon(1, defend);
         //   printf("------------------------------------------------\n");
            col(darkAqua);
      //      printf("You try to DEFEND,");
            break;
        case 'r': // Rest
            upperHalf(characters, opponent, turn);
            fightIcon(1, rest);
           // printf("------------------------------------------------\n");
            col(green);
      //      printf("You try to REST,");
            break;
        case 's': // Stun
            upperHalf(characters, opponent, turn);
            fightIcon(1, stun);
          //  printf("------------------------------------------------\n");
            col(yellow);
      //      printf("You try a STUNNING attack,");
            break;
        case 'p': // Potion
            upperHalf(characters, opponent, turn);
            fightIcon(1, potion);
          //  printf("------------------------------------------------\n");
            col(green);
       //     printf("You try to drink a POTION,");
            break;
        default:
            tryAgain = 1;
        }
    } while (tryAgain == 1);
    col(aqua);

    if (oppMove == 'a')
    {
        col(darkRed);
      //  printf("And your opponent tries to %s. ...\n", moveType);
        fightIcon(0, attack);
    }
    else
    {
        col(darkAqua);
     //   printf("And your opponent tries to %s. ...\n", moveType);
        fightIcon(0, defend);
    }

    col(aqua);

    return move;
}

void entityFollow(entities* characters, int gridSize, int settings[20], int scenery[MAX_SCENERY][MAX_SCENERY])
{
    int i;
    int j;
    int xDiff;
    int yDiff;
    int xMove = 1;
    int yMove = 1;
    int xSave;
    int ySave;

    for (i = palfore; i < MAX_CHARACTERS; i++)
    {
        if (i != monsterLv1) continue; // whitelist following monsters
        if (characters[i].x > 0 && characters[i].x <= gridSize && characters[i].y > 0 && characters[i].y <= gridSize)
        {
            updateScreen(characters, i, 0);
            xSave = characters[i].x;
            ySave = characters[i].y;
            xDiff = abs(characters[i].x - characters[player].x);
            yDiff = abs(characters[i].y - characters[player].y);

            if (characters[i].x > characters[player].x)
                xMove *= -1;
            if (characters[i].y > characters[player].y)
                yMove *= -1;

            if (xDiff == yDiff)
            {
                if (rand() % 100 < 50)
                    characters[i].x += xMove;
                else characters[i].y += yMove;
            }
            else if (xDiff > yDiff)
                characters[i].x += xMove;
            else if (xDiff < yDiff)
                characters[i].y += yMove;

            if (characters[i].x == characters[player].x && characters[i].y == characters[player].y)
            {
                interactionEvent(i, gridSize, characters, scenery);
                updateScreen(characters, i, 0);
            }
            else if (scenery[characters[i].x][characters[i].y] != null2)
            {
                characters[i].x = xSave;
                characters[i].y = ySave;
                updateScreen(characters, i, 1);
            }
            else
            {
                for (j = 0; j < MAX_CHARACTERS; j++)
                    if (characters[i].x == characters[j].x && characters[i].y == characters[j].y && i != j)
                    {
                        characters[i].x = xSave;
                        characters[i].y = ySave;
                        updateScreen(characters, i, 1);
                        break;
                    }
                if (j == MAX_CHARACTERS)
                    updateScreen(characters, i, 1);
            }
        }
    }



    return;
}

void clearText(int gridSize, int b)
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 2 * gridSize + 5;
    Position.Y = b;
    SetConsoleCursorPosition(hOut, Position);
    b++;
    if (b < 22)
    {
        printf("\t\t\t\t\t\t\t\n");
        clearText(gridSize, b++);
    }
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
    return;
}

void intro(entities* characters)
{
    int k;

    col(red);
    colorText(TEXT_SPEED, 1, 0, characters);
    col(darkYellow);
    printf("\n   ______________________________________________________\n");
    printf("  |======================================================\n");
    printf("  |------------------------------------------------------\n");
    for (k = 0; k < 4; k++)
        {printf("  |I|\t\t\t\t\t\t\t|\n");Sleep (50);}
         printf("  |I|            NOTEBOOK                               |\n");
    for (k = 0; k < 4; k++)
        {printf("  |I|\t\t\t\t\t\t\t|\n");Sleep (50);}
        printf("  |I|                     Name : ________________       |\n");
    for (k = 0; k < 6; k++)
        {printf("  |I|\t\t\t\t\t\t\t|\n");Sleep (50);}
        {
            HANDLE hOut;
            COORD Position;
            hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            Position.X = 32;
            Position.Y = 22;
            SetConsoleCursorPosition(hOut, Position);

            gets(characters[player].name);
            Position.X = 0;
            Position.Y = 30;
        }
        for (k = 0; k < 6; k++)
        printf("  |I|\t\t\t\t\t\t\t|\n");
        Sleep(1000);
        for (k = 0; k < 20; k++)
        {
            system("cls");
            if (k % 2 == 0)
                printf("\n");
            printf("\n        *You put the NoteBook into your jacket pocket\n");
            printf("        ...\n");
            printf("        huhuhuh... arrggg, huhuhuuhu\n");
            printf("        just..... *Gasps* One... more..\n");
            printf("        You stumble on a stone\n");
            printf("        ARRRGGGG!!!\n");
            printf("            *CRASH!*\n");
            Sleep(10);
        }
        pressEnter(0, 18);
        system("cls");
}

int startOptions(int settings[20])
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int l;
    char option = 0;
    do
    {
        option = 0;
        while (option != 'p' && option != 'o' && option != 'g' && option != 'c' && option != 'l')
            option = _getch();
        switch(option)
        {
        case 'p':
            system("cls");
            return 1;
            break;
        case 'o':
            settingsMenu(settings);
            system("cls");
            return 0;
            break;
        case 'g':
            guide(1);
            system("cls");
            return 0;
            break;
        case 'c':
            system("cls"); SetConsoleTextAttribute(hConsole, 12);
            printf("CREDITS:                                                 \n");SetConsoleTextAttribute(hConsole, 3);
            printf("------------------------------------                     \n");
            printf("Developper: Nawar Ismail                                 \n");
            printf("Guy who gives ideas: Devon Thomas                        \n");
            printf("Main Error Tester: Ellen Steele                          \n");
            printf("====================================                     \n");SetConsoleTextAttribute(hConsole, 8);
            printf("                                                                       \n");
            printf("Pictures:                                                \n");
            printf("\tN/A                                                    \n");
            printf("Code:                                                    \n");
            printf("\tSCREEN CLEARING: From: I LOST THE PAGE...                \n");
            printf("\tSCREEN COLORING:");
            printf("\n\t\tFrom: stackoverflow.com, colorful-text-using-printf-in-c\n\t\tBy: Tomas Zato\n");
            printf("                                                                       \n");
            printf("\tPress any key to go back.                                       \n");
            for (l = 0; l < 10; l++)
                printf("                                                 ");
            _getch();
            system("cls");
            return 0;
            break;
        case 'l':
   //         load();
            return 2;
            break;

        }
        if (option == 'l' || option == 'p')
            break;
    } while (option == 'g');

    return 0; // IDK if this is right
}

int startMenu(int settings[20])
{
            col(red);
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("           WELCOME to MALCHOVE'S MAZE!               \n");
            printf("=====================================================\n\n");
            col(green);
            printf("What would you like to do?:\n");
            printf("PLAY GAME (p)\n");
            printf("LOAD SAVE (l)\n");
            printf("OPTIONS   (o)\n");
            printf("GUIDE     (g)\n");
            printf("CREDITS   (c)\n");
            col(grey);
            printf("\nNotes: 1) You may press g at any time during the game to see the guide.\n");
            printf("       2) Saving the game will cause errors, if it is done before turn 4.\n");
            printf("       3) The game is not called MALCHOVE'S MAZE, it is a placeholder.\n");
            return startOptions(settings);
}

void colorText(int speed, int printTextNum, int gridsize, entities* characters)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int i;
    int j;
    int changeText[MAX_LETTERS] = {0};
    int len;
    char start[20] = "***";
    char end[20] = "***";
    char text[1000];

    for (i  = 0; i < 1000; i++)
        text[i]='\0';

    if (printTextNum == 1)
        readyText(0, -2);
    else
    {
        clearText(gridsize, 0);
        readyText(gridsize, 2);
    }


    start[3] = (printTextNum % 10000) / 1000 + 48;
    start[4] = (printTextNum % 1000)  / 100  + 48;
    start[5] = (printTextNum % 100)   / 10  + 48;
    start[6] = (printTextNum % 10)    / 1    + 48;
    start[8] = '\0';
    end[3] = ((printTextNum + 1) % 10000) / 1000 + 48;
    end[4] = ((printTextNum + 1) % 1000)  / 100  + 48;
    end[5] = ((printTextNum + 1) % 100)   / 10  + 48;
    end[6] = ((printTextNum + 1) % 10)    / 1    + 48;
    end[7] = '\0';

  //  start[3] = printTextNum;
    strcat(start, "***.");
  //  end[3] = printTextNum + 1;
    strcat(end, "***.");

   // printf("((%s)(%s))", start, end);

    FILE* inFile = NULL;
    inFile = fopen("dialog.txt", "r");
    if (inFile == NULL)
    {
        printf("ERROR 003: NO DIALOG FILE \"dialog.txt\" FOUND TO OPEN\n");
        pressEnter(0, gridsize);
    }
    char temp[150];

    // Find the starting sequence
    do
    {
        fgets(temp, 150, inFile);
        temp[strlen(temp)-1] = '.';
   //  puts(temp);

    } while (strcmp(start, temp) != 0 && !feof(inFile));

    fgets(temp, 150, inFile);

    temp[strlen(temp) - 1] = '>';
    temp[strlen(temp)] = '\0';
    strcat(text, temp);
    while (strcmp(end, temp) != 0 && !feof(inFile)) // not at end of line
    {
        fgets(temp, 150, inFile);
        temp[strlen(temp)-1] = '.';
     //   printf("\n(%s)(%s)", temp, text);
     //   _getch();
        if (strcmp(end, temp) != 0 && !feof(inFile))
            {
                temp[strlen(temp)-1] = '>';
                temp[strlen(temp)] = '\0';
                strcat(text, temp);
            }
    }
    fclose(inFile);


///Gets the length of the text for all texts up till the max
        len = strlen(text);

/// Identifies Name codes "Capitol_letter.... ':'"
    for (i = 0; i < len; i++)
    {
        if (text[i] == ':') // Tells me where ':' is
        {
            j = i;
            /// Enviroment/Thought/Effects (***:)
            if (text[i - 1] == '*')
            {
                changeText[i - 3] = white;
            }
            /// NAMES
            else
            {
                while (text[j - 1] >  'a' && text[j - 1] < 'z')
                    j--;
                if (text[j - 1] == '}') // Last letter of name (Character)
                    changeText[j - 1] = red;
                else if (text[j - 2] == 'P') // I dont really know where the [j-csnt comes from], to find guess and check
                    changeText[j - 2] = aqua;
                else if (text[j - 3] == 'O')
                    changeText[j - 3] = white;
                else if (text[j - 1] == 'D')
                    changeText[j - 1] = darkGreen;
                else if (text[j - 3] == '?')
                    changeText[j - 3] = darkPurple;
            }
        }
    }

    int enters = 3;

/// Prints Text
    for (i = 0; i < strlen(text); i++)
    {
        if (changeText[i] != 0)
            SetConsoleTextAttribute(hConsole, changeText[i]);
        if (text[i] == '`')      // ` results in press enter function
            {col(grey); pressEnter(0, gridsize); enters = 2; i++;}
        else if (text[i] == '}') // prints player name
            {printf("%s", characters[player].name); i++;}
        else if (text[i] == '/') // sleeps for 500
            {Sleep(TEXT_SPEED * 9); i++;}

   /*     else if (text[i] == '~' && text[i + 1] == '~')
            {screenShake(2); i++; i++;} */
        putchar(text[i]);
        if (text[i] == '{')
            {printf("\b ");for (j = 0; j <= strlen(characters[player].name); j++)printf(" ");}
        else if (text[i] == '>') // prints enters (writes over the > to signal the enter) probably better way to do this
        {
            printf("\b ");
            if (printTextNum != 1)
                readyText(18,enters++);
            else printf("\n");
        }
        Sleep(speed);
    }
    return;
}

void readyText(int gridSize, int a) // Moves cursor to text location at second entry (y)
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 2 * gridSize + 5;
    Position.Y = a;
    SetConsoleCursorPosition(hOut, Position);
    return;
}

void interactionEvent(int eventNum, int gridSize, entities* characters, int scenery[MAX_SCENERY][MAX_SCENERY])
{
    if (eventNum > 0)
    {
        switch(eventNum)
        {
        case palfore:
            switch(characters[eventNum].interact)
            {
            case 0:
                colorText(TEXT_SPEED, 8, gridSize, characters);
                turnDiff = 0;
                characters[eventNum].interact = 1;
                break;
            case 1:
                colorText(TEXT_SPEED, 9, gridSize, characters);
                characters[eventNum].interact = 2;
                turnDiff = -1;
                break;
            case 2:
                colorText(TEXT_SPEED, 11, gridSize, characters);
                createDoors(east, gridSize, scenery);
                colorText(TEXT_SPEED, 12, gridSize, characters);
                characters[eventNum].interact = 3;
                break;
            }
            break;
        case adrea:
            colorText(TEXT_SPEED, 4, gridSize, characters);
            clearText(18, 0);
            if (characters[eventNum].interact < 1)
            {
                colorText(TEXT_SPEED, 5, gridSize, characters);
                characters[monsterLv1].x = 12;
                characters[monsterLv1].y = 13;
                updateScreen(characters, monsterLv1, 1);
                placeScen(3, 7, fallenlog, scenery);
                characters[eventNum].interact = 1;

            }
            break;
        case monsterLv1:
            fight(characters, eventNum, scenery, gridSize);
            characters[eventNum].interact ++;
            if (characters[eventNum].interact == 1)
            {
                characters[decayedAdrea].x = characters[adrea].x;
                characters[decayedAdrea].y = characters[adrea].y;

                characters[adrea].x = -1;
                characters[adrea].y = -1;
                updateScreen(characters, decayedAdrea, 1);

                colorText(TEXT_SPEED, 10, gridSize, characters);

                characters[palfore].x = 8;
                characters[palfore].y = 4;
                colorText(TEXT_SPEED, 6, gridSize, characters);
                updateScreen(characters, palfore, 1);
            }
            break;
        }
    }
    else
    {
        switch(-eventNum)
        {
            case tree:
                break;
            case fallenlog:
                characters[player].weaponID = logWeap;
                showStats(characters, gridSize);
                colorText(TEXT_SPEED, 7, gridSize, characters);
                placeScen(3, 7, null2, scenery);
                break;
        }
    }
    return;
}

int collisionCheck(entities* characters, int scenery[MAX_SCENERY][MAX_SCENERY])
{
    int i;
    int j;
    int k;

    for (k = 2; k < MAX_CHARACTERS; k++)
        if (characters[player].x == characters[k].x && characters[player].y == characters[k].y)
            return k; // tells me who I inteacted with
    for (i = 1; i < MAX_SCENERY; i++)
        for (j = 1; j < MAX_SCENERY; j++)
            if (scenery[characters[player].x][characters[player].y] != null2)
                return -scenery[characters[player].x][characters[player].y];

    return 0;
}

void placeScen(int x, int y, int character, int scenery[MAX_SCENERY][MAX_SCENERY])
{
    scenery[x][y] = character;
    moveCursor(x,y);
    printScen(character);

    return;
}

void guide(int showScreen)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");SetConsoleTextAttribute(hConsole, 12);
    printf("GUIDE:\n");
    printf("------------------------------------\n");SetConsoleTextAttribute(hConsole, 11);
    printf("What would you like to learn about?\n");
    printf("Icons    (i)\n");
    printf("Keys     (k)\n");
    printf("Fighting (f)\n");
    printf("Go Back  (b)\n");
    char option = 0;
    while (option != 'i' && option != 'f' && option != 'b' && option != 'k')
        option = _getch();
    system("cls");SetConsoleTextAttribute(hConsole, 12);
    printf("GUIDE: ");
    switch(option)
    {
        case 'i':
            printf("Icons:\n");
            printf("------------------------------------\n"); SetConsoleTextAttribute(hConsole, 8);
            printf("Y : Player (You)\n");
            printf("T : Tree        \n");
            printf("P : Palfore      \n");
            printf("M : Monster     \n");
            printf("= : Brick       \n");
            printf("| : Door        \n");
            printf("^ : Mountains   \n");
            break;
        case 'k':
            printf("Keys:\n");
            printf("------------------------------------\n"); SetConsoleTextAttribute(hConsole, 8);
            printf("The following keys are available during regular game play.\n");
            printf("\tIf other options are available, they will be presented explicitly\n\n");
            printf("Arrow Keys: Move your character in the direction of the key pressed.\n");
            printf("W,A,S,D:    Same as above.\n");
            printf("M:          Brings up the Map.\n");
            printf("G:          Brings up the Guide.\n");
            break;
        case 'f':
            printf("Fighting:\n");
            printf("------------------------------------\n"); SetConsoleTextAttribute(hConsole, 8);
            printf("1) A fight begins when you are on the same square as a monster.\n");
            printf("2) You and your opponent's stats are displayed at the top of the screen.\n");
            printf("3) Attack increases hit chance, strength increases damage,\n\tdefence increases block chance.\n");
            printf("4) These are the the possible combat Options:\n");
            printf("\ti)  Heavy Attack \n\t\t(h): Low accuracy, High damage, Much less accuracy next attack.\n");
            printf("\tii) Light Attack \n\t\t(l): High accuracy, Low damage, less accuracy next attack.\n");
            printf("\tiii)Defend       \n\t\t(d): Chance to Block (100%% protection),\n\t\t     Reduce (% Based on Defence),\n");
            printf("\t\t     or recoil damage back.\n");
            printf("\tiv) Stun         \n\t\t(s): Fixed chance to stun opponent for the immediate turn\n\t\t     and possibly the next.\n");
            printf("\tv)  Rest         \n\t\t(r): Heals an amount of your Max Health.\n");
            printf("\tvi) Potion       \n\t\t(p): You use a collected potion to heal a chunk of health\n\t\t     based on your Max Health.\n");
            printf("5) You must decicide what action is best, based on the modifier bars.\n");
            printf("\tThese are NOT chances to perform actions,\n\tbut multipliers that affect a creatures chances.\n");
            printf("6)If you die, you are given a chance to try again, or quit the game.\n");
            printf("7)If you manage to defeat your opponent, you will get to choose a stat\n\tto be increased.");
            printf("Health will be boosted by 5 points,\n\tall the others will be boosted by 1.\n");
            break;
        case 'b':
            break;
    }
    if (option != 'b')
    {
        pressEnter(0, 18);
        system("cls");
        guide(0);
    }
}

void pressEnter(int printScreen, int gridsize) // Informs the user they may contiue if they press enter
{
    char keyPressed;

    printf("\tPress Enter or Space to continue.");
    while (keyPressed != '\n' && keyPressed != ' ')
    {
        keyPressed = _getch();
        if (keyPressed == 13)
        break;
    }
    clearText(gridsize, 0);
    if (printScreen == 1)
  printf("NOT YET AVAIBleE");//  showBoard(199,-1,-1);
}

void displayInfo(char displayType)
{
    system("cls");
    switch(displayType)
    {
    case 'm':
        system("cls");
    //    displayMap();
        pressEnter(0, 18);
        break;
    case 'g':
        guide(0);
        break;
    }
    system("cls");
    return;
}

void settingsMenu(int settings[20])
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char option;

    system("cls");SetConsoleTextAttribute(hConsole, 12);
    printf("SETTINGS/OPTIONS:\n");
    printf("------------------------------------\n"); SetConsoleTextAttribute(hConsole, 11);
    printf("F: Toggle Fighting.\n");
    printf("\tFighting is currently:");
    switch (settings[0])
    {
        case -1:
            col(red);
            printf("Off\n");
            break;
        case 1:
            col(green);
            printf("On\n");
            break;
        default:
            printf("ERROR: Fighting type UNKOWN\n");
    } SetConsoleTextAttribute(hConsole, 8);
    printf("\n\tPress b to go back.\n");
    option = 0;
    while (option != 'b' && option != 'f') //&& option != 'g' && option != 'c' && option != 'l')
        option = _getch();
    switch (option)
    {
        case 'f':
            settings[0] = -1 * settings[0];
            settingsMenu(settings);
            break;
        case 'b':
            break;
    }
    system("cls");
    return;
}

int playersTurn(entities* characters, int gridSize, int settings[20], int scenery[MAX_SCENERY][MAX_SCENERY])
{
    char move;
    int change = 0;

    do
    {
        move = 0;
        while ((move != 72 && move != 80 && move != 77 && move != 75 && move != 'm' && move != 'g' && move != 'r' && move != 'o') && move != 'w' && move != 's' && move != 'd' && move != 'a')
        {
            move = _getch();
        }
        switch (move) // if change is < 0 , it is affecting xPos.
        {
            case 'w':
            case 72: // Up arrow
                change = -2;
                break;
            case 's':
            case 80: // Down arrow
                change = 2;
                break;
            case 'd':
            case 77: // Right arrow
                change = 1;
                break;
            case 'a':
            case 75: // Left arrow
                change = -1;
                break;
            case 'm':
            case 'g':
                displayInfo(move);
                break;
            case 'o':
                settingsMenu(settings);
                break;
            case 'r': //s and w are used, so r  is used for w(r)ite
                return 1;
                break;
            default:
                printf("ERROR 420: Bad Char");
        }
        if (change == 0) // The player did not move, therefore pressed menu option
            showBoard(gridSize, characters, scenery);
    } while (change == 0);
    movePlayer(change, characters, gridSize, scenery);
    return 0;
}

void movePlayer(int move, entities* characters, int gridSize, int scenery[MAX_SCENERY][MAX_SCENERY])
{
    updateScreen(characters, player, 0);

    int xSave = characters[player].x;
    int ySave = characters[player].y;
    int interactedWith;

    switch(move)
    {
        case 1:
            if (characters[player].x < gridSize){
                characters[player].x++;
            }
            break;
        case 2:
            if (characters[player].y < gridSize){
                characters[player].y++;
            }
            break;
        case -1:
            if (characters[player].x > 1){
                characters[player].x--;
            }
            break;
        case -2:
            if (characters[player].y > 1){
                characters[player].y--;
            }
            break;
    }
    interactedWith = collisionCheck(characters, scenery);
    readyText(gridSize, 4);
    if (interactedWith)
    {
        characters[player].x = xSave;
        characters[player].y = ySave;
        updateScreen(characters, player, 1);
        interactionEvent(interactedWith, gridSize, characters, scenery);
    }
    updateScreen(characters, player, 1);

    return;
}

void initialize(entities* characters, int scenery[MAX_SCENERY][MAX_SCENERY])
{
    int i;
    int j;
    int who;

    for (int k = 0; k < MAX_CHARACTERS; k++)
    {
        if (k != player)
            strcpy(characters[k].name, "0\0");
        //characters[k].icon[2] = '\0';
        characters[k].color = 0;
        characters[k].x = 0;
        characters[k].y = 0;
        characters[k].maxHealth = 0;
        characters[k].health = 0;
        characters[k].attack = 0;
        characters[k].strength = 0;
        characters[k].defence = 0;
        characters[k].potions = 0;
        characters[k].coins = 0;
        characters[k].weaponID = 0;
        characters[k].armourID = 0;
        strcpy(characters[k].weapon, "0\0");
        strcpy(characters[k].armour, "0\0");
        characters[k].interact = 0;
        characters[k].stunTurns = 0;
    }

///CHARACTERS******************************
    ///Player
    who = player;
    characters[who].x = 4;
    characters[who].y = 7;
    characters[who].maxHealth = 17;
    characters[who].health = 17;
    characters[who].attack = 4;
    characters[who].strength = 3;
    characters[who].defence = 3;
    characters[who].potions = 1;
    characters[who].coins = 50;
    characters[who].weaponID = fist;
    characters[who].armourID = abominal;
    if (defaultIcons == 0){
        characters[who].icon[0] = -73;
        characters[who].icon[1] = -75;
    } else if (defaultIcons == 2){
        characters[who].icon[0] = 2;
        characters[who].icon[1] = ' ';
    } else {
        characters[who].icon[0] = 'Y';
        characters[who].icon[1] = ' ';
    }
    characters[who].color = yellow;
    characters[who].interact = 0;
    /// Palfore
    who = palfore;
    if (defaultIcons == 0){
        characters[who].icon[0] = -73;
        characters[who].icon[1] = -75;
    }  else if (defaultIcons == 2){
        characters[who].icon[0] = 1;
        characters[who].icon[1] = ' ';
    } else {
        characters[who].icon[0] = 'P';
        characters[who].icon[1] = ' ';
    }
    characters[who].color = blue;
    characters[who].interact = 0;
    /// Adrea
    who = adrea;
    if (defaultIcons == 0){
        characters[who].icon[0] = -113;
        characters[who].icon[1] = -110;
    } else {
        characters[who].icon[0] = 'T';
        characters[who].icon[1] = ' ';
    }
    characters[who].color = green;
    characters[who].interact = 0;
    /// Decayed Adrea
    who = decayedAdrea;
    if (defaultIcons == 0){
        characters[who].icon[0] = 'a';
        characters[who].icon[1] = ' ';
    } else {
        characters[who].icon[0] = 'a';
        characters[who].icon[1] = ' ';
    }
    characters[who].color = green;
    characters[who].interact = 0;
    /// Monster lv 1
    who = monsterLv1;
    characters[who].maxHealth = 9;
    characters[who].health = 9;
    characters[who].attack = 3;
    characters[who].strength = 1;
    characters[who].defence = 1;
    characters[who].potions = 0;
    if (defaultIcons == 0){
        characters[who].icon[0] = -73;
        characters[who].icon[1] = -75;
    } else if (defaultIcons == 2){
        characters[who].icon[0] = 2;
        characters[who].icon[1] = ' ';
    } else {
        characters[who].icon[0] = 'M';
        characters[who].icon[1] = ' ';
    }
    characters[who].color = red;
    characters[who].interact = 0;


///SCENERY*********************************
    for (i = 0; i < MAX_SCENERY; i++)
        for (j = 0; j < MAX_SCENERY; j++)
            {scenery[i][j] = null2;}
    return;
}

void col(int color)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    return;
}

void updateScreen(entities* characters, int name,  int c)
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 2 * (characters[name].x);
    Position.Y = (characters[name].y);
    SetConsoleCursorPosition(hOut, Position);
    if (c == 1)
        printChar(characters[name]);
    else printf("  ");
    return;
}

void printChar(entities thing)
{
    col(thing.color);
    printf("%s", thing.icon);
    return;
}

void printScen(int print)
{
    switch (print)
    {
    case tree:
        col(green);
        if (defaultIcons == 0) printf("%c%c", -74, -57);
        else printf("T ");
        break;
    case fallenlog:
        col(darkBlue);
        if (defaultIcons == 0) printf("%c%c", 'L', ' ');
        else printf("L ");
        break;
    case null2:
        printf("  ");
        break;
    default:
        printf("%d", print);
        break;
    }
    return;
}

void showBoard(int gridSize, entities* characters, int scenery[MAX_SCENERY][MAX_SCENERY]) // Displays the main screen, and is overly responible for events
{
    int i;
    int j;
    int k;
    int l;
    int boarderColor = aqua;

    printf(" ");
    for (l = 1; l < gridSize + 1; l++)
    {
        col(boarderColor);
        printf("--");
    }
    printf("                           \n");
    for (j = 1; j < gridSize + 1; j++)
    {
        col(boarderColor); printf("| ");
        for (i = 1; i < gridSize + 1; i++)
        {
            printScen(scenery[i][j]); // Scenery is printed, then written over
            for (k = 1; k < MAX_CHARACTERS; k++)
                if (characters[k].x == i && characters[k].y == j)
                {
                    printf("\b\b"); // Removes scenery
                    printChar(characters[k]);
                }
        }
        col(boarderColor); printf("|\n");
    }
    printf(" ");
    for (l = 1; l < gridSize + 1; l++)
    {
        col(boarderColor);
        printf("--");
    }
    showStats(characters, gridSize);
    return;
}

void showStats(entities* characters, int gridSize)
{
    col(darkGreen);

    switch(characters[player].weaponID)
    {
    case fist:
        strcpy(characters[player].weapon, "FIST");
        break;
    case logWeap:
        strcpy(characters[player].weapon, "LOG");
        break;
    }

    switch(characters[player].armourID)
    {
    case abominal:
        strcpy(characters[player].armour, "ABS");
        break;
    }

    bonuses(characters, 0);

    moveCursor(0, gridSize + 1);
    printf("\nHP:%2d/%2d [%3d%% ]|Att:%2d|Str:%2d|Def:%d\n", characters[player].health, characters[player].maxHealth,\
            (int) (100.0 * (float)characters[player].health / (float) characters[player].maxHealth), characters[player].attack, characters[player].strength, characters[player].defence);
    printf("$$:%4d|Weapon:%7s|Armour:%7s", characters[player].coins, characters[player].weapon, characters[player].armour);

    bonuses(characters, 1);


    return;
}

void createDoors(int doors, int gridSize, int scenery[MAX_SCENERY][MAX_SCENERY]) // Creates exits for the player to leave a room
{
    switch(doors)
    {
        case west:
            scenery[1][gridSize / 2] = null2;
            placeScen(1, gridSize / 2, null2, scenery);
            break;
        case east:
            scenery[gridSize][gridSize / 2] = null2;
            placeScen(gridSize, gridSize / 2, null2, scenery);
            break;
        case north:
            scenery[gridSize / 2][1] = null2;
            placeScen(gridSize / 2, 1, null2, scenery);
            break;
        case south:
            scenery[gridSize / 2][gridSize] = null2;
            placeScen(gridSize / 2, gridSize, null2, scenery);
            break;
        case all:
            scenery[1][gridSize / 2] = null2;
            scenery[gridSize][gridSize / 2] = null2;
            scenery[gridSize / 2][1] = null2;
            scenery[gridSize / 2][gridSize] = null2;
            placeScen(1           , gridSize / 2, null2, scenery);
            placeScen(gridSize    , gridSize / 2, null2, scenery);
            placeScen(gridSize / 2,            1, null2, scenery);
            placeScen(gridSize / 2,     gridSize, null2, scenery);
            break;
    }
}

void createBoarders(int lines, int type, int gridSize, int scenery[MAX_SCENERY][MAX_SCENERY]) // Creates a parameter of any symbol
{
    int z;
    switch(lines)
    {
        case 1: // vertical line at x = 0;
            {
                for (z = 1; z <= gridSize; z++)
                    scenery[1][z] = type;
                break;
            }
        case 2: // vertical line at x = maxGrideSize
            {
                for (z = 1; z <= gridSize; z++)
                    scenery[gridSize][z] = type;
                break;
            }
        case 3: // horozonal line at y = 0
            {
                for (z = 1; z <= gridSize; z++)
                    scenery[z][1] = type;
                break;
            }
        case 4: // horozonal line at y = 0
            {
                for (z = 1; z <= gridSize; z++)
                    scenery[z][gridSize] = type;
                break;
            }
        case 5:
            {
                for (z = 1; z <= gridSize; z++)
                {
                    scenery[1][z] = type;
                    scenery[gridSize][z] = type;
                    scenery[z][1] = type;
                    scenery[z][gridSize] = type;
                 }
                 break;
            }
    }
    return;
}

void createRoom(int inRoom, int* gridSize, int scenery[MAX_SCENERY][MAX_SCENERY], entities* characters) // Maybe have this function reset all char pos, and set only the ones you want. Defines the environment on screen
{
    for (int m = 0; m < MAX_SCENERY; m++)
    {
         for (int l = 0; l < MAX_SCENERY; l++)
             scenery[l][m] = null2;
    }
    for (int m = 0; m < MAX_CHARACTERS; m++)
    {
        if (m != player)
        {
            characters[m].x = 0;
            characters[m].y = 0;
        }
    }

    switch(inRoom)
    {
        case 0: // Starting room
            {
                *gridSize = 18;
                createBoarders(5, tree, *gridSize, scenery);
               // getScenery(0);
                scenery[4][3] = tree;
                scenery[7][13] = tree;
                scenery[10][15] = tree;
                scenery[15][9] = tree;
                scenery[3][5] = tree;
                scenery[13][7] = tree;
                scenery[8][4] = tree;
                if (characters[palfore].interact >= 2)
                {
                    createDoors(east, *gridSize, scenery);
                    characters[palfore].x = 8;
                    characters[palfore].y = 4;
                }
                break;
            }
        case 1:
            {
                *gridSize = 18;
                createBoarders(5, tree, *gridSize, scenery);
                createDoors(west, *gridSize, scenery);
                scenery[8][10] = tree;
                scenery[2][4] = tree;
                scenery[15][12] = tree;
                break;
            }
    }
    return;
}

