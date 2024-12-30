# HOW TO USE:

**Input/Output Text Files:**

You'll need to create two text files. One is for the input and the other is for the output. An example of an input (ExampleCast.txt) is already provided, as is an example of a previously-generated output (ExampleResult.txt).

Once you have your input doc, make your first line "__START" and your final line "__END" (quotation marks not included). This is used by the program to mark when it should start (__START) and stop (__END) gathering the contestant data. It's not a necessity that these are the very first/last lines and stuff can exist above __START or below __END, but the contestant data has to be between them.

Between the __START and __END lines, create your cast by repeating the format below until every contestant has their own slot (ExampleCast.txt has an example of this should you want to see it). **Ignore everything that isn't inside double square brackets**:

1. [[Name]]    STRING
2. [[Desperation Tier]]    SPECIFIC STRINGS (see advantage_roll.cpp's "map<string, int> Desperation" for the strings; found in int main())
3. [[Team Name]]    STRING
4. [[Real Clues for Advantage 1]]    INT
5. [[Fake Clues for Advantage 1]]    INT
6. [[Real Clues for Advantage 2]]    INT
7. [[Fake Clues for Advantage 2]]    INT (The number of integers below the desperation tier may vary depending on how many advantages you have. Go for (#Advantage_Types * 2).)
8. Empty Line for Separation    STRING

You're done with your input document now.


**advantage_roll.cpp main int() Customization**

Open up advantage_roll.cpp and head to int main(). As soon as you're there, you'll be met with most of the things you can customize.

First, you'll replace string Input and string Output's files with your own. For example, if your input document is called This_Is_My_Doc.txt (file format is required), you'll replace ExampleCast.txt with that.

After that, there's vector<vector<string>> Advantage_Types. New advantages should be formatted like this {"Advantage Name", "Base Denominator Number"} and placed inside the outer bracket, just as it appears in advantage_roll.cpp. If you want more than one advantage, just place a comma between each of these vectors like this: {"Advantage Name 1", "Base Denominator Number 1"}, {"Advantage Name 2", "Base Denominator Number 2"}.

Lastly, there's the desperation map. Here, you can create custom tier names and set their values. For example, if you want a tier named "AMAZING" and have its value be 5, you can add {"AMAZING", 5} into the map. **Be sure not to forget the comma at the end if this tier isn't the last one listed!**


**Formula Customization**

From there, if you press ctrl + f and type in "CUSTOM", you will see 2 results located in the Chance_Retriever function. This is where you want to be if you want to change the formula for the fractions' numerators and/or denominators. The program itself has explanations for the variables, functions, etc. you'll see in there and even provides 2 examples of custom formulas.

_Note: The variables seen below do not share the same names as the ones in the actual program. This is just a demonstration of my formula._
The formula that I created for this program is ((r + 1) ^ 2) / (d * ((f + 1) ^ 2) * (0.25 ^ (0.25t))), with both sides of the calculated fraction multiplied by 1000 then rounded to the nearest integer.

1. r = The amount of real clues a contestant is currently using. This increases the chance of the idol being found by the user.
2. d = The base denominator. Each advantage has its own d value. Assuming any given contestant has no chance modifiers of any kind and a desperation value of 0, they have a 1 in d chance of finding the advantage the d value represents.
3. f = The amount of fake clues a contestant is currently using. This decreases the chance of the idol being found by the user.
4. t = The desperation tier a contestant has at that moment. The values for each tier are listed besides their titles.
