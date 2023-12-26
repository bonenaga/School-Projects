import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Retrieve all of the user's transactions
    transactions = db.execute("SELECT symbol, SUM(shares) AS total_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])

    # Prepare a list to hold the user's current portfolio
    portfolio = []
    grand_total = 0  # To keep track of the total value of stocks

    # Iterate over each transaction to calculate the current price and total value
    for transaction in transactions:
        symbol = transaction["symbol"]
        shares = transaction["total_shares"]

        # Lookup the current price of the stock
        stock = lookup(symbol)
        if stock is not None:
            total_value = shares * stock["price"]
            grand_total += total_value
            portfolio.append({
                "symbol": symbol,
                "name": stock["name"],
                "shares": shares,
                "price": stock["price"],
                "total": total_value
            })

    # Get the user's current cash balance
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # Calculate the total asset value (cash + stocks)
    total_assets = grand_total + cash

    # Render the index page and pass the portfolio, cash, and total assets
    return render_template("index.html", portfolio=portfolio, cash=cash, total_assets=total_assets)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        # Validate inputs
        if not symbol or not shares or not shares.isdigit() or int(shares) < 1:
            return apology("invalid input", 400)

        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol", 400)

        # Calculate cost and check user's cash
        shares = int(shares)  # Convert shares to integer
        cost = quote["price"] * shares
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        if cost > user_cash:
            return apology("can't afford", 400)

        # Update user's cash
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", cost, session["user_id"])

        # Record the purchase as a transaction in the transactions table
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   session["user_id"], symbol, shares, quote["price"])

        # Redirect user to index page to see their current holdings
        return redirect("/")

    else:
        # User reached route via GET (show the buy form)
        return render_template("buy.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # Look up quote for symbol
        quote = lookup(symbol)

        # Ensure symbol exists
        if quote is None:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", stock=quote)

    else:
        # User reached route via GET
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted and doesn't already exist
        if not username or db.execute("SELECT * FROM users WHERE username = ?", username):
            return apology("invalid username", 400)

        # Ensure password was submitted and matches confirmation
        elif not password or password != confirmation:
            return apology("passwords must match", 400)

        # Hash password and insert new user into database
        hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

        # Redirect user to login page
        return redirect("/login")

    else:
        # User reached route via GET
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        # ... your selling logic ...

        if some_condition:
            # handle the condition, maybe with an apology or a redirect
            return apology("some error message", 400)
        elif another_condition:
            # handle another condition
            return apology("another error message", 400)
        else:
            # handle successful sell
            # ... update database etc ...
            return redirect("/")
    else:
        # method is GET, show the sell form
        # ... your code to setup the form ...
        return render_template("sell.html")

    # As a fallback, if none of the above conditions are met (which shouldn't happen in theory)
    return apology("Unexpected error", 500)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT symbol, shares, price, transacted FROM transactions WHERE user_id = ? ORDER BY transacted DESC", session["user_id"])

    for transaction in transactions:
        transaction["total"] = transaction["shares"] * transaction["price"]

    return render_template("history.html", transactions=transactions)


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    if request.method == "POST":
        # Get the amount of cash to add from the form
        cash_to_add = request.form.get("cash_to_add")

        # Validate the cash amount (ensure it's positive and a valid number)
        if not cash_to_add or not cash_to_add.isdigit() or float(cash_to_add) <= 0:
            return apology("Invalid amount", 400)

        # Update the user's cash amount in the database
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", float(cash_to_add), session["user_id"])

        # Redirect to the index page or a confirmation page
        return redirect("/")

    else:
        # If method is GET, display the add cash form
        return render_template("add_cash.html")
