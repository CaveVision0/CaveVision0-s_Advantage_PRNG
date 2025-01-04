// I PRESENT TO YOU... MY MASTERPIECE!!!
// Feast your eyes on the horrors of CaveVision0's shitty programming!

#include<iostream> // Inputs and outputs.
#include<fstream> // Used for my .txt files.
#include<string> // Used for my .txt files.
#include<map> // Used for the desperation dictionary.
#include<vector> // Used for vectors.
#include<random> // Used for the PRNG.
#include<exception> // Used for exception handling.
#include<cmath> // Used for math functions like pow(x, y).
#include<numeric> // Used for GCD.
#include<algorithm> // Used for sorting.
#include<chrono> // Used for me to track how long the program takes to run. It's not needed for anything important.

using namespace std;
using namespace std::chrono; // Lets me use the features chrono has.

// TO DO LIST:
// Additional data for whether or not there's only one in the entire game or if there's one per team.
// Additional sorting styles.
    // YES/no (?)
    // Chance (?)



// Structs

struct Slot_Size{
    unsigned int With_Clues;
    unsigned int Without_Clues;
};

struct Sorting_Styles{
    bool Teams;
    bool Alphabetical;
};

struct Advantage{
    string Name;
    unsigned int Base_Denominator;
};

struct Clues{
    unsigned int Real;
    unsigned int Fake;
};

struct Fraction{
    unsigned int Numerator;
    unsigned int Denominator;
};

struct Result{
    unsigned int Numerical;
    string Verbal;
};

struct Contestant_Stats{
    string Name;
    short int Desperation_Value;
    string Team;
    vector<Clues> Clues;
    vector<Fraction> Fractions;
    vector<Result> Results;
    unsigned int ID;
};



// Comparators

struct Team_Name_Comparator{
    bool operator()(string a, string b){
        return a < b;
    }
};

struct Name_Comparator{
    bool operator()(Contestant_Stats a, Contestant_Stats b){
        return a.Name < b.Name;
    }
};



// Functions

vector<Contestant_Stats> Contestant_Handler(vector<Contestant_Stats> CS, Slot_Size SS, map<string, short int> D, string I){
    Contestant_Stats Current_Contestant;
    Clues Current_Clues;
    string line;
    vector<unsigned int> Clues_Pusher;
    bool Counting = false; // Whether or not Counter is actually counting.
    unsigned int Counter = 0; // Counts up once per non-empty line. Used to know when to begin a new slot.
    unsigned int ID = 0; // Contestant ID numbers.

    // Opens the input file.
    ifstream Contestant_List (I);
    
    // If the list can't open, the string listed in the loop will print out and nothing will be returned.
    if(!Contestant_List){
        cout << "CAN'T OPEN INPUT!!!!!";
        return CS;
    }

    while(getline(Contestant_List, line)){
        if(line.find("__END") != string::npos){
            Counting = false;
        }

        // If the list scanning has begun, this counts the amount of lines that have been read.
        if(Counting && !line.empty()){
            ++Counter;
        }

        // Detects the phrase "__START" within the text document. Once it's found, the line counting begins.
        if(line.find("__START") != string::npos){
            Counting = true;
        }

        ID = floor(Counter / SS.With_Clues);

        // Name detector.
        if(Counter % SS.With_Clues == 1 && !line.empty()){
            Current_Contestant.Name = line;
            Current_Contestant.ID = ID;
        }

        // Desperation tier detector.
        else if(Counter % SS.With_Clues == 2 && !line.empty()){ 

            // Exception handling for invalid desperation tiers. Invalid tiers become "Unbothered" (AKA 1).
            try{
                Current_Contestant.Desperation_Value = D.at(line);
            }
            catch(const out_of_range& ex){
                Current_Contestant.Desperation_Value = 1;
                cout << "Conversion Error: " << Current_Contestant.Name << " has an invalid desperation tier! Invalid tier will be automatically replaced with \"Unbothered\" (AKA 1). All clue counts are set to 0 as well." << endl;
            }
        }

        // Team detector.
        else if(Counter % SS.With_Clues == 3 && !line.empty()){
            Current_Contestant.Team = line;
        }

        // Clue Detector.
        else if(Counting && Counter > 0 && (Counter % SS.With_Clues > SS.Without_Clues || Counter % SS.With_Clues == 0) && !line.empty()){
            if((Counter % SS.With_Clues) % 2 == (SS.Without_Clues + 1) % 2 && Counter % SS.With_Clues != 0){

                // Exception handling for non-integer clue counts. Invalid values become 0s.
                try{
                    Current_Clues.Real = stoi(line);
                }
                catch(const invalid_argument& ex){
                    cout << "Conversion Error: " << Current_Contestant.Name << " has at least 1 non-integer real clue counter! Invalid slot(s) will be automatically filled with 0s." << endl;
                    Current_Clues.Real = 0;
                }
                catch(const out_of_range& ex){
                    cout << "Conversion Error: " << Current_Contestant.Name << " has at least 1 real clue counter that's out of range! Invalid slot(s) will be automatically filled with 0s." << endl;
                    Current_Clues.Real = 0;
                }
            }
            else{
                
                // Exception handling for non-integer clue counts. Invalid values become 0s.
                try{
                    Current_Clues.Fake = stoi(line);
                }
                catch(const invalid_argument& ex){
                    cout << "Conversion Error: " << Current_Contestant.Name << " has at least 1 non-integer fake clue counter! Invalid slot(s) will be automatically filled with 0s." << endl;
                    Current_Clues.Fake = 0;
                }
                catch(const out_of_range& ex){
                    cout << "Conversion Error: " << Current_Contestant.Name << " has at least 1 fake clue counter that's out of range! Invalid slot(s) will be automatically filled with 0s." << endl;
                    Current_Clues.Fake = 0;
                }
                Current_Contestant.Clues.push_back(Current_Clues);
            }

            if(Counter % SS.With_Clues == 0){
                CS.push_back(Current_Contestant);
                Current_Contestant = {}; 
            }
        }

        // If a slot ends early via empty line, this sets the default values.
        if(Counter % SS.With_Clues != 0 && (Counting && (line.empty() || line == "__END"))){
            
            // Sets the default desperation tier to "Unbothered" if a slot cuts off after the name.
            if(Counter % SS.With_Clues == 1){
                Current_Contestant.Desperation_Value = 1;
                ++Counter;
            }

            // Sets the default team to "No Team" if a slot cuts off after the desperation tier.
            if(Counter % SS.With_Clues == 2){
                Current_Contestant.Team = "No Team";
                ++Counter;
            }

            // Fills all post-space clue slots with zeroes.
            if(Counter % SS.With_Clues >= SS.Without_Clues){
                for(int i = Counter % SS.With_Clues; i < SS.With_Clues; ++i){
                    if((Counter % SS.With_Clues) % 2 == (SS.Without_Clues + 1) % 2){
                        Current_Clues.Real = 0;
                    }
                    else{
                        Current_Clues.Fake = 0;
                        Current_Contestant.Clues.push_back(Current_Clues);
                    }
                    ++Counter;
                }
            }

            if(Counter % SS.With_Clues == 0){
                CS.push_back(Current_Contestant);
                Current_Contestant = {}; 
            }
        }
    }

    Contestant_List.close();
    return CS;
}

vector<Contestant_Stats> Chance_Retriever(vector<Advantage> AT, vector<Contestant_Stats> CS){
    Fraction Fraction_Pusher;
    unsigned int Factor; // Used to simplify fractions.
    unsigned int Value, d, r, f;
    int t;

    // For every advantage...
    for(int i = 0; i < AT.size(); ++i){
        // For every player...
        for(int j = 0; j < CS.size(); ++j){

            // The base denominator of advantage i.
            d = AT.at(i).Base_Denominator;

            // Player j's real clues for advantage i.
            r = CS.at(j).Clues.at(i).Real;

            // Player j's fake clues for advantage i.
            f = CS.at(j).Clues.at(i).Fake;

            // Player j's desperation tier value.
            t = CS.at(j).Desperation_Value;

            /*
            #include<cmath> functions for clarity:
                round(x) = Rounds x to the nearest integer.
                pow(x, y) = x to the power of y (AKA x^y).
            */

            /*
            Example Formulas:
                Linear Clue Amplification (progression is 2, 3, 4, 5, and so on):
                    x = 1000 * (1 + r);
                    Numerator_Pusher.push_back(x);
                    x = round(1000 * d * (1 + f) * pow(0.25, (0.25 * t)));
                    Denominator_Pusher.push_back(x);

                2^Clue (progression is 2, 4, 8, 16, and so on):
                    x = 1000 * pow(2, r);
                    Numerator_Pusher.push_back(x);
                    x = round(1000 * d * pow(2, f) * pow(0.25, (0.25 * t)));
                    Denominator_Pusher.push_back(x);
            */

            // CUSTOM: This line of code controls the numerator part of the fraction that defines odds of finding advantage i.
            // DEFAULT: 1000 * pow(r + 1, 2);
            Value = 1000 * pow(r + 1, 2);
            Fraction_Pusher.Numerator = Value;

            // CUSTOM: This line of code controls the denominator part of the fraction that defines odds of finding advantage i.
            // DEFAULT: round(1000 * d * pow(f + 1, 2) * pow(0.25, 0.25 * t));
            Value = round(1000 * d * pow(f + 1, 2) * pow(0.25, 0.25 * t));
            Fraction_Pusher.Denominator = Value;
            CS.at(j).Fractions.push_back(Fraction_Pusher);
        }
    }

    // For every Advantage...
    for(int i = 0; i < CS.size(); ++i){
        
        // For every player...
        for(int j = 0; j < CS.at(0).Fractions.size(); ++j){

            // Simplifies the fractions.
            Factor = gcd(CS.at(i).Fractions.at(j).Numerator, CS.at(i).Fractions.at(j).Denominator);
            CS.at(i).Fractions.at(j).Numerator /= Factor;
            CS.at(i).Fractions.at(j).Denominator /= Factor;
        }
    }

    return CS;
}

vector<Contestant_Stats> PRNG_Handler(vector<Advantage> AT, vector<Contestant_Stats> CS, int S){
    Result Result_Pusher;
    mt19937 PRNG_0; // The random number generater. Who doesn't love a good mt19937?
    PRNG_0.seed(S);

    for(int i = 0; i < CS.size(); ++i){
        for(int j = 0; j < AT.size(); ++j){
            
            // The PRNG.
            Result_Pusher.Numerical = (PRNG_0() % CS.at(i).Fractions.at(j).Denominator) + 1;
            
            // If x is less than or equal to the corresponding numerator, it's a "YES."
            if(Result_Pusher.Numerical <= CS.at(i).Fractions.at(j).Numerator){
                Result_Pusher.Verbal = "YES";
            }
            else{
                Result_Pusher.Verbal = "no";
            }
            CS.at(i).Results.push_back(Result_Pusher);
        }
    }

    return CS;
}

vector<string> Team_Gatherer(vector<string> TN, vector<Contestant_Stats> CS){
    bool New_Name;

    // Automatically assign's the first contestant's team as a new one.
    TN.push_back(CS.at(0).Team);

    // Gathers every present team name.
    for(int i = 0; i < CS.size(); ++i){
        New_Name = true;
        for(int j = 0; j < TN.size(); ++j){
            if(CS.at(i).Team == TN.at(j)){
                New_Name = false;
            }
        }
        if(New_Name){
            TN.push_back(CS.at(i).Team);
        }
    }

    return TN;
}

vector<Contestant_Stats> Sorter(Sorting_Styles S, vector<string> TN, vector<Contestant_Stats> CS){
    if(S.Teams && S.Alphabetical){
        sort(TN.begin(), TN.end(), Team_Name_Comparator());
    }

    if(S.Alphabetical){
        sort(CS.begin(), CS.end(), Name_Comparator());
    }
    return CS;
}

void Base_Text_Output(vector<Advantage> AT, vector<Contestant_Stats> CS, string O, int j){
    
    // Gets the output file.
    ofstream Results_File (O, std::ios::app);
    
    // If the output file can't open, this says so.
    if(!Results_File){
        cout << "CAN'T OPEN OUTPUT!!!!!";
    }

    Results_File << CS.at(j).Name << ":" << endl;

    // For every advantage...
    for(int k = 0; k < AT.size(); ++k){

        // Writes the name of the advantage.
        Results_File << AT.at(k).Name;

        // Bracket piece that has the fraction and the percent.
        Results_File << " [" << CS.at(j).Fractions.at(k).Numerator << "/" << CS.at(j).Fractions.at(k).Denominator << " (" << 100 * (float)CS.at(j).Fractions.at(k).Numerator / (float)CS.at(j).Fractions.at(k).Denominator << "%); "; // Advantage odds.
        
        // Bracket piece that has the results (YES/no).
        Results_File << CS.at(j).Results.at(k).Numerical << " = " << CS.at(j).Results.at(k).Verbal << "]" << endl; // Roll results.
    }
    Results_File << endl;
}

void Text_Output(vector<Advantage> AT, vector<Contestant_Stats> CS, vector<string> TN, string O, Sorting_Styles X){

    // Gets the output file.
    ofstream Results_File (O, std::ios::app);
    
    // If the output file can't open, this says so.
    if(!Results_File){
        cout << "CAN'T OPEN OUTPUT!!!!!";
    }

    // Prints out to the output file.

    if(X.Teams){
        // For every team name (this sorts everyone into those groups)... 
        for(int i = 0; i < TN.size(); ++i){

            // Prints the team names.
            Results_File << "__________________________________________________________________" << endl;
            Results_File << "[TEAM: " << TN.at(i) << "]" << endl << endl;

            // For each player...
            for(int j = 0; j < CS.size(); ++j){

                // If player j's team matches the current team...
                if(CS.at(j).Team == TN.at(i)){
                    Base_Text_Output(AT, CS, O, j);
                }
            }
        }
    }
    else{
        // For each player...
        for(int j = 0; j < CS.size(); ++j){
            Base_Text_Output(AT, CS, O, j);
        }
    }

    Results_File.close();
}



// int main()

int main(){
    Sorting_Styles Sorts;
    auto start = high_resolution_clock::now(); // Tests how long the program takes to work. This is the starting point.


    // ___________________________________________________________________________________________________________________________________________________
    // STUFF FOR YOU TO CUSTOMIZE BELOW!!!
    

    // Input and output files.
    string Input = "ExampleCast.txt";
    string Output = "ExampleResult.txt";
    
    // Declares the possible advantages. The format for each advantage is {Name, Base Denominator}.
    // Base Denominator must be an integer from 1 to 4294967295. Don't use 0. We all know what happens when you divide by 0.
    Advantage Advantages[] = {
        {"Immunity Idol", 100},
        {"Vote Pass", 50}
    };
    
    // Map for the desperation tiers (2nd row of each contestant's slot in the input file).
    // Base denominator will remain as the denominator post-calculations if a character has desperation tier 0 ("Comfy" in the default case) and no clues for that advantage.
    // Desperation numbers have to be anywhere from -128 to 127.
    map<string, short int> Desperation = {
        {"Occupied", -128},
        {"Comfy", 0},
        {"Unbothered", 1},
        {"Concerned", 2},
        {"Worried", 3},
        {"Desperate", 4}
    };

    // PPRNG_Seed can be any integer from -2147483648 to 2147483647.
    int PRNG_Seed = 010101;

    // Sorting styles. true means it will sort that way while false means it won't.
    Sorts.Teams = true;
    Sorts.Alphabetical = false;

    // If you want to customize the math formula used for the RNG chances, that's in the function "Chance_Retriever." Press ctrl + f and type "CUSTOM:"

    /*
    DEFAULTS:

    string Input = "ExampleCast.txt";
    string Output = "ExampleResult.txt";
    
    Advantage Advantages[] = {
        {"Immunity Idol", 100},
        {"Vote Pass", 50}
    };

    map<string, char> Desperation = {
        {"Occupied", -128},
        {"Comfy", 0},
        {"Unbothered", 1},
        {"Concerned", 2},
        {"Worried", 3},
        {"Desperate", 4}
    };

    int PPRNG_Seed = 010101;

    Sorts.Teams = true;
    Sorts.Alphabetical = false;
    */


    // STUFF FOR YOU TO CUSTOMIZE ABOVE!!!
    // ___________________________________________________________________________________________________________________________________________________

    
    vector<Advantage> Advantage_Types;
    for (int i = 0; i < sizeof(Advantages) / sizeof(Advantages[0]); ++i) {
        Advantage_Types.push_back(Advantages[i]);
    }

    // Variables, structs and vectors.
    vector<Contestant_Stats> Stats;
    Slot_Size Size;
    Size.Without_Clues = 3;
    Size.With_Clues = (Advantage_Types.size() * 2) + Size.Without_Clues; // Used for contestant slot sizes. Name = 1 slot, Desperation = 1 slot, Team = 1 slot, 2x = 2 additional slots per advantage (x)
    vector<string> Team_Names;


    // Where the magic happens
    cout << "Getting contestants..." << endl;
    Stats = Contestant_Handler(Stats, Size, Desperation, Input);
    cout << "Doing the math..." << endl;
    Stats = Chance_Retriever(Advantage_Types, Stats);
    cout << "Gathering results..." << endl;
    Stats = PRNG_Handler(Advantage_Types, Stats, PRNG_Seed);

    if(Sorts.Teams){
        cout << "Splitting list contents into teams..." << endl;
        Team_Names = Team_Gatherer(Team_Names, Stats);
    }

    cout << "Sorting..." << endl;
    Stats = Sorter(Sorts, Team_Names, Stats);

    // Empties the output file for overriding.
    ofstream Reset (Output);
    Reset.close();

    cout << "Porting to output document..." << endl;
    Text_Output(Advantage_Types, Stats, Team_Names, Output, Sorts);
    // We're done!!!

    auto stop = high_resolution_clock::now(); // Tests how long the program takes to work. This is the stopping point. This isn't needed for anything, I just wanted to see how fast it runs.
    auto duration = duration_cast<microseconds>(stop - start);
    float seconds = (float)duration.count() / 1000000;
    cout << "Program duration: " << seconds << " seconds" << endl;
}
