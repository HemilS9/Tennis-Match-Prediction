# Tennis-Match-Prediction
A command-line program that predicts the winner of an ATP tennis match given the names of the 2 players and the surface(Hard, Clay, Grass).

Instructions:
Run the program with the following command line arguments:
- "Player 1 Name"
- "Player 2 Name"
- Surface ("Hard", "Clay, "Grass)
- training_data.csv
- Please reference the following example: ./predict.exe "Novak Djokovic" "Rafael Nadal" Clay 2021_data.csv

Important Notes:
- The dataset is not updated weekly (it's updated at most a couple times each year), so the farther away a given, current tennis match is from the most recent matches in the dataset, the less accurate the prediction is expected to be. 
- As of now, the ELO score does not decrease due to tournament competition inactivity. Therefore, results on injured players or, in general, other players who have taken breaks spanning many months may likely be inaccurate.

Credits:
- The CSV parser is by Ben Strasser(ben-strasser).
- The match data is by Jeff Sackmann(JeffSachmann).

Created by Hemil Shah: <heshah@umich.edu>
