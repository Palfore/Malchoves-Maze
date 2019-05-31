#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include <math.h>

#define MAX_TEXTS 100
#define MAX_LETTERS 2000

/// unusable text characters: `, }, ~~

    enum direction{west, east, north, south};
    enum color{black, darkBlue, darkGreen, darkAqua, darkRed, darkPurple, darkYellow, grey, darkGrey, blue, green, aqua, red, purple, yellow, white};

    int xPos = 1;
    int yPos = 7;
    int characterBank[200]; // every 2 values is a new character
    int numInteract[100] = {0}; // ONLY even numbers (start at 0) are meaningful, odd are meant to help identify the characters in the bank above.
    int stats[1000]; //every 5 is a new char
    char statNames[5][20] = {"Max Health", "Health", "Attack", "Strength", "Defence"};
    int turnDiff = -1;
    int maxGridSize = 18;
    char scenery[40][40]; //z = f(x,y) => icon = x, y pos
    char boarderType[40] = {'T','=',' ',']','@','[','|','~','^','-','*','#','t'};
    int inRoom = 0;
    char xGrid[100];
    int numFights = 0;
    int settings[2] = {1, 1};
    int enteredRooms[30][30] = {0};
    int xMap = 15; // Current world location
    int yMap = 15;
    char playerName[20];

    void colorText(int speed, int printTextNum);
    void interactionEvent(int interactionNum);
    void displayMap();
    void configureMap(enum direction addRoomToThe); // Change the 30's if you change mapSize
    void clearScreenText();
    void clearScreen();
    void settingsMenu();
    void newRoomPos(enum direction cameFrom);
    void RoomChanging(); // Character's will only be updated here if the room changes
    void showStats(int fighting);
    int load();
    int save();
    void displayInfo(char displayType);
    void guide(int showScreen);
    void startOptions();
    void enemyEvents(); // May not be used much
    void increaseStat(); // Player gets to choose a stat to boost
    void pressEnter(int printScreen); // Informs the user they may contiue if they press enter
    void initializeStats(); // Sets all stats to their default
    void createDoors(int doors); // Creates exits for the player to leave a room
    void createBoarders(int lines, char type); // Creates a parameter of any symbol
    void createRoom(int rooms); // Maybe have this function reset all char pos, and set only the ones you want. Defines the environment on screen
    void showBoard (int bankPos, int bankPosChangeX, int bankPosChangeY); // Displays the main screen, and is overly responible for events
    void screenShake(int messageNum); // Causes the Screen to "shake" with a given message
    void giveChances(float hit, float defend, float oppHit, float oppDefend); // Gives the probablility bars in fight screen
    int nearOrHit();
    int moveChar();
    void fight(int monsterNum); // When there are more fights, call in the monster tiy are fighting // stunning doesnt work!
    int printText(int textNum); // Prints text
    int gameProgress(int turn); // Runs major events... Kind of (not really used anymore
    void moveMonster(int monsterNum); // Performs movement towards the player
    void regenRoom(int a, int b);

int main ()
{
    srand(time(NULL));
    int check;
    int i;

    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    enteredRooms[15][15] = 2;
    printText(-2);

    numInteract[24] =  1;
    numInteract[25] = -1;
    numInteract[26] =  1;

    for (i = 0; i < 1000; i++)
    {
        showBoard(0, xPos, yPos);

        if (i != 5 && i < 1000)
        gameProgress(i);
        moveMonster(4);
        moveMonster(14);
        moveMonster(16);

        printf("                                                                          \n");
        printf("                                                                          \n");
        printf("                                                                          \n");
        printf("                                                                          \n");
        printf("                                                                          \n");
        printf("                                                                          \n");
        printf("                                                                          \n");
        printf("                                                                          \n");



        check = moveChar();
        if (check > 0)
        {
            yPos = yPos - (check - 2);
        }
        else
        {
            if (check < 0)
            {
                xPos = xPos - (check + 2); //"/2"Because of the way the function is used to handle two variables.
            }
        }
     }
     printf("\n");
     return 0;
}

void regenRoom(int a, int b)
{
                        int randomNum = (rand() % 10) + 1; // Random num between 1 and 10
                        switch (randomNum)
                        {
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                            scenery[a][b]   = boarderType[2];
                            break;
                        case 7:
                        case 8:
                            scenery[a][b]   = boarderType[0];
                            break;
                        case 9:
                        case 10:
                            scenery[a][b]   = boarderType[12];
                            break;
                        }

}

void colorText(int speed, int printTextNum)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int i;
    int j;
    int changeText[MAX_LETTERS] = {0};
    int len[MAX_TEXTS];

// Backslash lets you continue on next line (gets rid of newline)
    char text[MAX_TEXTS][MAX_LETTERS] = \
    {
// TEXT 0*************************************************************************0
"}: Oy! My head! Why does it hurt so much?\n\
        Where am I?\n\
***:Perhaps you should explore your surroundings:***\n"
    ,
// TEXT 1*************************************************************************1
"}: Wait, what's that over there?`\b"
                                                                                    ,"2",
// TEXT 3*************************************************************************3
"***: You've obtained a LOG (+1 Strength) :***`\b"
                                                                                   ,"4","5","6","7",
// TEXT 8*************************************************************************8
"???: Hey!\n\
      ./././\n\
???: Behind you!`\b"
    ,
// TEXT 9*************************************************************************9
"???:Hello, }.\n\
}: What? How did you know my name??\n\
Palfore: Thats not important, my name is Palfore, and that...\n\
         thing you just fought...\n\
         It wasn't of this world, and I need your help to stop those things.\n\
         Poor Adrea...`\b\
}: What?? What are you talking about? Who's Adrea?\n\
   You're just a senile old man, I don't know what that thing was\n\
   but do know that you, ARE CRAZY!\n\
   And I'm leaving!`"
   ,
// TEXT 10*************************************************************************10
"Palfore: HEY!\n\
         THIS ISN'T A JOKE!! You and EVERYONE else in this world is in danger!\n\
         You were the last person to be near Adrea, before she whithered away...\n\
         Please come back... The fate of the world is in your hands...`\b"
                                                                                     ,"11","12","13","14","15","16","17",
// Text 18 ************************************************************************18
"}: ... Fine I will listen to your story, but don't talk crazy!\n\
    I've had enough of that already today!\n\
Palfore: Thank you...\n\
         So, let us start at the beginning...`\b\
Palfore: It all started over 100 years ago.\n\
         Damadaeus, was the protecter of our land...\n\
         He was a great powerhouse of a man!\n\
         He could move boulder with the touch of fin-\n\
}: BOOOOOORRRRRRING! Move on!\n\
Palfore: ...`\b\
\
Palfore: Very well...\n\
         DeMalae was the main reason we needed a protector.\n\
         He's wicked necromancer...\n\
         He started off weak, decaying the occasional leaf, but his power has grown unbounded.\n\
         Recently he managed to defeat Damadaeus\n\n\
         I... I don't know how, he was always so jealous of Damadaeus' power!`\b\
\
Palfore: Regardless, Damadaeus constantly embued Adrea to protect us from necromancy\n\
         Without Damadeaus, Adrea will continue to decay\n\
         And our protection agaisnt those fiends will disapear...\n\
         This is why we need you }...\n\
}: Why me? What could I do?? You seem a lot more able!\n\
Palfore: BAHE-HAEEeeee! *he whezzes a laugh*`\b\
         I'm far too old, I need you to go see the Oracle to the EAST!\n\
         Please... The world is in your hands...\n\
}: But I'm... I don't kn-\n\
Palfore: GO! Time is running out!`\n\
}: Hmmm... There's an opening in the trees to the EAST now!\n\
   Maybe the Oracle is there!`\n"
       ,
//TEXT 19*************************************************************************19
"***: You open the chest :***\n\
}: There's a book here!\n\
It's says BOOK OF DEFENCE\n\
I feel stronger just holding it!\n\
***: You've GAINED 2 DEFENCE! :***`\b\
}: This must be the Oracles house.\n\
      I wonder where she is...`\b~~\n\
It sounds like something happened outside!`\b"
            ,
//TEXT 20*************************************************************************20
"Oracle: Greetings....\n\
}: Hey, I was told to see you by a wiz-\n\
Oracle: Increase the protection around your chest\n\
        So night turns to day\n\
        As open goes to... what?\n\
}: Uhhhh....\
      What does that even mean??\n\
***: The Oracle does not respond to you :***\n"
                                                                                    ,"21","22","23",
//TEXT 24*************************************************************************24
"???: What a man can't do his job with out being yelled at anymore?? Darn kids!`\b\
}: Oh I'm sorry, I didn't see you there!\n\
???: No one ever sees me...\n\
     Everyone always thinks these trees just \"magically\" cut themselves down!\n\
     Everyone always thinks some great creator just \"deletes\" these trees,\n\
     creating some sort of major plot hole!\n\
     No one ever appreciates Denice and Gurly... Kids man...`\b\
}: You're Denice right? Then who's Gurly?\n\
Denice: Who's Gurly??\n\
        WHO'S GURLY?!?!\n\
        Why I oughta... kids man... THIS *shows axe* is Gurly!\n\
        I never leave her side...`\b\
}: You named your axe... not the weirdest thing I've seen so far...\n\
   Anyways, I'm }\n\
Denice: OOOHHHH she's a beaut aren't you Gurly??\n\
        ERRHEERM, *cough* I mean, wow great name!\n\
}: ...\n\
Denice: So er, what brings you here to my forest?\n\
}: I changed my mind about the weirdest thing I've seen...`\b\
}: I don't even know why I'm here...\n\
    Some old man told me he needed his help to stop an evil forc-\n\
Denice: Uh oh...\n\
        You're not an adventurer are you?\n\
}: Well, I didn't want to be!\n\
Denice: Oh brother... Nothing ever happens until one of you guys show up!\n\
        It's always so peaceful, perfect stilness.. And I want that back!\n\
        So I'll help you out, to get this over with as soon as possible!\n`",
//TEXT 25*************************************************************************25
"}: So what are we supposed to do?\n\
    Palfore told me to talk to the Oracle\n\
    But she was absolutely no help!\n\
Denice: Just between you and me, I wouldn't trust either of em\n\
        That wizard is getting on in his years, and the oracle gives\n\
        just gives useless riddles!\n\
}: Okay.. but I still don't know what to do.`\b\
\
Denice: This is about DeMadeaus right?\n\
}: Yeah! How did you know?\n\
Denice: How did I know? Heh, this kid!\n\
        HOW COULD I NOT KNOW?? Don't you see all the monsters?!\n\
}: Right!\n\
Denice: Well I suppose you know about Damadaeus right?`\b\
\
}: Yeah I've been told about him.\n\
Denice: Well he wasn't just any guy you know, he had help.\n\
}: What do you mean?\n\
Denice: There are four towers up about this land, I don't where they are...\n\
        I've tried looking but I can't find them.\n\
        Anyhow, in each of the towers there is a beacon.\n\
        That is where he got his power.`\b\
\
Denice: We need to turn all those beacons back on!\n\
        That's you goal I guess, if you're supposed to help us.\n\
}: Well then how do I get started?\n\
Denice: I don't know... you need to find someone you can ask for help.\n\
        Let's try going east`\b\
            "
    };
///Gets the length of the text for all texts up till the max
    for (i = 0; i < MAX_TEXTS; i++)
    {
        len[i] = strlen(text[i]);
    }
/// Identifies Name codes "Capitol_letter.... ':'"
    for (i = 0; i < len[printTextNum]; i++)
    {
        if (text[printTextNum][i] == ':') // Tells me where ':' is
        {
            j = i;
            /// Enviroment/Thought/Effects (***:)
            if (text[printTextNum][i - 1] == '*')
            {
                changeText[i - 3] = grey;
            }
            /// NAMES
            else
            {
                while (text[printTextNum][j - 1] >  'a' && text[printTextNum][j - 1] < 'z')
                    j--;
                if (text[printTextNum][j - 1] == '}') // Last letter of name (Character)
                    changeText[j - 1] = red;
                else if (text[printTextNum][j - 2] == 'P') // Palfore // I dont really know where the [j-csnt comes from], to find guess and check
                    changeText[j - 2] = aqua;
                else if (text[printTextNum][j - 3] == 'O')
                    changeText[j - 3] = white;
                else if (text[printTextNum][j - 1] == 'D')
                    changeText[j - 1] = darkGreen;
                else if (text[printTextNum][j - 3] == '?')
                    changeText[j - 3] = darkPurple;
            }
        }
    }
/// Prints Text
    for (i = 0; i < len[printTextNum]; i++)
    {
        if (changeText[i] != 0)
            SetConsoleTextAttribute(hConsole, changeText[i]);
        if (text[printTextNum][i] == '`')
            {printf("\n"); SetConsoleTextAttribute(hConsole, grey); pressEnter(1); i++;}
        else if (text[printTextNum][i] == '}')
            {printf("%s", playerName); i++;}
        else if (text[printTextNum][i] == '/')
            {Sleep(500); i++;}
        else if (text[printTextNum][i] == '~' && text[printTextNum][i + 1] == '~')
            {screenShake(2); i++; i++;}
        putchar(text[printTextNum][i]);
        Sleep(speed);
    }
}

void interactionEvent(int interactionNum)
{
    char interactKeyPress;
    int whichStand = 0;
/*Checks interactions with objects/characters*/
    if (interactionNum == 1000) // Interact with TREE
    {
        numInteract[0] = numInteract[0] + 1;
        if (numInteract[0] == 1)
        {
            gameProgress(interactionNum);
        }
    } else
    if (interactionNum == 3000) // Interact with LOG
    {
        gameProgress(interactionNum);
    } else
    if (interactionNum == 4000) // Interact with WIZARD
    {
        numInteract[4] = numInteract[4] + 1;
        if (numInteract[4] == 1)
        {
            gameProgress(interactionNum);
        }
        if (numInteract[4] == 2)
        {
            printText(18);
        }
    } else
    if (interactionNum == 7 && inRoom == 3) //Interact with door
    {
        createRoom(4);
        newRoomPos(west);
        clearScreen();
        showBoard(198,-1,-1);
        printf("Nothing here but an oven and a CHEST...\n");
        printf("\n\tYou may continue walking.\n");
        inRoom = 4;
    }else
    if ((characterBank[0] == maxGridSize) && (characterBank[1] == maxGridSize / 2 - 1) && (inRoom == 4)) // Leaves oracles house
    {
        maxGridSize = 18;
        createRoom(3);
        inRoom = 3;
        newRoomPos(west);
        characterBank[1] = characterBank[1] + 1;
        yPos = characterBank[1];
        clearScreen();
        if (numInteract[6] == 1)
        {
            createDoors(3);
        }
        showBoard(198,-1,-1);
        Sleep(1); // I dont understand how, but this has fixed a glitch
    }else
    if (interactionNum == 5 && inRoom == 4) //Interact with oven
    {
        printf("Just a standard oven.\n");
        printf("\n\tYou may continue walking.\n");
    }else
    if (interactionNum == 4 && inRoom == 4) //Interact with Chest
    {
        if (numInteract[6] < 1)
        {
            numInteract[6]++;
            printText(19);
        }else
        if (numInteract[6] == 1 && numInteract[8] != 1)
        {
            printf("Self: There's nothing left in the chest.\n");
            printf("\n\tYou may continue walking.\n");
        }
        if (numInteract[8] == 1)
        {
            printf("Self: Arg! There's nothing here!!\n");
            printf("*You slam the chest CLOSED in frustration.\n");
            printf("Self: Well that was a total wa...\n");
            printf("*You notice an inscription you must have missed before!\n");
            printf("Self: \"Go RIGHT before you are LEFT stranded.\"\n");
            printf("\t...\n");
            printf("\tI hate riddles...\n");
            numInteract[8] = 2;
            pressEnter(1);
            showBoard(199,-1,-1);
        }
    }else
    if (characterBank[0] == 8 && characterBank[1] == 5 && inRoom == 5 && numInteract[2] <= 1) // Crosses the bridge
    {
        numInteract[2]++;
        printf("Self: Oh... There actually weren't any monsters!\n");
        pressEnter(1);
     //   printf("???: BLARAGRARA!!!\n");
        screenShake(1);
        printf("Self: Uggg....\n");
        pressEnter(1);
        clearScreen();
        fight(2);
    }else
    if ((interactionNum == 10 || interactionNum == 6000) && inRoom == 6) //Interact with ledge / Oracle
    {
        if (numInteract[8] >= 1) // If you havent found the inscription on the chest, inter[8] == 2 means you did interact
        {
            characterBank[12] = 6; // Places oracle
            characterBank[13] = 10;
       //     Sleep(1);
        }
        if (numInteract[8] < 1) // If player interacted with ledge
        {
            scenery[6][10] = boarderType[-1]; //Removes ledge
            numInteract[8]++;

            showBoard(199,-1,-1);
            printText(20);
        Sleep(1); // I dont understand but this fixes a glitch??
        }else
        if (numInteract[8] == 1)
        {
            printf("The Oracle, looks spaced out and wont answer you.\n");
            printf("What did she say again? Something about protecting my chest and night time?...\n");
            printf("\n\tYou may continue walking.\n");
        Sleep(1);
        }else
        if (numInteract[8] == 2)
        {
            characterBank[12] = -1;
            characterBank[13] = -1;
            clearScreen();
            showBoard(199, -1, -1);
            printf("Hmm... The Oracle seems to have dispeared...");
        }
    Sleep(1);
    }else
    if (interactionNum == 12 && inRoom == 8) // Reads sign in double orb room
    {
        printf("*The sign reads:\n");
        printf("\tThe more you take the bigger your load...\n");
        printf("Self: What could that mean?\n");
    }else
    if (interactionNum == 11 && inRoom == 8) // Interacts with orbs
    {
        if (numInteract[16] == 0)
        {
           printf("Self: Woah! Its a floating orb!\n");
           printf("*You try touching it...\n"); Sleep(1000);
           printf("*You feel better able to defend! +3 Defence!\n");
           stats[4] = stats[4] + 3;
           printf("Self: That's awesome! I wonder if I can get even stronger!\n");
           numInteract[16] = 1;
        }else
        if (numInteract[16] == 1)
        {
            printf("*As you touch the orb, your body feels more sluggish. And you leave it.\n");
            printf("???: (Wispers) Greeeeeeed.....\n");
            stats[4] = stats[4] - 2;
            printf("Self: I'm not greedy!\n");
            printf("\tStupid orb!\n");
            numInteract[16] = 2;
        }else
        if (numInteract[16] == 2)
            printf("Self: Well I'm not touching those things again!\n");
    } else
    if (interactionNum == 7 && inRoom == 11) // Interact with any stand
    {
        printf("You see a stand infront of you.\n");
        if (!(numInteract[24] == 1 && numInteract[25] == 1 && numInteract[26] == -1))
        {
            switch (characterBank[1])
            {
            case 3:
            case 5:
            case 4:
                if (numInteract[24] == 1)
                    printf("There is a cabbage on this stand.\n");
                else printf("There is a cabbage on the ground.\n");
                whichStand = 1;
                break;
            case 7:
            case 9:
            case 8:
                if (numInteract[25] == -1)
                    printf("There is a CANDLE on the stand.\n");
                else printf("There is a CANDLE on the ground.\n");
                whichStand = 2;
                break;
            case 11:
            case 12:
            case 10:
                if (numInteract[26] == 1)
                    printf("There is some kind of MANUAL on the stand.\n");
                else printf("There is some kind of MANUAL on the ground.\n");
                whichStand = 3;
                break;
            }
            printf("\n\tWould you like to move it? (y or n)\n");
            while (interactKeyPress != 'y' && interactKeyPress != 'n')
               interactKeyPress = _getch();
            if (interactKeyPress == 'y')
            {
                 printf("You changed it's location!\n");
            }else printf("You leave it as is.\n");
            pressEnter(1);
            switch (whichStand)
            {
            case 1:
                if (interactKeyPress == 'y') // You take the first thing
                    numInteract[24] = - numInteract[24];
                break;
            case 2:
                if (interactKeyPress == 'y') // You take the second thing
                    numInteract[25] = - numInteract[25];
                break;
            case 3:
                if (interactKeyPress == 'y') // You take the third thing
                    numInteract[26] = - numInteract[26];
            }
            if (numInteract[24] == 1 && numInteract[25] == 1 && numInteract[26] == -1)
            {
                printf("*You hear a click...\n");
                printf("*And a flash!\n");
                printf("*A stone has appeared, it has three arrows which point upwards...\n");
                printf("*And some writting, it says follow the path and the entrance will present itself.\n");
                printf("Self: Plain english please next time please!\n");
                printf("Denice: Don't worry %s, I solved this riddle I can solve the next!\n", playerName);
                pressEnter(1);
                printf("Self: You didn't do anything!\n");
                printf("Denice: OAH! I NEVER! Well then, you can solve this one on your own!\n");
                printf("Self: Fine!\n");
                printf("Denice: Fine!\n");
                printf("\t... I hate it when we fight!\n");
                pressEnter(1);
            }
        }

        {
            printf("*There is a stone, which has three arrows which point upwards.\n");
            printf("*And some writting, it says follow the path and the entrance will present itself.\n");
        }
    }
    if (characterBank[0] > 15 && characterBank[1] < 9 && inRoom == 10) // Oracle appears and gives riddle
    {
        characterBank[12] = 5;
        characterBank[13] = 6;
        if (numInteract[22] == 1)
        {
            numInteract[22] = 2;
            showBoard(199, -1, -1);
            printf("Oracle: One HEADS north was no other REMAINS.\n");
            printf("\tKnow that the LIGHT must FALL for you move to move forward.\n");
            printf("\tOr he who GUIDES will RISE.\n");
            printf("Self: Great...\n");
            printf("Denice: Hah! What a crazy lady... Let's keep going!\n");
            printf("\tI'll worry about the riddle..\n");
            pressEnter(0);
            clearScreenText();
        }
    }else
    if (interactionNum == 6000 && inRoom == 10) // Get riddle again from Oracle
    {
        printf("Oracle: One HEADS north as no other REMAINS.\n");
        printf("\tKnow that the LIGHT must FALL for you move to move forward.\n");
        printf("\tOr he who GUIDES will RISE.\n");
        printf("Self: Hmmmm....\n");
        pressEnter(0);
        clearScreenText();
    }

}

void displayMap()
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int k;
    int l;
    int mapSizeX = 30;
    int mapSizeY = 30;

    system("cls");SetConsoleTextAttribute(hConsole, 12);
    printf("MAP:\n");SetConsoleTextAttribute(hConsole, 11);
    for (k = 0; k < mapSizeX; k++)
        printf("-");
    printf("--");
    printf("\n|");
    SetConsoleTextAttribute(hConsole, 11);
    for (k = 0; k < mapSizeY; k++)
    {
        if (l == mapSizeX) printf("|");
        for (l = 0; l < mapSizeX; l++)
        {
            if (enteredRooms[l][k] == 2)
             {SetConsoleTextAttribute(hConsole, 14); printf("%c", '*'); SetConsoleTextAttribute(hConsole, 11);}
            else
            if (enteredRooms[l][k] == 1)
             {SetConsoleTextAttribute(hConsole, 13); printf("%c", '#'); SetConsoleTextAttribute(hConsole, 11);}
            else
             printf("%c", ' ');

            if (l == mapSizeX - 1)
            {
                printf("|"); SetConsoleTextAttribute(hConsole, 3);
                if      (k == 0) printf("  LEGEND:");
                else if (k == 1) printf("  ----------------");
                else if (k == 2) printf("  *: Your location");
                else if (k == 3) printf("  #: Uncovered Map"); SetConsoleTextAttribute(hConsole, 11);
            }
        }
        printf("\n");
    }
    for (k = 0; k < mapSizeX; k++)
        printf("-");
    printf("--");
}

void configureMap(enum direction addRoomToThe) // Change the 30's if you change mapSize
{
    int k;
    int l;

    for (k = 0; k < 30; k++) // you've moved so it rests the *(1) to a #(2)
    {
        for (l = 0; l < 30; l++)
        {
            if (enteredRooms[l][k] == 2)
            enteredRooms[l][k] = 1;
        }
    }
    switch(addRoomToThe) // Global position is updated
    {
    case east:
        xMap++;
        break;
    case west:
        xMap--;
        break;
    case north:
        yMap--; // -- is not a mistake
        break;
    case south:
        yMap++;
        break;
    default:
        printf("MAP ERROR!\n");
    }
    enteredRooms[xMap][yMap] = 2; // Denotes that new position as * (2)
}

void clearScreenText()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = maxGridSize + 2;
    SetConsoleCursorPosition(hOut, Position);
}

void clearScreen()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void settingsMenu()
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
            printf("Off\n");
            break;
        case 1:
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
            settingsMenu();
            break;
        case 'b':
            break;
    }
    return;
}

void newRoomPos(enum direction cameFrom)
{
    if (!(inRoom == 4 || (inRoom == 3 && cameFrom == west))) // Stops weirdness when leaving oracles house
        configureMap(cameFrom);

    switch(cameFrom)
    {
    case west:
        {
            xPos = maxGridSize - 2;
            yPos = maxGridSize / 2 - 1;
            characterBank[0] = xPos;
            characterBank[1] = yPos;
            break;
        }
    case east:
        {
            xPos = 1;
            yPos = maxGridSize / 2 - 1;
            characterBank[0] = xPos;
            characterBank[1] = yPos;
            break;
        }
    case north:
        {
            xPos = maxGridSize / 2 - 1;
            yPos = maxGridSize - 2;
            characterBank[0] = xPos;
            characterBank[1] = yPos;
            break;
        }
    case south:
        {
            xPos = maxGridSize / 2 - 1;
            yPos = 1;
            characterBank[0] = xPos;
            characterBank[1] = yPos;
            break;
        }
     }
}

void RoomChanging() // Character's will only be updated here if the room changes
{
    enum direction atDoor;

// assignes the direction accoringly to atDoor, and moves denice if he is present
    {
         if ((characterBank[0] == -1) && (characterBank[1] == maxGridSize / 2 - 1))
         {
             atDoor = west;
            if (numInteract[18] >= 3)
            {
                characterBank[18] = maxGridSize - 1;
                characterBank[19] = maxGridSize / 2 - 1;
            }
         }
    else if ((characterBank[0] == maxGridSize) && (characterBank[1] == maxGridSize / 2 - 1))
         {atDoor = east;
             if (numInteract[18] >= 3)
             {
                characterBank[18] = -1 + 1;
                characterBank[19] = maxGridSize / 2 - 1;
             }
         }
    else if ((characterBank[0] == maxGridSize / 2 - 1) && (characterBank[1] == -1))
         {atDoor = north;
            if (numInteract[18] >= 3)
            {
                characterBank[18] = maxGridSize / 2 - 1;
                characterBank[19] = maxGridSize - 1;
            }
         }
    else if ((characterBank[0] == maxGridSize / 2 - 1) && (characterBank[1] == maxGridSize))
        {atDoor = south;
            if (numInteract[18] >= 3)
            {
                characterBank[18] = maxGridSize / 2 - 1;
                characterBank[19] = -1 + 1;
            }
        }

    }
 /*Checks if the player is changing rooms*/
    if ((atDoor == east) && (inRoom == 2)) //Enter the oracles house room
    {
        createRoom(3);
        newRoomPos(east);
        clearScreen();
        showBoard(198,-1,-1);
        inRoom = 3;
    }else
    if ((atDoor == west) && (inRoom == 3)) // Goes back to start
    {
//        createRoom(2);
//        showBoard(10, 5, 5);
//        newRoomPos(west);
//        clearScreen();
//        showBoard(198,-1,-1);
//        inRoom = 2;
    }else
    if ((atDoor == north) && (inRoom == 3)) // Enters the bridge to the north
    {
        createRoom(5);
        newRoomPos(north);
        clearScreen();
        showBoard(198,-1,-1);
        inRoom = 5;
        if (numInteract[10] < 1)
        {
            printf("Oh great, a bridge... Well there definitly won't be monsters there!\n");
            pressEnter(1);
            numInteract[10]++;
        }
    }else
    if ((atDoor == south) && (inRoom == 5)) // Comes back from the bridge
    {
        createRoom(3);
        newRoomPos(south);
        clearScreen();
        showBoard(198,-1,-1);
        inRoom = 3;
    }else
    if ((atDoor == north) && (inRoom == 5)) // Goes up to mountains with Oracle
    {
        createRoom(6);
        newRoomPos(north);
        if (numInteract[8] == 2 || numInteract[8] == 0) //You checked out the chest // If the oracle shows up randomly uncomment this
        {
            characterBank[12] = -1; // Removes oracle
            characterBank[13] = -1;
            scenery[6][10] = boarderType[9];
        }
        else
        {
            characterBank[12] = 6;
            characterBank[13] = 10;
        }
        clearScreen();
        showBoard(198,-1,-1);
        inRoom = 6;
        numInteract[12]++;
        if (numInteract[12] <= 1)
        {
            printf("Such beatiful mountains!\n");
            printf("Hmmm... It looks like theres a ledge there, maybe I should check that out!\n");
            pressEnter(1);
        }
    }else
    if ((atDoor == south) && (inRoom == 6)) // Comes back from mountains
    {
        createRoom(5);
        newRoomPos(south);
        characterBank[12] = -1;
        characterBank[13] = -1;
        clearScreen();
        showBoard(198,-1,-1);
        inRoom = 5;
    }else
    if ((atDoor == east) && (inRoom == 6)) // Goes east of Oracle mountain
    {
        createRoom(7);
        newRoomPos(east);
        clearScreen();
        showBoard(198,-1,-1);
        inRoom = 7;
        if (numInteract[14] < 1)
        {
            numInteract[14]++;
            printf("WHAT!? There are so many monsters! But the chest said go east!!!\n");
            pressEnter(1);
        }
    }else
    if ((atDoor == west) && (inRoom == 7)) // Comes back from east of Oracle mountain
    {
        createRoom(6);
        newRoomPos(west);
        clearScreen();
        showBoard(198,-1,-1);
        inRoom = 6;
    }else
    if ((atDoor == west) && (inRoom == 6)) // Goes west of Oracle mountain
    {
        createRoom(8);
        newRoomPos(west);
        clearScreen();
        showBoard(198,-1,-1);;
        if (numInteract[20] < 1)
        {
            numInteract[20]++;
            printf("Self: Hmmmm.... What are those? They seem like they're glowing!\n");
            printf("\tOh and theres a sign!\n");
            pressEnter(1);
        }
    }else
    if ((atDoor == east) && (inRoom == 8)) // Comes back from west of Oracle mountain
    {
        createRoom(6);
        newRoomPos(east);
        clearScreen();
        showBoard(198,-1,-1);
        inRoom = 6;
    }else
    if ((atDoor == north) && (inRoom == 6)) // Goes North of Oracle Mountain (to Denice Forest)
    {
        newRoomPos(north);
        createRoom(9);
        clearScreen();
        showBoard(198,-1,-1);
        if (numInteract[18] < 1)
        {
            gameProgress(5);
        }
    }else
    if ((atDoor == south) && (inRoom == 9)) // Comes back from North of Oracle mountain (back from Denice Forest)
    {
        newRoomPos(south);
        createRoom(6);
        clearScreen();
        showBoard(198,-1,-1);
    }else
    if ((atDoor == north) && (inRoom == 9)) // Goes North of Denice Forest
    {
        newRoomPos(north);
        createRoom(10);
        clearScreen();
        showBoard(198,-1,-1);
        if (numInteract[22] < 1)
        {
            numInteract[22] = 1;
            printText(25);
        }
    }else
    if ((atDoor == south) && (inRoom == 10)) // Comes back from Denice Forest
    {
        newRoomPos(south);
        createRoom(9);
        clearScreen();
        showBoard(198,-1,-1);
    }else
    if ((atDoor == east && inRoom == 10))
    {
        newRoomPos(east);
        createRoom(11);
        clearScreen();
        showBoard(199, -1, -1);
    }else
    if (atDoor == west && inRoom == 11)
    {
        newRoomPos(west);
        createRoom(10);
        clearScreen();
        showBoard(199, -1, -1);
    }else
    if (atDoor == north && inRoom == 11)
    {
        newRoomPos(north);
        createRoom(12);
        clearScreen();
        showBoard(199, -1, -1);
    }else
    if (atDoor == south && inRoom == 12) // leaves
    {
        newRoomPos(south);
        createRoom(11);
        clearScreen();
        showBoard(199, -1, -1);
    }else
    if (atDoor == north && inRoom == 12)
    {

        if (numInteract[28] != 3)
        {
            numInteract[28]++;
            yMap++;
            newRoomPos(north);
            createRoom(12);
            clearScreen();
            showBoard(199, -1, -1);
            printf("Self: Huh?\n");
            printf("\n\t You may continue walking.\n");
        }
        else
        {
            newRoomPos(north);
            createRoom(13);
            clearScreen();
            showBoard(199, -1, -1);
        }

    }else
    if (atDoor == east && inRoom == 12)
    {
        xMap--;
        newRoomPos(east);
        createRoom(12);
        clearScreen();
        showBoard(199, -1, -1);
        printf("Self: Huh?\n");
        printf("\n\t You may continue walking.\n");
    }else
    if (atDoor == west && inRoom == 12)
    {
        xMap++;
        newRoomPos(west);
        createRoom(12);
        clearScreen();
        showBoard(199, -1, -1);
        printf("Self: Huh?\n");
        printf("\n\t You may continue walking.\n");
    }else
    if (atDoor == south && inRoom == 13)
    {
        newRoomPos(south);
        createRoom(12);
        clearScreen();
        showBoard(199, -1, -1);
    }
    if (atDoor == east && inRoom == 13)
    {
        newRoomPos(east);
        createRoom(666);
        createDoors(1);
        clearScreen();
        showBoard(199, -1, -1);
    }


    if ((inRoom == 9 || inRoom == 11) && characterBank[12] != -1) // Removes Oracle
    {
        characterBank[12] = -1;
        characterBank[13] = -1;
        clearScreen();
        showBoard(199, -1, -1);
    }
}

void showStats(int fighting)
{
    int j;

    printf("*********************IT'S TIME TO FIGHT!************************\n");
    printf("Your Health  : %d/%d\t   Your opponent's Health  : %d/%d\n", stats[1] ,stats[0], stats[fighting + 1], stats[fighting]);
    for (j = 1; j < 4; j++)
    {
        printf("Your %-8s: %d\t   %Your opponent's %-8s: %d\n", statNames[j + 1], stats[j + 1], statNames [j + 1], stats[j + fighting + 1]);
    }
}

int load()
{
    int k = 0;
    int temp;
    system("cls");

    FILE* inFile = NULL;

    inFile = fopen("newSave.txt", "r");

    if (inFile == NULL)
    {
        printf("ERROR 003: NO SAVE FILE \"save.txt\" FOUND TO OPEN\n");
        pressEnter(0);
        return -1;
    }
/*numInteract*/
    for (k = 0; k < 100; k++)
    {
        fscanf(inFile, "%d ", &numInteract[k]);
    }
/*characterBank*/
    for (k = 0; k < 200; k++)
    {
        fscanf(inFile, "%d ", &characterBank[k]);
        if (k == 0)
            xPos = characterBank[k];
        else if (k == 1)
            yPos = characterBank[k];
    }
/*inRoom*/
    fscanf(inFile, "%d", &inRoom);
    createRoom(inRoom);
/*numFights*/
    fscanf(inFile, "%d", &numFights);


    printf("Opening file newSave.txt.\n");
    printf("Your save file has been read correctly.\n");
    printf("Please note that the following variables are not yet saved/loaded:\n");
    printf("\tStats\n\tturnDiff\n\t");
    printf("\n\ninRoom: %d, numfights: %d", inRoom, numFights);
    pressEnter(0);
    fclose(inFile);
    system("cls");
    return;
}

int save()
{
    int k;
    clearScreen();

    FILE* outFile = NULL;

    outFile = fopen("newSave.txt", "w");

    if (outFile == NULL)
    {
        printf("ERROR 002: NO FILE \"save.txt\" FOUND TO SAVE TO\n");
        return -1;
    }
/*numInteract*/
    for(k = 0; k < 100; k++)
    {
        fprintf(outFile, "%d ", numInteract[k]);
    }
    fprintf(outFile, "\n");
/*characterBank*/
    for(k = 0; k < 200; k++)
    {
        fprintf(outFile, "%d ", characterBank[k]);
    }
/*inRoom*/
    fprintf(outFile, "\n");
    fprintf(outFile, "%d", inRoom);
/*numFights*/
    fprintf(outFile, "\n");
    fprintf(outFile, "%d", numFights);

    fclose(outFile);
    printf("Your game state has been saved succesfully.\n");
    return;
}

void displayInfo(char displayType)
{
    clearScreen();
    switch(displayType)
    {
    case 'm':
        system("cls");
        displayMap();
        pressEnter(0);
        break;
    case 'g':
        guide(0);
        break;
    }
    clearScreen();
    showBoard(199, -1, -1);
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
            printf("W : Wizard      \n");
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
        pressEnter(0);
        system("cls");
        guide(0);
    }
}

void startOptions()
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int l;
    char option = 0;
    initializeStats();
    createRoom(1);
    do
    {
        option = 0;
        while (option != 'p' && option != 'o' && option != 'g' && option != 'c' && option != 'l')
            option = _getch();
        switch(option)
        {
        case 'p':
            system("cls");
            printText(-1);
            return;
            break;
        case 'o':
            settingsMenu();
            system("cls");
            printText(-2);
            break;
        case 'g':
            guide(1);
            system("cls");
            printText(-2);
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
            printText(-2);
            break;
        case 'l':
            load();
            return;
            break;

        }
        if (option == 'l' || option == 'p')
            break;
    } while (option == 'g');
}

void enemyEvents() // May not be used much
{
    switch(numFights)
    {
        case 5:
            createDoors(1);
            numFights++;
            showBoard(199, -1, -1);
            printf("So I should have gone east?\n");
            pressEnter(0);
            break;
    }
    return;
}

void increaseStat() // Player gets to choose a stat to boost
{
    char boostedStat;

    printf("\nYou feel slightly stronger... and more able to:\n");
    printf("Take more damage (h)\n");
    printf("Hit accurately   (a)\n");
    printf("Hit harder       (s)\n");
    printf("Defend often     (d)\n");

    while (boostedStat != 'a' && boostedStat != 's' && boostedStat != 'd' && boostedStat != 'h')
    {
        boostedStat = _getch();
    }
    switch(boostedStat)
    {
        case 'h':
            stats[0 + 0 * 5] = stats[0 + 0 * 5] + 5;
            break;
        case 'a':
            stats[2 + 0 * 5]++;
            break;
        case 's':
            stats[3 + 0 * 5]++;
            break;
        case 'd':
            stats[4 + 0 * 5]++;
            break;
    }
    printf("\nStats IMPROVED!\n");


}

void pressEnter(int printScreen) // Informs the user they may contiue if they press enter
{
    char keyPressed;

    printf("\n\tPress Enter or Space to continue.                                           \n");
    while (keyPressed != '\n' && keyPressed != ' ')
    {
        keyPressed = _getch();
        if (keyPressed == 13)
        break;
    }
    if (printScreen == 1)
    showBoard(199,-1,-1);
}

void initializeStats() // Sets all stats to their default
{
    int j;

    // All no stats characters : 1,3,4,5
    for (j = 0; j < 30; j++)
    {
        if (j != 0 || j != 2)
            {
                stats[0 + j * 5] = -1;
                stats[1 + j * 5] = -1;
                stats[2 + j * 5] = -1;
                stats[3 + j * 5] = -1;
                stats[4 + j * 5] = -1;
            }
    }
    for (j = 0; j <= 200; j++)
    {
        characterBank[j] = -1;
    }
    // Player
    stats[0 + 0 * 5] = 40; // Max Health
    stats[1 + 0 * 5] = 40; // Current Health
    stats[2 + 0 * 5] = 7;  // Attack
    stats[3 + 0 * 5] = 7;  // Strength
    stats[4 + 0 * 5] = 7;  // Defence
    // Monster 1
    stats[0 + 2 * 5] = 30;
    stats[1 + 2 * 5] = 30;
    stats[2 + 2 * 5] = 2;
    stats[3 + 2 * 5] = 4;
    stats[4 + 2 * 5] = 6;
    // Monster 2
    stats[0 + 7 * 5] = 30;
    stats[1 + 7 * 5] = 30;
    stats[2 + 7 * 5] = 6;
    stats[3 + 7 * 5] = 4;
    stats[4 + 7 * 5] = 2;
    // Monster 3
    stats[0 + 8 * 5] = 30;
    stats[1 + 8 * 5] = 30;
    stats[2 + 8 * 5] = 4;
    stats[3 + 8 * 5] = 6;
    stats[4 + 8 * 5] = 2;

}

void createDoors(int doors) // Creates exits for the player to leave a room
{
    switch(doors)
    {
        case 1: // West
            {
               scenery[0][maxGridSize / 2 - 1] = '.';
               break;
            }
        case 2: // East
            {
               scenery[maxGridSize - 1][maxGridSize / 2 - 1] = '.';
               break;
            }
        case 3: // North
            {
               scenery[maxGridSize / 2 - 1][0] = '.';
               break;
            }
        case 4: // South
            {
               scenery[maxGridSize / 2 - 1][maxGridSize - 1] = '.';
               break;
            }
    }
}

void createBoarders(int lines, char type) // Creates a parameter of any symbol
{
    int z;
    switch(lines)
    {
        case 1: // vertical line at x = 0;
            {
                for (z = 0; z <= maxGridSize; z++)
                    scenery[0][z] = boarderType[type];
                break;
            }
        case 2: // vertical line at x = maxGrideSize
            {
                for (z = 0; z <= maxGridSize; z++)
                    scenery[maxGridSize - 1][z] = boarderType[type];
                break;
            }
        case 3: // horozonal line at y = 0
            {
                for (z = 0; z <= maxGridSize; z++)
                    scenery[z][0] = boarderType[type];
                break;
            }
        case 4: // horozonal line at y = 0
            {
                for (z = 0; z <= maxGridSize; z++)
                    scenery[z][maxGridSize - 1] = boarderType[type];
                break;
            }
        case 5:
            {
                for (z = 0; z <= maxGridSize; z++)
                {
                    scenery[0][z] = boarderType[type];
                    scenery[maxGridSize - 1][z] = boarderType[type];
                    scenery[z][0] = boarderType[type];
                    scenery[z][maxGridSize - 1] = boarderType[type];
                 }
                 break;
            }
    }
    return;
}

void createRoom(int rooms) // Maybe have this function reset all char pos, and set only the ones you want. Defines the environment on screen
{
    int z;
    int l;
    int m;
    int k;
    int randomNum;
    int generatedDoors[4] = {0};
    int doorCount = 1;
    int xNode;
    int yNode;
    int checkPlusZ;
    int checkMinusZ;

    for (m = 0; m <= 40; m++)
    {
         for (l = 0; l <= 40; l++)
         scenery[l][m] = -1;
    }
    switch(rooms)
    {
        {
        case 1: // Starting room
            {
                createBoarders(5,0);
                inRoom = 1;
                break;
            }
        case 2: // Starting room with east exit
            {
                createBoarders(5,0);
                createDoors(2);
                inRoom = 2;
                characterBank[10] = 5;
                characterBank[11] = 5;
                break;
            }
        case 3: // Room with house
            {
                characterBank[10] = -1; //remove tree
                characterBank[11] = -1;
                characterBank[8]  = -1; //remove wizard
                characterBank[9]  = -1;
                characterBank[6]  = -1; //remove log
                characterBank[7]  = -1;

                createBoarders(5,0);
                createDoors(1);
                if (numInteract[6] >= 1)
                createDoors(3);
                inRoom = 3;
                characterBank[2] = -1;
                characterBank[3] = -1;
                characterBank[8] = -1;
                characterBank[9] = -1;
                /*Left wall*/
                scenery[14][6] = boarderType[1];
                scenery[14][7] = boarderType[1];
                scenery[14][8] = boarderType[1];
                scenery[14][9] = boarderType[6];
                scenery[14][10] = boarderType[1];
                scenery[14][11] = boarderType[1];
                scenery[14][12] = boarderType[1];
                /*Right Wall*/
                scenery[11][6] = boarderType[1];
                scenery[11][7] = boarderType[1];
                scenery[11][8] = boarderType[1];
                scenery[11][9] = boarderType[1];
                scenery[11][10] = boarderType[1];
                scenery[11][11] = boarderType[1];
                scenery[11][12] = boarderType[1];
                /*top & bottom Wall*/
                scenery[12][6] = boarderType[1];
                scenery[13][6] = boarderType[1];
                scenery[14][6] = boarderType[1];
                scenery[12][12] = boarderType[1];
                scenery[13][12] = boarderType[1];
                scenery[14][12] = boarderType[1];
                /*make it look housish*/
                scenery[12][7] = boarderType[2];
                scenery[13][7] = boarderType[2];
                scenery[12][11] = boarderType[2];
                scenery[13][11] = boarderType[2];
                break;
            }
        case 4: // Oracle's house
            {
                maxGridSize = 7;
                inRoom = 4;
                createBoarders(5,1);
                createDoors(2);
                scenery[1][3] = boarderType[4];
                scenery[1][4] = boarderType[4];
                scenery[1][5] = boarderType[5];
                scenery[2][5] = boarderType[3];
                break;
            }
        case 5: // Room with bridge
            {
                inRoom = 5;
                createBoarders(5,0);
                createDoors(4);
                createDoors(3);
                for(k = 0; k <= 17; k++)
                {
                    scenery[k][8] = boarderType[7];
                    scenery[k][7] = boarderType[7];
                }
                scenery[7][8] = boarderType[6]; //y,x
                scenery[7][7] = boarderType[6];
                scenery[7][6] = boarderType[6];
                scenery[9][8] = boarderType[6];
                scenery[9][7] = boarderType[6];
                scenery[9][6] = boarderType[6];
                scenery[8][8] = boarderType[2];
                scenery[8][7] = boarderType[2];
                scenery[8][6] = boarderType[2];
                break;
            }
        case 6: // Mountain Room with oracle
            {
                inRoom = 6;
                createBoarders(5,0);
                createDoors(4);
                if (numInteract[14] >= 1 && numInteract[16] >= 1) // Went to the East (and came back) & touched 1+ orbs
                {
                   createDoors(3);
                }
                scenery[7][7] = boarderType[8]; //y,x
                scenery[6][5] = boarderType[8];
                scenery[5][7] = boarderType[8];
                scenery[8][8] = boarderType[8];
                scenery[6][5] = boarderType[8];
                scenery[5][9] = boarderType[8];
                scenery[8][6] = boarderType[8];
               // if (numInteract[8] < 1 || numInteract[8] >= 2)
                if (numInteract[8] != 1)
                    scenery[6][10] = boarderType[9];
                scenery[5][14] = boarderType[8];
                scenery[6][11] = boarderType[8];
                scenery[8][11] = boarderType[8];
                scenery[10][9] = boarderType[8];
                scenery[7][15] = boarderType[8];
                scenery[12][13] = boarderType[8];
                scenery[10][12] = boarderType[8];
                if (numInteract[8] >= 2)
                {
                    createDoors(1);
                    createDoors(2);
                    scenery[6][10] = boarderType[9];
                }
                break;
            }
        case 7: // East of mountains
            {
                inRoom = 7;
                createBoarders(5,0);
                if (numFights >= 5)
                    createDoors(1);
                characterBank[12] = -1; // Remove oracle
                characterBank[12] = -1;
                if (numInteract[14] != 1)
                {
                    characterBank[4] = 16; // Add 3 Monsters
                    characterBank[5] = 12;
                    characterBank[14] = 14;
                    characterBank[15] = 8;
                    characterBank[16] = 3;
                    characterBank[17] = 2;
                }
                break;
            }
        case 8: // West of the mountains
            {
                inRoom = 8;
                createBoarders(5,0);
                createDoors(2);
                scenery[5][5] = boarderType[10];
                scenery[4][8] = boarderType[11];
                scenery[5][11] = boarderType[10];
                break;
            }
        case 9: // North of the mountans
            {
                inRoom = 9;
                createBoarders(5,0);
                for (l = 0; l <= 15; l++)
                {
                    for (k = 0; k <= 16; k++)
                        scenery[k][l] = boarderType[0];
                }
                scenery[7][12]  = boarderType[2];
                scenery[8][11]  = boarderType[2];
                scenery[3][5]   = boarderType[2];
                scenery[7][8]   = boarderType[2];
                scenery[8][5]   = boarderType[2];
                scenery[12][5]  = boarderType[2];
                scenery[15][6]  = boarderType[2];
                scenery[14][3]  = boarderType[2];
                scenery[9][15]  = boarderType[2];
                scenery[10][13] = boarderType[2];
                scenery[4][18]  = boarderType[2];
                scenery[6][15]  = boarderType[2];
                createDoors(4);
                if (numInteract[18] > 1) // clears the trees before denice appears if you reenter the room
                {
                    scenery[maxGridSize / 2 - 1][maxGridSize - 4] = boarderType[2];
                    scenery[maxGridSize / 2 - 1][maxGridSize - 3] = boarderType[2];
                }
                if (numInteract[18] == 3) // Moves
                {
                    for (k = 0; k < 18; k++)
                    {
                        scenery[maxGridSize / 2 - 1][maxGridSize - 5 - k] = boarderType[2];
                    }
                    showBoard(199, -1, -1);
                }else
                if (numInteract[18] == 1)
                {
                    scenery[maxGridSize / 2 - 1][maxGridSize - 3] = boarderType[2];
                    showBoard(199, -1, -1);
                    printf("Self: Huh?\n");
                    pressEnter(0);
                    clearScreen();
                    scenery[maxGridSize / 2 - 1][maxGridSize - 4] = boarderType[2];
                    showBoard(199, -1, -1);
                    printf("Self: Who the heck keeps cutting down all these trees!?!\n");
                    pressEnter(0);
                    scenery[maxGridSize / 2 - 1][maxGridSize - 5] = boarderType[2];
                    characterBank[18] = maxGridSize / 2 - 1;
                    characterBank[19] = maxGridSize - 5;
                    showBoard(199, -1, -1);
                    printText(24);
                }else
                if (numInteract[18] == 2)
                {
                    scenery[maxGridSize / 2 - 1][maxGridSize - 3] = boarderType[2];
                    scenery[maxGridSize / 2 - 1][maxGridSize - 4] = boarderType[2];
                    scenery[maxGridSize / 2 - 1][maxGridSize - 5] = boarderType[2];
                    k = 0;
                    while(characterBank[19] > 0)
                    {
                        characterBank[19] = maxGridSize - 5 - k;
                        scenery[maxGridSize / 2 - 1][maxGridSize - 5 - k] = boarderType[2];
                        showBoard(199, -1, -1);
                        printf("Denice: HEEEREARAAAAAAA!!!!!!!!!!!\n");
                        Sleep(60);
                        k++;
                    }
                    showBoard(199, -1, -1);
                    printf("Self: WOW!! That was incredible!!\n");
                    printf("Denice: All in a days work! You can thank Gurley for that!\n");
                    pressEnter(1);
                    k = 0;
                    while(characterBank[1] > 1 && numInteract[18] < 3) // Moves you across field
                    {
                        characterBank[1] = characterBank[1] - 1;
                        showBoard(199, -1, -1);
                        Sleep(25);
                        k++;
                    }
                    numInteract[18] = 3;
                    xPos = characterBank[0];
                    yPos = characterBank[1];
                    printf("Denice: Let's try going north! I'll follow you.\n");
                }
                break;
            }
        case 10:
            {
                if (numInteract[22] == 2) // (or 1)
                {
                    characterBank[12] = 5;
                    characterBank[13] = 6;
                }
                inRoom = 10;
                createBoarders(5,0);
                createDoors(4);
                createDoors(2);
                scenery[5][5]   = boarderType[0];
                scenery[6][8]   = boarderType[0];
                scenery[14][2]  = boarderType[0];
                scenery[6][10]  = boarderType[0];
                scenery[13][13] = boarderType[0];
                scenery[12][14] = boarderType[0];
                scenery[17][13] = boarderType[0];
                scenery[3][12]  = boarderType[0];
                scenery[6][4]   = boarderType[0];
                scenery[9][17]  = boarderType[0];
                scenery[10][9]  = boarderType[0];
                scenery[5][8]   = boarderType[0];
                break;
            }
        case 11: // Weight challenge
            {
                inRoom = 11;
                createBoarders(5,0);
                createDoors(1);
                createDoors(3);
                scenery[10][4]  = boarderType[6];
                scenery[10][8]  = boarderType[6];
                scenery[10][12] = boarderType[6];
                break;
            }
        case 12:
            {
                inRoom = 12;
                createBoarders(5,0);
                createDoors(1);
                createDoors(2);
                createDoors(3);
                createDoors(4);

                scenery[9][12]   = boarderType[0];
                scenery[4][14]   = boarderType[0];
                scenery[12][4]  = boarderType[0];
                break;
            }
        case 13:
            {
                inRoom = 13;
                createBoarders(5,0);
                createDoors(2);
                createDoors(4);

                scenery[5][14]   = boarderType[0];
                scenery[2][5]   = boarderType[0];
                scenery[15][3]  = boarderType[0];
                break;
            }
        }
        case 666: // Going to be hell to write...
            {
                createBoarders(5,0);
/// GENERATE ATLEAST ONE OTHER DOOR
numFights = 10;

{Sleep (2000);createRoom(1);
numFights++;

                while (doorCount <= 1)
                {
                    printf("!");
                    doorCount = 0;
                    randomNum = (rand() % 4) + 1; // Random num between 1 and 4
                    for (l = 0; l < randomNum; l++)
                    {
                        if (rand() % 2 == 0)  // if even (50%)
                        {
                            if (rand() % 2 == 0)
                                {createDoors(1); doorCount++;}
                            else
                                {createDoors(2);doorCount++;}
                        }
                        else
                        {
                            if (rand() % 2 == 0)
                                {createDoors(3);doorCount++;}
                            else
                                {createDoors(4);doorCount++;}
                        }
                    }
                    generatedDoors[0] = 0;generatedDoors[1] = 0;generatedDoors[2] = 0;generatedDoors[3] = 0;
                         if (scenery[0][maxGridSize / 2 - 1] == '.') {generatedDoors[0] = 1;}
                    else if (scenery[maxGridSize - 1][maxGridSize / 2 - 1] == '.') {generatedDoors[1] = 1;}
                    else if (scenery[maxGridSize / 2 - 1][0] == '.') {generatedDoors[2] = 1;}
                    else if (scenery[maxGridSize / 2 - 1][maxGridSize - 1] == '.') {generatedDoors[3] = 1;}
                }
                scenery[0][maxGridSize / 2 - 1] = '.';
                xNode = 0;                              // ...it is the first node.
                yNode = maxGridSize / 2 - 1;

                for (l = 1; l < maxGridSize - 1; l++)
                {
                    for (m = 1; m < maxGridSize - 1; m++)
                    {
                        regenRoom(l, m);
                        if (m == maxGridSize - 1 - 1) // You are at the bottom of the coloum
                        {
       //                    showBoard(199, -1,-1);// Remove this!
                     ///       printf("C(%d,%d) N(%d, %d)   ", l, m, xNode, yNode); Sleep (50);
                            if (scenery[xNode + 1][yNode] == ' ') /// If theres a opening to the right of the node
                            {
                                xNode = xNode + 1; //  It is the new node
                            }
                            else if (l > 1)
                            {
                                checkPlusZ = 1;
                                checkMinusZ = 1;
                                for (z = 1; z < 5 && (checkMinusZ == 1 || checkPlusZ == 1); z++) // Cycle through different lengths up and down
                                {
                                   // scenery[xNode][yNode + z] = '^';
                                   // scenery[xNode][yNode - z] = '^';
                                    if (scenery[xNode][yNode + z] != ' ')
                                        checkPlusZ = 0; // This means that there was a block below you, stop looking there
                                    if (scenery[xNode][yNode - z] != ' ')
                                        checkMinusZ = 0; // This means that there was a block below you, stop looking there
                                    showBoard(199, -1,-1);// Remove this!
                    ///                printf("z: %d, c-z: %d, c+z: %d \n", z, checkMinusZ, checkPlusZ);
                    ///                printf("z Below me is (%c), z above me is (%c)\n",  scenery[xNode][yNode + z], scenery[xNode][yNode - z]);
                    ///                printf(" 1)I detect +(%c), or -(%c), infront of the mountain\n", scenery[xNode + 1][yNode + z], scenery[xNode + 1][yNode - z]);
                                 //   _getch();

                                        if (scenery[xNode][yNode + z] == ' ' && checkPlusZ == 1 && yNode + z < maxGridSize) // If below is open and im still checking
                                        {
                            ///                printf(" 2)I detect +(%c), infront of the mountain\n", scenery[xNode + 1][yNode + z]);
                                       //     _getch();
                                            if (scenery[xNode + 1][yNode + z] == ' ') // If theres a hole to the right of that space
                                            {
                                                xNode = xNode + 1;
                                                yNode = yNode + z;
                                                z = 1000;      // Quit out of loop, you found a path
                                            }

                                        }
                                        else
                                        {
                                            if (scenery[xNode][yNode - z] == ' ' && checkMinusZ == 1 && yNode - z >  0)
                                            {
                               ///                 printf(" 3)I detect +(%c), infront of the mountain\n", scenery[xNode + 1][yNode - z]);
                                   //             _getch();
                                                if (scenery[xNode + 1][yNode - z] == ' ')
                                                {
                                                    xNode = xNode + 1;
                                                    yNode = yNode - z;
                                                    z = 1000;      // Quit out of loop, you found a path
                                               }
                                            }
                                        }
                                }
                                if (z != 1001)/// Nothing in the loop tripped and right didnt happen
                                {
                                    for (k = 1; k < maxGridSize - 1; k++)
                                    {
                                        regenRoom(l, k);                      /// If no other options, regen the coloum.
                          ///              printf("1111111111111");
                                    }
                                    m = 1;      // And reRun the coloum check
                                }
                            } else if (l == 1)
                                {
                                    for (k = 1; k < maxGridSize - 1; k++)
                                    {
                                        regenRoom(l, k);                      /// If no other options, regen the coloum.
                                    showBoard(199, -1,-1);printf("C(%d,%d) N(%d, %d)   ", l, m, xNode, yNode);
                         ///               printf("22222222222222222222"); // Sleep(500);
                                    }
                                    m = 1;
                                 }
                        }
             //           showBoard(199, -1,-1);// Remove this!
                        scenery[xNode][yNode] = '.';
                  //      printf("C(%d,%d) N(%d, %d)   ", l, m, xNode, yNode);
                    }
                }
         //       showBoard(199, -1,-1);// Remove this!
            } // end of testing loop

            }
    }
    return;
}

void showBoard (int bankPos, int bankPosChangeX, int bankPosChangeY) // Displays the main screen, and is overly responible for events
{

    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int i;
    int j;
    int k;
    int l;
    int xSave;
    int ySave;
    int loadSave;
    int SquareGridSize = (maxGridSize) - 1; //max of arg in xGrid

    xSave = characterBank[0];
    ySave = characterBank[1];
    characterBank[bankPos] = bankPosChangeX;
    characterBank[bankPos + 1] = bankPosChangeY;

    if (bankPos != 198)
    {
        clearScreen();
    }

    loadSave = nearOrHit();

    /*Running into an obstacle*/ //I know this is really inefficient code
    if (loadSave == 1000 || loadSave == 4000 || loadSave == 1 || loadSave == 2 || loadSave == 5 || loadSave == 4 || loadSave == 8 || loadSave == 9 || loadSave == 10)
    {
        characterBank[0] = xSave;
        characterBank[1] = ySave;
        xPos = xSave;
        yPos = ySave;
    }else
    if (loadSave == 13 || loadSave == 6000 || loadSave == 11 || loadSave == 12 || loadSave == 5000 || loadSave == 7)
    {
        characterBank[0] = xSave;
        characterBank[1] = ySave;
        xPos = xSave;
        yPos = ySave;
    }

    /*Moving Denice*/
    if (numInteract[18] == 3 && (characterBank[0] != xSave || characterBank[1] != ySave)) // Should also be if denice is on screen
    {
        characterBank[18] = xSave;
        characterBank[19] = ySave;
    }

    /*CALL FIGHTS HERE*/
    if (loadSave == 2000 || loadSave == 7000 || loadSave == 8000)
    {
        fight(loadSave / 1000); // fight(2) before
        clearScreen();
    }

    i = 0; // x position
    j = 0; // Used to initialize the whole row
    k = 0; // y position
    printf(" ");
    for (l = 0; l <= SquareGridSize; l++)
    {SetConsoleTextAttribute(hConsole, 9); printf("--"); SetConsoleTextAttribute(hConsole, 7);}
    printf("                           \n");
    while(k <= SquareGridSize)                                    // Prints the all the entries in xGrid in each row
    {
        SetConsoleTextAttribute(hConsole, 9); printf("|");SetConsoleTextAttribute(hConsole, 7);
        while(i <= SquareGridSize)                                // Prints all of xGrid
        {
            while (j <= SquareGridSize)                           // This is the initializing entries for xGrid
            {

                xGrid[j] = ' ';
              //  printf("scenery is T at (%d,%d\n", scenery[j][k],j,k);
                if (scenery[j][k] == 'T')
                {
                    xGrid[j] = 'T';
                }else
                if (scenery[j][k] == '=')
                {
                    xGrid[j] = '=';
                }else
                if (scenery[j][k] == ' ')
                {
                    xGrid[j] = ' ';
                }else
                if (scenery[j][k] == ']')
                {
                    xGrid[j] = ']';
                }else
                if (scenery[j][k] == '@')
                {
                    xGrid[j] = '@';
                }else
                if (scenery[j][k] == '[')
                {
                    xGrid[j] = '[';
                }else
                if (scenery[j][k] == '|')
                {
                    xGrid[j] = '|';
                }else
                if (scenery[j][k] == '~')
                {
                    xGrid[j] = '~';
                }else
                if (scenery[j][k] == '^')
                {
                    xGrid[j] = '^';
                }else
                if (scenery[j][k] == '-')
                {
                    xGrid[j] = '-';
                }else
                if (scenery[j][k] == '*')
                {
                    xGrid[j] = '*';
                }else
                if (scenery[j][k] == '#')
                {
                    xGrid[j] = '#';
                }else
                if (scenery[j][k] == 't')
                {
                    xGrid[j] = 't';
                }else
                if (scenery[j][k] == '.')
                {
                    xGrid[j] = '.';
                }


                if ((j == characterBank[0]) && (k == characterBank[1])) // Player
                {
                    xGrid[j] = 'Y';
                }else
                if ((j == characterBank[2]) && (k == characterBank[3]))  // Tree
                {
                    xGrid[j] = 'T';
                }else
                if ((j == characterBank[4]) && (k == characterBank[5]) ||(j == characterBank[14]) && (k == characterBank[15]) ||(j == characterBank[16]) && (k == characterBank[17]) )  // Monster
                {
                    xGrid[j] = 'M';
                }else
                if ((j == characterBank[6]) && (k == characterBank[7]))  // Log
                {
                    xGrid[j] = 'L';
                }else
                if ((j == characterBank[8]) && (k == characterBank[9]))  // Wizard
                {
                    xGrid[j] = 'W';
                }else
                if ((j == characterBank[10]) && (k == characterBank[11]))  // Decayed tree
                {
                    xGrid[j] = 't';
                }else
                if ((j == characterBank[12]) && (k == characterBank[13]))  // Oracle
                {
                    xGrid[j] = 'O';
                }else
                if ((j == characterBank[18]) && (k == characterBank[19]))  // Denice
                {
                    xGrid[j] = 'D';
                }

                j++;
            }
            /*Coloring*/
            if (i == characterBank[0] && k == characterBank[1]) {SetConsoleTextAttribute(hConsole, 14);} else
            if (i == characterBank[2] && k == characterBank[3]) {SetConsoleTextAttribute(hConsole, 10);} else
            if (i == characterBank[4] && k == characterBank[5]  ||(i == characterBank[14]) && (k == characterBank[15]) ||(i == characterBank[16]) && (k == characterBank[17])) {SetConsoleTextAttribute(hConsole, 12);} else
            if (i == characterBank[6] && k == characterBank[7]) {SetConsoleTextAttribute(hConsole, 6);} else
            if (i == characterBank[8] && k == characterBank[9]) {SetConsoleTextAttribute(hConsole, 11);} else
            if (i == characterBank[10] && k == characterBank[11]) {SetConsoleTextAttribute(hConsole, 6);} else
            if (i == characterBank[12] && k == characterBank[13]) {SetConsoleTextAttribute(hConsole, 15);} else
            if (i == characterBank[18] && k == characterBank[19]) {SetConsoleTextAttribute(hConsole, 3);} else
            if (scenery[i][k] == 'T') {SetConsoleTextAttribute(hConsole, 10);} else
            if (scenery[i][k] == '=') {SetConsoleTextAttribute(hConsole, 4);}  else
            if (scenery[i][k] == '@') {SetConsoleTextAttribute(hConsole, 8);}  else
            if (scenery[i][k] == ']') {SetConsoleTextAttribute(hConsole, 6);}  else
            if (scenery[i][k] == '[') {SetConsoleTextAttribute(hConsole, 6);}  else
            if (scenery[i][k] == '~') {SetConsoleTextAttribute(hConsole, 11);} else
            if (scenery[i][k] == '|') {SetConsoleTextAttribute(hConsole, 6);}  else
            if (scenery[i][k] == '^') {SetConsoleTextAttribute(hConsole, 7);}  else
            if (scenery[i][k] == '#') {SetConsoleTextAttribute(hConsole, 6);}  else
            if (scenery[i][k] == '*') {SetConsoleTextAttribute(hConsole, 3);}  else
            if (scenery[i][k] == '-') {SetConsoleTextAttribute(hConsole, 2);}  else
            if (scenery[i][k] == 't') {SetConsoleTextAttribute(hConsole, darkGreen);}

            printf(" %c", xGrid[i]);
            SetConsoleTextAttribute(hConsole, 7);
            if (i == SquareGridSize) // edge of board
            {
                SetConsoleTextAttribute(hConsole, 9); printf("|"); SetConsoleTextAttribute(hConsole, 8);
                if (k <= 4)
                    printf("\t%s : %d                 ", statNames[k], stats[k]);
                else
                if (k == 6)
                    printf("\tFights : %d Room: %d Pos: (%d,%d)      ", numFights, inRoom, xPos, yPos);

                else
                if (k == 7)
                    printf("\t inter24,25,26:  %d, %d, %d            ", numInteract[24], numInteract[25], numInteract[26]);



                else
                    printf("                                   ");
                    SetConsoleTextAttribute(hConsole, 7);
            }
            j = 0;
            i++;
        }
        printf("\n");

        i = 0;
        k++;
    }
    printf(" ");
    for (l = 0; l <= SquareGridSize; l++)
        {SetConsoleTextAttribute(hConsole, 9); printf("--"); SetConsoleTextAttribute(hConsole, 7);}
    printf("                                                     ");
    printf("\n");


    clearScreenText();
    for (l = 0; l < 30; l++)
        printf("                                                 ");
    clearScreenText();
    SetConsoleTextAttribute(hConsole, 8);
/*THIS EXCECUTES ACTIONS BASED ON NUMBER OF KILLS*/
    enemyEvents(numFights);

    interactionEvent(loadSave);
    RoomChanging();
    return;
}

void screenShake(int messageNum) // Causes the Screen to "shake" with a given message
{
    int j;
    for (j = 0; j < 15; j++)
    {
        Sleep(2);
        clearScreen();
        if ((j % 2 != 0))
        {
            printf("\n");
        }
        showBoard(198,-1,-1);
        switch(messageNum)
        {
        case 1:
            printf("???: BRAHLUGAH!!!!\n");
            break;
        case 2:
            printf("Self: Huh?");
            Sleep(10); // This gets activated in the small room, which prints too fast
            break;
        }

    }
}

void giveChances(float hit, float defend, float oppHit, float oppDefend) // Gives the probablility bars in fight screen
{
    int k;
    int j;
    int currentPrint;
    char bar[15];

        for (j = 0; j < 4; j++)
        {
            switch (j)
            {
            case 0:
                currentPrint = hit;
                printf("Attack Multi  : %3.0f%% :[", roundf(currentPrint));
                break;
            case 1:
                currentPrint = oppHit;
                printf("Opponent's Multi : %3.0f%% :[", roundf(currentPrint));
                break;
            case 2:
                currentPrint = defend;
                printf("Defend Chance : %3.0f%% :[", roundf(currentPrint));
                break;
            case 3:
                currentPrint = oppDefend;
                printf("Opponent's Chance: %3.0f%% :[", roundf(currentPrint));
                break;
            }
            for (k = 1; k < 15; k++)
            {
                if (k < roundf(currentPrint / 10))
                {
                    bar[k] = '=';
                }
                else
                {
                    if (k == roundf(currentPrint / 10))
                    {
                        bar[k] = ']';
                    }
                    else
                    {
                        bar[k] = ' ';
                    }
                }
                printf("%c", bar[k]);
            }
            if (j == 1 || j == 3)
            {
                printf("\n");
            }
        }

        printText(13);
}

int nearOrHit() // Collision detection
{
    int i = 0;
    int j = 0;
    int k = 0;
    int hitDetected = 0;
    for (i = 0; i <= maxGridSize - 1; i++) // was capped at 5
    {
        for (j = 0; j <= maxGridSize - 1; j++) //x of i == x or j && y of... //was capped at 4
        {
            if ((characterBank[2 * i] == characterBank[2 * j]) && (characterBank[2 * i + 1] == characterBank[2 * j + 1]) && (i != j))
            {
                if (characterBank[2 * i] > 0 && characterBank[2 * j] > 0 && characterBank[2 * i + 1] > 0 && characterBank[2 * j + 1] > 0)
                {
                    hitDetected = 100 * i + 1000 * j;
                    return hitDetected;
                }
            }
            for (k = 0; k <= 40; k++)
            {
                if (scenery[i][j] == boarderType[k] && characterBank[0] == i && characterBank[1] == j) //if theres a T at i,j and char is at i,j
                {
                    {
                        hitDetected = k + 1;
                        return hitDetected;
                    }
                }
            }
        }
    }
    return hitDetected;
}

int moveChar() // Moves the players character
{
    char move;
    int i;
    int change = 0;
    int k;

    do
    {
        move = 0;
        while ((move != 72 && move != 80 && move != 77 && move != 75 && move != 'm' && move != 32 && move != 'g' && move != 'r' && move != 'o') && move != 'w' && move != 's' && move != 'd' && move != 'a')
        {
            move = _getch();
        }
 //   printf("1)You pressed %d\n", move);


    switch (move) // if change is < 0 , it is affecting xPos.
    {
        case 'w':
        case 72: // Up arrow
            change = 3;
            break;
        case 's':
        case 80: // Down arrow
            change = 1;
            break;
        case 'd':
        case 77: // Right arrow
            change = -3;
            break;
        case 'a':
        case 75: // Left arrow
            change = -1;
            break;
        case 32:
            printf("space");
            break;
        case 'm':
        case 'g':
            displayInfo(move);
            break;
        case 'o':
            settingsMenu();
            break;
        case 'r': //s and w are used, so r  is used for (w)rite
            save();
            pressEnter(0);
            showBoard(199, -1, -1);
        //    return;
            break;
        default:
            printf("bad char");
    }
    } while (move == 'r');
    return (change);

}

void fight(int monsterNum)// When there are more fights, call in the monster tiy are fighting // stunning doesnt work!
{
    char action;
    int fightingnum = monsterNum * 5;
    int blockType = 0;
    int oppBlockType = 0;
    char cont = 't';
    int l = 0;
    int k;
    int reducedChance = 0;
    float myAttack = (float) stats[2];
    float myDefence = (float) stats[4];
    float theirAttack = (float) stats[fightingnum + 2];
    float theirDefence = (float) stats[fightingnum + 4];
    int healBy;
    int healTo;
    float hitModifier;
    float defendModifier;
    int stun = 0;
    int youHit;
    int heDefends;
    int heHit;

    system("cls");
    numFights++;
    if (settings[0] == -1)
    {
        system("cls");
        printf("You have fighting turned off. \nThis battle counts a win, but no score/stats are affected\n");
        printf("You may change this by going into the setting/options menu (o)");
        characterBank[2 * monsterNum] = -1;
        characterBank[2 * monsterNum + 1] = -1;
        stats[fightingnum + 1] = stats[fightingnum];
        printf("\n\tPress ANY key to continue.\n");
        _getch();
        system("cls");
        numInteract[2]++;
        if (numInteract[4] < 1)
            gameProgress(-1);
        showBoard(199,-1,-1);
        return;
    }

    while (cont == 't') // && stats[1] >= 0 && stats[fightingnum + 1] > 0)
    {
        float myMaxHealth = (float) stats[0];
        float myCurrentHealth = (float) stats[1];

        int actionChance = (rand() % 100);
        int oppActionChance = (rand() % 100);

        float hitModifier = (float) ((rand() % 100) + 40);
        float defendModifier = (float) ((rand() % 100) + 40);
        float heavyChance   = 100 * (1 - exp(( -2 * hitModifier    * myAttack / theirDefence) / 300));
        float lightChance   = 100 * (1 - exp(( -2 * hitModifier    * myAttack / theirDefence) / 200));
        float defenceChance = 100 * (1 - exp(( -2 * defendModifier * myAttack / theirDefence) / 100 ));

        float hitModOpp = (float) ((rand() % 100) + 40);
        float defendModOpp = (float) ((rand() % 100) + 40);
        float oppHit = 100 * (1 - exp(( -hitModifier * theirAttack / myDefence) / 250));
        float oppDef = 100 * (1 - exp( ( -2 * defendModifier * myAttack / theirDefence) / (100) ) );

        youHit = 0;
        heDefends = 0;
        blockType = 0;
        oppBlockType = 0;
        stun--;

        if (reducedChance == 1) //Heavy was used
        {
            heavyChance = heavyChance / 2.0;
            lightChance = lightChance / 2.0;
            defenceChance = defenceChance / 2.0;
            hitModifier = hitModifier / 2.0;
            reducedChance = 0;
        }
        if (reducedChance == 2) //Light was used
        {
            heavyChance = heavyChance / 1.2;
            lightChance = lightChance / 1.2;
            defenceChance = defenceChance / 1.2;
            hitModifier = hitModifier / 1.2;
            reducedChance = 0;
        }
        showStats(fightingnum); printText(5); giveChances(hitModifier, defendModifier, hitModOpp, defendModOpp);//Header, Default figures, Bars & Options
        l++;

        action = _getch(); // make it so only the 6 options work.



/***************************************************************************THIS IS TO DETERMINE THE BLOCK CONDITIONS FROM THE OPPONENT*/

        if (hitModifier * (140 - defendModOpp) > (hitModOpp * (140 - defendModifier) ) ) // better to defend //if yes, you will try defend
        {
            heDefends = 1; //Tries to defend
            if (oppDef >= oppActionChance) // more likely to get hit, than to hit //He successfully defended
                {
                    if ((oppDef / 3 >= oppActionChance))
                    {
                        oppBlockType = 1;
                    }
                    else
                    {
                        if (2 * oppDef / 3 >= oppActionChance)
                        {
                            oppBlockType = 2;
                        }
                        else
                        {
                            oppBlockType = 3;
                        }
                    }
                }
        }
        else //Otherwise, you try to attack
        {
            heDefends = 0; // He didnt defend or failed to defend
            if (oppDef >= oppActionChance)
            {
                 heHit;
            }
        }

/************************************************************************************************************************YOUR TURN*/

        system("cls");
        showStats(fightingnum);
        switch(action)
        {
            case 'h':
                if (heavyChance >= actionChance) // You hit
                {
                    youHit = 1;
                    if (heDefends == 0) // Did not try to defend
                    {
                        stats[fightingnum + 1] = stats[fightingnum + 1] - (2 * stats[3]); // Damage: Their health - your strength
                        printText(6);
                        printf("You try a HEAVY ATTACK!\n");
                        printf("You HIT! And do %d damage!\n", 2 * stats[3]);
                        reducedChance = 1;
                        printf("You feel MUCH LESS ABLE after using such a HEAVY ATTACK\n");
                    }
                    else //heDefends == 1, he tried to defend
                    {
                        if (oppBlockType == 1) // He blocked
                        {
                            printText(6);
                            printf("You try a HEAVY ATTACK!\n");
                            printf("Oh! But your opponent has BLOCKED it!\n");
                            reducedChance = 1;
                        }
                        else
                        {
                            if (oppBlockType == 2 || oppBlockType == 3) // You reduced
                            {
                                printText(6);
                                printf("You try a HEAVY ATTACK!\n");
                                printf("Oh! Your opponent has REDUCED your hit!\n");
                                reducedChance = 1;

                                float oppDamage = (1 / (2 * log(2 * stats[fightingnum + 4] + 1.2))) * stats[3];
                                int oppAppliedDamage = 2 * ((int) ceil(oppDamage));
                                stats[fightingnum + 1] = stats[fightingnum + 1] - oppAppliedDamage;
                                printf("You only hit for %d damage! (Reduced from %d) \n", oppAppliedDamage, 2 * stats[3]);
                                if (oppBlockType == 3)  // You recoiiled
                                {
                                    printf("ARG! And his defence has RECOILED some of the damage back to you!\n");
                                    stats[1] = stats[1] - (oppAppliedDamage);
                                }
                            }
                            else
                            {
                                printText(14);
                                printf("Your opponent tried to defend, but failed.\n");
                            }
                        }
                   }

                }
                else // Miss
                {
                    printText(14);
                    printf("You try a HEAVY ATTACK!\n");
                    printf("Oh shoot! You MISSED!\n");
                    reducedChance = 1;
                    printf("You feel MUCH LESS ABLE after using such a HEAVY ATTACK\n");
                }
                break;
            case 'l':
                if (lightChance >= actionChance) // You hit
                {
                    youHit = 1;
                    if (heDefends == 0) // He did not defend
                    {
                        stats[fightingnum + 1] = stats[fightingnum + 1] - (stats[3]); // Damage: Their health - your strength
                        printText(6);
                        printf("You try a LIGHT ATTACK!\n");
                        printf("You HIT! And do %d damage!\n", stats[3]);
                        reducedChance = 2;
                        printf("You feel A BIT LESS ABLE after using such a LIGHT ATTACK\n");
                    }
                    else
                    {
                        if (oppBlockType == 1) // he did block
                        {
                            printText(6);
                            printf("You try a LIGHT ATTACK!\n");
                            printf("Oh! But your opponent has BLOCKED it!\n");
                            reducedChance = 2;
                        }
                        else
                        {
                            if (oppBlockType == 2 || oppBlockType == 3)
                            {
                                printText(6);
                                printf("You try a LIGHT ATTACK!\n");
                                printf("Oh! Your opponent has REDUCED your hit!\n");
                                reducedChance = 2;

                                float oppDamage = (1 / (2 * log(2 * stats[fightingnum + 4] + 1.2))) * stats[3];
                                int oppAppliedDamage = (int) ceil(oppDamage);
                                stats[fightingnum + 1] = stats[fightingnum + 1] - ceil(oppDamage);
                                printf("You only hit for %d damage! (Reduced from %d) \n", oppAppliedDamage, stats[3]);
                                if (oppBlockType == 3)
                                {
                                    printf("ARG! And his defence has RECOILED some of the damage back to you!\n");
                                    stats[1] = stats[1] - (oppAppliedDamage);
                                }
                            }
                            else
                            {
                                printText(14);
                                printf("Your opponent tried to defend, but failed to block anything.\n"); //He defended but no block (he failed to block)
                            }
                        }
                    }
                }
                else // miss
                {
                    printText(14);
                    printf("You try a LIGHT ATTACK!\n");
                    printf("Oh shoot! You MISSED!\n");
                    reducedChance = 2;
                    printf("You feel A BIT LESS ABLE after using a LIGHT attack\n");
                }
                break;
            case 'd':
                if (defenceChance >= actionChance) // chance to either block, reduce, recoil. If action chance > base + 2 * defence: You block
                {
                    printText(11);
                    printf("You try to DEFEND!\n");
                    if ((defenceChance / 3 >= actionChance))
                    {
                        printf("You've managed to set up a BLOCK against an attack!\n");
                        blockType = 1;
                    }
                    else
                    {
                        if (2 * defenceChance / 3 >= actionChance)
                        {
                            printf("If your opponent attacks, you will REDUCE some of the damage\n");
                            blockType = 2;
                        }
                        else
                        {
                            printf("If your opponent attacks, you will REDUCE and RECOIL some of the damage!\n");
                            blockType = 3;
                        }
                    }
                }
                else
                {
                    system("cls");
                    showStats(fightingnum);
                    printText(14);
                    printf("You try to DEFEND!\n");
                    printf("Shoot, you haven't protected yourself!\n");
                }
                break;
            case 's':
                if (40 >= actionChance) // chance to either block, reduce, recoil. If action chance > base + 2 * defence: You block
                {
                    printText(16);
                    printf("You try to STUN!\n");
                    printf("You've managed to STUN him!\n");
                    stun = 3;
                }
                else
                {
                    printText(14);
                    printf("You try to STUN!\n");
                    printf("Darn, You're stun FAILED!\n");
                }
                break;
            case 'p':
                if (50 >= actionChance)
                {
                    printText(17);
                    printf("You try to use a POTION!\n");
                    printf("You could only find water. It feels refreshing, but does NOT restore any HEALTH.\n");
                }
                else
                {
                    printText(14);
                    printf("You try to use a POTION!\n");
                    printf("But... You found NOTHING to HEAL with...\n");
                }
                break;
            case 'r':
                printf("You try to REST!\n");
                if (myCurrentHealth < myMaxHealth)
                {
                    printText(15);
                    if (myCurrentHealth >= 0.95 * myMaxHealth)
                    {
                        int healTo = (int) myMaxHealth;
                        stats[1] = healTo;
                        printf("You've restored to full health!");
                    }
                    else
                    {
                        myMaxHealth = 0.05 * myMaxHealth;
                        int healBy = (int) myMaxHealth;
                        stats[1] = stats[1] + healBy;
                        printf("You've regained %d health", healBy);
                    }
                }
                else
                {
                    printText(14);
                    printf("Hmmm... It appears that you are already fully rested!");
                }
                break;
        }
        printf("\n\tPress ANY key to continue.");

/**********************************************************************************************************************END CONDITIONS*/
        if (stats[1] <= 0)
        {
            system("cls");
            showStats(fightingnum);printText(21);
            printf("Oh Dear, You've died...\n");
            pressEnter(0);
          //  printf("\n\tPress ANY key to continue.");
           // _getch();
            system("cls");
            showStats(fightingnum);printText(21);
            printf("\nPro Tip: Try not being garbage!\n");
            printf("Would you like to Try again (t) or Quit the game (q)?");
            cont = 0;
            while (cont != 't' && cont != 'q')
                cont = _getch(); // make it so it only gets t or q
            if (cont == 't')
            {
                stats[1] = stats[0];
                stats[fightingnum + 1] = stats[fightingnum];
                system("cls");
            }else
            if (cont == 'q')
            {
                exit(0);
            }
        }
        if (stats[fightingnum + 1] <= 0)
        {
            system("cls");
            showStats(fightingnum);printText(22);
            printf("Congratulations, you've DEFEATED your opponent!\n");
            increaseStat();
            characterBank[2 * monsterNum] = -1;
            characterBank[2 * monsterNum + 1] = -1;
            stats[fightingnum + 1] = stats[fightingnum];
            printf("\n\tPress ANY key to continue.\n");
            _getch();
            system("cls");
            numInteract[2]++;
            if (numInteract[4] < 1)
                gameProgress(-1);
            showBoard(199,-1,-1);
            return;
        }





/******************************************************************************************************************************Their turn! */

        _getch();
        system("cls");
        showStats(fightingnum);
        if (heDefends == 0) // he tried to attack
        {
            if (heDefends == 0 && stun <= 0) // he dont try to defend, means he try to attack   (heDefends == 0 && oppBlockType == 0 && stun <= 0)
            {
                if (blockType == 0)
                {
                     printText(7);
                }
                else
                {
                    printText(11);
                }
                printf("Now, your opponent attacks!\n");
                if (blockType == 1) //Block
                {
                    printf("Your opponent hits but does no damage, since you've blocked\n");
                }
                else
                {
                    if (blockType == 2 || blockType == 3) // Reduce or recoil
                    {
                        float damage = (1 / (2 * log(2 * stats[4] + 1.2))) * stats[fightingnum + 3];
                        int appliedDamage = (int) ceil(damage);
                        stats[1] = stats[1] - ceil(damage);
                        printf("Ah! You were hit for %d damage! (Reduced from %d) \n", appliedDamage, stats[fightingnum + 3]);
                        if (blockType == 3)  // Recoil
                        {
                            stats[fightingnum + 1] = stats[fightingnum + 1] - appliedDamage;
                            printf("And you've recoiled %d damage back!\n", appliedDamage);
                        }
                    }
                    else // No defence
                    {
                        printf("Ah! You were hit for %d damage!\n", stats[fightingnum + 3]);
                        stats[1] = stats[1] - stats[fightingnum + 3];
                    }
                }
            }
            else
            {
                if (stun <= 1)
                {
                    printText(5);
                    printf("Now, your opponent attacks!\n");
                    printf("Thankfully, they missed!\n");
                }
                else
                {
                    printText(16);
                    printf("Yes! He's stunned this turn!\n");
                }
            }
        }
        else //He tries to defend
        {
            if (heDefends == 1) // Tries to defend (explicitly stated)
            {
                system("cls");
                showStats(fightingnum);
                if (youHit != 1) // If I missed
                {
                    printText(11);
                    printf("He tries to DEFEND!\n");
                    printf("But you didn't hit him anyway!\n");
                }
                else
                {
                    switch(oppBlockType)
                    {
                    case 0:
                        printText(14);
                        printf("He tries to DEFEND!\n");
                        printf("But he FAILED to set up any DEFENCE!\n");
                        break;
                    case 1:
                        printText(11);
                        printf("He tries to DEFEND!\n");
                        printf("And his defence has BLOCKED your attack!\n");
                        break;
                    case 2:
                        printText(11);
                        printf("He tries to DEFEND!\n");
                        printf("He's managed to REDUCED some of your damage\n");
                        break;
                    case 3:
                        printText(11);
                        printf("He tries to DEFEND!\n");
                        printf("His defence will REDUCE your damage and RECOIL some of the damage back to you!\n");
                        break;
                    }
                }
            }
            else
            {
                system("cls");
                showStats(fightingnum);
                printText(14);
                printf("He tried to DEFEND!\n");
                printf("But it didn't work!\n");
            }
        }
        printf("\n\tPress ANY key to continue.\n");
        _getch();
        system("cls");


/**************************************************************************************************************************END CONDITIONS*/
        if (stats[1] <= 0)
        {
            system("cls");
            showStats(fightingnum);printText(21);
            printf("Oh Dear, You've died...");
            pressEnter(0);
        //  printf("\n\tPress ANY key to continue.\n");
         //   _getch();
            printf("\nPro Tip: Try not being garbage!\n");
            printf("Would you like to Try again (t) or Quit the game (q)?");
            cont = 0;
            while (cont != 't' && cont != 'q')
                cont = _getch(); // make it so that only q or t
            if (cont == 't')
            {
                stats[1] = stats[0];
                stats[fightingnum + 1] = stats[fightingnum];
                system("cls");
            }else
            if (cont == 'q')
            {
                exit(0);
            }
        }
        if (stats[fightingnum + 1] <= 0)
        {
            system("cls");
            showStats(fightingnum);printText(22);
            printf("Congratulations, you've DEFEATED your opponent!\n");
            printf("\n\tPress ANY key to continue.\n");
            _getch();
            increaseStat();
            characterBank[2 * monsterNum] = -1;
            characterBank[2 * monsterNum + 1] = -1;
            stats[fightingnum + 1] = stats[fightingnum];
            printf("\n\tPress ANY key to continue.\n");
            _getch();
            system("cls");
            numInteract[2]++;
            cont = 'f';
            if (numInteract[4] < 1)
                gameProgress(-1);
            showBoard(199,-1,-1);
            return;
        }
    }
    return;
}

int printText(int textNum) // Prints text
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int k;
    int j = 0;

    switch(textNum)
    {
        case -2:
            SetConsoleTextAttribute(hConsole, 12);
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("           WELCOME to MALCHOVE'S MAZE!          \n");
            printf("=====================================================\n\n");
            SetConsoleTextAttribute(hConsole, 11);
            printf("What would you like to do?:\n");
            printf("PLAY GAME (p)\n");
            printf("LOAD SAVE (l)\n");
            printf("OPTIONS   (o)\n");
            printf("GUIDE     (g)\n");
            printf("CREDITS   (c)\n");
            SetConsoleTextAttribute(hConsole, 8);
            printf("\nNotes: 1) Press 'g' anytime to see the guide.\n");
            printf("       2) You may need to enlarge your terminal.\n");
            printf("       3) Saving the game may cause errors, if it is done before turn 4.\n");
            startOptions();
            return;
            break;
        case -1:
            printf("\n        Arrrgg... I've been running for so long...    \n");
            printf("        Ah, my legs are killing me huhuhuhu...        \n\n");
            printf("        Just keep moving, I'll be back soon.\n");
            printf("        This NoteBook is getting really annoying\n\n");
            printf("           *Your body feels more sensitive to pressure now*\n");
            printf("        Running is terrible!\n");
            printf("            *You take a look at the NoteBook...   \n");
            printf("        You always liked seeing your name...\n");
            printf("   ______________________________________________________\n");
            printf("  |======================================================\n");
            printf("  |------------------------------------------------------\n");
        for (k = 0; k < 4; k++)
            {printf("  |I|\t\t\t\t\t\t\t|\n");Sleep (50);}
             printf("  |I|            Notebook                               |\n");
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
                Position.Y = 23;
                SetConsoleCursorPosition(hOut, Position);

                gets(playerName);
                Position.X = 0;
                Position.Y = 30;
            }
            for (k = 0; k < 6; k++)
            printf("  |I|\t\t\t\t\t\t\t|\n");
            printf("      %s...\n", playerName);
            pressEnter(0);
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
            pressEnter(0);

            break;
        case 0:
            colorText(2, 0);
            break;
        case 1:
            colorText(2, 1);
            break;
        case 2:
            printf("Self: It's a tree! hmmm... It looks very old, and yet oddly peaceful,\n");
            printf("\tOh and it's got some kind of inscription on it!\n");
            printf("\tIt says \"Adrea, No more shall rise.\" Hmmmm...\n");
            pressEnter(1);
            showBoard(4, 10, 10);
            printf("Self: Woah, what is that?!\n");
            pressEnter(1);
            screenShake(1);
            pressEnter(1);
            showBoard(4, 10, 9);
            printf("Self: Is it... is it moving towards me??\n");
            printf("\tQuick I need to find something fast!\n");
            pressEnter(1);
            showBoard(6, 6, 3);
            break;
        case 3:
            showBoard(2, 5, 5);
            colorText(2, 3);
            break;
        case 4:
            printf("*********************IT'S TIME TO FIGHT!************************\n");
            printf("Your Health  : %d/%d\t   Your opponent's Health  : %d/%d\n", stats[1] ,stats[0], stats[11], stats[10]);
            for (j = 1; j < 4; j++)
            {
                printf("Your %-8s: %d\t   %Your opponent's %-8s: %d\n", statNames[j + 1], stats[j + 1], statNames [j + 1], stats[j + 11]);
            }
            break;
        case 5:
            printf("--------------------------\n");
            printf("          0      0        \n");
            printf("         |||    |||       \n");
            printf("         |||-  -|||       \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 6:
            printf("--------------------------\n");
            printf("          0      0 Pow!   \n");
            printf("         ||-~-->|||       \n");
            printf("         ||    -|||       \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 7:
            printf("--------------------------\n");
            printf("     Pow! 0      0        \n");
            printf("         |||<--~-||       \n");
            printf("         |||-    ||       \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 8:
            colorText(2, 8);
            showBoard(8, 5, 2);
            break;
        case 9:
            colorText(2, 9);
            break;
        case 10:
            colorText(2,10);
            showBoard(199, -1, -1);
            break;
        case 11:
            printf("--------------------------\n");
            printf("Defended! 0      0        \n");
            printf("         ||--]<--||       \n");
            printf("         ||      ||       \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 12:
            break;
        case 13:
            printf("--------------------------\n");
            printf("What would you like to do?\n");
            printf("Heavy Attack(h)  Defend (d) \n");
            printf("Light Attack(l)  Stun   (s) \n");
            printf("Potion      (p)  Rest   (r) \n");
            printf("--------------------------\n");
            break;
        case 14:
            printf("--------------------------\n");
            printf("          0 Miss!0        \n");
            printf("     <----||    ||---->   \n");
            printf("          ||    ||        \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 15:
            printf("--------------------------\n");
            printf("   Zzzz.. 0      0        \n");
            printf("         |||    |||   \n");
            printf("         |||    |||       \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 16:
            printf("--------------------------\n");
            printf("    Stun! 0    **0**      \n");
            printf("         |||    |||       \n");
            printf("         |||    |||       \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 17:
            printf("--------------------------\n");
            printf("  Potion! 0*|^|* 0      \n");
            printf("         ||--v  |||       \n");
            printf("         ||     |||       \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 18:
            colorText(2, 18);
            createRoom(2);
            showBoard(199, -1, -1);
            break;
        case 19:
            stats[4] = stats[4] + 2;
            colorText(2, 19);
            break;
        case 20:
            characterBank[12] = 6;
            characterBank[13] = 10;
            showBoard(199, -1, -1);
            printf("Oracle: Greetings....\n");
            printf("Self: Hey, I was told to see you by a wiz-\n");
            printf("Oracle: Increase the protection around your chest\n");
            printf("\tSo night turns to day\n");
            printf("\tAs open goes to... what?\n");
            printf("Self: Uhhhh....");
            printf("\tWhat does that even mean??\n");
            printf("The Oracle does not respond to you.\n");
            pressEnter(1);
            showBoard(199,-1,-1);
            break;
        case 21:
            printf("--------------------------\n");
            printf("  Death!  x      0        \n");
            printf("         |||    |||       \n");
            printf("         |||-  -|||       \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 22:
            printf("--------------------------\n");
            printf("          0      x  Death!\n");
            printf("         |||    |||       \n");
            printf("         |||-  -|||       \n");
            printf("          |      |        \n");
            printf("         /\\     /\\      \n");
            printf("        |  |   |  |       \n");
            printf("--------------------------\n");
            break;
        case 23:
            printf("Self: Woah! There are so many trees! How am I supposed to get through all this?\n");
            pressEnter(0);
            numInteract[18] = 1;
            break;
        case 24:
            clearScreen();
            showBoard(199, -1, -1);
            colorText(2, 24);
            numInteract[18] = 2;
            createRoom(9);
            break;
        case 25:
            colorText(2, 25);
    }


    return;
}

int gameProgress(int turn) // Runs major events... Kind of (not really used anymore
{
    int j;
    switch(turn)
    {
        case -1: // First monster killed
            showBoard(2, 5, 5);
            Sleep(200);
            printf("Self: Wait what?");
            Sleep(2000);
            showBoard(2, -1, -1);
            printf("Self: Wait what?");
            showBoard(10, 5, 5);
            printf("Self: Wait what?");

            for (j = 0; j < 4; j++)
            {
                Sleep(1);
                showBoard(10, -1, -1);
                printf("Self: Wait what?");
                showBoard(2, 5, 5);
                printf("Self: Wait what?");
                Sleep(1);
                showBoard(2, -1, -1);
                printf("Self: Wait what?");
                showBoard(10, 5, 5);
                printf("Self: Wait what?");
            }
            printf("\n\tWhat happened to all the leaves?...\n");
            Sleep(3000);
            printText(8);
            break;
        case 0: // Game intro
            printText(0);
            break;
        case 4: // Turn 5, Tree apears
            if (numInteract[0] < 1)
            {
                showBoard(2, 5, 5);
                printText(1);
                showBoard(2, 5, 5);
            }
            break;
        case 5:
            printText(23);
            createRoom(9);
         //   showBoard(199, -1, -1);
            break;
        case 1000: // Interact with Tree
            printText(2);
            break;
        case 3000: // Interact with Log
            characterBank[6] = -1;
            characterBank[7] = -1;
            stats[3] = stats[3] + 1;
            printText(3);
            break;
        case 4000: // Interact with Wizard
            printText(9);
            turnDiff = 1;
            break;
    }
    if (turnDiff > 0)
    {
       turnDiff++;
        if (turnDiff == 6 && numInteract[4] < 2)
        {
            printText(10);
            turnDiff = 0;
        }
    }
    return;
}

void moveMonster(int monsterNum) // Performs movement towards the player
{
    int j;
    int xMonSave = characterBank[monsterNum];
    int yMonSave = characterBank[monsterNum + 1];
    int loadMonSave;

    if (characterBank[monsterNum] != -1)
    {
        for (j = 0; j <= 1; j++)
        {
            if (characterBank[0] < characterBank[monsterNum]) //Compare x's
                characterBank[monsterNum] = characterBank[monsterNum] - 1;
            else
            {
                if (characterBank[0] > characterBank[monsterNum]) //Compare x's
                    characterBank[monsterNum] = characterBank[monsterNum] + 1;
            //    else
            //    {
            loadMonSave = nearOrHit();
            switch(loadMonSave)
            {
            case 2100: // Tree
            case 3200: // Log
                characterBank[monsterNum] = xMonSave;
                characterBank[monsterNum + 1] = yMonSave;
            }
                    if (characterBank[1] > characterBank[monsterNum + 1]) //Compare y's
                         characterBank[monsterNum + 1] = characterBank[monsterNum + 1] + 1;
                     else
                    {
                        if (characterBank[1] < characterBank[monsterNum + 1]) //Compare y's
                           characterBank[monsterNum + 1] = characterBank[monsterNum + 1] - 1;
              //      }
                }
            }
        }
        loadMonSave = nearOrHit();;
        switch(loadMonSave)
            {
            case 2100:
            case 3200:
                characterBank[monsterNum] = xMonSave;
                characterBank[monsterNum + 1] = yMonSave;
            }



        showBoard(199, -1, -1);
    }
}

