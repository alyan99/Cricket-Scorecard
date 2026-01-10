#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class Player {
    string name;
    int playerID;
    int runsScored, ballsFaced, fours, sixes;
    bool isOut;
    string dismissalType;
    Player* next;

public:
    Player() : playerID(0), runsScored(0), ballsFaced(0), fours(0), sixes(0), 
               isOut(false), dismissalType("Not Out"), next(nullptr) {}

    void setName(string n) { name = n; }
    void setPlayerID(int id) { playerID = id; }
    string getName() { return name; }
    int getPlayerID() { return playerID; }
    int getRunsScored() { return runsScored; }
    bool getIsOut() { return isOut; }
    Player* getNext() { return next; }
    void setNext(Player* n) { next = n; }

    void addRuns(int r) {
        runsScored += r;
        ballsFaced++;
        if (r == 4) fours++;
        if (r == 6) sixes++;
    }

    void recordOut(string d) {
        isOut = true;
        dismissalType = d;
    }

    void displayStats() {
        cout << left << setw(18) << name 
             << setw(15) << dismissalType
             << right << setw(5) << runsScored 
             << setw(6) << ballsFaced 
             << setw(5) << fours 
             << setw(5) << sixes << endl;
    }
};

class BattingQueue {
    struct Node {
        Player* p;
        Node* next;
        Node(Player* plyr) : p(plyr), next(nullptr) {}
    };
    Node *front, *rear;

public:
    BattingQueue() : front(nullptr), rear(nullptr) {}

    void enqueue(Player* p) {
        Node* newNode = new Node(p);
        if (!rear) front = rear = newNode;
        else { rear->next = newNode; rear = newNode; }
    }

    Player* dequeue() {
        if (!front) return nullptr;
        Node* temp = front;
        Player* p = temp->p;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        return p;
    }
};

class CricketTeam {
    Player* head;
    string teamName;
    int totalRuns, totalWickets, extras;
    float totalOvers;

public:
    BattingQueue battingOrder;

    CricketTeam(string n = "") : head(nullptr), teamName(n), totalRuns(0), 
                                 totalWickets(0), extras(0), totalOvers(0.0) {}

    void addPlayer(string name, int id) {
        Player* p = new Player();
        p->setName(name); p->setPlayerID(id);
        if (!head) head = p;
        else {
            Player* t = head;
            while (t->getNext()) t = t->getNext();
            t->setNext(p);
        }
    }

    void setupBattingOrder() {
        Player* t = head;
        while (t) {
            battingOrder.enqueue(t);
            t = t->getNext();
        }
    }

    void updateScore(Player* p, int r) { if(p) { p->addRuns(r); totalRuns += r; } }
    void addExtra(int r) { totalRuns += r; extras += r; }
    void addWicket() { totalWickets++; }
    
    void updateOvers(int balls) {
        totalOvers = (balls / 6) + (balls % 6) / 10.0f;
    }

    void displayScorecard() {
        cout << "\n==========================================================\n";
        cout << " TEAM: " << left << setw(15) << teamName << " SCORE: " << totalRuns << "/" << totalWickets 
             << " (" << fixed << setprecision(1) << totalOvers << " Ov)\n";
        cout << " Extras: " << extras << "\n";
        cout << "----------------------------------------------------------\n";
        cout << left << setw(18) << "Batsman" << setw(15) << "Status" 
             << right << setw(5) << "R" << setw(6) << "B" << setw(5) << "4s" << setw(5) << "6s" << endl;
        Player* t = head;
        while (t) {
            t->displayStats();
            t = t->getNext();
        }
        cout << "==========================================================\n";
    }

    string getTeamName() { return teamName; }
    int getTotalRuns() { return totalRuns; }
};

class Match {
    CricketTeam *t1, *t2;
    CricketTeam* battingTeam;
    Player *striker, *nonStriker;
    int maxOvers, ballsBowled;

public:
    Match() : t1(nullptr), t2(nullptr), striker(nullptr), nonStriker(nullptr), 
              maxOvers(0), ballsBowled(0) {}

    void init() {
        string n1, n2; int p;
        cout << "Overs: "; cin >> maxOvers;
        cout << "Team 1: "; cin.ignore(); getline(cin, n1);
        t1 = new CricketTeam(n1);
        cout << "Squad size: "; cin >> p;
        for(int i=1; i<=p; i++) { string s; cout << "P" << i << ": "; cin >> s; t1->addPlayer(s, i); }

        cout << "\nTeam 2: "; cin.ignore(); getline(cin, n2);
        t2 = new CricketTeam(n2);
        cout << "Squad size: "; cin >> p;
        for(int i=1; i<=p; i++) { string s; cout << "P" << i << ": "; cin >> s; t2->addPlayer(s, i); }

        battingTeam = t1;
        startInnings();
    }

    void startInnings() {
        ballsBowled = 0;
        battingTeam->setupBattingOrder();
        striker = battingTeam->battingOrder.dequeue();
        nonStriker = battingTeam->battingOrder.dequeue();
    }

    void processBall() {
        if (!striker) { cout << "\n[!] Innings Over (All Out)\n"; return; }
        
        int action;
        cout << "\n(0-6) Runs | (7) Wkt | (8) Wide | (9) NB: ";
        if (!(cin >> action)) { cin.clear(); cin.ignore(100, '\n'); return; }

        if (action >= 0 && action <= 6) {
            battingTeam->updateScore(striker, action);
            ballsBowled++;
            if (action % 2 != 0) swap(striker, nonStriker);
        } else if (action == 7) {
            striker->recordOut("Out");
            battingTeam->addWicket();
            ballsBowled++;
            striker = battingTeam->battingOrder.dequeue();
        } else if (action == 8 || action == 9) {
            battingTeam->addExtra(1);
        }

        battingTeam->updateOvers(ballsBowled);

        if (ballsBowled > 0 && ballsBowled % 6 == 0 && action <= 7) {
            swap(striker, nonStriker);
        }
    }

    void showDashboard() {
        cout << "\n>>> " << battingTeam->getTeamName() << " | " << battingTeam->getTotalRuns() << " Runs | " << ballsBowled << " Balls";
        if(striker) cout << "\n * " << striker->getName() << ": " << striker->getRunsScored();
        if(nonStriker) cout << "\n   " << nonStriker->getName() << ": " << nonStriker->getRunsScored();
        cout << "\n----------------------------";
    }

    void showFullScore() {
        if(t1) t1->displayScorecard();
        if(t2) t2->displayScorecard();
    }
};

int main() {
    Match m;
    int choice;
    do {
        cout << "\n1. Start Match\n2. Next Ball\n3. View Scorecard\n4. Exit\nSelection: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(100, '\n'); continue; }

        switch(choice) {
            case 1: m.init(); break;
            case 2: m.showDashboard(); m.processBall(); break;
            case 3: m.showFullScore(); break;
        }
    } while (choice != 4);

    return 0;
}