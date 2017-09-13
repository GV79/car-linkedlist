#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/****************************************
CIS2520 Assignment 2
Part: Question 1
Gia Vinh Lam
0925571
glam01@mail.uoguelph.ca
October 2016
This program manages a car rental company
*****************************************/

struct Car {
    char * plateNum; //standard ON plates have 7 total number of letters and numbers
    int mileage;
    int returnDate;
    struct Car * next;
};
typedef struct Car Car;

// Function Prototypes
Car * addToList(Car * list, Car * car, int type);
Car * carTransfer(Car * list, char * plateNum);
Car * createList(char * plateNum, int  mileage);
Car * restoreList(Car * list, char * dataText, int parseList);
char * verifyPlate();
double mileageCosts(int mileage);
int compareDates(int dateOne, int dateTwo);
int checkDuplicatePlate(Car * list, Car * listTwo, Car * listThree, char * plateNum, int option);
int findMileage(Car * car, char * plateNum);
int verifyCommand(char * command);
int verifyDate();
int verifyMileage();
void printList(Car * list);
void saveLists(FILE* data, char * path, Car * list, Car * listTwo, Car * listThree);

// Main function which runs program from beginning to end. Takes in command-line arguments
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Need to add argument for data file.\n");
        exit(0);
    }
    int convertString;
    int duplicatePlate;
    int errorChecking;
    int mileage, totalMileage;
    int parseList;
    Car * car;
    Car * afr = NULL; // availible-for-rent
    Car * r = NULL; // rented
    Car * ir = NULL; // in repair
    char command[1000] = "";
    char dataText[1000] = "";
    char * plateNum = NULL;
    double income;
    FILE* data;
    convertString = 0;
    duplicatePlate = 0;
    errorChecking = 0;
    income = 0.0;
    mileage = 0;
    totalMileage = 0;
    parseList = 0;

    data = fopen(argv[1], "r");
    while (fgets(dataText, 1000, data))
    {
        if (strstr(dataText, "availibleList") != NULL)
        {
            parseList = 0; //setting certain values for each list so that when I parse I know which list to put the values in
            continue;
        }
        else if (strstr(dataText, "rentList") != NULL)
        {
            parseList = 1;
            continue;
        }
        else if (strstr(dataText, "repairList") != NULL)
        {
            parseList = 2;
            continue;
        }

        switch(parseList) {
            case 0:
                afr = restoreList(afr, dataText, parseList);
                break;
            case 1:
                r = restoreList(r, dataText, parseList);
                break;
            case 2:
                ir = restoreList(ir, dataText, parseList);
                break;
        }
    }

    printf("Welcome to the program. Enter one of the following numbers for it's corresponding functions:\n");
    while (convertString != 7)
    {
        while (errorChecking == 0)
        {
            printf("(1) Add a new car to the availible-for-rent list.\n");
            printf("(2) Add a returned car to the availible-for-rent list.\n");
            printf("(3) Add a returned car to the repair list.\n");
            printf("(4) Transfer a car from the repair list to the availible-for-rent list.\n");
            printf("(5) Rent the first availible car.\n");
            printf("(6) Print all the lists.\n");
            printf("(7) Quit.\n");
            scanf("%s", command);
            if (verifyCommand(command) == 1)
            {
                errorChecking = 1;
            }
            else
            {
                continue;
            }
        }
        errorChecking = 0;
        convertString = atoi(command);

        if (convertString == 1)
        {
            while (duplicatePlate == 0)
            {
                plateNum = verifyPlate(); //get proper plate number input from user
                duplicatePlate = checkDuplicatePlate(afr, r, ir, plateNum, 0); // check other lists to see if the plate number already exists
            }
            duplicatePlate = 0;
            mileage = verifyMileage();
            car = createList(plateNum, mileage);
            afr = addToList(afr, car, 0);
            printf("Car %s successfully added to availible-to-rent list.\n", plateNum);
        }
        else if (convertString == 2)
        {
            if (r == NULL)
            {
                printf("Error: No cars in the rented list (therefore none to return).\n");
            }

            duplicatePlate = 1;
            while (duplicatePlate != 0) // != 0 this time because now we want to make sure that the plate number exists in the list
            {
                plateNum = verifyPlate();
                duplicatePlate = checkDuplicatePlate(afr, r, ir, plateNum, 1);
            }

            printf("The mileage in this case refers to the distance travelled while the car was rented.\n");
            mileage = verifyMileage();
            totalMileage = findMileage(r, plateNum);
            r = carTransfer(r, plateNum);
            income = income + mileageCosts(mileage);
            mileage = mileage + totalMileage;
            printf("Car %s successfully transferred from the rent list to the availible-for-rent list.\n", plateNum);
            car = createList(plateNum, mileage); //not total yet !
            car->returnDate = 0;
            afr = addToList(afr, car, 0);
        }
        else if (convertString == 3)
        {
            if (r == NULL)
            {
                printf("Error: No cars in the rented list (therefore none to repair).\n");
            }

            duplicatePlate = 1;
            while (duplicatePlate != 0)
            {
                plateNum = verifyPlate();
                duplicatePlate = checkDuplicatePlate(afr, r, ir, plateNum, 2);
            }

            totalMileage = findMileage(r, plateNum);
            printf("The mileage in this case refers to the distance travelled while the car was rented.\n");
            mileage = verifyMileage();
            r = carTransfer(r, plateNum);
            income = income + mileageCosts(mileage);
            mileage = mileage + totalMileage;
            printf("Car %s successfully transferred from the rent list to the repair list.\n", plateNum);
            car = createList(plateNum, mileage);
            car->returnDate = 0;
            ir = addToList(ir, car, 2);
        }
        else if (convertString == 4)
        {
            if (ir == NULL)
            {
            	    printf("Error: No cars in the 'in repair' list (therefore unable to transfer).\n");
            }
            duplicatePlate = 1;
            while (duplicatePlate != 0)
            {
                plateNum = verifyPlate();
                duplicatePlate = checkDuplicatePlate(afr, r, ir, plateNum, 3);
            }
            ir = carTransfer(ir, plateNum);
            car = createList(plateNum, mileage);
            afr = addToList(afr, car, 0);
            printf("Car %s successfully transferred from the repair list to the availible-for-rent list.\n", plateNum);
        }
        else if (convertString == 5)
        {
            if (afr == NULL)
            {
                printf("Error: No availible cars to rent.\n");
            }
            else
            {
                printf("Car %s successfully rented from availible-for-rent list.\n", afr->plateNum);
                r = addToList(r, afr, 1);
                afr = afr->next; //get rid of first element, and make that the list
            }
        }
        else if (convertString == 6)
        {
            printf("Availible-for-rent list: \n");
            printList(afr);
            printf("\nRented list: \n");
            printList(r);
            printf("\nIn-repair list: \n");
            printList(ir);
            printf("\n");
        }
        duplicatePlate = 0;
    }
    printf("Thanks for using the program.\n");
    saveLists(data, argv[1], afr, r, ir);
    printf("Income from cars rented in this session: $%f\n", income);
    fclose(data);
    free(afr);
    free(r);
    free(ir);
    return 0;
}

// Function which adds a car to a list as well as sorts it depending on which list it is (as given by the value of int type)
Car * addToList(Car * list, Car * car, int type) // type = 0 -> car list, type = 1 -> rent list
{
    int returnDate = car->returnDate;
    Car * tempPrevious = NULL;
    Car * temp = list;
    if (temp == NULL)
    {
        if (type == 0 || type == 2)
        {
            return car;
        }
        else
        {
            Car * tempTwo;
            tempTwo = createList(car->plateNum, car->mileage);
            if (car->returnDate == 0) //if it's not 0 that means it's the availible-for-rent list, repair list or a value for the returnDate has already been given
            {
                returnDate = verifyDate();
            }
            tempTwo->returnDate = returnDate;
            return tempTwo;
        }
    }
    else
    {
        if (type == 1 && returnDate == 0) //type == 1 corresponds to rent list
        {
            returnDate = verifyDate();
        }

        while (temp != NULL)
        {
            if (type == 0)
            {
                if (car->mileage <= temp->mileage)
                {
                    if (tempPrevious != NULL) //if its not the first element
                    {
                        car->next = temp;
                        tempPrevious->next = car;
                        return list;
                    }
                    else
                    {
                        car->next = list; //if it is first element, just get rid of first node
                        return car;
                    }
                }
            }

            else if (type == 1)
            {
                if (compareDates(returnDate, temp->returnDate) == 1) //if returnDate < temp->returnDate
                {
                    Car * tempTwo;
                    if (tempPrevious == NULL)
                    {
                        tempTwo = createList(car->plateNum, car->mileage);
                        tempTwo->next = list;
                        tempTwo->returnDate = returnDate;
                        return tempTwo;
                    }
                    else
                    {
                        tempTwo = createList(car->plateNum, car->mileage);
                        tempTwo->next = temp;
                        tempPrevious->next = tempTwo;
                        tempTwo->returnDate = returnDate;
                        return list;
                    }
                }
            }
            tempPrevious = temp;
            temp = temp->next;
        }

        if (type == 0) //these are the cases for adding nodes to the back
        {
            tempPrevious->next = car;
            return list;
        }
        else
        {
            Car * tempTwo;
            tempTwo = createList(car->plateNum, car->mileage);
            tempTwo->returnDate = returnDate;
            tempPrevious->next = tempTwo;
            return list;
        }
    }
}

// Function for (2), (3), and (4) to get rid of the car that's being transferred
Car * carTransfer(Car * list, char * plateNum)
{
    Car * temp = list;
    Car * tempPrevious = NULL;

    while (temp != NULL)
    {
         if (strcmp(temp->plateNum, plateNum) == 0) // if they are equal
         {
             if (tempPrevious == NULL)
             {
                 temp = temp->next;
                 return temp;
             }
             else
             {
                 tempPrevious->next = temp->next; //gets rid of the node that had the same plate number (as we are transferring it)
                 return list;
             }
         }
         tempPrevious = temp;
         temp = temp->next;
    }
    return list;
}

// Standard function to create a linked list
Car * createList(char * plateNum, int mileage)
{
    Car * car = malloc(sizeof(Car));
    car->plateNum = malloc(sizeof(char)*7);
    strcpy(car->plateNum, plateNum);
    car->mileage = mileage;
    car->next = NULL;
    return car;
}

// Parses through data text file and retrieves information to store in the respective list
Car * restoreList(Car * list, char * dataText, int parseList)
{
    char * item;
    char plateNum[7];
    int mileage = 0;
    int returnDate = 0;

    if (dataText[0] == '\n') //ignore blank lines
    {
        return list;
    }
    item = strtok(dataText, " ");
    strcpy(plateNum, item);
    item = strtok(NULL, " ");
    mileage = atoi(item);
    if (parseList == 1) //only for the rent list do I need return date values
    {
        item = strtok(NULL, " ");
        returnDate = atoi(item);
    }

    Car * car = createList(plateNum, mileage);
    car->returnDate = returnDate;
    list = addToList(list, car, parseList);
    return list;
}

// Verifies user's input for the plate number (like a standard ON license plate: 4 capital letters and 3 digits)
char * verifyPlate()
{
    char * plateNum;
    int numLetters = 0;
    int numDigits = 0;
    int capitalLetter = 0;
    plateNum = malloc(sizeof(char) * 1000);
    if (plateNum == NULL)
    {
        printf("Memory allocation fail.\n");
        exit(0);
    }

    while (!(numLetters == 4 && numDigits == 3 && strlen(plateNum) == 7 && capitalLetter == 1))
    {
        numDigits = 0;
        numLetters = 0;
        printf("Enter a plate number: \n");
        scanf("%s", plateNum);
        capitalLetter = 1;
        for (int i = 0; i < strlen(plateNum); i++)
        {
            if (isdigit(plateNum[i]))
            {
                numDigits++;
            }
            else if (isalpha(plateNum[i]))
            {
                if (islower(plateNum[i]))
                {
                    capitalLetter = 0;
                }
                numLetters++;
            }
        }
        if (numLetters != 4 || numDigits != 3 || strlen(plateNum) != 7 || capitalLetter != 1)
        {
            printf("Error: Plate number must be composed of 4 capital letters and 3 digits.\n");
        }
    }
    return plateNum;
}

// Calculates costs from cars rented
double mileageCosts(int mileage)
{
    double cost = 0.0;
    double additionalCost = 0.0;
    if (mileage <= 100)
    {
        cost = 40.0;
    }
    else
    {
        additionalCost = (mileage-100.0)*0.15;
        cost = 40.0 + additionalCost;
    }
    return cost;
}

// Compares return dates with each other in order to determine which is earlier
int compareDates(int dateOne, int dateTwo)
{
    char firstDate[8] = "";
    char secondDate[8] = "";
    sprintf(firstDate, "%06d", dateOne); //%06d so that leading zero(es) remain (pads the strings with 0s if the previous leading zeroes got removed)
    sprintf(secondDate, "%06d", dateTwo);

    for(int i = 0; i < 8; i++)
    {
        if (firstDate[i] - '0' < secondDate[i] - '0')
        {
            return 1;
        }
        else if (firstDate[i] == secondDate[i])
        {
            continue;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

// Checks for duplicate plate numbers (so that there are no repeats when adding new car / making sure it exists when transferring a car)
int checkDuplicatePlate(Car * list, Car * listTwo, Car * listThree, char * plateNum, int option)
{
    Car * temp = list;
    Car * tempTwo = listTwo;
    Car * tempThree = listThree;

    while (tempTwo != NULL)
    {
        if (strcmp(tempTwo->plateNum, plateNum) == 0)
        {
            if (option == 0)
            {
                printf("Error: A car already exists with that plate number. Please try again.\n");
            }

            return 0;
         }
        tempTwo = tempTwo->next;
    }

    if (option == 1) //this is for the return transactions, want to confirm that plate number is in x list before transferring
    {
        printf("Error: No car exists with that plate number, unable to transfer. Please try again.\n");
        return 1;
    }

    if (option == 2)
    {
        printf("Error: No car exists with that plate number, unable to transfer. Please try again.\n");
        return 1;
    }

    while (tempThree != NULL)
    {
        if (strcmp(tempThree->plateNum, plateNum) == 0)
        {
            if (option == 0)
            {
            printf("Error: A car already exists with that plate number. Please try again.\n");
            }
            return 0;
        }
        tempThree = tempThree->next;
    }
    if (option == 3)
    {
        printf("Error: No car exists with that plate number, unable to transfer. Please try again.\n");
        return 1;
    }


    while (temp != NULL)
    {
        if (strcmp(temp->plateNum, plateNum) == 0)
        {
            printf("Error: A car already exists with that plate number. Please try again.\n");
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

// Finds the mileage of the car we searched for via plate number
int findMileage(Car * list, char * plateNum)
{
    Car * temp = list;

    while (temp != NULL)
    {
         if (strcmp(temp->plateNum, plateNum) == 0) // if they are equal
         {
             return temp->mileage;
         }
         temp = temp->next;
    }
    return 0;
}

// Verifies user's input for choosing a program function (ex. add car, print lists...etc.)
int verifyCommand(char * command)
{
    int input = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (!isdigit(command[i]))
        {
                printf("Error: can only enter one of the following digits: '1', '2', '3', '4', '5', '6', '7'. Please try again:\n");
                return 0;
        }
        else
        {
            input = atoi(command);
            if (command[0] == '0' || input > 7)
            {
                printf("Error: can only enter one of the following digits: '1', '2', '3', '4', '5', '6', '7'. Please try again:\n");
                return 0;
            }
        }
    }
    return 1;
}

// Verifies user's input for the return date (most consist of 6 digits)
int verifyDate()
{
    char * date;
    int error = 0;
    int convertDate = 0;
    date = malloc(sizeof(char) * 1000);
    if (date == NULL)
    {
        printf("Memory allocation fail.\n");
        exit(0);
    }

    while (error == 0)
    {
        printf("Please enter the expected return date (yymmdd): \n");
        scanf("%s", date);

        error = 1;
        for (int i = 0; i < strlen(date); i++)
        {
            if (!isdigit(date[i]) || strlen(date) != 6)
            {
                error = 0;
            }
        }
        if (error == 0)
        {
            printf("Error: Date consists of only 6 digits (yymmdd). Please try again.\n");
        }
    }
    convertDate = atoi(date);
    free(date);
    return convertDate;
}

// Verifies user's input for mileage (no leading zeroes, only digits...)
int verifyMileage()
{
    char * mileage;
    int error = 0;
    int convertMileage = 0;
    mileage = malloc(sizeof(char) * 1000);
    if (mileage == NULL)
    {
        printf("Memory allocation fail.\n");
        exit(0);
    }

    while (error == 0)
    {
        printf("Please enter the car's mileage: \n");
        scanf("%s", mileage);

        error = 1;

        if (mileage[0] == '0' && strlen(mileage) != 1)
        {
            printf("Error: No leading zeroes. Please try again.\n");
            error = 0;
            continue;
        }

        for (int i = 0; i < strlen(mileage); i++)
        {
            if (!isdigit(mileage[i]))
            {
                error = 0;
            }
        }
        if (error == 0)
        {
            printf("Error: Can only enter digits for car's mileage. Please try again.\n");
        }
    }
    convertMileage = atoi(mileage);
    free(mileage);
    return convertMileage;
}

// Function prints each car's plate number, mileage and (if a rented car) its return date in a given list
void printList(Car * list)
{
    Car * temp = list;

    if (temp == NULL)
    {
        printf("No cars in this list\n");
    }

    while (temp != NULL)
    {
        printf("Plate Number = %s, Mileage = %d", temp->plateNum, temp->mileage);
        if (temp->returnDate != 0)
        {
            printf(", Expected Return Date = %06d\n", temp->returnDate);
        }
        else
        {
            printf("\n");
        }
        temp = temp->next;
    }
    return;
}

// This function saves each list by storing it into a text file on the computer
// Stored in a way so that my parsing is able to retrieve the info next time
void saveLists(FILE* data, char * path, Car * list, Car * listTwo, Car * listThree)
{
    Car * temp;
    data = fopen(path, "w");

    temp = list;
    fputs("availibleList\n", data);
    while (temp != NULL)
    {
        fprintf(data, "%s %d\n", temp->plateNum, temp->mileage);
        temp = temp->next;
    }

    temp = listTwo;
    fputs("rentList\n", data);
    while (temp != NULL)
    {
        fprintf(data, "%s %d %d\n", temp->plateNum, temp->mileage, temp->returnDate);
        temp = temp->next;
    }

    temp = listThree;
    fputs("repairList\n", data);
    while (temp != NULL)
    {
        fprintf(data, "%s %d\n", temp->plateNum, temp->mileage);
        temp = temp->next;
    }
    return;
}
