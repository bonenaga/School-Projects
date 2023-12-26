-- Keep a log of any SQL queries you execute as you solve the mystery.

--Notes on table schema
.schema
CREATE TABLE crime_scene_reports (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    street TEXT,
    description TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE interviews (
    id INTEGER,
    name TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    transcript TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE atm_transactions (
    id INTEGER,
    account_number INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    atm_location TEXT,
    transaction_type TEXT,
    amount INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE bank_accounts (
    account_number INTEGER,
    person_id INTEGER,
    creation_year INTEGER,
    FOREIGN KEY(person_id) REFERENCES people(id)
);
CREATE TABLE airports (
    id INTEGER,
    abbreviation TEXT,
    full_name TEXT,
    city TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE flights (
    id INTEGER,
    origin_airport_id INTEGER,
    destination_airport_id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
    FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
);
CREATE TABLE passengers (
    flight_id INTEGER,
    passport_number INTEGER,
    seat TEXT,
    FOREIGN KEY(flight_id) REFERENCES flights(id)
);
CREATE TABLE phone_calls (
    id INTEGER,
    caller TEXT,
    receiver TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    duration INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE people (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number INTEGER,
    license_plate TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE bakery_security_logs (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    activity TEXT,
    license_plate TEXT,
    PRIMARY KEY(id)
);


--SQL Queries

--Check crime scene reports for references to the CS50 Duck
SELECT *
FROM crime_scene_reports
WHERE description LIKE '%duck%';
--CS50 duck theft captured in report ID 295
--10:15am at Humphrey Street Bakery
--Interviewed 3 witnesses
--Each transcript mentions bakery
--7/28/2021

--Check interviews with same ID
SELECT *
FROM interviews
WHERE transcript LIKE '%bakery%';
--Ruth: within 10 minutes of theft saw thief get into a car in parking lot and drive away, check security footage
--Raymond: as thief leaving bakery they called someone for <1 minute, taking earliest flight out of Fiftyville tomorrow, asked person on phone to purchase ticket
--Eugene: walking by Emma's Bakery saw thief withdraw money from ATM on Leggett Street

--Check security footage
SELECT *
FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10;
--Possibilities include 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55

--Check phone calls
SELECT caller, receiver
FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;
--Several possibilities (sender/receiver)
--| (499) 555-9472 | (892) 555-8872 |
--| (367) 555-5533 | (375) 555-8161 |
--| (499) 555-9472 | (717) 555-1342 |
--| (286) 555-6063 | (676) 555-6554 |
--| (770) 555-1861 | (725) 555-3243 |
--| (031) 555-6622 | (910) 555-3251 |
--| (826) 555-1652 | (066) 555-9701 |
--| (338) 555-6650 | (704) 555-2131 |

--Check flights
SELECT *
FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE flights.year = 2021 AND flights.month = 7 AND flights.day = 29 AND airports.city LIKE '%fiftyville%';
--earliest flight
--ID 36, origin 8 (fiftyville), destination 4, departed 8:20 on 29 Jul 2021

--Get destination
SELECT *
FROM airports
WHERE id = 4;
--LGA LaGuardia Airport, New York City

--Get passenger list (delay until later)
SELECT *
FROM passengers
WHERE flight_id = 36;
--| 36        | 7214083635      | 2A   |
--| 36        | 1695452385      | 3B   |
--| 36        | 5773159633      | 4A   |
--| 36        | 1540955065      | 5C   |
--| 36        | 8294398571      | 6C   |
--| 36        | 1988161715      | 6D   |
--| 36        | 9878712108      | 7A   |
--| 36        | 8496433585      | 7B   |

--Match license plates and passports to people
SELECT name, phone_number, people.passport_number, people.license_plate
FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE passengers.flight_id = 36
AND phone_calls.year = 2021 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration < 60
AND bakery_security_logs.year = 2021 AND bakery_security_logs.month = 7 AND bakery_security_logs.day = 28 AND bakery_security_logs.hour = 10;
--| Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
--| Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |

--Check ATM transactions
SELECT people.name
FROM bank_accounts
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
JOIN people ON people.id = bank_accounts.person_id
WHERE atm_transactions.atm_location LIKE '%legget%' AND atm_transactions.year = 2021 AND atm_transactions.month = 7 AND atm_transactions.day = 28;
--Bruce

--Match Bruce's call to accomplice and get name
SELECT name
FROM people
JOIN phone_calls ON phone_calls.receiver = people.phone_number
WHERE phone_calls.caller = '(367) 555-5533'
AND phone_calls.year = 2021 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration < 60;
--Robin
