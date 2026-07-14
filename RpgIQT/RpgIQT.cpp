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
    string name;
    int hp, level, gold;
    vector<Item> inventory;
    vector<Quest> quests;
};

void saveGame(const vector<Character>& roster) {
    ofstream outFile("save.amq");

    for (const Character& character : roster) {
        outFile << "CHARACTER," << character.name << "," << character.hp << "," << character.level << "," << character.gold << endl;

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
            string hpStr, levelStr, goldStr;

            getline(ss, character.name, ',');
            getline(ss, hpStr, ',');
            getline(ss, levelStr, ',');
            getline(ss, goldStr, ',');

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

int main() {

}