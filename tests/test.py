import os
import csv

# Change tournament to be the folder name of the tournament you want to test!
TOURNAMENT = "ao_2023_test"

# Compile the C++ code
os.system("make predict.exe")

# Test the predictions
num_correct = 0
num_total = 0
num_correct_higher_ranked = 0

def test_prediction(player1:str, player2:str, surface:str):
    cmd = "./predict.exe \"{p1}\" \"{p2}\" {s} {t}/past_year_data.csv".format(p1=player1, p2=player2, s=surface, t=TOURNAMENT)
    winner:str = player1
    prediction = os.popen(cmd).read().strip()
    # print(prediction)
    # print(winner)
    return (prediction == winner)

def test_default_prediction(player1:str, player2:str, player1_rank:int, player2_rank:int):
    winner:str = player1
    prediction = player1 if player1_rank < player2_rank else player2
    # print(prediction)
    # print(winner)
    return (prediction == winner)


actual_results = "{t}/correct.csv".format(t=TOURNAMENT)
with open(actual_results, "r") as testFile:
    reader = csv.DictReader(testFile)
    
    for row in reader:
        num_total += 1

        result:bool = test_prediction(row["winner_name"], row["loser_name"], row["surface"])
        if result:
            num_correct += 1

        result:bool = test_default_prediction(row["winner_name"], row["loser_name"], row["winner_rank"], row["loser_rank"])
        if result:
            num_correct_higher_ranked += 1


# Print the results
output_file = "{t}/result.txt".format(t=TOURNAMENT)
with open(output_file, 'w') as resultFile:
    resultFile.write("Number of correct predictions: {n}\n".format(n = num_correct))
    resultFile.write("Number of total predictions: {n}\n".format(n = num_total))
    resultFile.write("Accuracy: {a}%\n".format(a = round(100*num_correct/num_total, 3)))

    resultFile.write("Accuracy if always predicting the higher seed: {a}%\n".format(a = round(100*num_correct_higher_ranked/num_total, 3)))

# Clean up
os.system("make clean")
