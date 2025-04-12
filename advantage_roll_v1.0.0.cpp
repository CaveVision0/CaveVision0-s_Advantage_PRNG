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





// Structs

struct Slot_Size{
    unsigned int With_Clues;
    unsigned int Without_Clues;
};


struct Splitting_Styles{
    bool Teams;
};


struct Sorting_Styles{
    bool Alphabetical;
    bool Chance;
    bool Verbal_Result;
    bool Is_Ascending; // If true, sorts and splits are sorted in ascending order. Otherwise, they're sorted in descending order.
};


struct Advantage{
    std::string Name;
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
    std::string Verbal;
};


struct Config{
    std::string Input, Output; // Input and output files.
    int Seed, Element_ID; // The RNG seed.
    std::vector<Advantage> Advantages; // Declares the possible advantages. The format for each advantage is {Name, Base Denominator}. Base Denominator must be an integer from 1 to 4294967295. Don't use 0. We all know what happens when you divide by 0.
    std::map<std::string, short int> Desperation; // Map for the desperation tiers (2nd row of each contestant's slot in the input file).
    Sorting_Styles Sorts; // The different ways the list can be sorted.
    Splitting_Styles Splits; // The different ways the list can be sorted.
};


struct Contestant_Stats{
    std::string Name;
    short int Desperation_Value;
    std::string Team;
    std::vector<Clues> Clue;
    std::vector<Fraction> Fractions;
    std::vector<Result> Results;
    unsigned int ID;
};





// Comparators

struct String_Comparator{
    bool Is_Ascending;
    
    bool operator()(std::string a, std::string b){
        return Is_Ascending ? (a < b) : (a > b);
    }

    String_Comparator(bool IA) {
        Is_Ascending = IA;
    }
};


struct Team_Name_Comparator{
    bool Is_Ascending;
    
    bool operator()(Contestant_Stats a, Contestant_Stats b){
        return Is_Ascending ? (a.Team < b.Team) : (a.Team > b.Team);
    }

    Team_Name_Comparator(bool IA) {
        Is_Ascending = IA;
    }
};


struct Name_Comparator{
    bool Is_Ascending;
    
    bool operator()(Contestant_Stats a, Contestant_Stats b){
        return Is_Ascending ? (a.Name < b.Name) : (a.Name > b.Name);
    }

    Name_Comparator(bool IA) {
        Is_Ascending = IA;
    }
};


struct Verbal_Result_Comparator{
    unsigned int Element_ID;
    bool Is_Ascending;
    
    bool operator()(Contestant_Stats a, Contestant_Stats b){
        return Is_Ascending ? (a.Results.at(Element_ID).Verbal < b.Results.at(Element_ID).Verbal) : (a.Results.at(Element_ID).Verbal > b.Results.at(Element_ID).Verbal);
    }

    Verbal_Result_Comparator(bool IA, unsigned int EID) {
        Is_Ascending = IA;
        Element_ID = EID;
    }
};


struct Chance_Comparator{
    unsigned int Element_ID;
    bool Is_Ascending;
    
    bool operator()(Contestant_Stats a, Contestant_Stats b){
        return Is_Ascending ?
        (100 * (float)a.Fractions.at(Element_ID).Numerator / (float)a.Fractions.at(Element_ID).Denominator <
        100 * (float)b.Fractions.at(Element_ID).Numerator / (float)b.Fractions.at(Element_ID).Denominator) :

        (100 * (float)a.Fractions.at(Element_ID).Numerator / (float)a.Fractions.at(Element_ID).Denominator >
        100 * (float)b.Fractions.at(Element_ID).Numerator / (float)b.Fractions.at(Element_ID).Denominator);
    }

    // Parameterized constructor
    Chance_Comparator(bool IA, unsigned int EID) {
        Is_Ascending = IA;
        Element_ID = EID;
    }
};





// Config

// See "struct Config," "struct Sorts," and "struct Splits" above.
Config Config_Basic(Config Conf){
    
    // Input & Output Files
    Conf.Input = "ExampleCast.txt";
    Conf.Output = "ExampleResult.txt";

    // RNG Seed
    Conf.Seed = 010101;

    // Advantages
    Conf.Advantages = {
        {"Hidden Immunity Idol", 100},
        {"Vote Pass", 50}
    };

    // Desperation Tiers
    Conf.Desperation = {
        {"Occupied", -128},
        {"Comfy", 0},
        {"Unbothered", 1},
        {"Concerned", 2},
        {"Worried", 3},
        {"Desperate", 4}
    };

    // Sorts & Splits
    Conf.Sorts.Alphabetical = true;
    Conf.Sorts.Chance = false;
    Conf.Sorts.Verbal_Result = false;

    Conf.Splits.Teams = true;

    Conf.Sorts.Is_Ascending = true;

    Conf.Element_ID = 0; // Used for things like sorting for a specific advantage in the Verbal_Result sorter. A value of 0 would mean Advantage 0 is being used for sorting.

    return Conf;
}


// Allows the individual users to create custom formulas for every advantage.
// To create a new one, create a new case within the switch statement.
// Advantage IDs range from 0 to (number of different advantage types - 1). In the default advantage settings, case 0 is the Hidden Immunity Idol and case 1 is the vote pass.
Fraction Config_Formulas(Fraction Pusher, unsigned int Advantage_ID, unsigned int Base_Denominator, unsigned int Real_Clues, unsigned int Fake_Clues, unsigned int Desperation_Tier){
    switch(Advantage_ID){
        default:
            Pusher.Numerator = (int)(1000 * pow(Real_Clues + 1, 2));
            Pusher.Denominator = (int)round(1000 * Base_Denominator * pow(Fake_Clues + 1, 2) * pow(0.25, 0.25 * Desperation_Tier));
    }

    return Pusher;
}





// Functions

std::vector<Contestant_Stats> Contestant_Handler(std::vector<Contestant_Stats> Contestants, Slot_Size Size, std::map<std::string, short int> Desperation, std::string Input){
    Contestant_Stats Current_Contestant;
    Clues Current_Clues;
    std::string line;
    std::vector<unsigned int> Clues_Pusher;
    bool Counting = false; // Whether or not Counter is actually counting.
    unsigned int Counter = 0; // Counts up once per non-empty line. Used to know when to begin a new slot.
    unsigned int ID = 0; // Contestant ID numbers.

    // Opens the input file.
    std::ifstream Contestant_List (Input);
    
    // If the list can't open, the string listed in the loop will print out and nothing will be returned.
    if(!Contestant_List){
        std::cout << "CAN'T OPEN INPUT!!!!!";
        return Contestants;
    }

    while(getline(Contestant_List, line)){
        if(line.find("__END") != std::string::npos){
            Counting = false;
        }

        // If the list scanning has begun, this counts the amount of lines that have been read.
        if(Counting && !line.empty()){
            ++Counter;
        }

        // Detects the phrase "__START" within the text document. Once it's found, the line counting begins.
        if(line.find("__START") != std::string::npos){
            Counting = true;
        }

        ID = floor(Counter / Size.With_Clues);

        // Name detector.
        if(Counter % Size.With_Clues == 1 && !line.empty()){
            Current_Contestant.Name = line;
            Current_Contestant.ID = ID;
        }

        // Desperation tier detector.
        else if(Counter % Size.With_Clues == 2 && !line.empty()){ 

            // Exception handling for invalid desperation tiers. Invalid tiers become "Unbothered" (AKA 1).
            try{
                Current_Contestant.Desperation_Value = Desperation.at(line);
            }
            catch(const std::out_of_range& ex){
                Current_Contestant.Desperation_Value = 1;
                std::cout << "Conversion Error: " << Current_Contestant.Name << " has an invalid desperation tier! Invalid tier will be automatically replaced with \"Unbothered\" (AKA 1). All clue counts are set to 0 as well." << std::endl;
            }
        }

        // Team detector.
        else if(Counter % Size.With_Clues == 3 && !line.empty()){
            Current_Contestant.Team = line;
        }

        // Clue Detector.
        else if(Counting && Counter > 0 && (Counter % Size.With_Clues > Size.Without_Clues || Counter % Size.With_Clues == 0) && !line.empty()){
            if((Counter % Size.With_Clues) % 2 == (Size.Without_Clues + 1) % 2 && Counter % Size.With_Clues != 0){

                // Exception handling for non-integer clue counts. Invalid values become 0s.
                try{
                    Current_Clues.Real = stoi(line);
                }
                catch(const std::invalid_argument& ex){
                    std::cout << "Conversion Error: " << Current_Contestant.Name << " has at least 1 non-integer real clue counter! Invalid slot(s) will be automatically filled with 0s." << std::endl;
                    Current_Clues.Real = 0;
                }
                catch(const std::out_of_range& ex){
                    std::cout << "Conversion Error: " << Current_Contestant.Name << " has at least 1 real clue counter that's out of range! Invalid slot(s) will be automatically filled with 0s." << std::endl;
                    Current_Clues.Real = 0;
                }
            }
            else{
                
                // Exception handling for non-integer clue counts. Invalid values become 0s.
                try{
                    Current_Clues.Fake = stoi(line);
                }
                catch(const std::invalid_argument& ex){
                    std::cout << "Conversion Error: " << Current_Contestant.Name << " has at least 1 non-integer fake clue counter! Invalid slot(s) will be automatically filled with 0s." << std::endl;
                    Current_Clues.Fake = 0;
                }
                catch(const std::out_of_range& ex){
                    std::cout << "Conversion Error: " << Current_Contestant.Name << " has at least 1 fake clue counter that's out of range! Invalid slot(s) will be automatically filled with 0s." << std::endl;
                    Current_Clues.Fake = 0;
                }
                Current_Contestant.Clue.push_back(Current_Clues);
            }

            if(Counter % Size.With_Clues == 0){
                Contestants.push_back(Current_Contestant);
                Current_Contestant = {}; 
            }
        }

        // If a slot ends early via empty line, this sets the default values.
        if(Counter % Size.With_Clues != 0 && (Counting && (line.empty() || line == "__END"))){
            
            // Sets the default desperation tier to "Unbothered" if a slot cuts off after the name.
            if(Counter % Size.With_Clues == 1){
                Current_Contestant.Desperation_Value = 1;
                ++Counter;
            }

            // Sets the default team to "No Team" if a slot cuts off after the desperation tier.
            if(Counter % Size.With_Clues == 2){
                Current_Contestant.Team = "No Team";
                ++Counter;
            }

            // Fills all post-space clue slots with zeroes.
            if(Counter % Size.With_Clues >= Size.Without_Clues){
                for(int i = Counter % Size.With_Clues; i < Size.With_Clues; ++i){
                    if((Counter % Size.With_Clues) % 2 == (Size.Without_Clues + 1) % 2){
                        Current_Clues.Real = 0;
                    }
                    else{
                        Current_Clues.Fake = 0;
                        Current_Contestant.Clue.push_back(Current_Clues);
                    }
                    ++Counter;
                }
            }

            if(Counter % Size.With_Clues == 0){
                Contestants.push_back(Current_Contestant);
                Current_Contestant = {}; 
            }
        }
    }

    Contestant_List.close();
    return Contestants;
}


std::vector<Contestant_Stats> Chance_Retriever(std::vector<Advantage> Advantages, std::vector<Contestant_Stats> Contestants){
    Fraction Fraction_Pusher;
    unsigned int Factor; // Used to simplify fractions.
    unsigned int Value, d, r, f;
    int t;

    // For every player...
    for(int i = 0; i < Contestants.size(); ++i){

        // Player i's desperation tier value.
        t = Contestants.at(i).Desperation_Value;

        // For every player...
        for(int j = 0; j < Advantages.size(); ++j){

            // The base denominator of advantage j.
            d = Advantages.at(j).Base_Denominator;

            // Player i's real clues for advantage j.
            r = Contestants.at(i).Clue.at(j).Real;

            // Player i's fake clues for advantage j.
            f = Contestants.at(i).Clue.at(j).Fake;

            // Gets the fraction using each individual advantage's formula.
            Contestants.at(i).Fractions.push_back(Config_Formulas(Fraction_Pusher, i, d, r, f, t));
        }
    }

    // For every player...
    for(int i = 0; i < Contestants.size(); ++i){
        
        // For every advantage...
        for(int j = 0; j < Contestants.at(0).Fractions.size(); ++j){

            // Simplifies the fractions.
            Factor = std::gcd(Contestants.at(i).Fractions.at(j).Numerator, Contestants.at(i).Fractions.at(j).Denominator);
            Contestants.at(i).Fractions.at(j).Numerator /= Factor;
            Contestants.at(i).Fractions.at(j).Denominator /= Factor;
        }
    }

    return Contestants;
}


std::vector<Contestant_Stats> PRNG_Handler(std::vector<Advantage> Advantages, std::vector<Contestant_Stats> Contestants, int Seed){
    Result Result_Pusher;
    std::mt19937 PRNG_0; // The random number generater. Who doesn't love a good mt19937?
    PRNG_0.seed(Seed);

    for(int i = 0; i < Contestants.size(); ++i){
        for(int j = 0; j < Advantages.size(); ++j){
            
            // The PRNG.
            Result_Pusher.Numerical = (PRNG_0() % Contestants.at(i).Fractions.at(j).Denominator) + 1;
            
            // If x is less than or equal to the corresponding numerator, it's a "YES."
            if(Result_Pusher.Numerical <= Contestants.at(i).Fractions.at(j).Numerator){
                Result_Pusher.Verbal = "YES";
            }
            else{
                Result_Pusher.Verbal = "no";
            }

            Contestants.at(i).Results.push_back(Result_Pusher);
        }
    }

    return Contestants;
}


std::vector<std::string> Team_Gatherer(std::vector<std::string> Teams, std::vector<Contestant_Stats> Contestants){
    bool New_Name;

    // Automatically assign's the first contestant's team as a new one.
    Teams.push_back(Contestants.at(0).Team);

    // Gathers every present team name.
    for(int i = 0; i < Contestants.size(); ++i){
        
        New_Name = true;
        
        for(int j = 0; j < Teams.size(); ++j){
            if(Contestants.at(i).Team == Teams.at(j)){
                New_Name = false;
            }
        }

        if(New_Name){
            Teams.push_back(Contestants.at(i).Team);
        }
    }

    return Teams;
}


std::vector<std::string> Splitter(Sorting_Styles Sorts, std::vector<std::string> Teams){
    
    if(Sorts.Alphabetical){
        std::sort(Teams.begin(), Teams.end(), String_Comparator(Sorts.Is_Ascending));
    }

    return Teams;
}


std::vector<Contestant_Stats> Sorter(Sorting_Styles Sorts, std::vector<Contestant_Stats> Contestants, unsigned int Element_ID){
    if(Sorts.Alphabetical){
        sort(Contestants.begin(), Contestants.end(), Name_Comparator(Sorts.Is_Ascending));
    }

    if(Sorts.Verbal_Result){
        sort(Contestants.begin(), Contestants.end(), Verbal_Result_Comparator(Sorts.Is_Ascending, Element_ID));
    }

    if(Sorts.Chance){
        sort(Contestants.begin(), Contestants.end(), Chance_Comparator(Sorts.Is_Ascending, Element_ID));
    }

    return Contestants;
}


void Base_Text_Output(std::vector<Advantage> Advantages, std::vector<Contestant_Stats> Contestants, std::string Output, int j){
    
    // Gets the output file.
    std::ofstream Results_File (Output, std::ios::app);
    
    // If the output file can't open, this says so.
    if(!Results_File){
        std::cout << "CAN'T OPEN OUTPUT!!!!!";
    }

    Results_File << Contestants.at(j).Name << ":" << std::endl;

    // For every advantage...
    for(int k = 0; k < Advantages.size(); ++k){

        // Writes the name of the advantage.
        Results_File << Advantages.at(k).Name;

        // Bracket piece that has the fraction and the percent.
        Results_File << " [" << Contestants.at(j).Fractions.at(k).Numerator << "/" << Contestants.at(j).Fractions.at(k).Denominator << " (" << 100 * (float)Contestants.at(j).Fractions.at(k).Numerator / (float)Contestants.at(j).Fractions.at(k).Denominator << "%); "; // Advantage odds.
        
        // Bracket piece that has the results (YES/no).
        Results_File << Contestants.at(j).Results.at(k).Numerical << " = " << Contestants.at(j).Results.at(k).Verbal << "]" << std::endl; // Roll results.
    }
    Results_File << std::endl;
}


void Text_Output(std::vector<Advantage> Advantages, std::vector<Contestant_Stats> Contestants, std::vector<std::string> Teams, std::string Output, Config Conf){

    // Gets the output file.
    std::ofstream Results_File (Output, std::ios::app);
    
    // If the output file can't open, this says so.
    if(!Results_File){
        std::cout << "CAN'T OPEN OUTPUT!!!!!";
    }

    // Prints out to the output file.

    if(Conf.Splits.Teams){
        // For every team name (this sorts everyone into those groups)... 
        for(int i = 0; i < Teams.size(); ++i){

            // Prints the team names.
            Results_File << "__________________________________________________________________" << std::endl;
            Results_File << "[TEAM: " << Teams.at(i) << "]" << std::endl << std::endl;

            // For each player...
            for(int j = 0; j < Contestants.size(); ++j){

                // If player j's team matches the current team...
                if(Contestants.at(j).Team == Teams.at(i)){
                    Base_Text_Output(Advantages, Contestants, Output, j);
                }
            }
        }
    }

    else{
        // For each player...
        for(int j = 0; j < Contestants.size(); ++j){
            Base_Text_Output(Advantages, Contestants, Output, j);
        }
    }

    Results_File.close();
}





// int main()

int main(){
    using namespace std::chrono; // Lets me use the features chrono has.
    auto start = high_resolution_clock::now(); // Tests how long the program takes to work. This is the starting point.

    // Variables, structs and vectors.
    Config Conf;
    std::vector<Contestant_Stats> Contestants;
    std::vector<std::string> Team_Names;
    
    // Gets the config stuff you set.
    Conf = Config_Basic(Conf);

    Slot_Size Size;
    Size.Without_Clues = 3;
    Size.With_Clues = (Conf.Advantages.size() * 2) + Size.Without_Clues; // Used for contestant slot sizes. Name = 1 slot, Desperation = 1 slot, Team = 1 slot, 2x = 2 additional slots per advantage (x)

    // Where the magic happens
    std::cout << "Getting contestants..." << std::endl;
    Contestants = Contestant_Handler(Contestants, Size, Conf.Desperation, Conf.Input);
    std::cout << "Doing the math..." << std::endl;
    Contestants = Chance_Retriever(Conf.Advantages, Contestants);
    std::cout << "Gathering results..." << std::endl;
    Contestants = PRNG_Handler(Conf.Advantages, Contestants, Conf.Seed);

    if(Conf.Splits.Teams){
        std::cout << "Splitting list contents into teams..." << std::endl;
        Team_Names = Team_Gatherer(Team_Names, Contestants);
        Team_Names = Splitter(Conf.Sorts, Team_Names);
    }

    std::cout << "Sorting..." << std::endl;
    Contestants = Sorter(Conf.Sorts, Contestants, Conf.Element_ID);

    // Empties the output file for overriding.
    std::ofstream Reset(Conf.Output);
    Reset.close();

    std::cout << "Porting to output document..." << std::endl;
    Text_Output(Conf.Advantages, Contestants, Team_Names, Conf.Output, Conf);
    // We're done!!!

    auto stop = high_resolution_clock::now(); // Tests how long the program takes to work. This is the stopping point. This isn't needed for anything, I just wanted to see how fast it runs.
    auto duration = duration_cast<microseconds>(stop - start);
    float seconds = (float)duration.count() / 1000000;
    std::cout << "Program duration: " << seconds << " seconds" << std::endl;
}