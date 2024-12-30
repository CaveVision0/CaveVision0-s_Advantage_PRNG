# HOW TO USE:

**Input/Output Docs**
First off, you'll need to create two text files. One is for the input and the other is for the output. An example of an input (ExampleCast.txt) is already provided, as is an example of a previously-generated output (ExampleResult.txt).

Once you have your input doc, make your first line "__START" and your final line "__END" (quotation marks not included). This is used by the program to mark when it should start (__START) and stop (__END) gathering the contestant data.

Between those two lines, create your cast with this format (ignore everything that isn't in brackets):

[[Name]]    STRING
[[Desperation Tier]]    SPECIFIC STRINGS (see advantage_roll.cpp's map for the strings; found in int main())
[[Team Name]]    STRING
[[Real Clues for Advantage 1]]    INT
[[Fake Clues for Advantage 1]]    INT
[[Real Clues for Advantage 2]]    INT
[[Fake Clues for Advantage 2]]    INT (The number of integers below the desperation tier may vary depending on how many advantages you have. Go for (#Advantage_Types * 2).)
Whitespace For Contestant Separation    STRING
