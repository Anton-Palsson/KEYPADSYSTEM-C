#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct Card {
    int cardNumber;
    time_t timestamp;
    bool access;
};

void remoteOpenDoor() {
    printf("CURRENTLY LAMP IS: GREEN\n");
    sleep(3);
    printf("CURRENTLY LAMP IS: OFF\n");
}

void printAllCards(struct Card *cardArray, int size) {
    printf("\nAll Cards in the System:\n");

    for (int i = 0; i < size; i++) {
        char timeStr[20];
        struct tm *tm_info = localtime(&cardArray[i].timestamp);
        strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", tm_info);

        printf("Card Number: %d\n", cardArray[i].cardNumber);
        printf("Access: %s\n", cardArray[i].access ? "true" : "false");
        printf("Timestamp: %s\n", timeStr);
        printf("--------------------\n");
    }
}

void saveCardsToFile(struct Card *cardArray, int size) {
    FILE *file = fopen("C:\\Users\\anton\\Desktop\\cdemo\\cards.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < size; i++) {
            char timeStr[20];
            struct tm *tm_info = localtime(&cardArray[i].timestamp);
            strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", tm_info);

            fprintf(file, "Card Number: %d\n", cardArray[i].cardNumber);
            fprintf(file, "Access: %d\n", cardArray[i].access);
            fprintf(file, "Timestamp: %04d-%02d-%02d %02d:%02d:%02d\n",
                    tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
                    tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
        }
        fclose(file);
    } else {
        printf("Unable to open file for writing.\n");
    }
}

void loadCardsFromFile(struct Card **cardArray, int *size) {
    FILE *file = fopen("C:\\Users\\anton\\Desktop\\cdemo\\cards.txt", "r");
    if (file != NULL) {
        struct tm tm_info;
        char timeStr[20];
        while (1) {
            struct Card newCard;
            if (fscanf(file, "Card Number: %d\nAccess: %d\n", &newCard.cardNumber, &newCard.access) != 2) {
                break;
            }

            if (fscanf(file, "Timestamp: %d-%d-%d %d:%d:%d\n",
                       &tm_info.tm_year, &tm_info.tm_mon, &tm_info.tm_mday,
                       &tm_info.tm_hour, &tm_info.tm_min, &tm_info.tm_sec) != 6) {
                break;
            }

            tm_info.tm_year -= 1900;
            tm_info.tm_mon -= 1;
            newCard.timestamp = mktime(&tm_info);
            *size = *size + 1;
            *cardArray = realloc(*cardArray, *size * sizeof(struct Card));

            if (*cardArray == NULL) {
            }

            (*cardArray)[*size - 1] = newCard;
        }

        fclose(file);
    } else {
        printf("Unable to open file for reading.\n");
    }
}

bool isCardNumberUnique(struct Card *cardArray, int size, int cardNumber) {
    for (int i = 0; i < size; i++) {
        if (cardArray[i].cardNumber == cardNumber) {
            return false;
        }
    }
    return true;
}

void addCardFunction(struct Card **cardArray, int *size, int *lastCardNumber, time_t timestamp) {
    int accessInt;
    bool access;

    do {
        printf("Enter access status (1 for access, 0 for no access): ");
        if (scanf("%d", &accessInt) != 1) {
            printf("Invalid input. Please enter 0 or 1.\n");
            while (getchar() != '\n');
            continue;
        }

        if (accessInt == 0 || accessInt == 1) {
            access = (bool)accessInt;
            break;
        } else {
            printf("Invalid input. Please enter 0 or 1.\n");
            while (getchar() != '\n');
        }
    } while (1);

    int cardNumber;
    do {
        printf("Enter card number: ");
        if (scanf("%d", &cardNumber) != 1) {
            printf("Invalid input. Please enter a valid series of numbers.\n");
            while (getchar() != '\n');
            continue;
        }

        if (!isCardNumberUnique(*cardArray, *size, cardNumber)) {
            printf("Card number is already in use. Please choose a different one.\n");
        } else {
            break;
        }
    } while (1);

    *size = *size + 1;
    *cardArray = realloc(*cardArray, *size * sizeof(struct Card));
    if (*cardArray == NULL) {
    }

    (*cardArray)[*size - 1].cardNumber = cardNumber;
    (*cardArray)[*size - 1].timestamp = timestamp;
    (*cardArray)[*size - 1].access = access;

    char timeStr[20];
    struct tm *tm_info = localtime(&timestamp);
    strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("New Card Added:\n");
    printf("Card Number: %d\n", cardNumber);
    printf("Access: %s\n", access ? "Access" : "No access");
    printf("Added to system: %s\n", timeStr);
}

void fakeTestScanCard(struct Card *cardArray, int size, int *currentTestCardIndex) {
    printf("Initiating FAKE TEST SCAN CARD\n");
    sleep(1);
    int cardNumber;
    printf("Please scan card: ");
    scanf("%d", &cardNumber);

    for (int i = 0; i < size; i++) {
        if (cardArray[i].cardNumber == cardNumber) {
            *currentTestCardIndex = i;
            break;
        }
    }

    if (*currentTestCardIndex == -1) {
        printf("Card not found in the system.\n");
        return;
    }

    if (cardArray[*currentTestCardIndex].access) {
        printf("CURRENTLY LAMP IS: OFF\n");
        sleep(1);
        remoteOpenDoor();
    } else {
        printf("CURRENTLY LAMP IS: OFF\n");
        sleep(1);
        printf("CURRENTLY LAMP IS: RED\n");
    }
}

void adminMenu() {
    FILE *file = fopen("C:\\Users\\anton\\Desktop\\cdemo\\cards.txt", "r");
    if (file != NULL) {
        fclose(file);
    } else {
        perror("Unable to open file for creating");
    }
    int choice;
    struct Card *cardArray = NULL;
    int size = 0;
    int lastCardNumber = 0;
    int currentTestCardIndex = -1;
    loadCardsFromFile(&cardArray, &size);

    while (1) {
        printf("\nAdmin Menu\n");
        printf("1. Remote open door\n");
        printf("2. List all cards in the system\n");
        printf("3. Add card\n");
        printf("4. Save and exit\n");
        printf("5. FAKE TEST SCAN CARD\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("1: Remote open door\n");
                printf("--------------------\n");
                remoteOpenDoor();
                break;
            case 2:
                printf("2: List all cards in the system\n");
                printf("--------------------\n");
                printAllCards(cardArray, size);
                break;
            case 3:
                printf("3: Add card\n");
                printf("--------------------\n");
                addCardFunction(&cardArray, &size, &lastCardNumber, time(NULL));
                break;
            case 4:
                printf("4: Save and exit\n");
                saveCardsToFile(cardArray, size);
                printf("--------------------\n");
                free(cardArray);
                return;
            case 5:
                printf("5: FAKE TEST SCAN CARD\n");
                fakeTestScanCard(cardArray, size, &currentTestCardIndex);
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    }
}

int main() {
    adminMenu();
}
