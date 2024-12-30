// I PRESENT TO YOU... MY MASTERPIECE!!!
// Feast your eyes on the horrors of CaveVision0's shitty programming!

#include<iostream> // Inputs and outputs.
#include<fstream> // Used for my .txt files.
#include<exception> // Used for exception handling.
#include<string> // Used for my .txt files.
#include<cmath> // Used for math functions like pow(x, y).
#include<numeric> // Used for GCD.
#include<map> // Used for the desperation dictionary.
#include<vector> // Used for vectors.
#include<random> // Used for the PRNG.
#include<ctime> // Used for the PRNG's seeding.
#include<tuple> // Used for the PRNG's seeding.

using namespace std;

// TO DO LIST:
// Additional data for whether or not there's only one in the entire game or if there's one per team.
// The RTF will acknowledge if more than one person finds an advantage.
// RTF text formatting.



// Structs

struct Contestant_Stats{
    string Name;
    float Desperation_Value;
    string Team;
};

struct Advantage{
    string Name;
    float Base_Denominator;
};

struct Contestant_Data{
    vector<Contestant_Stats> Stats; // Main: Players;   Tuple: 0 = Name, 1 = Desperation Value, 2 = Team
    vector<vector<unsigned int>> Clue_Counts; // Main: Players;   Sub: Clue Counts (all grouped together; evens are real, odds are fake)
    unsigned int Size = 3; // the amount of members inside struct Contestant_Stats.
};



// Functions

Contestant_Data Contestant_Handler(unsigned int SS, map<string, float> D, string I){
    struct Contestant_Stats CS;
    struct Contestant_Data CD;
    string line;
    vector<unsigned int> Clues_Pusher = {};
    bool Counting = false; // Whether or not Counter is actually counting.
    unsigned int Counter = 0; // Counts up once per non-empty line. Used to know when to begin a new slot.
    unsigned int ID = 0; // Contestant ID numbers.

    // Pulls up a .txt file listing every remaining contestant.
    ifstream Contestant_List (I);
    
    // If the list can't open, the string listed in the loop will print out and nothing will be returned.
    if(!Contestant_List){
        cout << "CAN'T OPEN INPUT!!!!!";
        return CD;
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

        ID = floor(Counter / SS);

        // Name detector. Slot #1 of the Contestant_Stats sub-vector (or whatever it's called, I'm tired).
        if(Counter % SS == 1 && !line.empty()){
            CS.Name = line;
        }

        // Desperation tier detector. Slot #2 of the aforementioned sub-vectors.
        else if(Counter % SS == 2 && !line.empty()){ 
            // Exception handling for invalid desperation tiers. Invalid tiers become "Unbothered" (AKA 1).
            try{
                CS.Desperation_Value = D.at(line);
            }
            catch(const out_of_range& ex){
                CS.Desperation_Value = 1;
                cout << "Conversion Error: " << CD.Stats.at(ID - 1).Name << " has an invalid desperation tier! Invalid tier will be automatically replaced with \"Unbothered\" (AKA 1). All clue counts are set to 0 as well." << endl;
            }
        }

        // Team detector. Slot #3 of the aforementioned sub-vectors.
        else if(Counter % SS == 3 && !line.empty()){
            CS.Team = line;
            CD.Stats.push_back(CS);
            CS = {};
        }

        // CLUES
        else if(Counting && Counter > 0 && (Counter % SS > CD.Size || Counter % SS == 0) && !line.empty()){
            // Exception handling for non-integer clue counts. Invalid values become 0s.
            try{
                Clues_Pusher.push_back(stoul(line));
            }
            catch(const invalid_argument& ex){
                cout << "Conversion Error: " << CD.Stats.at(ID - 1).Name << " has at least 1 non-integer clue counter! Invalid slot(s) will be automatically filled with 0s." << endl;
                Clues_Pusher.push_back(0);
            }
            catch(const out_of_range& ex){
                cout << "Conversion Error: " << CD.Stats.at(ID - 1).Name << " has at least 1 clue counter that's out of range! Invalid slot(s) will be automatically filled with 0s." << endl;
                Clues_Pusher.push_back(0);
            }
        }

        // If a slot ends early via empty line, this sets the default values.
        if(Counting && line.empty() || line == "__END"){
            
            // Sets the default desperation tier to "Unbothered" if a slot cuts off after the name.
            if(Counter % SS == 1){
                CS.Desperation_Value = 1;
                ++Counter;
            }

            // Sets the default team to "No Team" if a slot cuts off after the desperation tier.
            if(Counter % SS == 2){
                CS.Team = "No Team";
                CD.Stats.push_back(CS);
                CS = {};
                ++Counter;
            }

            // Fills all post-space clue slots with zeroes.
            if(Counter % SS >= CD.Size){
                for(int i = Counter % SS; i < SS; ++i){
                    Clues_Pusher.push_back(0);
                }
                CD.Clue_Counts.push_back(Clues_Pusher);
                Clues_Pusher = {};
            }

            if(Counter % SS > 0){
                Counter += (SS - (Counter % SS));
            }
        }

        if(Counting && Counter > 0 && Counter % SS == 0 && !line.empty()){
            CD.Clue_Counts.push_back(Clues_Pusher);
            Clues_Pusher = {};
        }
    }

    Contestant_List.close();
    return CD;
}

tuple<vector<vector<unsigned int>>, vector<vector<unsigned int>>> Chance_Retriever(vector<Advantage> AT, struct Contestant_Data CD, tuple<vector<vector<unsigned int>>, vector<vector<unsigned int>>> F){
    vector<vector<unsigned int>> True_Numerator; // The numerator in the finished fractions.
    vector<vector<unsigned int>> True_Denominator; // The denominator in the finished fractions.
    vector<unsigned int> Numerator_Pusher;
    vector<unsigned int> Denominator_Pusher;
    int Factor; // Used to simplify fractions.
    int Value, r, d, f, t;

    // For every advantage...
    for(int i = 0; i < AT.size(); ++i){
        
        // For every player...
        for(int j = 0; j < CD.Stats.size(); ++j){
            
            // The base denominator of advantage i.
            d = AT.at(i).Base_Denominator;

            // Player j's real clues for advantage i.
            r = CD.Clue_Counts.at(j).at(2 * i);

            // Player j's fake clues for advantage i.
            f = CD.Clue_Counts.at(j).at((2 * i) + 1);

            // Player j's desperation tier value.
            t = CD.Stats.at(j).Desperation_Value;

            /*
            // #include<cmath> functions for clarity:
                // round(x) = Rounds x to the nearest integer.
                // pow(x, y) = x to the power of y (AKA x^y).
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
            // DEFAULT: round(1000 * pow(r + 1, 2));
            Value = 1000 * pow(r + 1, 2);
            Numerator_Pusher.push_back(Value);

            // CUSTOM: This line of code controls the denominator part of the fraction that defines odds of finding advantage i.
            // DEFAULT: round(1000 * d * pow(f + 1, 2) * pow(0.25, 0.25 *t));
            Value = 1000 * d * pow(f + 1, 2) * pow(0.25, 0.25 *t);
            Denominator_Pusher.push_back(Value);
        }

        // Pushes the players' vectors into the advantages' vectors.
        True_Numerator.push_back(Numerator_Pusher);
        True_Denominator.push_back(Denominator_Pusher);
        Numerator_Pusher = {};
        Denominator_Pusher = {};
    }


    // For every Advantage...
    for(int i = 0; i < True_Numerator.size(); ++i){

        // For every player...
        for(int j = 0; j < True_Numerator.at(0).size(); ++j){
            Factor = gcd(True_Numerator.at(i).at(j),True_Denominator.at(i).at(j));
            True_Numerator.at(i).at(j) /= Factor;
            True_Denominator.at(i).at(j) /= Factor;
        }
    }

    get<0>(F) = True_Numerator;
    get<1>(F) = True_Denominator;
    True_Numerator = {};
    True_Denominator = {};
    return F;
}

vector<vector<string>> PRNG_Handler(vector<Advantage> AT, struct Contestant_Data CD, tuple<vector<vector<unsigned int>>, vector<vector<unsigned int>>> F, vector<vector<string>> R, int S){
    vector<string> Results_Pusher;
    vector<string> Advantage_Received_Pusher;
    vector<unsigned int> Factor; // Used to simplify fractions.
    int x;
    mt19937 PRNG_0; // The random number generater. Who doesn't love a good mt19937?
    PRNG_0.seed(S);

    // PRNG
    for(int i = 0; i < get<0>(F).size(); ++i){
        for(int j = 0; j < get<0>(F).at(0).size(); ++j){
            
            // The true PRNG.
            int x = (PRNG_0() % get<1>(F).at(i).at(j)) + 1;
            Results_Pusher.push_back(to_string(x));
            
            // If x is less than or equal to the corresponding numerator, it's a "YES."
            if(x <= get<0>(F).at(i).at(j)){
                Advantage_Received_Pusher.push_back("YES");
            }
            else{
                Advantage_Received_Pusher.push_back("no");
            }
        }
        R.push_back(Results_Pusher);
        Results_Pusher = {};
        R.push_back(Advantage_Received_Pusher);
        Advantage_Received_Pusher = {};
    }

    return R;
}

vector<string> Teams_Gatherer(vector<string> TN, struct Contestant_Data CD){
    // Automatically assign's the first contestant's team as a new one.
    TN.push_back(CD.Stats.at(0).Team);

    // Gathers every present team name.
    for(int i = 0; i < CD.Stats.size(); ++i){
        bool New_Name = true;
        for(int j = 0; j < TN.size(); ++j){
            if((CD.Stats.at(i).Team) == TN.at(j)){
                New_Name = false;
            }
        }
        if(New_Name){
            TN.push_back(CD.Stats.at(i).Team);
        }
    }

    return TN;
}

int Text_Output(vector<Advantage> AT, struct Contestant_Data CD, tuple<vector<vector<unsigned int>>, vector<vector<unsigned int>>> F, vector<vector<string>> R, vector<string> TN, string O){
    // Gets the output file.
    ofstream Results_File (O);
    
    if(!Results_File){
        cout << "CAN'T OPEN OUTPUT!!!!!";
    }

    // Prints out to the output file.

    // For every team name (this sorts everyone into those groups)... 
    for(int i = 0; i < TN.size(); ++i){
        Results_File << "__________________________________________________________________" << endl;
        Results_File << "[TEAM: " << TN.at(i) << "]" << endl << endl;

        // For every contestant...
        for(int j = 0; j < CD.Stats.size(); ++j){
            
            if((CD.Stats.at(j).Team) == TN.at(i)){
                
                // Writes the name of the player.
                Results_File << CD.Stats.at(j).Name << ":" << endl;

                // For every advantage...
                for(int k = 0; k < AT.size(); ++k){

                    // Writes the name of the advantage.
                    Results_File << AT.at(k).Name;

                    // Bracket piece that has the fraction and the percent.
                    Results_File << " [" << get<0>(F).at(k).at(j) << "/" << get<1>(F).at(k).at(j) << " (" << 100 * (float)get<0>(F).at(k).at(j) / (float)get<1>(F).at(k).at(j) << "%); "; // Advantage odds.
                    
                    // Bracket piece that has the results.
                    Results_File << R.at(k * 2).at(j) << " = " << R.at((k * 2) + 1).at(j) << "]" << endl; // Roll results.
                }
                Results_File << endl;
            }
        }
    }

    Results_File.close();
    return 0;
}


// int main()

int main(){

    // ___________________________________________________________________________________________________________________________________________________
    // STUFF FOR YOU TO CUSTOMIZE BELOW!!!
    
    // Input and output files.
    string Input = "ExampleCast.txt";
    string Output = "ExampleResult.txt";
    
    // Declares the possible advantages. The format for each advantage is {Name, Base Denominator}.
    vector<Advantage> Advantage_Types = {
        {"Immunity Idol", 100},
        {"Vote Pass", 50}
    };
    
    // Dictionary that maps the desperation tiers (2nd row of each contestant's slot in the input file) to integers.
    // Base denominator will remain as the denominator post-calculations if a character has desperation tier 0 ("Comfy" in the default case) and no clues for that advantage.
    map<string, float> Desperation = {
        {"Occupied", -2147483648},
        {"Comfy", 0},
        {"Unbothered", 1},
        {"Concerned", 2},
        {"Worried", 3},
        {"Desperate", 4}
    };

    int PPRNG_Seed = 010101;
    // If you want to customize the math formula used for the RNG chances, that's in the function "Chance_Retriever." Press ctrl + f and type "CUSTOM:"


    /*
    DEFAULTS:

    string Input = "ExampleCast.txt";
    string Output = "ExampleResult.txt";
    
    vector<vector<string>> Advantage_Types = {
        {"Immunity Idol", "100"},
        {"Vote Pass", "50"}
    };

    map<string, float> Desperation = {
        {"Occupied", -2147483648},
        {"Comfy", 0},
        {"Unbothered", 1},
        {"Concerned", 2},
        {"Worried", 3},
        {"Desperate", 4}
    };

    int PPRNG_Seed = 010101;
    */


    // STUFF FOR YOU TO CUSTOMIZE ABOVE!!!
    // ___________________________________________________________________________________________________________________________________________________

    struct Contestant_Data Data;
    unsigned int Slot_Size = (Advantage_Types.size() * 2) + Data.Size; // Used for contestant slot sizes. Name = 1 slot, Desperation = 1 slot, Team = 1 slot, 2x = 2 additional slots per advantage (x)
    tuple<vector<vector<unsigned int>>, vector<vector<unsigned int>>> Fractions; // Tuple: Numerators/Denominators;   Sub: Advantages;   Subsub: Players
    vector<vector<string>> Results; // Main: Number + Y/N (Alternates Between the Two);   Sub: Advantage Rolls
    vector<string> Team_Names;

    cout << "Getting contestants..." << endl;
    Data = Contestant_Handler(Slot_Size, Desperation, Input);
    cout << "Doing the math..." << endl;
    Fractions = Chance_Retriever(Advantage_Types, Data, Fractions);
    cout << "Gathering results..." << endl;
    Results = PRNG_Handler(Advantage_Types, Data, Fractions, Results, PPRNG_Seed);
    cout << "Sorting contestants by team..." << endl;
    Team_Names = Teams_Gatherer(Team_Names, Data);
    cout << "Porting to output document..." << endl;
    Text_Output(Advantage_Types, Data, Fractions, Results, Team_Names, Output);
}
