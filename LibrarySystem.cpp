#include <iostream>
#include <string>
#include <vector>
using namespace std;

// difine book info.
struct Book
{
    int id;
    string title;
    string classification;
    bool isBorrowed;

    Book(int i, string t, string c)
    {
        id = i;
        title = t;
        classification = c;
        isBorrowed = false;
    }
};

// historyNode by linked list
struct HistoryNode
{
    string action; // borrrow , return
    int bookID;
    HistoryNode *next;

    HistoryNode(string a, int id)
    {
        action = a;
        bookID = id;
        next = nullptr;
    }
};

class HistoryList
{

private:
    HistoryNode *head; // ※need a head,maybe tail is also needed

public:
    HistoryList()
    {
        head = nullptr; // headPtr should be null
    }

    void addHistory(string action, int bookID) // append
    {
        HistoryNode *newNode = new HistoryNode(action, bookID);

        if (head == nullptr)
        {
            head = newNode;
            return; // need to return!!!!!!!!
        }

        HistoryNode *temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }

        temp->next = newNode;
    }

    void disPlay() // traversal data
    {
        HistoryNode *temp = head;
        while (temp != nullptr)
        {
            cout << temp->action << "   Book ID: " << temp->bookID << endl;
            temp = temp->next;
        }
    }
};

void TestData(vector<Book> books) // for test
{
    books = {
        Book(101, "Data Structure", "Computer Science"),
        Book(102, "Operating Systems", "Computer Science"),
        Book(201, "Discrete Mathematics", "Mathematics"),
        Book(301, "Linear Algebra", "Mathematics"),
        Book(401, "Database Systems", "Computer Science"),
    };
}

void bookHistory(HistoryList &history) // for test
{
    history.addHistory("BORROW", 101);
    history.addHistory("BORROW", 102);
    history.addHistory("RETURN", 101);
    history.addHistory("BORROW", 201);
    history.addHistory("BORROW", 301);
}


void bookMenu(vector<Book> &books)//bookMenu UI function
{
    int choice;
    while(true)
    {
        cout << "\n--- Book Management ---\n";
        cout << "1. Add Book\n";
        cout << "2. View Books\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin>>choice;

        if(choice==0) break;

        switch(choice)
        {
            
        }
    }
}


void borrowMenu(vector<Book>& books, HistoryList& history);

void addBookUI(vector<Book>& books);
void displayBooks(const vector<Book>& books);

void borrowBookUI(vector<Book>& books, HistoryList& history);
void returnBookUI(vector<Book>& books, HistoryList& history);



int main()
{
    vector<Book> books;
    HistoryList history;

    TestData(books);
    bookHistory(history);

    int choice;

    while (true)
    {
        cout << "\n=== Library System ===\n";
        cout << "1. Book Management\n";
        cout << "2. Borrow / Return\n";
        cout << "3. View History\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        if (choice == 0)
            break;

        switch (choice)
        {
        case 1:
            bookMenu(books);
            break;

        case 2:
            borrowMenu(books, history);
            break;

        case 3:
            history.disPlay();
            break;

        default:
            cout << "Invalid choice\n";
        }
    }

    return 0;
}