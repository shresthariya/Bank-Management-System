 #include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Customer {
public:
    string name;
    string address;
    string phone;

    Customer(string n, string a, string p) : name(n), address(a), phone(p) {}

    void display() const {
        cout << "Customer Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl;
    }

    void update(string newAddress) {
        address = newAddress;
    }

    void update(string newAddress, string newPhone) {
        address = newAddress;
        phone = newPhone;
    }
};

class Transaction {
public:
    string type;
    double amount;
    time_t timestamp;

    Transaction(string t, double a) : type(t), amount(a) {
        timestamp = time(0);
    }

    void display() const {
        cout << "Type: " << type << ", Amount: Rs." << amount << ", Date: " << ctime(&timestamp);
    }
};

class Account {
protected:
    string accountNumber;
    double balance;
    Customer customer;
    vector<Transaction> transactionHistory;

public:
    Account(string accNo, double bal, Customer cust)
        : accountNumber(accNo), balance(bal), customer(cust) {}

    virtual void display() const {
        cout << "Account Number: " << accountNumber << endl;
        customer.display();
        cout << "Balance: Rs." << balance << endl;
    }

    virtual void deposit(double amount) {
        if (amount <= 0) {
            cout << "Deposit amount must be positive!" << endl;
            return;
        }
        balance += amount;
        transactionHistory.emplace_back("Deposit", amount);
        cout << "Deposited Rs." << amount << " successfully." << endl;
    }

    virtual void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Withdrawal amount must be positive!" << endl;
            return;
        }
        if (amount > balance) {
            cout << "Insufficient funds!" << endl;
            return;
        }
        balance -= amount;
        transactionHistory.emplace_back("Withdrawal", amount);
        cout << "Withdrew Rs." << amount << " successfully." << endl;
    }

    void displayTransactionHistory() const {
        cout << "Transaction History for Account " << accountNumber << ":" << endl;
        for (const auto& transaction : transactionHistory) {
            transaction.display();
        }
    }

    friend class Bank;
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(string accNo, double bal, Customer cust, double rate)
        : Account(accNo, bal, cust), interestRate(rate) {}

    void applyInterest() {
        double interest = balance * (interestRate / 100);
        balance += interest;
        transactionHistory.emplace_back("Interest", interest);
        cout << "Interest of Rs." << interest << " applied." << endl;
    }

    void display() const override {
        cout << "Savings Account" << endl;
        Account::display();
    }
};

class currentAccount : public Account {
public:
   currentAccount(string accNo, double bal, Customer cust)
        : Account(accNo, bal, cust) {}

    void display() const override {
        cout << "current Account" << endl;
        Account::display();
    }
};

class Loan {
public:
    string loanNumber;
    double amount;
    double interestRate;
    time_t startDate;
    time_t endDate;

    Loan(string ln, double amt, double rate, time_t start, time_t end)
        : loanNumber(ln), amount(amt), interestRate(rate), startDate(start), endDate(end) {}

    void display() const {
        cout << "Loan Number: " << loanNumber << endl;
        cout << "Loan Amount: Rs." << amount << endl;
        cout << "Interest Rate: " << interestRate << "%" << endl;
        cout << "Start Date: " << ctime(&startDate);
        cout << "End Date: " << ctime(&endDate);
    }
};

class Employee {
public:
    string name;
    string employeeID;
    string position;

    Employee(string n, string id, string pos) : name(n), employeeID(id), position(pos) {}

    void display() const {
        cout << "Employee Name: " << name << endl;
        cout << "Employee ID: " << employeeID << endl;
        cout << "Position: " << position << endl;
    }
};

class FinancialStatement {
public:
    static void generateStatement(const vector<Account*>& accounts) {
        cout << "Financial Statement:" << endl;
        for (const auto& account : accounts) {
            account->display();
            cout << "---------------------" << endl;
        }
    }
};

class Bank {
private:
    vector<Account*> accounts;
    vector<Loan> loans;
    vector<Employee> employees;
    int nextAccountNumber = 1;  

    string generateAccountNumber() {
        return to_string(nextAccountNumber++);
    }

public:
    void addAccount(Account* account) {
        accounts.push_back(account);
    }

    void addLoan(const Loan& loan) {
        loans.push_back(loan);
    }

    void addEmployee(const Employee& employee) {
        employees.push_back(employee);
    }

    void deposit(string accNo, double amount) {
        for (auto acc : accounts) {
            if (acc->accountNumber == accNo) {
                acc->deposit(amount);
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void withdraw(string accNo, double amount) {
        for (auto acc : accounts) {
            if (acc->accountNumber == accNo) {
                acc->withdraw(amount);
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void displayAccounts() const {
        for (auto acc : accounts) {
            acc->display();
            cout << "---------------------" << endl;
        }
    }

    void displayLoans() const {
        for (const auto& loan : loans) {
            loan.display();
            cout << "---------------------" << endl;
        }
    }

    void displayEmployees() const {
        for (const auto& employee : employees) {
            employee.display();
            cout << "---------------------" << endl;
        }
    }

    void displayAccountTransactionHistory(string accNo) const {
        for (auto acc : accounts) {
            if (acc->accountNumber == accNo) {
                acc->displayTransactionHistory();
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void updateCustomerAddress(string accNo, string newAddress) {
        for (auto acc : accounts) {
            if (acc->accountNumber == accNo) {
                acc->customer.update(newAddress);
                cout << "Address updated successfully." << endl;
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void updateCustomerPhone(string accNo, string newPhone) {
        for (auto acc : accounts) {
            if (acc->accountNumber == accNo) {
                acc->customer.update(acc->customer.address, newPhone);
                cout << "Phone number updated successfully." << endl;
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    vector<Account*>& getAccounts() {
        return accounts;
    }

    void createAccount() {
        string name, address, phone, accType;
        double initialDeposit, interestRate = 0.0;

        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter address: ";
         
        getline(cin, address);
        cout << "Enter phone number: ";
        getline(cin, phone);
        cout << "Enter initial deposit: ";
        cin >> initialDeposit;
        cout << "Enter account type (savings/current): ";
        cin >> accType;

        Customer cust(name, address, phone);
        string accNo = generateAccountNumber();

        if (accType == "savings") {
            cout << "Enter interest rate: ";
            cin >> interestRate;
            Account* acc = new SavingsAccount(accNo, initialDeposit, cust, interestRate);
            addAccount(acc);
        } else if (accType == "current") {
            Account* acc = new currentAccount(accNo, initialDeposit, cust);
            addAccount(acc);
        } else {
            cout << "Invalid account type!" << endl;
            return;
        }

        cout << "Account created successfully with Account Number: " << accNo << endl;
    }
};

int main() {
    Bank bank;

     
    bank.createAccount();
    bank.createAccount();

     
    bank.displayAccounts();

  
    string accNoToUpdateAddress;
    string newAddress;
    cout << "Enter account number to update address: ";
    cin >> accNoToUpdateAddress;
    cout << "Enter new address: ";
    cin.ignore();
    getline(cin, newAddress);
    bank.updateCustomerAddress(accNoToUpdateAddress, newAddress);

     
    string accNoToUpdatePhone;
    string newPhone;
    cout << "Enter account number to update phone number: ";
    cin >> accNoToUpdatePhone;
    cout << "Enter new phone number: ";
    cin >> newPhone;
    bank.updateCustomerPhone(accNoToUpdatePhone, newPhone);

     
    bank.displayAccounts();

     
    string accNoToDeposit;
    double amount;
    cout << "Enter account number to deposit: ";
    cin >> accNoToDeposit;
    cout << "Enter amount to deposit: ";
    cin >> amount;
    bank.deposit(accNoToDeposit, amount);

    string accNoToWithdraw;
    cout << "Enter account number to withdraw: ";
    cin >> accNoToWithdraw;
    cout << "Enter amount to withdraw: ";
    cin >> amount;
    bank.withdraw(accNoToWithdraw, amount);

    
    string accNoToDisplayHistory;
    cout << "Enter account number to display transaction history: ";
    cin >> accNoToDisplayHistory;
    bank.displayAccountTransactionHistory(accNoToDisplayHistory);

    
    Loan loan1("LN001", 50000, 5.5, time(0), time(0) + 365*24*60*60);
    bank.addLoan(loan1);
    Loan loan2("LN002", 100000, 6.5, time(0), time(0) + 2*365*24*60*60);
    bank.addLoan(loan2);
    bank.displayLoans();

     
    Employee emp1("Julie Shrestha", "BN23045", "Manager");
    bank.addEmployee(emp1);
    Employee emp2("Puntey Shrestha", "BN23044", "Internal Auditor");
    bank.addEmployee(emp2);
    bank.displayEmployees();

    
    FinancialStatement::generateStatement(bank.getAccounts());

    return 0;
}
