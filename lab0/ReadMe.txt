Lab 0
Nicole Wang (l.wang12@wustl.edu), Lin Li(lilin1@wustl.edu)



EXTRA CREDIT
Approach to the extra credit part:
1. Arguments: 4 arguments
1). execution file name
2). the name of a script fragment file
3). the name of a configuration file to generate
4). the name of the play

2. Main Function: check for right number of arguments (is number of arguments = 4?)
1).	If yes, enter extraCredit() function
2).	If no, print the usage message and return error code numberArgumentIncorrect

3. ExtraCredit Function: 
1). Input file stream for the script fragment file
2). Output file stream for the configuration file 
3). Output file streams for the part file for each character

Steps:
1. Open 1) and 2) file streams, initialized a map to store keys, the character name(Guildenstern, King, Queen, Rosencrantz), for corresponding values, the character file name(Guildenstern.txt, King.txt, Queen.txt, and Rosencrantz.txt).If the file streams are not opened, we will print out "File cannot open" error message and return an error code fileDoesNotExist.

2. Set a boolean value changeCharacter to indicate the change of character, so we need to open the corresponding character file and append new scripts. 
Idea: At the beginning, we set changeCharacter as true to read the first character infomation and scripts. As long as we have checked changeCharacter as true, we will set it to false, until it encounters a empty line, we set it back to true.

3. Stroing character into map: when we encounter a character being read, we will first check if it is already existing in map. If not, insert a new character. 

4. If changeCharacter is false, we will append the current line to the part file for that particular character(the characterName is stored in variable), and we will increment the ordering variable called i in our code. 

5. After all lines read, 
1). If the character file is opened, we will close it. 
2). We will iterate throught the map and write the keys and values to the configuration file.
3).	We will close the script fragment file and the configuration file.



Give instructions for how to unpack, build, and use it:
1. Download extraCredit directory 
2. Open with Visual Studio 2019 
3. Open lab0extra.sln in the extraCredit directory
4. Build the solution
5. Open Terminal or CMD window, direct to Debug directory and find the execution file extraCredit.exe and the sample files(hamlet_act_ii_scene_2.txt)
6. Type in command as following format: extraCredit.exe <inputFileName> <outputFileName> <playName>
Example: extraCredit.exe hamlet_act_ii_scene_2.txt hamlet_ii_2_config.txt Hamlet_Prince_of_Denmark_ACT_II_Scene_II_A_room_in_the_Castle_by_William_Shakespeare



Document how you used it in your testing of the assignment
1. Open Terminal or CMD window, direct to Debug directory and find the execution file extraCredit.exe and the sample files(hamlet_act_ii_scene_2.txt)
2. Type in command as following format: extraCredit.exe <inputFileName> <outputFileName> <playName>
Example: extraCredit.exe hamlet_act_ii_scene_2.txt hamlet_ii_2_config.txt Hamlet_Prince_of_Denmark_ACT_II_Scene_II_A_room_in_the_Castle_by_William_Shakespeare
3. Output: 
1). Files: hamlet_ii_2_config.txt, Guildenstern.txt, King.txt, Queen.txt, and Rosencrantz.txt

2). hamlet_ii_2_config.txt: 
Hamlet Prince of Denmark ACT II Scene II A room in the Castle by William Shakespeare
Guildenstern Guildenstern.txt
King King.txt
Queen Queen.txt
Rosencrantz Rosencrantz.txt

3). Guildenstern.txt:
31 We both obey, 
32 And here give up ourselves, in the full bent, 
33 To lay our service freely at your feet, 
34 To be commanded.
40 Heavens make our presence and our practices 
41 Pleasant and helpful to him!

4). King.txt:
1  Welcome, dear Rosencrantz and Guildenstern! 
2  Moreover that we much did long to see you, 
3  The need we have to use you did provoke 
4  Our hasty sending. Something have you heard 
5  Of Hamlet's transformation; so I call it, 
6  Since nor the exterior nor the inward man 
7  Resembles that it was. What it should be, 
8  More than his father's death, that thus hath put him 
9  So much from the understanding of himself, 
10 I cannot dream of: I entreat you both 
11 That, being of so young days brought up with him, 
12 And since so neighbour'd to his youth and humour, 
13 That you vouchsafe your rest here in our court 
14 Some little time: so by your companies 
15 To draw him on to pleasures, and to gather, 
16 So much as from occasion you may glean, 
17 Whether aught, to us unknown, afflicts him thus, 
18 That, open'd, lies within our remedy.
35 Thanks, Rosencrantz and gentle Guildenstern.

5). Queen.txt:
19 Good gentlemen, he hath much talk'd of you, 
20 And sure I am two men there are not living 
21 To whom he more adheres. If it will please you 
22 To show us so much gentry and good-will 
23 As to expend your time with us awhile, 
24 For the supply and profit of our hope, 
25 Your visitation shall receive such thanks 
26 As fits a king's remembrance.
36 Thanks, Guildenstern and gentle Rosencrantz: 
37 And I beseech you instantly to visit 
38 My too-much-changed son.--Go, some of you, 
39 And bring these gentlemen where Hamlet is.
42 Ay, amen!

6). Rosencrantz.txt
27 Both your majesties 
28 Might, by the sovereign power you have of us, 
29 Put your dread pleasures more into command 
30 Than to entreaty.
