# HOW TO USE:

**Input/Output Text Files:**

You'll need to create two text files. One is for the input and the other is for the output. An example of an input (ExampleCast.txt) is already provided, as is an example of a previously-generated output (ExampleResult.txt).

Once you have your input doc, make your first line "__START" and your final line "__END" (quotation marks not included). This is used by the program to mark when it should start (__START) and stop (__END) gathering the contestant data. It's not a necessity that these are the very first/last lines and stuff can exist above __START or below __END, but the contestant data has to be between them.

Between the __START and __END lines, create your cast by repeating the format below until every contestant has one (ExampleCast.txt has an example of this should you want to see it). **Ignore everything that isn't inside double square brackets**:

1. [[Name]]    STRING
2. [[Desperation Tier]]    SPECIFIC STRINGS (see advantage_roll.cpp's "map<string, int> Desperation" for the strings; found in int main())
3. [[Team Name]]    STRING
4. [[Real Clues for Advantage 1]]    INT
5. [[Fake Clues for Advantage 1]]    INT
6. [[Real Clues for Advantage 2]]    INT
7. [[Fake Clues for Advantage 2]]    INT (The number of integers below the desperation tier may vary depending on how many advantages you have. Go for (#Advantage_Types * 2).)
8. Empty Line for Separation    STRING

You're done with your input document now.


**advantage_roll.cpp Customization**

Open up advantage_roll.cpp and head to int main(). As soon as you're there, you'll be met with the things you can customize.
