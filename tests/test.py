import os
import csv

# Change tournament to be the folder name of the tournament you want to test!
TOURNAMENT = "nitto_2022_test"

# Compile the C++ code
os.system("make predict.exe")


# Test the predictions
num_correct = 0
num_total = 0

def test_prediction(player1:str, player2:str, surface:str):
    cmd = "./predict.exe \"{p1}\" \"{p2}\" {s} {t}/past_year_data.csv".format(p1=player1, p2=player2, s=surface, t=TOURNAMENT)
    winner:str = player1
    prediction = os.popen(cmd).read().strip()
    # print(prediction)
    # print(winner)
    return (prediction == winner)


actual_results = "{t}/correct.csv".format(t=TOURNAMENT)
with open(actual_results, "r") as testFile:
    reader = csv.DictReader(testFile)
    print("Testing...")
    
    for row in reader:
        num_total += 1
        result:bool = test_prediction(row["winner_name"], row["loser_name"], row["surface"])
        if result:
            num_correct += 1


# Print the results
output_file = "{t}/result.txt".format(t=TOURNAMENT)
with open(output_file, 'w') as resultFile:
    resultFile.write("Number of correct predictions: {n}\n".format(n = num_correct))
    resultFile.write("Number of total predictions: {n}\n".format(n = num_total))
    resultFile.write("Accuracy: {a}%\n".format(a = 100*num_correct/num_total))

# Clean up
os.system("make clean")
