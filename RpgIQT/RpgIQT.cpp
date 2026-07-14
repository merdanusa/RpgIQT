#include <iostream>;
#include <vector>;
#include <string>;
#include <fstream>;
#include <sstream>;

using namespace std;

class Item {
public:
    string name;
    int value;
    string type;
};


class Quest{
public:
    string title;
    int reward;
    bool completed;
};

class Character {
public:
    int index;
    string name;
    int hp, level, gold;
    vector<Item> inventory;
    vector<Quest> quests;
};

void saveGame(const vector<Character>& roster) {
    ofstream outFile("save.amq");

    for (const Character& character : roster) {
        outFile << "CHARACTER," << character.index <<character.name << "," << character.hp << "," << character.level << "," << character.gold << endl;

        for (const Item& item : character.inventory) {
            outFile << "ITEM," << item.name << "," << item.value << "," << item.type << endl;
        }

        for (const Quest& quest : character.quests) {
            outFile << "QUEST," << quest.title << "," << quest.reward << "," << quest.completed << endl;
        }
    }

    outFile.close();
}

vector<Character> loadGame() {
    vector<Character> roster;

	ifstream inFile("save.amq");
    string line;
    Character* current = nullptr;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string tag;
        getline(ss, tag, ',');

        if (tag == "CHARACTER") {
            Character character;
            string indexStr, hpStr, levelStr, goldStr;

            getline(ss, indexStr, ',');
            getline(ss, character.name, ',');
            getline(ss, hpStr, ',');
            getline(ss, levelStr, ',');
            getline(ss, goldStr, ',');

            character.index = stoi(indexStr);
            character.hp = stoi(hpStr);
            character.level = stoi(levelStr);
            character.gold = stoi(goldStr);

            roster.push_back(character);
            current = &roster.back();
        }
        else if (tag == "ITEM" && current != nullptr) {
            Item item;
            string valueStr;

            getline(ss, item.name, ',');
            getline(ss, valueStr, ',');
            getline(ss, item.type, ',');

            item.value = stoi(valueStr);
            current->inventory.push_back(item);
        }
        else if (tag == "QUEST" && current != nullptr) {
			Quest quest;
			string rewardStr, completedStr;

            getline(ss, quest.title, ',');
            getline(ss, rewardStr, ',');
            getline(ss, completedStr, ',');

			quest.reward = stoi(rewardStr);
			quest.completed = (completedStr == "1");
			roster.back().quests.push_back(quest);
        }
    }

    return roster;
}

void createCharacter(vector<Character>& roster) {
	Character newCharacter;
	newCharacter.index = roster.size() + 1;
	cout << "Enter character name: ";
	cin >> newCharacter.name;
	cout << "Enter character HP: ";
	cin >> newCharacter.hp;
	cout << "Enter character level: ";
	cin >> newCharacter.level;
	cout << "Enter character gold: ";
	cin >> newCharacter.gold;
	roster.push_back(newCharacter);
}

int main() {
    bool gameOn = true;

    vector<Character> roster = loadGame();

	cout << "Welcome to the RPG Inventory and Quest Tracker!\n" << "Please select a character to view their inventory and quests!" << endl;


    while (gameOn) {
        int index;
      
            for (const Character& character : roster) {

                if (roster.empty()) {
                    cout << "Looks like there's no characters, to create a new one type (0): " << endl;
                    cin >> index;
                    if (index == 0) {
                        createCharacter(roster);
                    }
                }
                cout << character.index << ". " << character.name << endl;
                cout << "To create a new type (0)!" << endl;

                cin >> index;

                if (index == 0) {
                    createCharacter(roster);
                }
            }
       
    }
}