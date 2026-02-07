#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

/* Book Structure */
struct Book
{
    int id;
    string title;
    string classification;
    bool isBorrowed;

    Book(int i, string t, string c)
        : id(i), title(t), classification(c), isBorrowed(false) {}
};

/* 新加了Tree Implementation所以我把Vector去掉了, 有影响的也调了*/
/* Book Tree Node */
struct TreeNode
{
    Book data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Book b)
            : data(b), left(nullptr), right(nullptr) {}    
};

/* Book Tree Class */
class BookTree 
{   
    private:
        TreeNode* root;

    public:
        BookTree(){ root = nullptr; }
        ~BookTree(){ Destroy(root); }
        void Destroy(TreeNode* tree){
            if(tree!=nullptr){
                Destroy(tree->left);
                Destroy(tree->right);
                delete(tree);
            }
        }

        bool isEmpty() const{
            if(root == NULL)
                return true;
            else    
                return false;
        }

        int NumberOfNodes() const;

        TreeNode* SearchNode(TreeNode* tree, int id)
        {
            if (tree == nullptr || tree->data.id == id)
                return tree;

            if (id < tree->data.id)
                return SearchNode(tree->left, id);
            else
                return SearchNode(tree->right, id);
        }

        Book* SearchByID(int id)
        {
            TreeNode* result = SearchNode(root, id);
            if (result != nullptr)
                return &result->data;
            return nullptr;
        }



        void InsertBook(Book b){ Insert(root, b); }
        void Insert(TreeNode*& tree, Book b)
        {
            if (tree == NULL){
                tree = new TreeNode(b);
                tree->right = NULL;
                tree->left = NULL;
                tree->data = b;
            } else if (b.id < tree->data.id)
                Insert(tree->left, b);
            else
                Insert (tree->right, b);
        }

        void DeleteBook(Book b);

        void DisplayBookList() const{
            cout << "\n--- Book List ---\n";
            cout << "-------------------------------------------------------------\n"
                << left<<setw(5)<<"ID"<<setw(24)<<"Title"<<setw(20)<<"Class"<<setw(10)<<"Status"<<endl
                << "-------------------------------------------------------------\n";
            
            Print(root);
        }

        void Print(TreeNode* tree) const{
            if(tree == NULL)
                return;

            Print(tree->left);
            cout <<left
                 << setw(5) << tree->data.id
                 << setw(24) << tree->data.title
                 << setw(20) << tree->data.classification
                 << setw(10) << (tree->data.isBorrowed ? "Borrowed" : "Available")
                 << endl;
            
            Print(tree->right);
            
        }

};



//queueNode 链表队列
struct QueueNode
{
    int bookID;
    QueueNode* next;

    QueueNode(int id) : bookID(id), next(nullptr) {}
};



/*    Linked List for History   */
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
        cout << "-------- Borrow History --------\n";
        cout << "--------------------------------\n"
             << left << setw(8) << "Action"<< setw(5)<<"ID"<<setw(24)<<"Title"<<endl
             << "--------------------------------\n";
        HistoryNode* cur = head;
        while (cur)
        {
             cout <<left
             << setw(8) << cur->action
             << setw(5) << cur->bookID
             << setw(24) << cur->bookTitle
             << endl;
            cur = cur->next;
        }
    }
};

/* =======================
   Test Data
   ======================= */
void TestData(BookTree& books)
{
    books.InsertBook(Book(101, "Data Structures", "Computer Science"));
    books.InsertBook(Book(102, "Operating Systems", "Computer Science"));
    books.InsertBook(Book(201, "Discrete Mathematics", "Mathematics"));
    books.InsertBook(Book(301, "Linear Algebra", "Mathematics"));
    books.InsertBook(Book(401, "Database Systems", "Computer Science"));

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
void addBookUI(BookTree& books)
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

        books.InsertBook(Book(id, title, cls));
        cout << "Book added.\n";
    }
}

void displayBooks(BookTree& books)
{
    books.DisplayBookList();
}

/* =======================
   Borrow / Return UI
   ======================= */
void borrowBookUI(BookTree& books, HistoryList& history)
{
    while (true)
    {
        int id;
        cout << "\nEnter Book ID to borrow (0 to stop): ";
        cin >> id;
        if (id == 0) break;

        Book* b = books.SearchByID(id);

        if (b == nullptr)
        {
            cout << "Book not found.\n";
        }
        else if (b->isBorrowed)
        {
            cout << "Book already borrowed.\n";
        }
        else
        {
            b->isBorrowed = true;
            history.addHistory("BORROW", id, b->title);
            cout << "Borrow success.\n";
        }
    }
}


void returnBookUI(BookTree& books, HistoryList& history)
{
    while (true)
    {
        int id;
        cout << "\nEnter Book ID to return (0 to stop): ";
        cin >> id;
        if (id == 0) break;

        Book* b = books.SearchByID(id);

        if (b == nullptr)
        {
            cout << "Book not found.\n";
        }
        else if (!b->isBorrowed)
        {
            cout << "Book was not borrowed.\n";
        }
        else
        {
            b->isBorrowed = false;
            history.addHistory("RETURN", id, b->title);
            cout << "Return success.\n";
        }
    }

}



/*因为从vector换成tree, 所以改了一点点 大多在parameter*/
/* ========UI=========== 不要动*/  
void bookMenu(BookTree& books)
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

void borrowMenu(BookTree& books, HistoryList& history)
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
    BookTree books;
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
