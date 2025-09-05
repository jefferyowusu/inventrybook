#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct Book {
    string id;
    string title;
    string author;
    int quantity;
    double price;
};

// --- User Progress Functions ---
void loadProgress(const string& userId) {
    string filename = userId + "_progress.txt";
    ifstream file(filename);
    if (file.is_open()) {
        string lastAction;
        getline(file, lastAction);
        cout << "Welcome back, " << userId << "!\n";
        cout << "Your last action was: " << lastAction << "\n";
        file.close();
    } else {
        cout << "Welcome, " << userId << "! Starting fresh.\n";
    }
}

void saveProgress(const string& userId, const string& action) {
    string filename = userId + "_progress.txt";
    ofstream file(filename);
    if (file.is_open()) {
        file << action << "\n";
        file.close();
    } else {
        cerr << "Could not save progress for " << userId << "\n";
    }
}

// --- Book File Handling ---
void writeBookToFile(const Book& book, bool append = true) {
    ofstream file("books.txt", append ? ios::app : ios::trunc);
    if (file.is_open()) {
        file << book.id << "," << book.title << "," << book.author << ","
             << book.quantity << "," << fixed << setprecision(2) << book.price << "\n";
        file.close();
    } else {
        cerr << "Error: Could not write to file.\n";
    }
}

vector<Book> readBooksFromFile() {
    vector<Book> books;
    ifstream file("books.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, title, author, qtyStr, priceStr;
        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, qtyStr, ',');
        getline(ss, priceStr, ',');
        Book b = {id, title, author, stoi(qtyStr), stod(priceStr)};
        books.push_back(b);
    }
    return books;
}

// --- Core Functions ---
void addBook() {
    Book b;
    cout << "Enter 8-digit Unique ID: ";
    cin >> b.id;
    if (b.id.length() != 8) {
        cout << "Invalid ID length.\n";
        return;
    }

    cin.ignore(); // clear buffer
    cout << "Enter title: ";
    getline(cin, b.title);
    cout << "Enter author: ";
    getline(cin, b.author);
    cout << "Enter quantity: ";
    cin >> b.quantity;
    if (b.quantity < 0) {
        cout << "Quantity must be non-negative.\n";
        return;
    }

    cout << "Enter price: ";
    cin >> b.price;
    if (b.price < 0) {
        cout << "Price must be non-negative.\n";
        return;
    }

    writeBookToFile(b);
    cout << "Book added successfully.\n";
}

void viewBooks() {
    vector<Book> books = readBooksFromFile();
    if (books.empty()) {
        cout << "No books found.\n";
        return;
    }
    for (const Book& b : books) {
        cout << "\nID: " << b.id
             << "\nTitle: " << b.title
             << "\nAuthor: " << b.author
             << "\nQuantity: " << b.quantity
             << "\nPrice: $" << fixed << setprecision(2) << b.price << "\n";
    }
}

void updateBook() {
    string id;
    cout << "Enter the ID of the book to update: ";
    cin >> id;

    vector<Book> books = readBooksFromFile();
    bool found = false;

    for (Book& b : books) {
        if (b.id == id) {
            found = true;
            cin.ignore();
            cout << "Enter new title: ";
            getline(cin, b.title);
            cout << "Enter new author: ";
            getline(cin, b.author);
            cout << "Enter new quantity: ";
            cin >> b.quantity;
            cout << "Enter new price: ";
            cin >> b.price;
            break;
        }
    }

    if (found) {
        ofstream file("books.txt", ios::trunc);
        for (const Book& b : books) {
            writeBookToFile(b, true);
        }
        cout << "Book updated successfully.\n";
    } else {
        cout << "Book with ID " << id << " not found.\n";
    }
}

void deleteBook() {
    string id;
    cout << "Enter the ID of the book to delete: ";
    cin >> id;

    vector<Book> books = readBooksFromFile();
    bool found = false;
    vector<Book> updatedBooks;

    for (const Book& b : books) {
        if (b.id == id) {
            found = true;
            continue;
        }
        updatedBooks.push_back(b);
    }

    if (found) {
        ofstream file("books.txt", ios::trunc);
        for (const Book& b : updatedBooks) {
            writeBookToFile(b, true);
        }
        cout << "Book deleted successfully.\n";
    } else {
        cout << "Book with ID " << id << " not found.\n";
    }
}

void displayMenu() {
    cout << "\n--------- MAIN MENU ---------\n";
    cout << "1. Add a new book\n";
    cout << "2. View all books\n";
    cout << "3. Update a book\n";
    cout << "4. Delete a book\n";
    cout << "5. Exit\n";
    cout << "Choose an option (1-5): ";
}

// --- Main Program ---
int main() {
    string userId;
    cout << "Enter your User ID: ";
    cin >> userId;

    loadProgress(userId);

    int choice;
    do {
        displayMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                addBook();
                saveProgress(userId, "Added a new book");
                break;
            case 2:
                viewBooks();
                saveProgress(userId, "Viewed inventory");
                break;
            case 3:
                updateBook();
                saveProgress(userId, "Updated a book");
                break;
            case 4:
                deleteBook();
                saveProgress(userId, "Deleted a book");
                break;
            case 5:
                cout << "Exiting program. Goodbye!\n";
                saveProgress(userId, "Exited the program");
                break;
            default:
                cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
