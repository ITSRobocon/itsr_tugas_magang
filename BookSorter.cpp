#include <iostream>
#include <vector>

using namespace std;
vector<int> unsorted;


class Books{
    public:
    string title;
    string publisher;
    int year;
    void bookCall(){
        cout<<"Title:\t\t"<<title;
        cout<<"\nPublisher:\t"<<publisher;
        cout<<"\nYear Published:\t"<<year<<endl;
    }
};

vector<Books> book;

void InsertionSortYear(){
    for(int j = 0;j<book.size();j++){
        if(j == 0) continue;
        for(int i = j-1;i>=0;i--){
            Books temp;
            if(book[j].year <= book[i].year){
                break;
            }
            else{
                temp = book[j];
                book[j] = book[i];
                book[i] = temp;
                j--;
            }
        }
    }
}

void InsertionSortTitle(){
    for(int j = 0;j<book.size();j++){
        if(j == 0) continue;
        for(int i = j-1;i>=0;i--){
            Books temp;
            if(book[j].title >= book[i].title){
                break;
            }
            else{
                temp = book[j];
                book[j] = book[i];
                book[i] = temp;
                j--;
            }
        }
    }
}

int yearValidation(){
    while(true){
        int toBe;
        string inputYear;
        cout<<"Year Published: ";
        getline(cin, inputYear);

        try {
            toBe = stoi(inputYear);
            if(toBe<0){
                cout<<"\nInvalid input, years can't go to negatives.\n";
            }
            else{
                cout<<endl;
                unsorted.push_back(toBe);
                return toBe;
                break;
            }
        }
        catch(const invalid_argument& e){
            cout<<"\nInvalid input, please input numbers only.\n";
        }

    }
}

bool continueAdd(){
    while(true){
        string input;
        cout<<"\nWould you like to continue adding more books? (y/n)\n> ";
        getline(cin, input);
    if(input == "y" || input == "Y"){
        return true;
        break;
    }
    else if(input == "n" || input == "N"){
        return false;
        break;
    }
        cout<<"\nInvalid input, please try again.\n";
        //continueAdd();
    }
}

void userInput(){
    int toBe;
    string inputTitle;
    string inputPub;
    string inputYear;
    bool isRunning = 1;

    cout<<"\nPlease input the books you would like to sort.";
    do{
        Books books;
        cout<<"\nBook Title: ";
        getline(cin, inputTitle);
        books.title = inputTitle;
        cout<<endl;


        cout<<"Publisher: ";
        getline(cin, inputPub);
        books.publisher = inputPub;
        cout<<endl;

        books.year = yearValidation();

        book.push_back(books);
        isRunning = continueAdd();

    } while(isRunning);
}

void userInputSortType(){
    while(true){
        string input;
        int result;
        cout<<"How would you like to sort the books by?\n1.Title (A-Z)\n2.Year (Newest to Oldest)\n3.Publisher Name (A-Z)\n";
        getline(cin, input);
        try {
            result = stoi(input);
            if(result<=0 || result > 3){
                cout<<"\nInvalid input, please input numbers 1 to 3 to pick one of the options.\n";
            }
            else{
                switch (result)
                {
                case 1:
                    InsertionSortTitle();
                    break;
                case 2:
                    InsertionSortYear();
                    break;
                case 3:
                    InsertionSortYear();
                    break;    
                }
                break;
                }
            }
        catch(const invalid_argument& e){
            cout<<"\nInvalid input, please input numbers 1 to 3 to pick one of the options.\n";
        }

    }
}

int main(){
    

    cout<<"\nWelcome to the Sorting Machine!";
    userInput();

    userInputSortType();
    //InsertionSortYear();

    cout<<"\nResult: ";
    for(int j = 0;j<book.size();j++){
        cout<<"\n\n";
        book[j].bookCall();
    }
}