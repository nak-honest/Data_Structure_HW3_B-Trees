#include "bag.h"
#include <string>

using namespace std;

int main() {
    bag<int> b;
    string command;
    int num;

    cout << "Enter the command and number." << endl;
    cout << "insert x : insert x to the current bag (B-tree)" << endl;
    cout << "erase_one x : erase one occurrence of x from the current bag "
         << "(B-tree)" << endl;
    cout << "count x : counts the number of x’s in the current bag (B-tree); "
         << "returns the count" << endl;
    cout << "quit : quit the program" << endl << endl;
    while (1) {
        cout << "Enter the commands : " << endl;

        cin >> command;

        if (command.compare("quit") == 0) {
            break;
        } else if (command.compare("insert") && command.compare("count") &&
                   command.compare("erase_one")) {
            cout << "<!Enter the correct commands!>" << endl;
            cin.ignore(100, '\n');
            continue;
        }

        cin >> num;

        if (cin.fail()) {
            cout << "<!Enter the correct commands!>" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            continue;
        } else {
            cin.ignore(100, '\n');
        }

        if (command.compare("insert") == 0) {
            b.insert(num);
        } else if (command.compare("erase_one") == 0) {
            b.erase_one(num);
        } else if (command.compare("count") == 0) {
            cout << "The count of '" << num << "' is " << b.count(num) << endl;
        }

        cout << endl << "<The current state of B-tree>" << endl << endl;
        b.show_contents();
        cout << endl;
    }

    return 0;
}
