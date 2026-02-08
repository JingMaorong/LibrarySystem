#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

//queueNode 链表队列
struct ReservationNode
{
    string userName;
    ReservationNode* next;

    ReservationNode(string name) : userName(name), next(nullptr) {}
};

class ReservationQueue
{
    private:
        ReservationNode* front;
        ReservationNode* rear;

    public:
        ReservationQueue() : front(nullptr), rear(nullptr) {}

        bool isEmpty(){
            return front == NULL;
        }

        void enqueue(string name){
            ReservationNode* newNode = new ReservationNode(name);

            if (rear == NULL){
                front = rear = newNode;
            } else {
                rear->next = newNode;
                rear = newNode;
            }
        }

        string dequeue(){
            if(isEmpty())
                return "";
            
            ReservationNode* temp = front;
            string name = temp->userName;

            front = front->next;

            if(front == NULL)
                rear = NULL;

            delete temp;
            return name;
        }

        void displayQueue(){
            ReservationNode* currNode =  front;
            cout << "Reservation List: ";
            while(currNode!=NULL){
                cout << currNode->userName << endl;
                currNode = currNode->next;
            }

            cout << endl;

        }
};

/* Book Structure */
struct Book
{
    int id;
    string title;
    string classification;
    string currentUser;
    bool isBorrowed;
    ReservationQueue rQueue;

    Book(int i, string t, string c)
        : id(i), title(t), classification(c), currentUser(""), isBorrowed(false) {}
};

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

        //binary seacrh implementation
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
                
            } else if (b.id < tree->data.id)
                Insert(tree->left, b);
            else
                Insert (tree->right, b);
        }

        void DisplayBookList() const{
            cout << "\n--- Book List ---\n";
            cout << "-------------------------------------------------------------\n"
                << left<<setw(5)<<"ID"<<setw(24)<<"Title"<<setw(20)<<"Class"<<setw(10)<<"Status"<<endl
                << "-------------------------------------------------------------\n";
            
            Print(root);
        }

        //inorder traversal
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





/*    Linked List for History   */
struct HistoryNode
{
    string action;
    string userName;
    int bookID;
    string bookTitle;
    HistoryNode* next;

    HistoryNode(string a, string u, int id,string title)
        : action(a), userName(u), bookID(id), bookTitle(title),next(nullptr) {}
};



class HistoryList
{
private:
    HistoryNode* head;

public:
    HistoryList() : head(nullptr) {}

    void addHistory(string action, string userName, int bookID, string bookTitle)
    {
        HistoryNode* node = new HistoryNode(action, userName, bookID,bookTitle);
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
             << left << setw(8) << "Action"<<setw(10)<<"Name"<< setw(5)<<"ID"<<setw(24)<<"Title"<<endl
             << "--------------------------------\n";
        HistoryNode* cur = head;
        while (cur)
        {
             cout <<left
             << setw(8) << cur->action
             << setw(10) << cur->userName
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

void bookHistory(BookTree& books, HistoryList& history)
{
    Book* b1 = books.SearchByID(101);
    b1->isBorrowed = true;
    b1->currentUser = "Adam";
    history.addHistory("BORROW", "Adam", 101, b1->title);

    Book* b2 = books.SearchByID(102);
    b2->isBorrowed = true;
    b2->currentUser = "Lily";
    history.addHistory("BORROW", "Lily", 102, b2->title);

    b1->isBorrowed = false;
    history.addHistory("RETURN", "Adam", 101, b1->title);
    b1->currentUser = "";
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


void searchBookUI(BookTree& books)
{
    while (true)
    {
        int id;
        cout << "\nEnter Book ID (0 to stop): ";
        cin >> id;
        if (id == 0) break;

        Book* b = books.SearchByID(id);

        if (b == nullptr)
            cout << "Book not found.\n";
        else
        {
            cout << "\nBook found\n";
            cout << "-------------------------------------------------------------\n"
                << left<<setw(5)<<"ID"<<setw(24)<<"Title"<<setw(20)<<"Class"<<setw(10)<<"Status"<<endl
                << "-------------------------------------------------------------\n";
            cout <<left
                 << setw(5) << b->id
                 << setw(24) << b->title
                 << setw(20) << b->classification
                 << setw(10) << (b->isBorrowed ? "Borrowed" : "Available")
                 << endl;
                    
        }
    }
}

/* =======================
   Borrow / Return UI
   ======================= */
void reserveBookUI(Book* b, string userName){
    b->rQueue.enqueue(userName);
    cout << "You are added to reservation queue for Book: "
         << b->title << endl;
    
}

void borrowBookUI(BookTree& books, HistoryList& history)
{
    while (true)
    {
        int id;
        string name;
        cout << "\nEnter your name (0 to stop): ";
        cin.ignore();
        getline(cin, name);
        if (name == "0") break;

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
            cout << "Do you want to reserve? (Y/N)\n";
            char choice;
            do{
                cin >> choice;
                if(choice == 'Y')
                    reserveBookUI(b, name);
                else if (choice != 'Y' && choice != 'N')
                    cout <<"Invalid input. Please enter your choice again (Y/N): ";

            }while(choice != 'Y' && choice != 'N');
        }
        else
        {
            b->isBorrowed = true;
            history.addHistory("BORROW", name, id, b->title);
            b->currentUser = name;
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
            history.addHistory("RETURN",b->currentUser, id, b->title);

            if(!b->rQueue.isEmpty()){
                string nextUser = b->rQueue.dequeue();
                b->currentUser = nextUser;
                b->isBorrowed = true;

                history.addHistory("BORROW", nextUser, id, b->title);
                cout << "Book assigned to next reserved user: " 
                     << nextUser << "." << endl;
            }
            else{
                
                b->currentUser = "";
                b->isBorrowed = false;
                cout << "Return success.\n";
            }

            
        }
    }

}



/* ========UI=========== 不要动*/  
void bookMenu(BookTree& books)
{
    int choice;
    while (true)
    {
        cout << "\n--- Book Management ---\n";
        cout << "1. Add Book\n";
        cout << "2. View All Books\n";
        cout << "3. Search Book By ID\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) addBookUI(books);
        else if (choice == 2) books.DisplayBookList();
        else if (choice == 3) searchBookUI(books);
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
    bookHistory(books, history);

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
