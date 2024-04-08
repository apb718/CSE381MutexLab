// Copyright 2024 Alan Ferrenberg
#include <unistd.h>
#include <iostream>
#include <vector>

using std::cout, std::endl;
using fVector = std::vector<float>;

// Function to deposit money into an account.  An actual deposit
// would be more complex than just updating a vector (e.g. doing
// checks, retrieving data over the network, etc.) so a sleep
// statement is put into the function to mimic that.
void deposit(int acc, fVector& account, double amt) {
    account[acc] += amt;
    sleep(1);
}

// Function to withdraw money from an account.  An actual withdraw
// would be more complex than just updating a vector (e.g. doing
// checks, retrieving data over the network, etc.) so a sleep
// statement is put into the function to mimic that.
// Note that before the withdraw is processed, the account balance
// must be checked to make sure there are sufficient funds.
void withdraw(int acc, fVector& account, double amt) {
    if (account[acc] >= amt) {
        account[acc] -= amt;
    } else {
        std::cerr << "Insufficient funds to withdraw $" << amt << endl;
    }
    sleep(1);
}

// Function to transfer money from one account to another.  An actual transfer
// would be more complex than just updating a vector (e.g. doing
// checks, retrieving data over the network, etc.) so a sleep
// statement is put into the function to mimic that.
// Note that before the withdraw from the "from" is processed, the account
// balance must be checked to make sure there are sufficient funds.
void transfer(int from, int to, fVector& account, double amt) {
    if (account[from] >= amt) {
        account[from] -= amt;
        account[to] += amt;
    } else {
        std::cerr << "Insufficient funds to transfer $" << amt << endl;
    }
    sleep(1);
}

// Essentially a "getter" function that returns the balance for an account
float getBalance(int acc, fVector account) {
    return account[acc];
}

// Initialize the account balances
void initializeAccounts(fVector& accts) {
    accts.push_back(1000);
    accts.push_back(1400);
    accts.push_back(2300);
    accts.push_back(5500);
    accts.push_back(6000);
}

// A function that processes all of the transactions, updating the vector with
// the account balances via the reference.
void processTransactions(fVector& accts) {
    std::vector<double> transactions = {100, 50, 75, 200, 800, 950, 230};

    for (double amt : transactions) {
        deposit(0, accts, amt);
        transfer(1, 2, accts, amt);
        withdraw(3, accts, amt);
        transfer(4, 3, accts, amt);
        deposit(4, accts, amt);
        withdraw(0, accts, amt);
        transfer(2, 0, accts, amt);
        transfer(4, 1, accts, amt);
        deposit(1, accts, amt);
        withdraw(3, accts, amt);
    }
}

int main() {
    // Create a vector to hold the account balances, then initialize it.  The
    // index of the vector will serve as the account number in this simple
    // example.
    fVector accts;
    initializeAccounts(accts);

    // Process the transactions
    processTransactions(accts);

    // Print out the final account balances
    cout << "Final balances\n";
    for (size_t acc = 0; acc < accts.size(); acc++) {
        cout << acc << " $" << getBalance(acc, accts) << endl;
    }
    return 0;
}
