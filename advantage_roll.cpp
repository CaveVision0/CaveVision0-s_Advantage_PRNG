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

using namespace std;

// TO DO LIST:
// Additional data for whether or not there's only one in the entire game or if there's one per team.
// The RTF will acknowledge if more than one person finds an advantage.
// RTF text formatting.

vector<vector<string>> Contestant_Handler(vector<vector<string>> CS, vector<vector<unsigned int>> CC, unsigned int SS, map<string, int> D, string I){
    string line;
    vector<string> Stats_Pusher = {}; // Used to push the contestant stat subvectors into the main vector.
    bool Counting = false; // Whether or not Counter is actually counting.
    unsigned int Counter = 0; // Counts up once per non-empty line. Used to know when to begin a new slot.

    // Pulls up a .txt file listing every remaining contestant.
    ifstream Contestant_List (I);
    
    // If the list can't open, the string listed in the loop will print out and nothing will be returned.
    if(!Contestant_List){
        cout << "CAN'T OPEN!!!!!";
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

        // Name detector. Slot #1 of the Contestant_Stats sub-vector (or whatever it's called, I'm tired).
        if(Counter % SS == 1 && !line.empty()){
            Stats_Pusher.push_back(line);
        }

        // Desperation tier detector. Slot #2 of the aforementioned sub-vectors.
        else if(Counter % SS == 2 && !line.empty()){ 
            // Exception handling for invalid desperation tiers. Invalid tiers become "Unbothered" (AKA 1).
            try{
                Stats_Pusher.push_back(to_string(D.at(line)));
            }
            catch(const out_of_range& ex){
                Stats_Pusher.push_back("1");
                cout << "Conversion Error: " << Stats_Pusher.at(0) << " has an invalid desperation tier! Invalid tier will be automatically replaced with \"Unbothered\" (AKA 1). All clue counts are set to 0 as well." << endl;
            }
        }

        // Team detector. Slot #3 of the aforementioned sub-vectors.
        else if(Counter % SS == 3 && !line.empty()){
            Stats_Pusher.push_back(line);
            CS.push_back(Stats_Pusher);
            Stats_Pusher = {};
        }

        // If a slot ends early via empty line, this sets the default values.
        if(Counting && line.empty() || line == "__END"){
            
            // Sets the default desperation tier to "Unbothered" if a slot cuts off after the name.
            if(Counter % SS == 1){
                Stats_Pusher.push_back("1");
                Stats_Pusher.push_back("No Team");
                CS.push_back(Stats_Pusher);
                Stats_Pusher = {};
            }
            else if(Counter % SS == 2){
                Stats_Pusher.push_back("No Team");
                CS.push_back(Stats_Pusher);
                Stats_Pusher = {};
            }

            if(Counter % SS > 0){
                Counter += (SS - (Counter % SS));
            }
        }
    }

    Contestant_List.close();
    return CS;
}

vector<vector<unsigned int>> Clue_Handler(vector<vector<string>> CS, vector<vector<unsigned int>> CC, unsigned int SS, string I){
    string line;
    vector<unsigned int> Clues_Pusher = {}; // Used to push the clue counter subvectors into the main vector.
    bool Counting = false; // Whether or not Counter is actually counting.
    unsigned int Counter = 0; // Counts up once per non-empty line. Used to know when to begin a new slot.
    unsigned int ID = 0; // Contestant ID numbers.

    // Pulls up a .txt file listing every remaining contestant.
    ifstream Contestant_List (I);
    
    // If the list can't open, the string listed in the loop will print out and nothing will be returned.
    if(!Contestant_List){
        cout << "CAN'T OPEN!!!!!";
        return CC;
    }

    while(getline(Contestant_List, line)){

        ID = floor(Counter / SS);

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

        // CLUES
        if(Counting && Counter > 0 && (Counter % SS > 3 || Counter % SS == 0) && !line.empty()){
            // Exception handling for non-integer clue counts. Invalid values become 0s.
            try{
                Clues_Pusher.push_back(stoul(line));
            }
            catch(const invalid_argument& ex){
                cout << "Conversion Error: " << CS.at(ID - 1).at(0) << " has at least 1 non-integer clue counter! Invalid slot(s) will be automatically filled with 0s." << endl;
                Clues_Pusher.push_back(0);
            }
            catch(const out_of_range& ex){
                cout << "Conversion Error: " << CS.at(ID - 1).at(0) << " has at least 1 clue counter that's out of range! Invalid slot(s) will be automatically filled with 0s." << endl;
                Clues_Pusher.push_back(0);
            }
        }

        if(Counting && Counter > 0 && Counter % SS == 0 && !line.empty()){
            CC.push_back(Clues_Pusher);
            Clues_Pusher = {};
        }
        
        // If a slot ends early via empty line, this sets the default values.
        if(line.empty() || line == "__END"){
            
            // Fills all post-space clue slots with zeroes.
            if(Counter % SS > 3){
                for(int i = Counter % SS; i < SS; ++i){
                    Clues_Pusher.push_back(0);
                }
                CC.push_back(Clues_Pusher);
                Clues_Pusher = {};
            }

            if(Counter % SS > 0){
                Counter += (SS - (Counter % SS));
            }
        }
    }

   Contestant_List.close();

   return CC;
}

vector<vector<vector<unsigned int>>> Chance_Retriever(vector<vector<string>> AT, vector<vector<string>> CS, vector<vector<unsigned int>> CC, vector<vector<vector<unsigned int>>> F){
    vector<unsigned int> Base_Denominator; // Base denominator for each advantage.
    vector<vector<unsigned int>> True_Numerator; // The numerator in the finished fraction.
    vector<vector<unsigned int>> True_Denominator; // The denominator in the finished fraction.
    vector<unsigned int> Numerator_Pusher;
    vector<unsigned int> Denominator_Pusher;
    int Factor; // Used to simplify fractions.
    int x;

    // Converts the base denominator slots from int main()'s vectors into integers. Includes exception handling which defaults to 100.

    for(int i = 0; i < AT.size(); ++i){
        try{
            Base_Denominator.push_back(stod(AT.at(i).at(1)));
        }
        catch(const invalid_argument& ex){
            cout << "Conversion Error: " << AT.at(i).at(0) << "'s base denominator isn't an integer! Defaulting to 100." << endl;
            Base_Denominator.push_back(100);
        }
        catch(const out_of_range& ex){
            cout << "Conversion Error: " << AT.at(i).at(0) << "'s base denominator is out of range! Defaulting to 100." << endl;
            Base_Denominator.push_back(100);
        }
        
    }

    // For every advantage...
    for(int i = 0; i < Base_Denominator.size(); ++i){
        
        // For every player...
        for(int j = 0; j < CS.size(); ++j){
            /*
            // #include<cmath> functions for clarity:
                // round(x) = Rounds x to the nearest integer.
                // pow(x, y) = x to the power of y (AKA x^y).
            */

            /*
            // Variable definitions for clarity:
                // CC.at(j).at(2*i) = Player j's clues. Pulled from "int main()"'s Clue_Counts. The two groups aren't in separate vectors from each other, instead alternating.
                    // 2*i consists of real clues of advantage i.
                    // (2*i) + 1 consist of fake ones of advantage i.
                // Base_Denominator.at(i) = The base denominator of advantage i. This, in turn, is pulled from "int main()"".
                // stoi(CS.at(j).at(1)) = The desperation tier of player j.
                    // In a previous function, each contestant's desperation tier was converted into the number it represents then turned back into a string. stoi() converts that string back into an integer so that we can do math with it.
            */

            /*
            Example Formulas:
                Chances are multiplied by clue count (progression is 1, 2, 3, 4, and so on):
                    x = 1 + CC.at(j).at(2 * i);
                    Numerator_Pusher.push_back(x);
                    x = Base_Denominator.at(i) * (1 + CC.at(j).at(2 * i)) * (0.25 * stoi(CS.at(j).at(1)));
                    Denominator_Pusher.push_back(x);

                Every real clue doubles the chance while every fake clue halves it (progression is 1, 2, 4, 8, and so on):
                    x = pow(2, CC.at(j).at(2 * i));
                    Numerator_Pusher.push_back(x);
                    x = Base_Denominator.at(i) * (1 + CC.at(j).at((2 * i) + 1)) * (0.25 * stoi(CS.at(j).at(1)));
                    Denominator_Pusher.push_back(x);
            */

            // CUSTOM: This line of code controls the numerator part of the fraction that defines odds of finding advantage i.
            // DEFAULT: round(1000 * (pow(CC.at(j).at(2 * i) + 1, 2)));
            x = round(1000 * (pow(CC.at(j).at(2 * i) + 1, 2)));
            Numerator_Pusher.push_back(x);

            // CUSTOM: This line of code controls the denominator part of the fraction that defines odds of finding advantage i.
            // DEFAULT: round(1000 * Base_Denominator.at(i) * pow(CC.at(j).at((2 * i) + 1) + 1, 2) * pow(0.25, (0.25 * stoi(CS.at(j).at(1)))));
            x = round(1000 * Base_Denominator.at(i) * pow(CC.at(j).at((2 * i) + 1) + 1, 2) * pow(0.25, (0.25 * stoi(CS.at(j).at(1)))));
            Denominator_Pusher.push_back(x);
        }

        // Pushes the players' subvectors into the advantages' ones.
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

    F.push_back(True_Numerator);
    F.push_back(True_Denominator);
    True_Numerator = {};
    True_Denominator = {};
    return F;
}

vector<vector<string>> PRNG_Handler(vector<vector<string>> AT, vector<vector<string>> CS, vector<vector<unsigned int>> CC, vector<vector<vector<unsigned int>>> F, vector<vector<string>> R, int S){
    vector<vector<string>> Pusher_2D;
    vector<string> Results_Pusher;
    vector<string> Advantage_Received_Pusher;
    vector<unsigned int> Factor; // Used to simplify fractions.
    int x;
    mt19937 PRNG_0; // The random number generater. Who doesn't love a good mt19937?


    // Seed will be XXYYZZ. X is the season number, Y is the episode, and Z is the nth time people are able to search this episode.
    // For example, the first searching session of season 1, episode 1 is 010101.
    PRNG_0.seed(S);

    // PRNG
    for(int i = 0; i < F.at(0).size(); ++i){
        for(int j = 0; j < F.at(0).at(0).size(); ++j){
            
            // The true PRNG.
            int x = (PRNG_0() % F.at(1).at(i).at(j)) + 1;
            Results_Pusher.push_back(to_string(x));
            
            // If x is less than or equal to the corresponding numerator, it's a "YES."
            if(x <= F.at(0).at(i).at(j)){
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

vector<string> Teams_Gatherer(vector<string> TN, vector<vector<string>> CS){
    // Automatically assign's the first contestant's team as a new one.
    TN.push_back(CS.at(0).at(2));

    // Gathers every present team name.
    for(int i = 0; i < CS.size(); ++i){
        bool New_Name = true;
        for(int j = 0; j < TN.size(); ++j){
            if(CS.at(i).at(2) == TN.at(j)){
                New_Name = false;
            }
        }
        if(New_Name){
            TN.push_back(CS.at(i).at(2));
        }
    }

    return TN;
}

int Text_Output(vector<vector<string>> AT, vector<vector<string>> CS, vector<vector<vector<unsigned int>>> F, vector<vector<string>> R, vector<string> TN, string O){
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
        for(int j = 0; j < CS.size(); ++j){
            
            if(CS.at(j).at(2) == TN.at(i)){
                
                // Writes the name of the player.
                Results_File << CS.at(j).at(0) << ":" << endl;

                // For every advantage...
                for(int k = 0; k < AT.size(); ++k){

                    // Writes the name of the advantage,
                    Results_File << AT.at(k).at(0);

                    // Bracket piece that has the fraction and the percent.
                    Results_File << " [" << F.at(0).at(k).at(j) << "/" << F.at(1).at(k).at(j) << " (" << 100 * (float)F.at(0).at(k).at(j) / (float)F.at(1).at(k).at(j) << "%); "; // Advantage odds.
                    
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

int main(){
    
    // STUFF FOR YOU TO CUSTOMIZE BELOW!!!
    // STUFF FOR YOU TO CUSTOMIZE BELOW!!!
    
    /*
    DEFAULT:
        string Input = "ExampleCast.txt";
        string Output = "ExampleResult.txt";
    */
   // Input and output files.
    string Input = "ExampleCast.txt";
    string Output = "ExampleResult.txt";

    /*
    DEFAULT:
        vector<vector<string>> Advantage_Types = {{"Immunity Idol", "100"}, {"Vote Pass", "50"}};
    */
   // Declares the possible advantages. The format for each advantage is {Name, Base Denominator}.
    vector<vector<string>> Advantage_Types = {{"Immunity Idol", "100"}, {"Vote Pass", "50"}}; // Advantage and base denominator list.

    /*
    DEFAULT:
        map<string, int> Desperation = {
            {"Occupied", -2147483648},
            {"Comfy", 0},
            {"Unbothered", 1},
            {"Concerned", 2},
            {"Worried", 3},
            {"Desperate", 4}
        };
    */
    // Dictionary that maps the desperation tiers (2nd row of each contestant's slot in the input file) to integers.
    // Base denominator will remain as the denominator post-calculations if a character has desperation tier 0 ("Comfy") and no clues.
    map<string, int> Desperation = {
        {"Occupied", -2147483648},
        {"Comfy", 0},
        {"Unbothered", 1},
        {"Concerned", 2},
        {"Worried", 3},
        {"Desperate", 4}
    };

    /*
    DEFAULT:
        int PPRNG_Seed = 010101;
    */
    int PPRNG_Seed = 010101;
    // If you want to customize the math formula used for the RNG chances, that's in the function "Chance_Retriever." Press ctrl + f and type "CUSTOM."

    // STUFF FOR YOU TO CUSTOMIZE ABOVE!!!
    // STUFF FOR YOU TO CUSTOMIZE ABOVE!!!
    
    unsigned int Slot_Size = (Advantage_Types.size() * 2) + 3; // Used for contestant slot sizes. Name = 1 slot, Desperation = 1 slot, 2x = 2 additional slots per advantage (x).
    vector<vector<string>> Contestant_Stats; // 
    vector<vector<unsigned int>> Clue_Counts; // Main: Players;   Sub: Clue Counts (all grouped together; evens are real, odds are fake).
    vector<vector<vector<unsigned int>>> Fractions; // Main: Numerators/Denominators;   Sub: Advantages;   Subsub: Players
    vector<vector<string>> Results; // Main: Number + Y/N (Alternates Between the Two);   Sub: Advantage Rolls
    vector<string> Team_Names;

    cout << "Getting contestants..." << endl;
    Contestant_Stats = Contestant_Handler(Contestant_Stats, Clue_Counts, Slot_Size, Desperation, Input);
    cout << "Getting clue counters..." << endl;
    Clue_Counts = Clue_Handler(Contestant_Stats, Clue_Counts, Slot_Size, Input);
    cout << "Doing the math..." << endl;
    Fractions = Chance_Retriever(Advantage_Types, Contestant_Stats, Clue_Counts, Fractions);
    cout << "Gathering results..." << endl;
    Results = PRNG_Handler(Advantage_Types, Contestant_Stats, Clue_Counts, Fractions, Results, PPRNG_Seed);
    cout << "Sorting contestants by team..." << endl;
    Team_Names = Teams_Gatherer(Team_Names, Contestant_Stats);
    cout << "Porting to output document..." << endl;
    Text_Output(Advantage_Types, Contestant_Stats, Fractions, Results, Team_Names, Output);
}
