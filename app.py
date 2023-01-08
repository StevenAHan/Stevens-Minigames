from flask import Flask, render_template, redirect, url_for, request, flash

import subprocess
import logging
import sys

app = Flask(__name__)

app.config["SECRET_KEY"] = "HAVEFUN!"

app.logger.addHandler(logging.StreamHandler(sys.stdout))
app.logger.setLevel(logging.ERROR)

# Default route
@app.route("/")
def home():
    return render_template("index.html")

@app.route("/24solver", methods=["GET", "POST"])
def twenty_four():
    if request.method == "POST":
        numbers = request.form.get("numbers")
        show_ans = request.form.get("show-ans")
        if(numbers == "" or show_ans == ""):
            flash("Incorrect Inputs")
        to_input = ["./twentyFourSolver"]
        numbers_arr = numbers.split()
        to_input += numbers_arr
        to_input.append(show_ans)
        output = subprocess.run(to_input, stdout = subprocess.PIPE, universal_newlines = True).stdout
        output = output[:-1] # remove endline at the end of output
        outputs = output.split("\n")
        outputs[0] += " for " + str(numbers)
        if(len(outputs) - 1 > 0):
            outputs.insert(1, "Number of Solution(s): " + str(len(outputs) - 1))
        return render_template("twenty_four.html", outputs = outputs)

    return render_template("twenty_four.html", outputs = [""])

@app.route("/wip")
def wip():
    return render_template("wip.html")

@app.route("/sudoku_solver", methods=["GET", "POST"])
def sudoku_solver():
    if request.method == "POST":
        second_input = "["
        for i in range(1,82):
            second_input += request.form.get("cell" + str(i)) if request.form.get("cell" + str(i)) != "" else "."
            if(i % 9 == 0):
                second_input += "]["
        second_input = second_input[:-1]
        to_input = ['./sudokuSolver', second_input]
        output = subprocess.run(to_input, stdout = subprocess.PIPE, universal_newlines = True).stdout
        # if there is no solution
        if(output[0] == "T"):
            return render_template("sudoku_solver.html", tbl = output)
        outputs = output.split("\n")
        for i in range(len(outputs)):
            outputs[i] = outputs[i][1:-1]
            outputs[i] = outputs[i].split(",")
        outputs = outputs[:-1]
        return render_template("sudoku_solver.html", tbl=outputs)

    return render_template("sudoku_solver.html")

@app.route("/spellcast_solver", methods=["GET", "POST"])
def spellcast_solver():
    if request.method == "POST":
        letters = ""
        for i in range(1,26):
            letters += request.form.get("cell" + str(i))
        unique_tiles = request.form.get("unique-tiles")
        letters = letters.strip()
        to_input = ["./spellcastSolver", letters]
        to_input += unique_tiles.split()
        print(to_input)
        output = subprocess.run(to_input, stdout = subprocess.PIPE, universal_newlines = True).stdout
        return render_template("spellcast_solver.html", output = output)

    return render_template("spellcast_solver.html")

    
if __name__ == "__main__":
    app.run(debug=True)