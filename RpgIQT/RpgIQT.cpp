#include <iostream>;
#include <vector>;
#include <string>;
#include <fstream>;
#include <sstream>;

using namespace std;

class Item {
    string name;
    int value;
    string type;
};


class Quest{
    string title;
    string description;
    int goldReward;
    bool completed;
};

class Character {
    string name;
    int hp, level, gold;
    vector<Item> inventory;
    vector<string> completedQuests;
};

void saveGame(const vector<Character> roster) {
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

    }
}

int main() {

}