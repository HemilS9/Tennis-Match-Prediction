# Tennis-Match-Prediction
A command-line program that predicts the winner of an ATP tennis match given the names of the 2 players, the surface ("Hard", "Clay", "Grass"), and a csv file with match data.

# Instructions:
Compile the program with: ` make predict.exe `

Run the program with the following command line arguments:
- "Player 1 Name"
- "Player 2 Name"
- Surface ("Hard", "Clay, "Grass")
- training_data.csv
- Please reference the following example: 
` ./predict.exe "Novak Djokovic" "Rafael Nadal" Clay 2022_data.csv `

# Important Notes:
- The match dataset is not updated weekly (it's updated a few times each year), so the farther away a given, current tennis match is from the most recent matches in the dataset, the less accurate the prediction is expected to be.

# How does it work?
At a high level, each player is assigned an ELO that is initialized based on their world ranking and modified after each match a player plays.

The training phase modifies a player's ELO based on several metrics, and each metric is assigned a weight between 0 and 1 (which can be easily modified, if wanted, near the top of the source code in predict.cpp). 
The metrics include:
- The score
- The time taken to complete the match
- The difference between players' rankings
- Number of aces each player made
- Number of double faults each player made
- Break point conversion of each player

# Credits:
- The CSV parser used in this project is by Ben Strasser: https://github.com/ben-strasser/fast-cpp-csv-parser
- The match data in CSV format is compiled by Jeff Sackmann: https://github.com/JeffSackmann/tennis_atp

Created by Hemil Shah: <heshah@umich.edu>
