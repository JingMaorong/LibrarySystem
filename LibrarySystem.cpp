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

        void enqueue(string name) {
        ReservationNode* newNode = new ReservationNode(name);
        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    string dequeue() {
        if (isEmpty()) return "";
        ReservationNode* temp = front;
        string name = temp->userName;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        return name;
    }

    void displayQueue() {
        ReservationNode* currNode = front;
        cout << "Reservation List: ";
        while (currNode != nullptr) {
            cout << currNode->userName << endl;
            currNode = currNode->next;
        }
        cout << endl;
    }
};

/* Use Linked List Implements Stack */
struct StackNode {
    string data;
    StackNode* next;
    StackNode(string value) : data(value), next(nullptr) {}
};

class LinkedListStack {
private:
    StackNode* top_node;

public:
    LinkedListStack() : top_node(nullptr) {}

    ~LinkedListStack() {
        clear();
    }

    void push(string value) {
        StackNode* newNode = new StackNode(value);
        newNode->next = top_node;
        top_node = newNode;
    }

    string pop() {
        if (isEmpty()) {
            return ""; // 或者抛出异常
        }
        StackNode* oldTop = top_node;
        string value = oldTop->data;
        top_node = top_node->next;
        delete oldTop;
        return value;
    }

    string peek() const {
        if (isEmpty()) {
            return "";
        }
        return top_node->data;
    }

    bool isEmpty() const {
        return top_node == nullptr;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    vector<string> getElementsAsVector() const {
        vector<string> elements;
        StackNode* current = top_node;
        while (current != nullptr) {
            elements.push_back(current->data);
            current = current->next;
        }
        return elements; // 返回的是从栈顶到栈底的顺序
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
    LinkedListStack statusStack; // 使用链表实现的栈

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
        cout << left << setw(5) << tree->data.id << setw(24) << tree->data.title << setw(20) << tree->data.classification << setw(10) << (tree->data.isBorrowed ? "Borrowed" : "Available") << endl;
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
        if (tree == nullptr) {
            tree = new TreeNode(b);
        }
        else if (b.id < tree->data.id)
            Insert(tree->left, b);
        else
            Insert(tree->right, b);
    }

    void DisplayBookList() const {
        cout << "\n--- Book List ---\n";
        cout << "-----------------------------------------------------------------\n"
             << left << setw(5) << "ID" << setw(24) << "Title" << setw(20) << "Class" << setw(10) << "Status" << endl
             << "-----------------------------------------------------------------\n";
        Print(root);
    }

    //打印特定书籍的状态历史
    void DisplayBookStatusHistory(int bookId){
        Book* book = SearchByID(bookId);
        if (book == nullptr) {
            cout << "Book with ID " << bookId << " not found." << endl;
            return;
        }

        cout << "\n--- Status History for Book: " << book->title << " (ID: " << book->id << ") ---" << endl;
        
        vector<string> history_list = book->statusStack.getElementsAsVector(); // 获取栈内元素
        
        if (history_list.empty()) {
            cout << "No status history available." << endl;
        } else {
            for (size_t i = 0; i < history_list.size(); ++i) {
                cout << "- " << history_list[i] << endl;
            }
        }
        cout << "Current Status: " << (book->isBorrowed ? "Borrowed" : "Available") << endl;
        cout << "Current User: " << (book->isBorrowed ? book->currentUser : "None") << endl;
    }
    
    //撤销最后一次操作
    bool UndoLastOperation() {
        TreeNode* current = root;
        TreeNode* parent = nullptr;
        int target_id = -1;

        // 寻找一个有状态历史且可撤销的书籍
        findLastModifiedBook(root, nullptr, target_id, parent);

        if(target_id == -1){
            cout << "No operation to undo." << endl;
            return false;
        }

        Book* book_to_undo = SearchByID(target_id);
        if (book_to_undo == nullptr || book_to_undo->statusStack.isEmpty()) {
            cout << "Error: Cannot undo on book with no status history." << endl;
            return false;
        }

        string last_status = book_to_undo->statusStack.pop(); // 弹出最后一条记录

        // 解析最后一条状态信息并撤销
        if (last_status.find("BORROWED") != string::npos) {
            // 撤销借阅 -> 恢复为可用
            book_to_undo->isBorrowed = false;
            book_to_undo->currentUser = "";
            cout << "Undo successful. Reverted BORROW of book ID " << book_to_undo->id << ". Status is now Available." << endl;
        }
        else if (last_status.find("RETURNED") != string::npos) {
            // 撤销归还 -> 恢复为借出状态
            // 注意：这需要知道上次是谁借的，我们从状态信息中解析
            size_t pos = last_status.find(" by ");
            if(pos != string::npos){
                string user_name = last_status.substr(pos + 4); // 提取 " by " 后的名字
                book_to_undo->isBorrowed = true;
                book_to_undo->currentUser = user_name;
                cout << "Undo successful. Reverted RETURN of book ID " << book_to_undo->id << ". Status is now Borrowed by " << user_name << "." << endl;
            } else {
                 cout << "Error: Could not parse previous borrower from status log. Cannot fully undo." << endl;
                 // 尽管解析失败，我们仍撤销了状态栈中的记录
                 return false;
            }
            
        }
        else {
            cout << "Error: Unknown status in history: " << last_status << endl;
            book_to_undo->statusStack.push(last_status); // 如果无法处理，则将记录放回去
            return false;
        }
        return true;
    }

private:
    // 辅助函数：查找最近修改过的书籍（即状态栈最深或最近有操作的）
    void findLastModifiedBook(TreeNode* node, TreeNode* p, int& latest_id, TreeNode*& parent_of_latest) const {
        if(node == nullptr) return;

        findLastModifiedBook(node->left, node, latest_id, parent_of_latest);
        
        // 检查当前节点
        if (!node->data.statusStack.isEmpty() && 
            (latest_id == -1 || node->data.id > latest_id)) { // 假设ID越大越新，或者可以按时间戳等
            latest_id = node->data.id;
            parent_of_latest = p;
        }
        
        findLastModifiedBook(node->right, node, latest_id, parent_of_latest);
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
             << left << setw(8) << "Action" << setw(10) << "Name" << setw(5) << "ID" << setw(24) << "Title" << endl
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
        if(head == nullptr) cout << "No history records found." << endl;
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
    b1->statusStack.push("Book ID 101 BORROWED by Adam");
    
    Book* b2 = books.SearchByID(102);
    b2->isBorrowed = true;
    b2->currentUser = "Lily";
    history.addHistory("BORROW", "Lily", 102, b2->title);
    b2->statusStack.push("Book ID 102 BORROWED by Lily");

    b1->isBorrowed = false;
    history.addHistory("RETURN", "Adam", 101, b1->title);
    b1->statusStack.push("Book ID 101 RETURNED by Adam");
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
                {
                    reserveBookUI(b, name);
                    break; // 用户已选择预约，跳出确认循环
                }
                else if (choice != 'Y' && choice != 'N')
                    cout << "Invalid input. Please enter your choice again (Y/N): ";

            }while(choice != 'Y' && choice != 'N');
        }
        else
        {
            b->isBorrowed = true;
            history.addHistory("BORROW", name, id, b->title);
            b->currentUser = name;
            b->statusStack.push("Book ID " + to_string(b->id) + " BORROWED by " + name);
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
        else {
            history.addHistory("RETURN", b->currentUser, id, b->title);
            b->statusStack.push("Book ID " + to_string(b->id) + " RETURNED by " + b->currentUser);

            if (!b->rQueue.isEmpty()) {
                string nextUser = b->rQueue.dequeue();
                b->currentUser = nextUser;
                b->isBorrowed = true;
                history.addHistory("BORROW", nextUser, id, b->title);
                b->statusStack.push("Book ID " + to_string(b->id) + " BORROWED by " + nextUser);
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
void queryBookStatusHistoryUI(BookTree& books) {
    int id;
    cout << "\nEnter Book ID to view its status history (0 to cancel): ";
    cin >> id;
    if (id == 0) return;
    books.DisplayBookStatusHistory(id);
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
        cout << "4. Query Book Status History\n"; 
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) addBookUI(books);
        else if (choice == 2) books.DisplayBookList();
        else if (choice == 3) searchBookUI(books);
        else if (choice == 4) queryBookStatusHistoryUI(books); 
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
        cout << "3. Undo Last Operation\n"; 
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) borrowBookUI(books, history);
        else if (choice == 2) returnBookUI(books, history);
        else if (choice == 3) books.UndoLastOperation(); 
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
