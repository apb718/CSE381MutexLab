// Copyright 2024 Alan Ferrenberg
/*
0 $3405
1 $3805
2 $2300
3 $3095
4 $3595
*/
#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <utility>

using std::cout, std::endl, std::thread, std::ref;
using fVector = std::vector<float>;

// Function to deposit money into an account.  An actual deposit
// would be more complex than just updating a vector (e.g. doing
// checks, retrieving data over the network, etc.) so a sleep
// statement is put into the function to mimic that.
void deposit(int acc, fVector& account, double amt,
        std::unordered_map<int, std::unique_ptr<std::mutex>>& mutexes) {
    if (true) {
        std::lock_guard<std::mutex> lock(*mutexes[acc]);
        account[acc] += amt;
        // std::cout << "depo done" << std::endl;
    }
    sleep(1);
}

// Function to withdraw money from an account.  An actual withdraw
// would be more complex than just updating a vector (e.g. doing
// checks, retrieving data over the network, etc.) so a sleep
// statement is put into the function to mimic that.
// Note that before the withdraw is processed, the account balance
// must be checked to make sure there are sufficient funds.
void withdraw(int acc, fVector& account, double amt,
        std::unordered_map<int, std::unique_ptr<std::mutex>>& mutexes) {
    if (true) {
        std::scoped_lock<std::mutex> lock(*mutexes[acc]);
        if (account[acc] >= amt) {
            account[acc] -= amt;
        } else {
            std::cerr << "Insufficient funds to withdraw $" << amt << endl;
        }
    }
    // std::cout << "with done" << std::endl;
    sleep(1);
}

// Function to transfer money from one account to another.  An actual transfer
// would be more complex than just updating a vector (e.g. doing
// checks, retrieving data over the network, etc.) so a sleep
// statement is put into the function to mimic that.
// Note that before the withdraw from the "from" is processed, the account
// balance must be checked to make sure there are sufficient funds.
void transfer(int from, int to, fVector& account, double amt,
     std::unordered_map<int, std::unique_ptr<std::mutex>>& mutexes) {
    if (true) {
        std::scoped_lock lock(*mutexes[from], *mutexes[to]);
        if (account[from] >= amt) {
            account[from] -= amt;
            account[to] += amt;
        } else {
            std::cerr << "Insufficient funds to transfer $" << amt << endl;
        }
    }
    // std::cout <</ "transf done" << std::endl;
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

void initializeMutex(fVector& a, std::unordered_map<int,
                     std::unique_ptr<std::mutex>>& m) {
    for (size_t i = 0; i < a.size(); i++) {
        m[i] = std::make_unique<std::mutex>();
    }
}
// A function that processes all of the transactions, updating the vector with
// the account balances via the reference.
void processTransactions(fVector& accts,
    std::unordered_map<int, std::unique_ptr<std::mutex>>& mutexes) {
    std::vector<thread> thrs;
    std::vector<double> transactions = {100, 50, 75, 200, 800, 950, 230};
    for (double amt : transactions) {
        thrs.push_back(thread(deposit, 0, ref(accts), amt, ref(mutexes)));
        thrs.push_back(thread(transfer, 1, 2, ref(accts), amt, ref(mutexes)));
        thrs.push_back(thread(withdraw, 3, ref(accts), amt, ref(mutexes)));
        thrs.push_back(thread(transfer, 4, 3, ref(accts), amt, ref(mutexes)));
        thrs.push_back(thread(deposit, 4, ref(accts), amt, ref(mutexes)));
        thrs.push_back(thread(withdraw, 0, ref(accts), amt, ref(mutexes)));
        thrs.push_back(thread(transfer, 2, 0, ref(accts), amt, ref(mutexes)));
        thrs.push_back(thread(transfer, 4, 1, ref(accts), amt, ref(mutexes)));
        thrs.push_back(thread(deposit, 1, ref(accts), amt, ref(mutexes)));
        thrs.push_back(thread(withdraw, 3, ref(accts), amt, ref(mutexes)));
    }

    for (auto& item : thrs) {
        item.join();
    }
}

int main() {
    // Create a vector to hold the account balances, then initialize it.  The
    // index of the vector will serve as the account number in this simple
    // example.
    fVector accts;
    std::unordered_map<int, std::unique_ptr<std::mutex>> mutexMap;

    initializeAccounts(accts);
    initializeMutex(accts, mutexMap);

    sleep(10);
    // Process the transactions
    processTransactions(accts, mutexMap);

    // Print out the final account balances
    cout << "Final balances\n";
    for (size_t acc = 0; acc < accts.size(); acc++) {
        cout << acc << " $" << getBalance(acc, accts) << endl;
    }
    return 0;
}
