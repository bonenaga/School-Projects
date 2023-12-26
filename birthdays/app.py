import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Capture the information from the form submission
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        # Insert the new birthday into the birthdays database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)

        # Redirect to the home page
        return redirect("/")

    else:
        # Query all birthdays from the database
        birthdays = db.execute("SELECT * FROM birthdays")

        # Render index.html with all birthdays
        return render_template("index.html", birthdays=birthdays)
