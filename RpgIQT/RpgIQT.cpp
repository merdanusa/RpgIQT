#include <iostream>;
#include <vector>;

using namespace std;

class Item {
    string name;
    int value;
    string type;
};

class Character {
    string name;
    int hp, level, gold;
    vector<Item> inventory;
    vector<string> completedQuests;
};

class Quest {
    string title;
    string description;
    int goldReward;
    bool completed;
};

int main() {

}