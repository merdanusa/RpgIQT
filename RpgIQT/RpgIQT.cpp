#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;

class Item {
public:
    string name;
    int value;
    string type;
};

class Quest {
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
        outFile << "CHARACTER," << character.index << "," << character.name << "," << character.hp << "," << character.level << "," << character.gold << endl;

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

            quest.index = stoi(indexStr);
            quest.reward = stoi(rewardStr);
            quest.completed = (completedStr == "1");
            current->quests.push_back(quest);
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

    character.inventory = {
        {"pickaxe", 0, "tool"},
        {"sword", 10, "weapon"},
        {"scissors", 10, "tool to cut"}
    };

    character.quests = {
        {1, "Mine some silver for a minute", 5, "tool", false},
        {2, "Defeat the dragon", 10, "weapon", false},
        {3, "Shave 10 ships", 10, "tool to cut", false}
    };

    roster.push_back(character);
    saveGame(roster);

    cout << "Character created!" << endl;
}

void displayCharacter(const Character& character) {
    cout << "Name: " << character.name << endl;
    cout << "HP: " << character.hp << endl;
    cout << "Level: " << character.level << endl;
    cout << "Gold: " << character.gold << endl;
    cout << "Inventory:" << endl;
    for (const Item& item : character.inventory) {
        cout << "- " << item.name << " (Value: " << item.value << ", Type: " << item.type << ")" << endl;
    }
    cout << "Quests:" << endl;
    for (const Quest& quest : character.quests) {
        cout << quest.index << ". " << quest.title << " (Reward: " << quest.reward << ", Requirement: " << quest.requiredItemType << ", Completed: "
            << (quest.completed ? "Yes" : "No") << ")" << endl;
    }
}

bool hasRequiredItem(const Character& character, const string& requiredType) {
    for (const Item& item : character.inventory) {
        if (item.type == requiredType) {
            return true;
        }
    }
    return false;
}

void completeQuest(Character& character, int questIndex) {
    Quest* target = nullptr;

    for (Quest& quest : character.quests) {
        if (quest.index == questIndex) {
            target = &quest;
        }
    }

    if (target == nullptr) {
        cout << "No quest with that index." << endl;
        return;
    }

    if (target->completed) {
        cout << "You already completed this quest." << endl;
        return;
    }

    if (!hasRequiredItem(character, target->requiredItemType)) {
        cout << "You need an item of type '" << target->requiredItemType << "' to do this." << endl;
        return;
    }

    if (target->requiredItemType == "tool") {
        cout << "Mining... please wait 3 seconds." << endl;
        this_thread::sleep_for(chrono::seconds(3));
    }
    else if (target->requiredItemType == "tool to cut") {
        cout << "Hold Enter for 3 seconds to shave, then release: ";
        cin.ignore();
        auto start = chrono::steady_clock::now();
        cin.get();
        auto end = chrono::steady_clock::now();

        chrono::duration<double> elapsed = end - start;
        if (elapsed.count() < 3.0) {
            cout << "You let go too early! Quest failed." << endl;
            return;
        }
    }

    target->completed = true;
    character.gold += target->reward;
    cout << "Quest completed! Gained " << target->reward << " gold." << endl;
}

int main() {
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
                    displayCharacter(character);

                    cout << "Type a quest number to complete it, or -1 to go back: ";
                    int questChoice;
                    cin >> questChoice;

                    if (questChoice != -1) {
                        completeQuest(character, questChoice);
                        saveGame(roster);
                    }
                }
            }
            if (!found) {
                cout << "Unknown selection." << endl;
            }
        }
    }
}