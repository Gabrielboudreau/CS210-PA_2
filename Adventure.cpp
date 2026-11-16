#include "Adventure.h"
#include <iostream>



/* Personal Note: A lot of the code in this is based on or sourced from stack overflow implementations of a variety
 * of similar projects. My general method when stuck was to find similar problems with answers, analyze and trace
 * code soloutions and then repurpose them for my work. With that being said some structure may resemble
 * external projects as a result, many of which that I derived inspiration from.
 */
// ===================== GIVEN HELPERS (do not change) =====================

// Reads the next line that is not blank.
// Spaces at the start of the line are removed.
// Returns false when the file has no more lines.
bool Adventure::nextLine(std::ifstream& in, std::string& line) {
    std::string raw;
    while (std::getline(in, raw)) {
        int start = 0;
        while (start < (int)raw.size() && (raw[start] == ' ' || raw[start] == '\t')) {
            start++;
        }
        line = raw.substr(start);
        // Files saved on Windows can end each line with an extra '\r'.
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }
        if (!line.empty()) {
            return true;
        }
    }
    return false;
}

// Reads the next line and checks that it starts with keyword.
// Example: for the line "HP -2" and keyword "HP", value becomes "-2".
// Prints an error and returns false if the line is missing or wrong.
bool Adventure::readField(std::ifstream& in, const std::string& keyword, std::string& value) {
    std::string line;
    if (!nextLine(in, line)) {
        std::cout << "Error: expected a " << keyword << " line but the file ended." << std::endl;
        return false;
    }
    std::string prefix = keyword + " ";
    if (line.substr(0, prefix.size()) != prefix) {
        std::cout << "Error: expected a " << keyword << " line but found: " << line << std::endl;
        return false;
    }
    value = line.substr(prefix.size());
    return true;
}

// Adds one choice number to a path.
// extendPath("", 2) gives "2". extendPath("2-1", 3) gives "2-1-3".
std::string Adventure::extendPath(const std::string& path, int choiceNumber) const {
    if (path.empty()) {
        return std::to_string(choiceNumber);
    }
    return path + "-" + std::to_string(choiceNumber);
}

// ===================== LOADING =====================

// *TODO 10:*
bool Adventure::load(const std::string& fileName) {
    //open file
    std::ifstream in(fileName);
    if (!in) {
        std::cout << "Error: could not open file " << fileName << std::endl;
        return false;
    }

    //Read startng HP and save
    std::string value;
    if (!readField(in, "START_HP", value)) {
        return false;
    }
    startHp_ = std::stoi(value);

    //Builds story
    Scene<Moment>* root = readScene(in, "START");
    if (root == nullptr) {
        return false;
    }
    //Give story to tree
    tree_.setRoot(root);
    return true;
}

// *TODO 11:*
Scene<Moment>* Adventure::readScene(std::ifstream& in, const std::string& label) {
    // Variables to access/store story info
    std::string text, hpText, lootText, item, choicesText;

    if (!readField(in, "SCENE", text)) {
        return nullptr;
    }
    if (!readField(in, "HP", hpText)) {
        return nullptr;
    }
    if (!readField(in, "LOOT", lootText)) {
        return nullptr;
    }
    if (!readField(in, "ITEM", item)) {
        return nullptr;
    }
    if (!readField(in, "CHOICES", choicesText)) {
        return nullptr;
    }

    // String -> int
    int hpChange, loot, numChoices;

    try {
        hpChange = std::stoi(hpText);
        loot = std::stoi(lootText);
        numChoices = std::stoi(choicesText);
    }
    //Catch all error types assoc with previous block,
    catch (...) {
        return nullptr;
    }
    // Uses previous code to limit choices within paramaters
    if (numChoices < 0 || numChoices > Scene<Moment>::MAX_CHOICES) {
        std::cout << "Error: scene \"" << text << "\" has " << numChoices
                  << " choices. The limit is 3." << std::endl;
        return nullptr;
    }

    // Creates "Moment" with current scene info
    Moment* moment = new Moment(text, label, hpChange, loot, item);
    // Create Scene, allocate space for children pointers
    Scene<Moment>* scene = new Scene<Moment>(moment, numChoices);

    // Iterates each choice so that it can create its child scene recursively
    for (int i = 0; i < numChoices; i++) {
        std::string childLabel;
        if (!readField(in, "LABEL", childLabel)) {
            delete scene;
            return nullptr;
        }


        // Read scene following choice
        Scene<Moment>* child = readScene(in, childLabel);
        // cleans up in case of failure
        if (child == nullptr) {
            delete scene;
            return nullptr;
        }

        // Connects children to parent scenes
        scene->setChoice(i, child);
    }
    // Close and return
    return scene;
}

// ===================== PLAYING =====================

// *TODO 12:*
//   *- If the input ends (getline fails), return 0.*
int Adventure::askChoice(int numChoices) {
    std::string line;
    while (true) {
        std::cout << "Your choice: ";
        //read user input
        if (!std::getline(std::cin, line)) return 0;
        //check w choices
        for (int i = 1; i <= numChoices; i++) {
            if (line == std::to_string(i)) return i;
        }
        //error if invalid
        std::cout << "Please type a number from 1 to " << numChoices << "." << std::endl;
    }
}

// *TODO 13:*

void Adventure::play() {
    int hp = startHp_;
    int loot = 0;
    List<Item> bag;
    Scene<Moment>* scene = tree_.getRoot();

    //check story is loaded
    if (scene == nullptr) {
        return;
    }
    //cont until ending
    while (true) {
        Moment* m = scene->getData();
        //this changess the loot and hp from scene to scene so it is dynamic
        hp += m->getHpChange();
        loot += m->getLoot();
        //Display the current scene
        std::cout << "----------------------------------------" << std::endl;
        std::cout << m->getText() << std::endl;
        //Add new item
        if (m->hasItem()) {
            if (!bag.search(Item(m->getItem()))) {
                bag.addFront(new Item(m->getItem()));
                std::cout << "You picked up: " << m->getItem() << std::endl;
            }
        }

        // Display stats
        std::cout << "HP: " << hp << " Loot: " << loot << std::endl;

        // Death
        if (hp <= 0) {
            std::cout << "Your HP hit 0. GAME OVER." << std::endl;
            break;
        }

        //Ending checker
        if (scene->isEnding()) {
            std::cout << "THE END." << std::endl;
            break;
        }
        //Next choices
        for (int i = 0; i < scene->getNumChoices(); i++) {
            std::cout << "[" << i + 1 << "] "
                      << scene->getChoice(i)->getData()->getLabel()
                      << std::endl;
        }
        //"Next choice" selection
        int choice = askChoice(scene->getNumChoices());

        // Exit if input ends unexpectedly
        if (choice == 0) {
            return;
        }

        // Move to the next
        scene = scene->getChoice(choice - 1);
    }
    // Display the final
    std::cout << "Final HP: " << hp << " Final loot: " << loot << std::endl;
    std::cout << "Inventory (" << bag.size() << " items):" << std::endl;
    bag.print();
}

// *TODO 14:*
void Adventure::printStats() const {
    std::cout << "Scenes: " << tree_.countScenes() << std::endl;
    std::cout << "Endings: " << tree_.countEndings() << std::endl;
    std::cout << "Longest playthrough: " << tree_.height() << " choices" << std::endl;
}

// *TODO 15:*
void Adventure::printSpoilers() const {
    printEndings(tree_.getRoot(), "");
}

// *TODO 16:*
void Adventure::printEndings(Scene<Moment>* scene, const std::string& path) const {
    if (scene == nullptr) {
        return;
    }
    if (scene->isEnding()) {
        std::cout << "Path " << path << ": "
                  << scene->getData()->getText() << std::endl;
        return;
    }
    for (int i = 0; i < scene->getNumChoices(); i++) {
        std::string nextPath = extendPath(path, i + 1);
        printEndings(scene->getChoice(i), nextPath);
    }
}

// *TODO 17:*
void Adventure::greedyRun() const {
    // Initialize current scene, HP, loot, and path
    Scene<Moment>* scene = tree_.getRoot();
    int hp = startHp_;
    int loot = 0;
    std::string path = "";
    bool died = false;
    if (scene == nullptr) {
        return;
    }
    //Continue until ending
    while (true) {
        //Current loot and hp change
        hp += scene->getData()->getHpChange();
        loot += scene->getData()->getLoot();

        // Death
        if (hp <= 0) {
            died = true;
            break;
        }
        //Ending
        if (scene->isEnding()) {
            break;
        }
        int best = 0;

        // Compare scenes to initial to find best
        for (int i = 1; i < scene->getNumChoices(); i++) {
            int thisLoot = scene->getChoice(i)->getData()->getLoot();
            int bestLoot = scene->getChoice(best)->getData()->getLoot();
            if (thisLoot > bestLoot) {
                best = i;
            }
        }
        path = extendPath(path, best + 1);

        // Move to the selected child scene
        scene = scene->getChoice(best);
    }
    //display the greedy results
    std::cout << "Greedy advisor" << std::endl;
    std::cout << "  Path: " << path << std::endl;
    std::cout << "  Loot: " << loot << std::endl;

    if (died) {
        std::cout << "  Result: died" << std::endl;
    } else {
        std::cout << "  Result: reached an ending" << std::endl;
    }
}

// *TODO 18:*
int Adventure::bestLoot(Scene<Moment>* scene, int hp, const std::string& path, std::string& bestPath) const {
    // -1 if no scen
    if (scene == nullptr) {
        return -1;
    }
    // change hp
    hp += scene->getData()->getHpChange();
    if (hp <= 0) {
        return -1;
    }
    // If this ends, return loot and save
    if (scene->isEnding()) {
        bestPath = path;
        return scene->getData()->getLoot();
    }
    // Track best loot from children path
    int best = -1;
    for (int i = 0; i < scene->getNumChoices(); i++) {
        std::string childPath;
        // recursion
        int result = bestLoot(
            scene->getChoice(i),
            hp,
            extendPath(path, i + 1),
            childPath
        );
        if (result > best) {
            best = result;
            bestPath = childPath;
        }
    }

    //return -1 if every child path dies and add to current
    if (best == -1) {
        return -1;
    }
    return best + scene->getData()->getLoot();
}

// *TODO 19:*
void Adventure::optimalRun() const {
    std::string bestPath;
    int best = bestLoot(tree_.getRoot(), startHp_, "", bestPath);
    std::cout << "Optimal advisor" << std::endl;
    if (best == -1) {
        std::cout << "  Result: no path survives this story" << std::endl;
        return;
    }
    std::cout << "  Path: " << bestPath << std::endl;
    std::cout << "  Loot: " << best << std::endl;
    std::cout << "  Result: reached an ending" << std::endl;
}