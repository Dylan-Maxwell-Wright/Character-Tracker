// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <time.h>
#include <ostream>
#include <istream>
#include <fstream>
#include <vector>

using std::string;
using std::ifstream;
using std::ofstream;
using std::fstream;



//Global Functions
void selectionSort(int targetArray[], int length)
{
	//Looping itterators and temp variable for swapping
	int i, j, temp, min;
	for (i = 0; i < length - 1; i++)
	{
		min = i;
		for (j = i + 1; j < length - 1; j++)
		{
			if (targetArray[j] < targetArray[min])
			{
				min = j;
			}
			temp = targetArray[i];
			targetArray[i] = targetArray[min];
			targetArray[min] = temp;
		}
	}
}

//Auto stat roller, chooses best 3 of 4 die rolls
int statRoller()
{
	int singleRoll;
	int allRolls[4];
	int counter = 0;
	int score = 0;

	while (counter < 4)
	{
		singleRoll = (rand() % 6) + 1; //Rolls a d20
		allRolls[counter] = singleRoll;
		++counter;
	}
	selectionSort(allRolls, 4);
	for (int i = 0; i < 3; i++)
	{
		score += allRolls[i];
	}
	return score;
}

//for reading input commands
string inputToLowercase(string enteredString)
{
	string finalString, stringReader;
	for (char c : enteredString)
	{
		stringReader = tolower(c);
		finalString.append(stringReader);
	}

	return finalString;
}

std::unordered_map<int, string> Stats = { {0, "Strength"}, {1, "Dexterity"}, {2, "Constitution"}, {3, "Intellegence"}, {4, "Wisdom"}, {5, "Charisma"} };


//Global line reading variables
string inString1, inString2, command, tempString;
char lineReader[50];
char* lineReaderPtr = &lineReader[0];
bool inputError, exitCommand, backCommand, characterLoaded;


class CharacterTracker
{
private:

	string firstName, lastName, fullName;
	string fileName;
	int Strength = 10, strModifier = 0;
	int Dexterity = 10, dexModifier = 0;
	int Constitution = 10, conModifier = 0;
	int Intellegence = 10, intModifier = 0;
	int Wisdom = 10, wisModifier = 0;
	int Charisma = 10, chaModifier = 0;
	int Platinum = 0, Gold = 15, Silver = 0, Bronze = 0;
	int armorClass = 0;

	bool inventoryEmpty = true;
	std::vector <string> items;

public:

	//Constructors
	CharacterTracker()
	{

	}
	
	//Pointers
	string* name1Ptr = &firstName;
	string* name2Ptr = &lastName;
	string* fullNamePtr = &fullName;

	//Accessors
	string getFirstName()
	{
		return *name1Ptr;
	}
	string getFullName()
	{
		return *fullNamePtr;
	}
	string getFileName()
	{
		return fileName;
	}

	void setName(string name1, string name2) //set first and last name
	{
		firstName = name1;
		lastName = name2;
		fullName = name1 + " " + name2;
		tempString = fullName + ".txt";
		fileName = tempString.c_str();
	}
	void setName(string name1) //Set first name only
	{
		firstName = name1;
		lastName = "";
		fullName = name1;
		tempString = fullName + ".txt";
		fileName = tempString.c_str();
	}

	void changeName()
	{
		string nameHolder = getFullName();

		do { //Create a character object, initialize with the name constructor
			inputError = false;
			std::cout << "Please enter " << nameHolder << "'s new full name: ";
			std::cin.getline(lineReader, sizeof(lineReader), '\n');

			if (std::cin.fail()) //error occured in first name input, likely too many characters
			{
				inputError = true;
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				std::cout << "Something went wrong, please keep the name below 50 characters." << std::endl;
			}

			else
			{
				//reset global variables for appending
				inString1 = "";
				inString2 = "";
				//Seperate first name from full name
				for (char* cPtr = &lineReader[0]; *cPtr != '\0'; cPtr++)
				{
					if (*cPtr == ' ') //Check for last name
					{
						cPtr++; //Discard the space to not add it to the front of the last name
						while (*cPtr != '\0') //Seperate String for Seperate Constructor
						{
							inString2 += *cPtr;
							cPtr++;
						}
						break;
					}

					else
					{
						inString1 += *cPtr;
					}
				}

				if (inString2.empty()) //no last name was provided, character created with first name
				{
					this->setName(inString1);
					std::cout << nameHolder << "'s name has been changed to " << this->getFullName() << "." << std::endl;
					inputError = false;
				}

				else  //First and Last Name success
				{
					this->setName(inString1, inString2);
					std::cout << nameHolder << "'s name has been changed to " << this->getFullName() << "." << std::endl;
					inputError = false;
				}
			}

		} while (inputError == true);
	}

	void setAC(int stat)
	{
		armorClass = stat;
	}
	int getStat(string stat) //Automatically tests user input for stat names 
	{
		string abbrv, temp;

		for (char c : stat)
		{
			temp = tolower(c);
			abbrv.append(temp);
		}

		abbrv = abbrv.substr(0, 3);

		if (abbrv == "str")
		{
			std::cout << "Strength: " << Strength << " (" << std::showpos << strModifier << ")" << std::noshowpos << std::endl;
		}

		else if (abbrv == "dex")
		{
			std::cout << "Dexterity: " << Dexterity << " (" << std::showpos << dexModifier << ")" << std::noshowpos << std::endl;
		}

		else if (abbrv == "con")
		{
			std::cout << "Constitution: " << Constitution << " (" << std::showpos << conModifier << ")" << std::noshowpos << std::endl;
		}

		else if (abbrv == "int")
		{
			std::cout << "Intellegence: " << Intellegence << " (" << std::showpos << intModifier << ")" << std::noshowpos << std::endl;
		}

		else if (abbrv == "wis")
		{
			std::cout << "Wisdom: " << Wisdom << " (" << std::showpos << wisModifier << ")" << std::noshowpos << std::endl;
		}

		else if (abbrv == "cha")
		{
			std::cout << "Charisma: " << Charisma << " (" << std::showpos << chaModifier << ")" << std::noshowpos << std::endl;
		}
		
		else if (abbrv == "ac" || abbrv == "arm") //For Armor Class
		{
			std::cout << "Armor Class: " << armorClass << std::endl;
		}

		else
		{
			std::cout << "Invalid stat, please ask for Strength, Dexterity, Constitution, Intellegence, Wisdom, or Charisma." << std::endl;
			return 0;
		}
		return 1;
	}
	int setStat(string stat, int value) //Automatically tests user input for stat names 
	{
		string abbrv, temp;

		for (char c : stat)
		{
			temp = tolower(c);
			abbrv.append(temp);
		}

		abbrv = abbrv.substr(0, 3);

		//Stat values can only be between 0 and 35
		if (value < 0)
		{
			std::cout << "Please enter a positive value" << std::endl;
			return 0;
		}
		else if (value > 35)
		{
			std::cout << "Error, the maximum of any stat is 35" << std::endl;
			return 0;
		}

		else
		{
			if (abbrv == "str")
			{
				Strength = value;
				strModifier = (Strength / 2) - 5;
			}

			else if (abbrv == "dex")
			{
				Dexterity = value;
				dexModifier = (Dexterity / 2) - 5;
			}

			else if (abbrv == "con")
			{
				Constitution = value;
				conModifier = (Constitution / 2) - 5;
			}

			else if (abbrv == "int")
			{
				Intellegence = value;
				intModifier = (Intellegence / 2) - 5;
			}

			else if (abbrv == "wis")
			{
				Wisdom = value;
				wisModifier = (Wisdom / 2) - 5;
			}

			else if (abbrv == "cha")
			{
				Charisma = value;
				chaModifier = (Charisma / 2) - 5;
			}

			else if (abbrv == "arm" || abbrv == "ac")
			{
				armorClass = value;
			}

			else
			{
				std::cout << "Invalid stat, please ask for Strength, Dexterity, Constitution, Intellegence, Wisdom, or Charisma." << std::endl;
				return 0;
			}
			return 1;
		}
	}
	int changeStat(string stat, int value) //Automatically tests user input for stat names 
	{
		string abbrv, temp;
		int boundTester = 0;

		for (char c : stat)
		{
			temp = tolower(c);
			abbrv.append(temp);
		}

		abbrv = abbrv.substr(0, 3);

		if (abbrv == "str")
		{
			boundTester = Strength + value;
			if (boundTester < 0)
			{
				std::cout << "Error, this would make your strength less than 0. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else if (boundTester > 35)
			{
				std::cout << "Error, this would make your strength greater than 35. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else
			{
				Strength += value;
				strModifier = (Strength / 2) - 5;
				return 1;
			}
		}

		else if (abbrv == "dex")
		{
			boundTester = Dexterity + value;
			if (boundTester < 0)
			{
				std::cout << "Error, this would make your dexterity less than 0. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else if (boundTester > 35)
			{
				std::cout << "Error, this would make your dexterity greater than 35. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else
			{
				Dexterity += value;
				dexModifier = (Dexterity / 2) - 5;
				return 1;
			}
		}

		else if (abbrv == "con")
		{
			boundTester = Constitution + value;
			if (boundTester < 0)
			{
				std::cout << "Error, this would make your constitution less than 0. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else if (boundTester > 35)
			{
				std::cout << "Error, this would make your constitution greater than 35. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else
			{
				Constitution += value;
				conModifier = (Constitution / 2) - 5;
				return 1;
			}
		}

		else if (abbrv == "int")
		{
			boundTester = Intellegence + value;
			if (boundTester < 0)
			{
				std::cout << "Error, this would make your intellegence less than 0. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else if (boundTester > 35)
			{
				std::cout << "Error, this would make your intellegence greater than 35. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else
			{
				Intellegence += value;
				intModifier = (Intellegence / 2) - 5;
				return 1;
			}
		}

		else if (abbrv == "wis")
		{
			boundTester = Wisdom + value;
			if (boundTester < 0)
			{
				std::cout << "Error, this would make your wisdom less than 0. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else if (boundTester > 35)
			{
				std::cout << "Error, this would make your wisdom greater than 35. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else
			{
				Wisdom += value;
				wisModifier = (Wisdom / 2) - 5;
				return 1;
			}
		}

		else if (abbrv == "cha")
		{
			boundTester = Charisma + value;
			if (boundTester < 0)
			{
				std::cout << "Error, this would make your charisma less than 0. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else if (boundTester > 35)
			{
				std::cout << "Error, this would make your charisma greater than 35. Please enter a modifier which keeps your strength stat between 0 and 35." << std::endl;
				return 0;
			}
			else
			{
				Charisma += value;
				chaModifier = (Charisma / 2) - 5;
				return 1;
			}
		}

		else if (abbrv == "ac" || "arm")
		{
			armorClass += value;
			return 1;
		}

		else
		{
			std::cout << "Invalid stat, please ask for Strength, Dexterity, Constitution, Intellegence, Wisdom, or Charisma." << std::endl;
			return 0;
		}
	}
	int changeMoney()
	{
		int tempPP, tempGP, tempSP, tempBP;
		try
		{
			std::cout << "Enter your character's new number of platinum pieces: ";
			std::cin.getline(lineReader, sizeof(lineReader));
			command = inputToLowercase(lineReader);
			tempPP = std::stoi(command);

			std::cout << "Enter your character's new number of gold pieces: ";
			std::cin.getline(lineReader, sizeof(lineReader));
			command = inputToLowercase(lineReader);
			tempGP = std::stoi(command);

			std::cout << "Enter your character's new number of silver pieces: ";
			std::cin.getline(lineReader, sizeof(lineReader));
			command = inputToLowercase(lineReader);
			tempSP = std::stoi(command);

			std::cout << "Enter your character's new number of bronze pieces: ";
			std::cin.getline(lineReader, sizeof(lineReader));
			command = inputToLowercase(lineReader);
			tempBP = std::stoi(command);

			//store temporarily in case of error, so all must be updated, or none.
			Platinum = tempPP;
			Gold = tempGP;
			Silver = tempSP;
			Bronze = tempBP;

			save();
			printMoney();
			return 0;
		}
		catch (std::invalid_argument)
		{
			std::cout << "An error occured, please only enter integer values. \n";
			return 1;
		}
	}
	int addItem()
	{
		do {
			inputError = false;
			std::cout << "Enter the name of the item you would wish to add, or type \"cancel\" to cancel: ";
			std::cin.getline(lineReader, sizeof(lineReader));
			command = inputToLowercase(lineReader); //for cancel command only, to remain case sensitive
			inString1 = lineReader;

			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				std::cout << "Error, please keep your input below 50 characters. \n";
				inputError = true;
			}
			else if (command == "cancel")
			{
				return 0;
			}
			else
			{
				items.push_back(inString1);
				save();
				inventoryEmpty = false;
				printItems();
				return 1;
			}

		} while (!inputError);
	}
	int removeItem()
	{
		if (inventoryEmpty)
		{
			std::cout << "No items in inventory." << std::endl;
			return 0;
		}
		else
		{			
			printItems();
			bool itemFound = false;
			int itemFinder = 0;
			string itemIdentifier;
			
			do {
				inputError = false;
				itemFound = false;
				std::cout << "Enter the name of the item you would wish to remove, or type \"cancel\" to cancel: ";
				std::cin.getline(lineReader, sizeof(lineReader));
				command = inputToLowercase(lineReader);
				inString1 = lineReader;

				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(32767, '\n');
					std::cout << "Error, please keep your input below 50 characters. \n";
					inputError = true;
				}
				else if (command == "cancel")
				{
					break;
				}
				else 
				{					
					for (string item : items)
					{						
						itemIdentifier = item;
						if (inString1 == itemIdentifier)
						{
							items.erase(items.begin() + itemFinder);
							std::cout << itemIdentifier << " removed. ";
							save();
							if (items.empty())
							{
								inventoryEmpty = true;
							}
							printItems();
							itemFound = true;
							break;
						}
						itemFinder++;
					}
					if (!itemFound)
					{
						std::cout << "That item was not found in your inventory. Items are case sensitive. \n";
						inputError = true;						
					}
				}

			} while (!inputError);
			return 1;
		}
	}

	void save()
	{
		ofstream fileSaver;
		fileSaver.open(fileName);

		string fileWriter = "";

		//Write name
		fileWriter = fileName + '\n';
		fileSaver.write(fileWriter.c_str(), fileWriter.size());

		//Write stats, csv
		fileWriter = std::to_string(Strength) + ',' + std::to_string(Dexterity) + ',' + std::to_string(Constitution) + ',' + std::to_string(Intellegence) + ',' + std::to_string(Wisdom) + ',' + std::to_string(Charisma) + ',' + std::to_string(armorClass) + '\n';
		fileSaver.write(fileWriter.c_str(), fileWriter.size());

		//Write wallet, csv
		fileWriter = "";
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				fileWriter += std::to_string(Platinum);
				fileWriter += ',';
				break;
			case 1:
				fileWriter += std::to_string(Gold);
				fileWriter += ',';
				break;
			case 2:
				fileWriter += std::to_string(Silver);
				fileWriter += ',';
				break;
			case 3:
				fileWriter += std::to_string(Bronze);
				fileWriter += '\n';
				break;
			default:
				break;
			}
		}
		fileSaver.write(fileWriter.c_str(), fileWriter.size());

		//Write items, csv
		fileWriter = "";
		if (items.empty() == false)
		{
			for (string item : items)
			{
				fileWriter += item + ',';
			}

			//Replace last comma with a newline character
			fileWriter.erase(fileWriter.end());
			fileWriter += '\n';
			fileSaver.write(fileWriter.c_str(), fileWriter.size());
		}		

		fileSaver.close();
	}
	void load()
	{
		string statValue, moneyValue, itemName;
		int moneyInt;
		ifstream fileLoader;

		fileName = fullName + ".txt";
		fileLoader.open(fileName);

		string fileReader;

		//File Name
		std::getline(fileLoader, fileReader);

		//Stat reader
		for (int i = 0; i < 6; i++)
		{
			std::getline(fileLoader, fileReader, ',');
			setStat(Stats[i], std::stoi(fileReader));
		}
		//last number is ac, followed by newline char
		std::getline(fileLoader, fileReader, '\n');
		armorClass = std::stoi(fileReader);

		//Wallet reader
		for (int i = 0; i < 3; i++)
		{
			std::getline(fileLoader, fileReader, ',');
			moneyValue = fileReader;
			moneyInt = std::stoi(moneyValue);
			
			switch (i)
			{
			case 0:
				Platinum = moneyInt;
			case 1:
				Gold = moneyInt;
			case 2:
				Silver = moneyInt;
			default:
				break;
			}
		}
		//Bronze pieces are followed by newline character
		std::getline(fileLoader, fileReader, '\n');
		Bronze = std::stoi(fileReader);

		//Item reader
		while (std::getline(fileLoader, fileReader, ','))
		{
			items.push_back(fileReader);
		}

		if (items.empty())
		{
			inventoryEmpty = true;
		}
		else
		{
			inventoryEmpty = false;
		}
	}

	void printMoney()
	{
		std::cout << firstName << " is currently carrying " << Platinum << "pp, " << Gold << "gp, " << Silver << "sp, and " << Bronze << "bp." << std::endl;
	}
	void printStats()
	{
		int tempStat, tempMod;
		std::cout << firstName << "'s stats are as follows: " << std::endl;
		for (int i = 0; i < 6; i++)
		{
			switch (i)
			{
			case 0:
				tempStat = Strength;
				tempMod = strModifier;
				break;
			case 1:
				tempStat = Dexterity;
				tempMod = dexModifier;
				std::cout << ", ";
				break;
			case 2:
				tempStat = Constitution;
				tempMod = conModifier;
				std::cout << ", ";
				break;
			case 3:
				tempStat = Intellegence;
				tempMod = intModifier;
				std::cout << ", ";
				break;
			case 4:
				tempStat = Wisdom;
				tempMod = wisModifier;
				std::cout << ", ";
				break;
			case 5:
				tempStat = Charisma;
				tempMod = chaModifier; 
				std::wcout << ", ";
				break;
			default:
				break;
			}
			std::cout << Stats[i] << ": " << tempStat << " (" << std::showpos << strModifier << ")" << std::noshowpos;
		}
		std::cout << ", " << "Armor Class: " << armorClass << ". \n";
	}
	
	void printItems()
	{
		if (inventoryEmpty)
		{
			std::cout << "No items in inventory." << std::endl;
		}
		else
		{
			int itemCounter = 1;
			std::cout << firstName << " is carrying the following items: " << std::endl;
			for (string item : items)
			{
				if (itemCounter == items.size())
				{
					std::cout << item << ". " << std::endl;
				}
				else //last item ends with a period, not a comma
				{
					std::cout << item << ", ";
				}
				itemCounter++;
			}
		}
	}
}; 

int main()
{
	srand(time(NULL));
	CharacterTracker character;

	std::cout << "Hello, please enter your character's full name: ";

	do { //Script for setting name
		inputError = false;
		std::cin.getline(lineReader, sizeof(lineReader), '\n');

		if (std::cin.fail()) //error, lenth exceeded
		{
			inputError = true;
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "Something went wrong, please keep the name below 50 characters." << std::endl;
		}

		else
		{
			//Seperate first name from full name
			for (char* cPtr = &lineReader[0]; *cPtr != '\0'; cPtr++)
			{
				if (*cPtr == ' ') //Check for last name
				{
					cPtr++; //Discard the space
					while (*cPtr != '\0') //Seperate String for overloaded naming function
					{
						inString2 += *cPtr;
						cPtr++;
					}
					break;
				}

				else
				{
					inString1 += *cPtr;
				}
			}

			if (inString2.empty()) //no last name was provided, character created with first name
			{
				character.setName(inString1);
				inputError = false;
			}
			else  //First and Last Name success
			{
				character.setName(inString1, inString2);
				inputError = false;
			}

			ifstream file;
			tempString = character.getFileName();
			file.open(tempString);

			if (file.is_open())
			{
				file.close();
				character.load();
				std::cout << "Welcome back " << character.getFirstName() << ". Your character's profile has been loaded." << std::endl;
				characterLoaded = true;
			}
			else
			{
				std::cout << character.getFullName() << " has been created." << std::endl;
				file.close();
				characterLoaded = false;
			}

		}

	} while (inputError);

	bool nextStep = false;

	if (characterLoaded == false) //If character is new, begin setting stats
	{
		do
		{
			inputError = false;
			std::cout << "The next step is to enter or generate " << character.getFirstName() << "'s stats. Type \"roll\" to begin entering " << character.getFirstName() <<
				"'s core stats. For a list of commands, type \"help\": ";
			std::cin.getline(lineReader, sizeof(lineReader));
			command = inputToLowercase(lineReader);

			if (std::cin.fail())
			{
				std::cout << "Please keep the command below 50 characters" << std::endl;
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				inputError = true;
			}
			else if (command == "help") 
			{
				std::cout << "The commands currently available are: \n" <<
					"roll stats   ---- Begin rolling your character's full stats. once this is complete, more commands will become available." << std::endl <<
					"rename       ---- Rename your character." << std::endl <<
					"exit         ---- Quit the program. Your character will not be saved since stats have not been rolled." << std::endl <<
					"help         ---- Bring up this prompt to display possible commands." << std::endl << std::endl;
				nextStep = false;
			}
			else if (command == "roll stats" || command == "roll")
			{

				int inputStat, doCounter = 0;
				bool armorSet = false;


				do {
					inputStat = 0;
					std::cout << "Please enter your " << Stats[doCounter] << ", or type \"100\" to auto roll for this stat: ";
					std::cin >> inputStat;



					if (std::cin.fail()) //Failure, first input was not an integer
					{
						std::cin.clear();
						std::cin.ignore(32767, '\n');
						std::cout << "Error, please enter a valid integer between 0 and 35, or 100 to auto roll.\n";

					}

					else if (inputStat == 100) 
					{
						character.setStat(Stats[doCounter], statRoller());
						character.getStat(Stats[doCounter]);
						doCounter++;
					}

					else if (inputStat < 0 || inputStat > 35)
					{
						std::cout << "Error, please enter a valid integer between 0 and 35, or 100 to auto roll.\n";
					}

					else 
					{
						character.setStat(Stats[doCounter], inputStat);
						character.getStat(Stats[doCounter]);
						doCounter++;
					}
					std::cin.ignore(32767, '\n');

				} while (doCounter < 6);

				do
				{
					std::cout << "Please enter your armor class: ";
					std::cin.getline(lineReader, sizeof(lineReader));

					try {
						inputStat = std::stoi(lineReader);
						character.setAC(inputStat);
						armorSet = true;
					}
					catch (std::invalid_argument)
					{
						armorSet = false;
						std::cout << "You must enter a valid integer." << std::endl;
					}

				} while (!armorSet);

				nextStep = true;
				character.save();
				std::cout << character.getFirstName() << "'s stats have been updated and saved." << std::endl;
			}
			else if (command == "rename")
			{
				character.changeName();
				nextStep = false;
				character.save();
			}

		} while (inputError || !nextStep);
	}

	do {
		exitCommand = false;
		inputError = false;

		if (characterLoaded) //If character is loaded, a different prompt will greet the user. Once this prompt is used, the characterLoaded variable is no longer needed
		{
			std::cout << "You may now enter commands to navigate your profile, or type \"help\" for a list of commands: ";
			characterLoaded = false;
		}
		else
		{
			std::cout << "You may now enter another command, or \"help\" for a list of commands: ";
		}
		std::cin.getline(lineReader, sizeof(lineReader));
		command = inputToLowercase(lineReader);

		if (std::cin.fail()) 
		{
			std::cout << "Error, please keep your input below 50 characters." << std::endl;
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			inputError = true;
		}
		else if (command == "quit" || command == "exit")
		{
			character.save();
			exitCommand = true;
			std::cout << "Goodbye, " << character.getFullName() << ".";
		}
		else if (command == "help")
		{
			std::cout << "The commands currently available are: \n" <<
				"change stats ---- Change one or all of your character's core stats, including armor class." << std::endl <<
				"view stats   ---- View one or all of your character's stats." << std::endl <<
				"rename       ---- Rename your character." << std::endl <<
				"inventory    ---- View or edit your characters's inventory." << std::endl <<
				"quit or exit ---- Save your character and exit the program." << std::endl <<
				"help         ---- Bring up this prompt to display possible commands." << std::endl << std::endl;

		}
		else if (command == "rename")
		{
			character.changeName();
		}
		else if (command == "inventory")
		{
			bool goAgain = false;
			bool secondCycle = false;
			do {

				if (secondCycle == false)
				{
					std::cout << "Enter \"view\" to view the items in your inventory, \"add\" to add an item, \"remove\" to remove an item (due to selling or using), or \"wallet\" to edit your money: ";
					std::cin.getline(lineReader, sizeof(lineReader));
				}
				else
				{					
					std::cout << "Enter \"view\" to view the items in your inventory, \"remove\" to remove an item (due to selling or using), \"wallet\" to view or edit your money, or \"back\" to choose a different command: ";
					std::cin.getline(lineReader, sizeof(lineReader));
				}

				command = inputToLowercase(lineReader);

				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(32767, '\n');
					std::cout << "Error, please keep your input below 50 characters. \n";
					secondCycle = true;
					goAgain = true;
				}
				else if (command == "exit" || command == "quit")
				{
					exitCommand = true;
				}
				else if (command == "back")
				{
					break;
				}
				else if (command == "view")
				{
					character.printItems();
					secondCycle = true;
					goAgain = true;
				}
				else if (command == "add")
				{
					character.addItem();
					secondCycle = true;
					goAgain = true;
				}
				else if (command == "remove")
				{
					character.removeItem();
					secondCycle = true;
					goAgain = true;
				}
				else if (command == "wallet")
				{
					inputError = false;
					do 
					{
						inputError = false;
						std::cout << "Enter \"view\" to view your wallet, \"edit\" to view and then edit your wallet contents, or \"back\" to choose a different command: ";
						std::cin.getline(lineReader, sizeof(lineReader));
						command = inputToLowercase(lineReader);

						if (std::cin.fail())
						{
							std::cin.clear();
							std::cin.ignore(32767, '\n');
							std::cout << "Error, please keep your input below 50 characters. \n";
							inputError = true;
						}
						else if (command == "edit")
						{
							character.printMoney();
							int moneyChanger;
							
							do
							{
								moneyChanger = character.changeMoney();
							} while (moneyChanger != 0);
						}
						else if (command == "view")
						{
							character.printMoney();
						}
						else if (command == "back")
						{
							break;
						}
						else if (command == "quit" || command == "exit")
						{
							exitCommand = true;
						}
						else
						{
							inputError = true;
							std::cout << "Invalid command, please try again." << std::endl;
						}


					} while (inputError == false && !exitCommand);					
				}
				else
				{
					std::cout << "Invalid command, please try again." << std::endl;
					secondCycle = true;
					goAgain = true;
				}

			} while (goAgain && !exitCommand);

		}
		else if (command == "change stat" || command == "change stats")
		{
			int successfulChange = 0;
			int inputStat = 0;
			bool secondCycle = false;

			do {
				inputError = false;

				if (!secondCycle)
				{
					std::cout << "This command is for incrementing the stat values from leveling up, or from combat modifiers." << std::endl;
					std::cout << "Please enter the stat you would like to change: ";
				}
				else
				{
					std::cout << "Please enter the stat you would like to change, or \"back\" to choose a different command: ";
				}
				
				std::cin.getline(lineReader, sizeof(lineReader));
				command = inputToLowercase(lineReader);

				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(32767, '\n');
					std::cout << "Error, please keep your entry below 50 characters." << std::endl;
					inputError = true;
				}
				else if (command == "back")
				{
					break;
				}
				else if (command == "exit" || command == "quit")
				{
					exitCommand = true;
					break;
				}
				else 
				{
					successfulChange = character.getStat(command);

					if (successfulChange == 1)
					{
						std::cout << "Enter how much you would like to change the stat: ";
						std::cin >> inputStat;
						std::cin.ignore(32767, '\n');

						if (std::cin.fail())
						{
							std::cin.clear();
							std::cin.ignore(32767, '\n');
							std::cout << "Error, please enter a valid integer.";
							inputError = true;
						}
						else
						{
							successfulChange = character.changeStat(command, inputStat);

						}
					}
					secondCycle = true;

					if (successfulChange)
					{
						character.getStat(command);
						character.save();
					}
				}


			} while (successfulChange == 0 || inputError);
		}
		else if (command == "view stats" || command == "view stat")
		{
			int tryStat = 0;
			do {

				std::cout << "Please name the stat you would like to view, or type \"all\" to view all stats: ";
				std::cin.getline(lineReader, sizeof(lineReader));
				command = inputToLowercase(lineReader);

				if (std::cin.fail())
				{
					inputError = true;
					std::cin.clear();
					std::cin.ignore(32767, '\n');
					std::cout << "Keep your entry below 50 characters." << std::endl;
				}
				else if (command == "exit" || command == "quit")
				{
					exitCommand = true;
					std::cout << "Goodbye, " << character.getFullName() << ".";
				}
				else if (command == "back")
				{
					break;
				}
				else if (command == "all")
				{
					character.printStats();
				}
				else
				{
					tryStat = character.getStat(command);
				}

			} while (!tryStat);
		}
		else 
		{
			inputError = true;
			std::cout << "Invalid command, please try again." << std::endl;
		}

	} while (!exitCommand|| inputError);

	return 0;
}


