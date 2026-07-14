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
    int index;
    string title;
    int reward;
    string requiredItemType;
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
        outFile << "CHARACTER," << character.index << "," <<character.name << "," << character.hp << "," << character.level << "," << character.gold << endl;

        for (const Item& item : character.inventory) {
            outFile << "ITEM," << item.name << "," << item.value << "," << item.type << endl;
        }

        for (const Quest& quest : character.quests) {
            outFile << "QUEST," << quest.index << "," << quest.title << "," << quest.reward << "," << quest.requiredItemType << "," << quest.completed << endl;
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
			string indexStr, rewardStr, completedStr;

            getline(ss, indexStr, ',');
            getline(ss, quest.title, ',');
            getline(ss, rewardStr, ',');
            getline(ss, quest.requiredItemType, ',');
            getline(ss, completedStr, ',');

			quest.reward = stoi(rewardStr);
			quest.completed = (completedStr == "1");
			roster.back().quests.push_back(quest);
        }
    }

    return roster;
}

void createCharacter(vector<Character>& roster) {
    Character character;
    character.index = static_cast<int>(roster.size()) + 1;

    cout << "Enter name: ";
    cin >> character.name;

    character.hp = 100;
    character.level = 1;
    character.gold = 0;

    roster.push_back(character);
    saveGame(roster);

    cout << "Character created!" << endl;
}

void displayCharacter(const Character& character, const vector<Quest>& quests) {
	cout << "Name: " << character.name << endl;
	cout << "HP: " << character.hp << endl;
	cout << "Level: " << character.level << endl;
	cout << "Gold: " << character.gold << endl;
	cout << "Inventory:" << endl;
	for (const Item& item : character.inventory) {
		cout << "- " << item.name << " (Value: " << item.value << ", Type: " << item.type << ")" << endl;
	}
	cout << "Quests:" << endl;
	for (const Quest& quest : quests) {
		cout << quest.index << quest.title << " (Reward: " << quest.reward << "Requirment: " << quest.requiredItemType << ", Completed: "
			<< (quest.completed ? "Yes" : "No") << ")" << endl;
	}
}

void completeQuest(Character& character, int questIndex) {
    bool found = false;

    for (Quest& quest : character.quests) {
        if (quest.index == questIndex) {
            found = true;
            quest.completed = true;
            character.gold += quest.reward;
            cout << "Quest completed! Gained " << quest.reward << " gold." << endl;
        }
    }

    if (!found) {
        cout << "No quest with that index." << endl;
    }
}

int main() {
    vector<Item> items = {
        {"pickaxe", 0, "tool"},
        {"sword", 10, "weapon"},
        {"axe", 10, "weapon"},
        {"scissors", 10, "tool to cut"},
    };

    vector<Quest> quests = {
        {1, "Mine some silver for a minute", 5, "tool", false},
        {2, "Defeat the dragon", 10, "weapon", false},
        {3, "Shave 10 ships", 10, "tool to cut", false},
    };

    bool gameOn = true;
    vector<Character> roster = loadGame();

    cout << "Welcome to the RPG Inventory and Quest Tracker!" << endl;

    while (gameOn) {
        if (roster.empty()) {
            cout << "No characters yet. Type 0 to create one, or -1 to quit: ";
        }
        else {
            for (const Character& character : roster) {
                cout << character.index << ". " << character.name << endl;
            }
            cout << "Type 0 to create a new character, or -1 to quit: ";
        }

        int input;
        cin >> input;

        if (input == -1) {
            gameOn = false;
        }
        else if (input == 0) {
            createCharacter(roster);
        }
        else {
            bool found = false;
            for (Character& character : roster) {
                if (character.index == input) {
                    found = true;
                    displayCharacter(character, quests);
                }
            }
            if (!found) {
                cout << "Unknown selection." << endl;
            }
        }
    }
}