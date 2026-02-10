#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

/* =======================
   Queue Implementation
   ======================= */

//Queue Node Structure: store reservation data(username)
struct ReservationNode
{
    string userName;            //store reservation record with username
    ReservationNode* next;
    ReservationNode(string name) : userName(name), next(nullptr) {}
};

//Queue Implementation: Reservation queue for each book
class ReservationQueue
{
    private:
        ReservationNode* front;
        ReservationNode* rear;

    public:
        ReservationQueue() : front(nullptr), rear(nullptr) {}
        ~ReservationQueue() {
            ReservationNode* cur = front;
            while(cur != NULL){
                ReservationNode* temp = cur;
                cur = cur->next;
                delete temp;
            }
            front = rear = NULL;
        }

        bool isEmpty(){    //if queue is empty, no next user assigned
            return front == NULL;
        }

        void enqueue(string name) {     //add new reservation
            ReservationNode* newNode = new ReservationNode(name);
            if (rear == nullptr) {
                front = rear = newNode;
            }
            else {
                rear->next = newNode;
                rear = newNode;
            }
        }

        string dequeue() {     //assign book to next queueing user
            if (isEmpty()) return "";
            ReservationNode* temp = front;
            string name = temp->userName;
            front = front->next;
            if (front == nullptr) rear = nullptr;
            delete temp;
            return name;
        }

};


/* Book Structure */
struct Book
{
    int id;
    string title;
    string classification;
    string currentUser;     //record who is holding the book
    bool isBorrowed;
    ReservationQueue rQueue;    //reservation queue for this book

    Book(int i, string t, string c)
        : id(i), title(t), classification(c), currentUser(""), isBorrowed(false) {}
};



/* =======================
   Stack Implementation
   ======================= */

//Action structure: store ADD, DELETE, BORROW, RETURN actions
struct Action {
    string type;
    Book book_copy;     //copy of whole book structure to support undo

    Action() : type(""), book_copy(0,"","") {}
};


//Stack implementation using Linked List
//Track system actions and support undo operation
//StackNode: store most recent action
struct StackNode {
    Action data;
    StackNode* next;
    StackNode(Action value) : data(value), next(nullptr) {}
};

//Action Stack: stack of most recent action to support undo
class ActionStack {
private:
    StackNode* top_node;

public:
    ActionStack() : top_node(nullptr) {}
    ~ActionStack() {
        clear();
    }

    void push(Action value) {
        StackNode* newNode = new StackNode(value);
        newNode->next = top_node;
        top_node = newNode;
    }

    Action pop() {
        if (isEmpty()) {
            return Action(); // 或者抛出异常
        }
        StackNode* oldTop = top_node;
        Action value = oldTop->data;
        top_node = top_node->next;
        delete oldTop;
        return value;
    }

    bool isEmpty() const {
        return top_node == nullptr;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};




/* ==========================
   Linked List Implementatian
   ========================== */

//History Node: stores action history
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

//History Linked List
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

    void displayHistory() const
    {
        cout << "------------ Borrow History ------------\n";
        cout << "----------------------------------------\n"
             << left << setw(8) << "Action" << setw(10) << "Name" << setw(5) << "ID" << setw(24) << "Title" << endl
             << "----------------------------------------\n";
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
        if(head == nullptr) cout << "No history records found." << endl;
    }

    void displayBookHistory(int bookID) const{
        HistoryNode* cur = head;
        bool found = false;

        while (cur)
        {
            if (cur->bookID == bookID)
            {
                cout << left
                    << setw(8) << cur->action
                    << setw(10) << cur->userName
                    << setw(5) << cur->bookID
                    << setw(24) << cur->bookTitle
                    << endl;
                found = true;
            }
            cur = cur->next;
        }

        if (!found)
            cout << "No history found for this book.\n";
    }

    //remove the last history entry when user choose undo operation
    void removeLastHistory() {      
        if (head == NULL) return;
        if (head->next == NULL){
            delete head;
            head = NULL;
            return;
        }

        HistoryNode* cur = head;
        while (cur->next->next !=NULL)
            cur = cur->next;

        delete cur->next;
        cur->next = NULL;
    }
};



/* ====================
   Tree Implementation
   ==================== */

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
class BookTree {
private:
    TreeNode* root;

    void Destroy(TreeNode* tree) {
        if (tree != nullptr) {
            Destroy(tree->left);
            Destroy(tree->right);
            delete(tree);
        }
    }

    TreeNode* SearchNode(TreeNode* tree, int id) {
        if (tree == nullptr || tree->data.id == id)
            return tree;
        if (id < tree->data.id)
            return SearchNode(tree->left, id);
        else
            return SearchNode(tree->right, id);
    }

    void Print(TreeNode* tree) const {
        if (tree == nullptr) return;
        Print(tree->left);
        cout << left 
             << setw(5) << tree->data.id 
             << setw(24) << tree->data.title 
             << setw(20) << tree->data.classification 
             << setw(10) << (tree->data.isBorrowed ? "Borrowed" : "Available") 
             << endl;

        Print(tree->right);
    }

public:
    BookTree() { root = nullptr; }

    ~BookTree() { Destroy(root); }

    bool isEmpty() const {
        if (root == nullptr) return true;
        else return false;
    }

    Book* SearchByID(int id) {
        TreeNode* result = SearchNode(root, id);
        if (result != nullptr) return &result->data;
        return nullptr;
    }

    void InsertBook(Book b) {
        Insert(root, b);
    }

    void Insert(TreeNode*& tree, Book b) {
        if (tree == nullptr) tree = new TreeNode(b);
        else if (b.id < tree->data.id) Insert(tree->left, b);
        else Insert(tree->right, b);
    }

    void DeleteBook(int id) {
        root = Delete(root, id);
    }

    
    TreeNode* Delete(TreeNode* tree, int id)
    {
        if (tree == nullptr) return nullptr;

        if (id < tree->data.id) tree->left = Delete(tree->left, id);

        else if (id > tree->data.id) tree->right = Delete(tree->right, id);

        else {
            // Case 1: No child
            if (tree->left == nullptr && tree->right == nullptr) {
                delete tree;
                return nullptr;
            }

            // Case 2: One child
            else if (tree->left == nullptr) {
                TreeNode* temp = tree->right;
                delete tree;
                return temp;
            }
            else if (tree->right == nullptr) {
                TreeNode* temp = tree->left;
                delete tree;
                return temp;
            }
        }

        return tree;
    }


    void DisplayBookList() const {
        cout << "\n--- Book List ---\n";
        cout << "-----------------------------------------------------------------\n"
             << left << setw(5) << "ID" << setw(24) << "Title" << setw(20) << "Class" << setw(10) << "Status" << endl
             << "-----------------------------------------------------------------\n";
        Print(root);
    }

    //打印特定书籍的状态历史
    void DisplayBookStatusHistory(int bookId, HistoryList& history){
        Book* book = SearchByID(bookId);
        if (book == nullptr) {
            cout << "Book with ID " << bookId << " not found." << endl;
            return;
        }

        cout << "\n--- Status History for Book: ---\n" 
             << "Book: " << book->title << " (ID: " << book->id << ")" << endl;
        cout << "-----------------------------------------\n"
             << left << setw(8) << "Action" << setw(10) << "Name" << setw(5) << "ID" << setw(24) << "Title" << endl
             << "-----------------------------------------\n";
        
        history.displayBookHistory(bookId);
        cout << "\nCurrent Status: " << (book->isBorrowed ? "Borrowed" : "Available") << endl;
        cout << "Current User: " << (book->isBorrowed ? book->currentUser : "None") << endl;
    }
    
};

//Undo operation
void undoLastAction(BookTree& books, ActionStack& action, HistoryList& history)
{
    if (action.isEmpty()) {
        cout << "No operation to undo.\n";
        return;
    }

    Action last = action.pop();
    Book* b = books.SearchByID(last.book_copy.id);
    
    if (last.type == "ADD") {
        books.DeleteBook(last.book_copy.id);
        history.removeLastHistory();
        cout << "Undo ADD Book: "<<last.book_copy.title<<" successful.\n";

    }
    else if (last.type == "DELETE") {
        books.InsertBook(last.book_copy);
        history.removeLastHistory();
        cout << "Undo DELETE Book: "<<last.book_copy.title<<" successful.\n";

    }
    else if (b != nullptr) {
        
        if (last.type == "BORROW") {
            b->isBorrowed = false;
            b->currentUser = "";
            b->rQueue.enqueue(last.book_copy.currentUser); // put back into reservation queue
            history.removeLastHistory();
            cout << "Undo BORROW Book: "<<b->title<<" successful.\n";
            
        }
        else if (last.type == "RETURN") {
            b->isBorrowed = true;
            b->currentUser = last.book_copy.currentUser;
            history.removeLastHistory();
            cout << "Undo RETURN Book: "<<b->title<<" successful.\n";
            
        }
        else if (last.type == "MODIFY"){
            *b = last.book_copy;
            history.removeLastHistory();
            cout << "Undo MODIFY Book: "<<b->title<<" successful.\n";
        }
        
    }
    else{
        cout << "Book not found.\n";
        return;
    }
}


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

void bookHistory(BookTree& books, HistoryList& history, ActionStack& action)
{
    Book* b1 = books.SearchByID(101);
    b1->isBorrowed = true;
    b1->currentUser = "Adam";
    history.addHistory("BORROW", "Adam", 101, b1->title);
    Action a;
    a.type = "BORROW";
    a.book_copy = *b1;
    action.push(a);
    
    Book* b2 = books.SearchByID(102);
    b2->isBorrowed = true;
    b2->currentUser = "Lily";
    history.addHistory("BORROW", "Lily", 102, b2->title);
    Action b;
    b.type = "BORROW";
    b.book_copy = *b2;
    action.push(b);

    b1->isBorrowed = false;
    history.addHistory("RETURN", "Adam", 101, b1->title);
    b1->currentUser = "";
    Action c;
    c.type = "RETURN";
    c.book_copy = *b1;
    action.push(c);
}


/* =======================
   Book Management UI
   ======================= */
void addBookUI(BookTree& books, ActionStack& action, HistoryList& history)
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
        Action a;
        a.type = "ADD";
        a.book_copy = Book(id, title, cls);
        action.push(a);
        history.addHistory("ADD", "-", id, title);
        cout << "Book '"<<title<<"' is added.\n";
        
    }
}

void deleteBookUI(BookTree& books, ActionStack& action, HistoryList& history){
    while (true)
    {
        int id;
        cout << "\nEnter Book ID (0 to stop): ";
        cin >> id;
        if (id == 0) break;

        Book* temp = books.SearchByID(id);
        if(temp == NULL){
            cout <<"Book not found.\n";
            continue;
        }

        cout << "Book '"<<temp->title<<"' is removed.\n";
        Action a;
        a.type = "DELETE";
        a.book_copy = *temp;
        action.push(a);
        history.addHistory("DELETE", "-", id, temp->title);
        books.DeleteBook(id);
        
        
    }
}

void modifyBookUI(BookTree& books, ActionStack& action, HistoryList& history){
    while (true)
    {
        int id;
        cout << "\nEnter Book ID (0 to stop): ";
        cin >> id;
        if (id == 0) break;

        Book* temp = books.SearchByID(id);
        if(temp == NULL){
            cout <<"Book not found.\n";
            continue;
        }

        Action a;
        a.type = "MODIFY";
        a.book_copy = *temp;
        action.push(a);

        cout<<"Enter the new title: ";
        cin.ignore();
        getline(cin, temp->title);
        
        cout<<"Enter the new classification: ";
        getline(cin, temp->classification);

        history.addHistory("MODIFY", "-", temp->id, temp->title);

        cout<< "Book modification success!\n"; 

        
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

void borrowBookUI(BookTree& books, HistoryList& history, ActionStack& action)
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
                {
                    reserveBookUI(b, name);
                    cout << "You are added into the reservation";
                    break; // 用户已选择预约，跳出确认循环
                }
                else if (choice != 'Y' && choice != 'N')
                    cout << "Invalid input. Please enter your choice again (Y/N): ";

            }while(choice != 'Y' && choice != 'N');
        }
        else
        {
            Action a;
            a.type = "BORROW";
            a.book_copy = *b;   //copy of book data for undo modification
            action.push(a);
            b->isBorrowed = true;
            history.addHistory("BORROW", name, id, b->title);
            b->currentUser = name;
            cout << "Borrow success.\n";
        }
    }
}

void returnBookUI(BookTree& books, HistoryList& history, ActionStack& action)
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
        else {
            string originalUser = b->currentUser;

            // Push RETURN action first
            Action returnAction;
            returnAction.type = "RETURN";
            returnAction.book_copy = *b;
            action.push(returnAction);

            history.addHistory("RETURN", originalUser, id, b->title);

            if (!b->rQueue.isEmpty()) {

                string nextUser = b->rQueue.dequeue();

                // Push BORROW for next user
                Action borrowAction;
                borrowAction.type = "BORROW";
                borrowAction.book_copy = *b;
                action.push(borrowAction);

                b->currentUser = nextUser;
                b->isBorrowed = true;

                history.addHistory("BORROW", nextUser, id, b->title);

                cout << "Book assigned to next reserved user: "
                    << nextUser << "." << endl;
            }
            else {
                b->currentUser = "";
                b->isBorrowed = false;
                cout << "Return success.\n";
            }

        }
    }
}

//查询特定书籍状态历史的UI
void queryBookStatusHistoryUI(BookTree& books, HistoryList& history) {
    int id;
    cout << "\nEnter Book ID to view its status history (0 to cancel): ";
    cin >> id;
    if (id == 0) return;
    books.DisplayBookStatusHistory(id, history);
}



/* ========UI=========== 不要动*/  
void bookMenu(BookTree& books, ActionStack& action, HistoryList& history)
{
    int choice;
    while (true)
    {
        cout << "\n--- Book Management ---\n";
        cout << "1. Add Book\n";
        cout << "2. Delete Book\n";
        cout << "3. Modify Book\n";
        cout << "4. View All Book\n";
        cout << "5. Search Book (ID)\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) addBookUI(books, action, history);
        else if (choice == 2) deleteBookUI(books, action, history);
        else if (choice == 3) modifyBookUI(books, action, history);
        else if (choice == 4) books.DisplayBookList();
        else if (choice == 5) searchBookUI(books);

        else cout << "Invalid choice\n";
    }
}

void borrowMenu(BookTree& books, HistoryList& history, ActionStack& action)
{
    int choice;
    while (true)
    {
        cout << "\n--- Borrow / Return ---\n";
        cout << "1. Borrow Book\n";
        cout << "2. Return Book\n";
        cout << "3. View Book Status History\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) borrowBookUI(books, history, action);
        else if (choice == 2) returnBookUI(books, history, action);
        else if (choice == 3) queryBookStatusHistoryUI(books, history); 
        else cout << "Invalid choice\n";
    }
}




int main()
{
    BookTree books;
    HistoryList history;
    ActionStack action;
    TestData(books);
    bookHistory(books, history, action);

    int choice;
    while (true)
    {
        cout << "\n=== Library System ===\n";
        cout << "1. Book Management\n";
        cout << "2. Borrow / Return\n";
        cout << "3. View History\n";
        cout << "4. Undo Last Operation\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        if (choice == 0) break;
        if (choice == 1) bookMenu(books, action, history);
        else if (choice == 2) borrowMenu(books, history, action);
        else if (choice == 3) history.displayHistory();
        else if (choice == 4) undoLastAction(books, action, history);
        else cout << "Invalid choice\n";
    }

    return 0;
}
