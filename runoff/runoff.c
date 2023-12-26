#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Store candidates in array
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoff rounds until there is a winner
    while (true)
    {
        // Calculate votes for remaining candidates
        tabulate();

        // Check if there is a winner
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        int top_candidate = preferences[i][0];
        if (!candidates[top_candidate].eliminated)
        {
            candidates[top_candidate].votes++;
        }
        else
        {
            // If the top-ranked candidate is eliminated, find the next non-eliminated candidate
            for (int j = 1; j < candidate_count; j++)
            {
                int next_candidate = preferences[i][j];
                if (!candidates[next_candidate].eliminated)
                {
                    candidates[next_candidate].votes++;
                    break;
                }
            }
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Iterate through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if a candidate has more than half of the total votes
        if (candidates[i].votes > voter_count / 2)
        {
            // Print the winner's name and return true
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    // No winner found
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // Initialize to a value greater than the maximum possible votes
    int min_votes = voter_count;

    // Iterate through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate is not eliminated and has fewer votes than the current minimum
        if (!candidates[i].eliminated && candidates[i].votes < min_votes)
        {
            // Update the minimum votes
            min_votes = candidates[i].votes;
        }
    }

    return min_votes;
}

// Return true if the election is tied between all candidates, otherwise false
bool is_tie(int min)
{
    // Iterate through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate is not eliminated and has a different number of votes than the minimum
        if (!candidates[i].eliminated && candidates[i].votes != min)
        {
            // At least one candidate has a different number of votes
            return false;
        }
    }

    // All non-eliminated candidates have the same number of votes
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // Iterate through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the candidate has the minimum number of votes
        if (candidates[i].votes == min)
        {
            // Eliminate the candidate by updating their eliminated status
            candidates[i].eliminated = true;
        }
    }
}
