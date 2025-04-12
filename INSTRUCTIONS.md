# HOW TO USE:

# Input/Output Text Files:

You'll need to create two text files **within the same folder the program is in**. One is for the input and the other is for the output. An example of an input (ExampleCast.txt) is already provided, as is an example of a previously-generated output (ExampleResult.txt).

Once you have your input doc, make your first line "__START" and your final line "__END" (quotation marks not included). This is used by the program to mark when it should start (__START) and stop (__END) gathering the contestant data. It's not a necessity that these are the very first/last lines and stuff can exist above __START or below __END, but the contestant data has to be between them.

Between the __START and __END lines, create your cast by repeating the format below until every contestant has their own slot (ExampleCast.txt has an example of this should you want to see it). **Ignore everything that isn't inside double square brackets**:

1. [[Name]] // STRING
2. [[Desperation Tier]] // SPECIFIC STRINGS (see advantage_roll's "Config_Basic()" function)
3. [[Team Name]] // STRING
4. [[Real Clues for Advantage 1]] // UNSIGNED INT (0 to 4294967295)
5. [[Fake Clues for Advantage 1]] // UNSIGNED INT (0 to 4294967295)
6. [[Real Clues for Advantage 2]] // UNSIGNED INT (0 to 4294967295)
7. [[Fake Clues for Advantage 2]] // UNSIGNED INT (0 to 4294967295)
8. Whitespace for Contestant Separation    EMPTY STRING

Notes:
1. The amount of these unsigned int rows varies depending on how many advantages you have. Create 2 for each advantage. If you plan on having one advantage, you can delete the rows stated to be for advantage 2.

You're done with your input document now.


# Config_Basic()

Open up advantage_roll and head to "config Config_Basic(), which is right below the comparators." As soon as you're there, you'll be met the things you can customize within the program itself.

First, you'll replace std::string Input and std::string Output's files with your own. For example, if your input document is called This_Is_My_Doc.txt, you'll replace ExampleCast.txt with that.

Next, there's int seed which is for the PRNG to use. Each unique seed will cause the program to create different results.

std::vector<Advantage> Advantages can be used to create your own advantages. E.g., if you want an advantage called "Funny Almond Water" with a base denominator (more info later) of 32, you can insert a comma right by the last item listed and insert {"Funny Almond Water", 32} after said last item. Make sure your advantage is inside the curly brackets. The last item in the list can't have a comma at the end of it.

std::map<std::string, short int> Desperation can be used to create your own desperation tiers. It follows the same syntax rules as std::vector<Advantage> Advantages. The integer you insert will be used in the math formula used to find the chance of finding a given advantage. Said formula is also customizable.

The sorts listed allow the user to sort the results however they want. To sort the results in any way, set the value of the sort's associated bool to true. Is_Ascending is used to determine whether to display the results in ascending or descending order. True means ascending and false means descending.


# Config_Formulas()

Remember how I mentioned the base denominator each advantage has? This is where it comes into play. You can customize the formula used to calculate the odds should you want to. You can even have a different formula for each unique advantage. To do so, insert a case into the switch statement with the number corresponding to the desired advantage's position within std::vector<Advantage> Advantages, starting with 0. Look up case and switch statements if you're unsure what I mean.

The amount of calculations done is (Contestant Count * Advantage Count), with one done for each combination.
Advantage_ID = The current advantage.
Base_Denominator = The current advantage's base denominator. This is set within std::vector<Advantage> Advantages.
Real_Clues = The number of real clues the current contestant has.
Fake_Clues = The number of fake clues the current contestant has.
Desperation_Tier = The current contestant's desperation tier.

# The End

That about covers it. Make sure to tell me if you see any bugs and have fun!
