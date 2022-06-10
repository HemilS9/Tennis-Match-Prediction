# Tennis-Match-Prediction
A command-line program that predicts the winner of an ATP tennis match in **under 1 second** given the names of the 2 players, the surface (Hard, Clay, Grass), and a csv file with match data. 

# Important Notes:
- The match dataset is not updated weekly (although I believe it is updated around once a month), so the further away a given, current tennis match is from the most recent matches in the dataset, the less accurate the prediction is expected to be.
- For best results, use the "past_year_data.csv" file. It includes all matches exactly 1 year from the most recent tournament updated by the source of the dataset

# Instructions:
Compile the program with: ` make predict.exe `

Run the program with the following command line arguments:
- "Player 1 Name"
- "Player 2 Name"
- Surface ("Hard", "Clay", "Grass")
- match_data.csv
- Please reference the following example: 
` ./predict.exe "Novak Djokovic" "Rafael Nadal" Clay 2022_data.csv `

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

During the prediction stage, a recent form multiplier is added to players' elo score before determining the winner.

# Conclusion
Like most sports, tennis matches do not have definitive answers for the winners of matches (for example, Novak Djokovic and Rafael Nadal have a 30-29 head to head!).
Rising youngsters, unexpected injuries, occasional upsets, and more, all cause lots of difficulty in predicting winners. Because of this, I decided to go with an ELO based prediction method that utilizes various metrics to update players after each match played.

# Credits:
- The CSV parser used in this project is by Ben Strasser: https://github.com/ben-strasser/fast-cpp-csv-parser
- The match data in CSV format is compiled by Jeff Sackmann: https://github.com/JeffSackmann/tennis_atp

Created by Hemil Shah: <heshah@umich.edu>
