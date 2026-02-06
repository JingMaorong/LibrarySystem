#include <iostream>
#include <string>
#include <vector>
using namespace std;

/* =======================
   Book Structure
   ======================= */
struct Book
{
    int id;
    string title;
    string classification;
    bool isBorrowed;

    Book(int i, string t, string c)
        : id(i), title(t), classification(c), isBorrowed(false) {}
};





/* =======================
   Linked List for History
   ======================= */
struct HistoryNode
{
    string action;
    int bookID;
    string bookTitle;
    HistoryNode* next;

    HistoryNode(string a, int id,string title)
        : action(a), bookID(id), bookTitle(title),next(nullptr) {}
};






class HistoryList
{
private:
    HistoryNode* head;

public:
    HistoryList() : head(nullptr) {}

    void addHistory(string action, int bookID,string bookTitle)
    {
        HistoryNode* node = new HistoryNode(action, bookID,bookTitle);
        if (head == nullptr)
        {
            head = node;
            return;
        }
        HistoryNode* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }

    void disPlay() const
    {
        cout << "\n--- Borrow History ---\n";
        HistoryNode* cur = head;
        while (cur)
        {
             cout << cur->action
             << " | Book ID: " << cur->bookID
             << " | Title: " << cur->bookTitle
             << endl;
            cur = cur->next;
        }
    }
};

/* =======================
   Test Data
   ======================= */
void TestData(vector<Book>& books)
{
    books = {
        Book(101, "Data Structures", "Computer Science"),
        Book(102, "Operating Systems", "Computer Science"),
        Book(201, "Discrete Mathematics", "Mathematics"),
        Book(301, "Linear Algebra", "Mathematics"),
        Book(401, "Database Systems", "Computer Science")
    };
}

void bookHistory(HistoryList& history)
{
    history.addHistory("BORROW", 101,"Data Structures");
    history.addHistory("BORROW", 102,"Operating Systems");
    history.addHistory("RETURN", 101,"Data Structures");
}

/* =======================
   Book Management UI
   ======================= */
void addBookUI(vector<Book>& books)
{
    while (true)
    {
        int id;
        cout << "\nEnter Book ID (0 to stop): ";
        cin >> id;
        if (id == 0) break;

        cin.ignore();
        string title, cls;
        cout << "Title: ";
        getline(cin, title);
        cout << "Classification: ";
        getline(cin, cls);

        books.push_back(Book(id, title, cls));
        cout << "Book added.\n";
    }
}

void displayBooks(const vector<Book>& books)
{
    cout << "\n--- Book List ---\n";
    for (const auto& b : books)
    {
        cout << "ID: " << b.id
             << " | Title: " << b.title
             << " | Class: " << b.classification
             << " | Status: "
             << (b.isBorrowed ? "Borrowed" : "Available")
             << endl;
    }
}

/* =======================
   Borrow / Return UI
   ======================= */
void borrowBookUI(vector<Book>& books, HistoryList& history)
{
    while (true)
    {
        int id;
        cout << "\nEnter Book ID to borrow (0 to stop): ";
        cin >> id;
        if (id == 0) break;

        bool found = false;
        for (auto& b : books)
        {
            if (b.id == id)
            {
                found = true;
                if (b.isBorrowed)
                {
                    cout << "Book already borrowed.\n";
                }
                else
                {
                    b.isBorrowed = true;
                    history.addHistory("BORROW", id,b.title);
                    cout << "Borrow success.\n";
                }
                break;
            }
        }
        if (!found) cout << "Book not found.\n";
    }
}

void returnBookUI(vector<Book>& books, HistoryList& history)
{
    while (true)
    {
        int id;
        cout << "\nEnter Book ID to return (0 to stop): ";
        cin >> id;
        if (id == 0) break;

        bool found = false;
        for (auto& b : books)
        {
            if (b.id == id)
            {
                found = true;
                if (!b.isBorrowed)
                {
                    cout << "Book was not borrowed.\n";
                }
                else
                {
                    b.isBorrowed = false;
                    history.addHistory("RETURN", id,b.title);
                    cout << "Return success.\n";
                }
                break;
            }
        }
        if (!found) cout << "Book not found.\n";
    }
}




/* ========UI=========== 不要动*/  
void bookMenu(vector<Book>& books)
{
    int choice;
    while (true)
    {
        cout << "\n--- Book Management ---\n";
        cout << "1. Add Book\n";
        cout << "2. View Books\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) addBookUI(books);
        else if (choice == 2) displayBooks(books);
        else cout << "Invalid choice\n";
    }
}

void borrowMenu(vector<Book>& books, HistoryList& history)
{
    int choice;
    while (true)
    {
        cout << "\n--- Borrow / Return ---\n";
        cout << "1. Borrow Book\n";
        cout << "2. Return Book\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) borrowBookUI(books, history);
        else if (choice == 2) returnBookUI(books, history);
        else cout << "Invalid choice\n";
    }
}




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
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) bookMenu(books);
        else if (choice == 2) borrowMenu(books, history);
        else if (choice == 3) history.disPlay();
        else cout << "Invalid choice\n";
    }

    return 0;
}
