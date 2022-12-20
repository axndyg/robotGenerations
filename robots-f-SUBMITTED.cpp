// Andres Gutierrez
// robots-f.cpp
// 11 - 4 - 2022

/* 
    LEGEND: 

        vector<vector<int>> genes          : holds 16 4 sequence matches for sensor readings
            singualr gene                  : holds 5 sequence numbers
                                               first four, random number b/t 0-2 
                                               last, random number b/t 0-3;

            for readings: 
                0                              : empty space detected
                1                              : space with wall detected:
                2                              : space with battery detected 

            for last number 
                0                              : paths[0]   north 
                1                              : paths[1]   south
                2                              : paths[2]   west
                3                              : paths[3]   east 
                4                              : when recognized in momvenet, call a random path 


            
*/

#include <iostream> 
#include <iomanip>
#include <ctime>
#include <vector> 
#include <algorithm>
#include <fstream> 

using namespace std;

class Robot { 
    private: 
        long int robotID; 
        int posX = rand() % 10, posY = rand() % 10, fitness = 0, energy = 5, turns = 0;
        int senN, senS, senE, senW;
        vector<char> paths{'N', 'S', 'E', 'W', 'R'};
        vector<vector<int>> genes;
 
    public:
        void moveX(int newX) { posX = newX; }
        void moveY(int newY) { posY = newY; }
        void Turn() { energy--; turns++; };
        void addBat() {energy += 5; fitness+= 5; }; 
        void updateSens(int nReading, int sReading, int eReading, int wReading); 
        char movement();
        void reset() { posX = rand() % 10; posY = rand() % 10; fitness = 0; energy = 5, turns = 0; }

        const int retX() { return posX; }
        const int retY() { return posY; }
        const int retFit() {return fitness; }
        const int retEnergy() {return energy; }
        const int retTurns() {return turns; }
        const int retID() {return robotID; }

        Robot(); 

        // operator overloading
        bool operator<(const Robot &right) const { return (fitness < right.fitness); }     
        void operator=(const Robot &from) { genes = from.genes; robotID = from.robotID; posX = from.posX; posY = from.posY; fitness = from.fitness; energy = from.energy; turns = from.turns;  }
        friend Robot operator+(const Robot &par1, const Robot &par2);
        friend ostream &operator<<(ostream &output, Robot &from); 

        // test functions 
        void stats(); 
};

void pgrmGreet(); 
void pgrmEnd();

void simulation(Robot &testRobot);
void testSimulation(Robot &testRobot);
void roboHub(); 
vector<vector<int>> genMap(); 
void displayMap(vector<vector<int>> map, Robot dummy);
void reproduce(vector<Robot> &population); 
void my_sort(vector<Robot> &mypop);

int main() {

    srand(time(0)); 
    pgrmGreet();

    roboHub();

    pgrmEnd(); 
    return 0;
}

void roboHub() { 
    
    Robot oneB, oneW, lastW, lastB;
    Robot Best; int genBest; 
    vector<Robot> testPool; 
    int totalGens = 0;
    float avgFit = 0, firFit, lastFit;  
    for (int curRobo = 0; curRobo < 200; curRobo++) { Robot dummy; testPool.push_back(dummy); } 

        cout << R"( 
            
        00]   Welcome! Please Enter the amount
      /|__|\  of generations you wish to go through. 
        ''    I reccomend no more than 25000 at a time. 
        )";
        cout << '.' << endl << endl; 
    
    char repeat = 'y'; 
    do { 

        int generations = 0; 
        cout << "Enter Number of Generations: "; cin.sync(); cin >> generations; 

        while (cin.fail() || generations <= 0 || generations > 25000) { 
            cin.clear(); cin.ignore(120, '\n'); 
            cout << "\n   Oops, that didn't compute. Try again: ";
            cin >> generations; 
        } 
    
        // Actual Simulation 
        for (int curGen = 0; curGen < generations; curGen++) {

            for (int curSim = 0; curSim < 200; curSim++) { simulation(testPool[curSim]); }  
            my_sort(testPool); 

            if (curGen == 0) { oneB = testPool[199]; oneW = testPool[0]; }; if (curGen == generations - 1) { lastB = testPool[199]; lastW = testPool[0]; }
            if ( Best < testPool[199]) {Best = testPool[199];  genBest = totalGens + curGen + 1; }
            if ((curGen + 1) % 500 == 0) cout << endl << "    Procressed generation: " <<  curGen + 1 + totalGens;
        
            float thisFit = 0; 
            for (int curRobo = 0; curRobo < 200; curRobo++) {thisFit += testPool[curRobo].retFit(); }
            thisFit /= 200; 
            if (curGen == 0) {firFit = thisFit;} if (curGen == generations - 1) { lastFit = thisFit; } 
            if (avgFit == 0) {avgFit = thisFit;} else {avgFit = (avgFit + thisFit) / 2; }
        
            reproduce(testPool);
            for (int curRobo = 99; curRobo < 200; curRobo++) { testPool[curRobo].reset(); }
        }
        totalGens += generations; 
 
        { // STATS 
            cout << R"(  

        00]   Let's take a look at the results
      /|__|\  over the generations. Press enter to see the stats
        ''    
            )";
            cin.sync(); cin.get();
            cout << "       Top scorer, first gen. of cycle: " << endl;
                oneB.stats();
                cout << endl;
            cout << "       Top scorer, latest gen. : " << totalGens << endl;
                lastB.stats();
                cout << endl;
            cout << "       Top scorer, overall, from gen. : " << genBest << endl;
                Best.stats();
                cout << endl;  

            cout << "       Worst scorer, first gen. of cycle: " << endl;
                oneW.stats();
                cout << endl;
            cout << "       Worst scorer, latest gen. : " << totalGens << endl;
                lastW.stats();
                cout << endl;
            cout << "      Avg. Fitness, first gen. in Cycle : " << firFit << endl;
            cout << "      Avg. Fitness, latest gen.         : " << lastFit << endl << endl;
            cout << "      Avg. Fitness, across " << totalGens << " gens. : " << avgFit << endl;
        }

        {  // REPROMPT 
            cout << "\nWould you like to simulate more generations? (y/n): "; 
                cin.sync(); cin >> repeat;
            repeat = tolower(repeat); 

            while (repeat != 'n' && repeat != 'y') {
                cin.ignore(110, '\n');
                cout<<"\nDid not compute, try again (y/n): ";
                    cin >> repeat;
                    repeat = tolower(repeat);
            }

            cout << endl; 
        }
   
    } while (repeat != 'n');
    
    cout << R"( 
            
        00]   What a great run.
      /|__|\  Go you!
        ''    
              Please press enter to recieve your farewell.
        )"; 
    cin.sync(); cin.get();
} 
void simulation(Robot &testRobot) { 

    vector<vector<int>> map = genMap();
 
    do {  
        int curX = testRobot.retX(), curY = testRobot.retY(); 

        // spacecheck 
        if (map[curY][curX] == 2) {
            testRobot.addBat(); map[curY][curX] = 0; 
        }

        // readings 
        int nReading, sReading, eReading, wReading; 
        if (curX == 0 ) { wReading = 1; eReading = map[curY][curX + 1]; }
        else if (curX == 9) { eReading = 1; wReading = map[curY][curX - 1]; }
        else { eReading = map[curY][curX + 1]; wReading = map[curY][curX - 1]; }

        if (curY == 0 ) { nReading = 1; sReading = map[curY + 1][curX]; }
        else if (curY == 9) { sReading = 1; nReading = map[curY - 1][curX];  }
        else { nReading = map[curY - 1][curX]; sReading = map[curY + 1][curX]; }

        testRobot.updateSens(nReading, sReading, eReading, wReading); 

        //movement
        char roboMove = testRobot.movement();
        int moveX = 0, moveY = 0; 

        switch (roboMove) {
            case 'N':
                if (curY != 0) {moveY--;} break; // seems counterintituve but in arrays, Y goes from top to bottom (0 to 9)
            case 'S':
                if (curY != 9) {moveY++;} break; // same here 
            case 'E':
                if (curX != 9) {moveX++;} break;
            case 'W': 
                if (curX != 0) {moveX--;} break; 
            default: cout << "\n\n ERR. PROGRAM HAS ENTERED FAULTY PATH IN FUNCTION \"simulation\" ERR.\n\n";  
        }

        testRobot.moveX(curX + moveX); testRobot.moveY(curY + moveY); 

        testRobot.Turn(); 
    } while (testRobot.retEnergy() > 0);
   
}
void reproduce(vector<Robot> &population) { 

    int par1 = 199, par2 = 198;
    int child = 0, child2 = 1;

    while (par1 > 97) { 
        population[child] = population[par1] + population[par2];
        population[child2] = population[par1] + population[par2];

        par1 -= 2; par2 = par1 - 1; 
        child += 2; child2 = child + 1;
    }
}
void my_sort(vector<Robot> &mypop) { 

    for (int i = 0, n = mypop.size() - 1; i < n; i++) { 
        for (int j  = 0, k = mypop.size() -1; j < k; j++) {
            if (!(mypop[j] < mypop[j+1])) { 
                swap(mypop[j], mypop[j+1]);
            }
        }
    }
}

vector<vector<int>> genMap() { 

    vector<vector<int>> retMap; 

    for (int curLine = 0; curLine < 10; curLine++) retMap.push_back({0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

    for (int curBat = 0; curBat < 40; curBat++) { 
        
        int xPlace = rand() % 10, yPlace = rand() % 10; 
        while (retMap[xPlace][yPlace] != 0) { 
            xPlace = rand() % 10, yPlace = rand() % 10; 
        }
        retMap[xPlace][yPlace] = 2;
    }

    return retMap; 
}
void pgrmGreet(){ 
    string welcome = R"(

        ;     /        ,--.     WELCOME TO: 
       ["]   ["]  ,<  |__**|    ROBOT GENERATIONS
      /[_]\  [~]\/    |//  |   
       ] [   OOO      /o|__|    A PROGRAM BY: ANDY GUTIERREZ

    )";
    cout << welcome << endl; 
}
void pgrmEnd(){
    cout << R"(

        _________________________________
        |.--------_--_------------_--__--.|
        ||    /\ |_)|_)|   /\ | |(_ |_   ||
        ;;`,_/``\|__|__|__/``\|_| _)|__ ,:|
        ((_(-,-----------.-.----------.-.)`)
        \__ )        ,'     `.        \ _/
        :  :        |_________|       :  :
        |-'|       ,'-.-.--.-.`.      |`-|
        |_.|      (( (*  )(*  )))     |._|       THANKS FOR 
        |  |       `.-`-'--`-'.'      |  |       SIMULATING 
        |-'|        | ,-.-.-. |       |._|
        |  |        |(|-|-|-|)|       |  |
        :,':        |_`-'-'-'_|       ;`.;
         \  \     ,'           `.    /._/
          \/ `._ /_______________\_,'  /
            \  / :   ___________   : \,'
             `.| |  |           |  |,'
               `.|  |           |  |
                 |  |           |  |

    )"; 
    cin.get(); 
}

// Robot Constructors: 
Robot::Robot() { 

    robotID = rand() % 1000000001;
    
    for (int curGene = 0; curGene < 16; curGene++) { 
        genes.push_back({rand() % 3, rand() % 3, rand() % 3, rand() % 3, rand() % 5});
    }

}
Robot operator+(const Robot &par1, const Robot &par2) { 
    
    Robot kid; 
    vector<int> order {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    random_shuffle(order.begin(), order.end()); 

    for (int curGene = 0; curGene < 16; curGene++) { 
        if (curGene < 8) { 
            kid.genes[curGene] = par1.genes[order[curGene]];
        }
        else { 
            kid.genes[curGene] = par2.genes[order[curGene]];
        }
    }

    vector<vector<int>> mutations{{1, 1, 1, 1, 3},
                                 {0, 0, 0, 0, 4},
                                 {1, 0, 0, 0, 1},
                                 {0, 0, 1, 1, 2}, 
                                 {2, 2, 2, 0, 3}}; 
    int mutate = rand() % 101;
    if (mutate <= 5) kid.genes[rand() % 16] = mutations[rand() % 5]; 
    
    return kid;

    /* 
        Logic Notes: In preditcable reproduction, the first 8 genes of the 1st parent
        and the second 8 genes of 2nd parent would go to the kid, 

        with my order, the kids first 8 genes then splice random 8 from par1 by order 
        and the 8 genes thereafter splice from par2 in a random order 

        order is a vector set holding the index of the parents gene to go to,
        once we shuffle that, we gurantee a random index of the parents genes will go to 
        the childs with no duplicates, unless in the extreme odd case both parents share a gene 
        and happen to pass on 

    */
}

// Robot Functions; 
void Robot::updateSens(int nReading, int sReading, int eReading, int wReading) { 
    senN = nReading; senS = sReading; senE = eReading; senW = wReading; 
}
char Robot::movement() { 

    for (int curGene = 0; curGene < 16; curGene++) { 
        int geneN = genes[curGene][0], geneS = genes[curGene][1]; 
        int geneE = genes[curGene][2], geneW = genes[curGene][3];
        int geneM = genes[curGene][4];
        if (geneN == senN && geneS == senS && geneW == senW && geneE == senE) {
            if (geneM != 4) return paths[(genes[curGene][4])];
            else {
                return paths[rand() % 4]; 
            }
        }
    }
    int defaultMove = genes[15][4];
    if (defaultMove != 4) return paths[defaultMove];
    else return paths[rand() % 4]; 
}

// Test Functions
ostream &operator<<(ostream &output, Robot &from) { 
    output << "\nGENE READING FOR ROBOT: " << from.robotID << endl;
    for (int curGene =0; curGene< 16; curGene++) {

        output << "Current Gene Set: " << curGene + 1 << endl;
        output << "  North Gene: " << from.genes[curGene][0] << endl;
        output << "  South Gene: " << from.genes[curGene][1] << endl;
        output << "  East Gene : " << from.genes[curGene][2] << endl;
        output << "  West Gene : " << from.genes[curGene][3] << endl << endl;
        output << "  Directed Movement (Pure Numerical)  : " << from.genes[curGene][4] << endl;
        output << "  Directed Movement (Character Mapped): ";
            if (from.genes[curGene][4] != 4) output << from.paths[(from.genes[curGene][4])];
            else {
                output << from.paths[rand() % 4]; 
            }
        output << endl << endl;
    }
    return output;
}
void displayMap(vector<vector<int>> map, Robot dummy) { 

    for (int curLine = 0, maxRows = map.size(); curLine < maxRows; curLine++ ) {
        cout << setw(6); 
        for (int curBox = 0, maxBoxes = map[curLine].size(); curBox < maxBoxes; curBox++) {
            cout << " [";
            if (dummy.retY() == curLine && dummy.retX() == curBox && map[curLine][curBox] == 2) { cout << "X"; }
            else if (dummy.retY() == curLine && dummy.retX() == curBox) {
                cout << "C";
            }
            else if (map[curLine][curBox] == 2) cout << "B"; 
            else cout << " ";
            cout << "]" ;
        }
        cout << endl;
    }
    cout << endl; 

}
void testSimulation(Robot &testRobot) { 

    vector<vector<int>> map = genMap();
 
    do {  
        displayMap(map, testRobot);
        int curX = testRobot.retX(), curY = testRobot.retY(); 

        // spacecheck 
        if (map[curY][curX] == 2) {
            testRobot.addBat(); map[curY][curX] = 0; 
        }

        // readings 
        int nReading, sReading, eReading, wReading; 
        if (curX == 0 ) { wReading = 1; eReading = map[curY][curX + 1]; }
        else if (curX == 9) { eReading = 1; wReading = map[curY][curX - 1]; }
        else { eReading = map[curY][curX + 1]; wReading = map[curY][curX - 1]; }

        if (curY == 0 ) { nReading = 1; sReading = map[curY + 1][curX]; }
        else if (curY == 9) { sReading = 1; nReading = map[curY - 1][curX];  }
        else { nReading = map[curY - 1][curX]; sReading = map[curY + 1][curX]; }

        testRobot.updateSens(nReading, sReading, eReading, wReading); 

        //movement
        char roboMove = testRobot.movement();
        cout << "\nN: " << nReading << "  S: " << sReading << "  E: " << eReading << "  W: " << wReading << endl;  
        cout << "Char: " << roboMove << endl << endl; 
        int moveX = 0, moveY = 0; 

        switch (roboMove) {
            case 'N':
                if (curY != 0) {moveY--;} break; // seems counterintituve but in arrays, Y goes from top to bottom (0 to 9)
            case 'S':
                if (curY != 9) {moveY++;} break; // same here 
            case 'E':
                if (curX != 9) {moveX++;} break;
            case 'W': 
                if (curX != 0) {moveX--;} break; 
            default: cout << "\n\n ERR. PROGRAM HAS ENTERED FAULTY PATH IN FUNCTION \"simulation\" ERR.\n\n";  
        }

        testRobot.moveX(curX + moveX); testRobot.moveY(curY + moveY); 

        testRobot.Turn(); 
    } while (testRobot.retEnergy() > 0);
   
}
void Robot::stats() { 
    cout << "       Simulation results for Robot: " << robotID << endl << endl; 
    cout << "        | Turns Survived    : " << turns << endl;
    cout << "        | Energy Accumulated: " << fitness << endl; 

}

