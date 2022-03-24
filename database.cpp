#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ostream>
#include <cassert>
#include <csignal>
#include <regex>
#include <type_traits>
#include <set>

using namespace std;

const int MAX_MARTIANS = 100;

struct Martian {
    std::string fname_;
    std::string lname_;
    std::string SSN_;
    Martian(std::string fn, std::string ln, std::string ssn)
            : fname_(fn), lname_(ln), SSN_(ssn) { }

    // you may add other things to the structure
};

enum class Criteria { first, last, ssn};

class MartianDatabase {
public:
    // creates a new empty Martian Database initialized with the
    // contents of filename.
    MartianDatabase(std::istream& infile);
    // add item m to database, return true if successfully added.
    bool addMartian(const Martian &m);
    // delete item with the ssn in database, return true if successfully
    // deleted.
    bool deleteMartian(std::string ssn);
    // return true if the database does not contain any items, false
    // otherwise.
    bool isEmpty();
    // return true if the database is at its maximum storage, false
    // otherwise.
    bool isFull();
    // prints the list of Martians, sorted by last name, sorted
    // by first name and by MSSN.
    void printMartians(Criteria type);
    // deletes all the dynamically created memory
    ~MartianDatabase();
private:
    Martian* m_unsorted[MAX_MARTIANS];
    Martian* m_fname[MAX_MARTIANS];
    Martian* m_lname[MAX_MARTIANS];
    Martian* m_ssn[MAX_MARTIANS];
    int count = 0;

    // you may add other private members to the class
};

MartianDatabase::MartianDatabase(std::istream& infile){
    // creates a new empty Martian Database initialized with the
    // contents of filename.
//    cerr<<"in"<<endl;
    string str;
    string words[3];

    while(getline(infile, str)){
        istringstream ss(str);
//        cerr<<"str "<<str<<endl;
        int index = 0;
        string word;
        while (ss >> word){
            words[index] = word;
            index++;
        }
//        for (int i=0; i<3; i++){
//            cerr<<i<<","<<words[i]<<" ";
//        }
//        cerr<<endl;
        Martian *temp = new Martian(words[0],words[1],words[2]);
        m_unsorted[count] = temp;
        count++;
    }
}

bool MartianDatabase::addMartian(const Martian &m){
    // add item m to database, return true if successfully added.
    if (!isFull()){
        int temp_count = 0;
        while(temp_count != count){
            if(m_unsorted[temp_count]->SSN_ == m.SSN_){
                cout<<"Same SSN already used!"<<endl;
                return false;
                break;
            }
            temp_count++;
        }
        Martian *temp = new Martian(m.fname_, m.lname_, m.SSN_);
        m_unsorted[count] = temp;
        count++;
        return true;
    }
    cout<<"Our Database is already full!!"<<endl;
    return false;
}


bool MartianDatabase::deleteMartian(std::string ssn){
    // delete item with the ssn in database, return true if successfully
    // deleted.
    if (!isEmpty()){
        int temp_count = 0;
        while (temp_count != count){
            if (m_unsorted[temp_count]->SSN_ == ssn){
                delete m_unsorted[temp_count];
                count--;
                while (temp_count != count){
                    m_unsorted[temp_count] = m_unsorted[temp_count+1];
                    temp_count++;
                }
                return true;
                break;
            }
            temp_count++;
        }
        cout<<"SSN not found"<<endl;
        return false;
    }
    cout<<"There is no data in the database!!"<<endl;
    return false;
}


bool MartianDatabase::isEmpty(){
    // return true if the database does not contain any items, false
    // otherwise.
    if (count == 0)
        return true;
    return false;
}


bool MartianDatabase::isFull(){
    // return true if the database is at its maximum storage, false
    // otherwise.
    if (count == MAX_MARTIANS)
        return true;
    return false;
}

bool compareFname (Martian *a, Martian *b){
    return a->fname_ < b->fname_;
}
bool compareLname (Martian *a, Martian *b){
    return a->lname_ < b->lname_;
}
bool compareSsn (Martian *a, Martian *b){
    return a->SSN_ < b->SSN_;
}

void MartianDatabase::printMartians(Criteria type){
    // prints the list of Martians, sorted by last name, sorted
    // by first name and by MSSN.
    int temp_count = 0;
    switch (type){
        case Criteria::first:
            cout<<"----- Sorted by First Name -----"<<endl;
            cout<<"\tFirst Name\tLast Name\tMSSN"<<endl;
            while (temp_count != count){
                m_fname[temp_count] = m_unsorted[temp_count];
                temp_count++;
            }
            sort(m_fname,m_fname+count, compareFname);
            temp_count = 0;
            while (temp_count != count){
                cout<<temp_count+1<<'\t'<<m_fname[temp_count]->fname_<<"\t"
                    <<m_fname[temp_count]->lname_<<"\t"
                    <<m_fname[temp_count]->SSN_<<endl;
                temp_count++;
            }
            break;
        case Criteria::last:
            cout<<"----- Sorted by Last Name -----"<<endl;
            cout<<"\tFirst Name\tLast Name\tMSSN"<<endl;
            while (temp_count != count){
                m_lname[temp_count] = m_unsorted[temp_count];
                temp_count++;
            }
            sort(m_lname, m_lname+count, compareLname);
            temp_count = 0;
            while (temp_count != count){
                cout<<temp_count+1<<'\t'<<m_lname[temp_count]->fname_<<'\t'
                    <<m_lname[temp_count]->lname_<<"\t"
                    <<m_lname[temp_count]->SSN_<<endl;
                temp_count++;
            }
            break;
        case Criteria::ssn:
            cout<<"----- Sorted by Mars SSN -----"<<endl;
            cout<<"\tFirst Name\tLast Name\tMSSN"<<endl;
            while (temp_count != count){
                m_ssn[temp_count] = m_unsorted[temp_count];
                temp_count++;
            }
            sort(m_ssn, m_ssn+count, compareSsn);
            temp_count = 0;
            while (temp_count != count){
                cout<<temp_count+1<<'\t'<<m_ssn[temp_count]->fname_<<"\t"
                    <<m_ssn[temp_count]->lname_<<"\t"
                    <<m_ssn[temp_count]->SSN_<<endl;
                temp_count++;
            }
            break;
    }
}


MartianDatabase::~MartianDatabase(){
    // deletes all the dynamically created memory
    if (count != 0){
        int temp_count = 0;
//        cerr << "destructor" <<endl;
        while(temp_count != (count-1)){
            delete m_unsorted[temp_count];
//        delete m_fname[temp_count];
//        delete m_lname[temp_count];
//        delete m_ssn[temp_count];
            temp_count++;
        }
    }
}

int choiceMenu(){
    bool flag = true;
    cout<<"Martian Database\n"<<endl;
    cout<<"1. Add a martian"<<endl;
    cout<<"2. Delete a martian"<<endl;
    cout<<"3. Print by first name"<<endl;
    cout<<"4. Print by last name"<<endl;
    cout<<"5. Print by SSN"<<endl;
    cout<<"6. Quit"<<endl;
    cout<<"-------------------------"<<endl;
    while (flag){
        try{
            cout<<">";
            char input;
            cin.ignore(0,'\n');
            cin>>input;
            if (input > '0' && input < '7'){
                flag = false;
                return static_cast<int>(input) - 48;
            }else{
                flag = true;
                throw input;
            }
        }catch(char error){
            cout<<"Please enter an integer 1 to 6"<<endl;
            flag = true;
        }
    }
}
string enterValue(string str){
    cout<<str<<endl;
    string temp_str = "";
    cin>>temp_str;
    return temp_str;
}
void orderMenu(int sel, MartianDatabase &db){
    if (sel == 1){
        Martian *temp_add = new Martian(enterValue("Please enter a first name: "),
                                        enterValue("Please enter a last name: "),
                                        enterValue("Please enter a SSN: "));
        if (!db.addMartian(*temp_add)){
            cout<<"Add Error!"<<endl;
        }
    }else if (sel == 2){
        if (!db.deleteMartian(enterValue("Please enter a SSN: ")))
            cout<<"Delete Error!"<<endl;
    }else if (sel == 3){
        db.printMartians(Criteria::first);
    }else if (sel == 4){
        db.printMartians(Criteria::last);
    }else if (sel == 5){
        db.printMartians(Criteria::ssn);
    }else if (sel == 6){
        cout<<"Thank you for using Martian Database"<<endl;
        cout<<"Bye!!"<<endl;
    }
}

//Test cases
class streambuf_switcher
{
public:
    streambuf_switcher(ios& dest, ios& src)
            : dest_stream(dest), saved_streambuf(dest.rdbuf(src.rdbuf()))
    { }
    ~streambuf_switcher()
    {
        dest_stream.rdbuf(saved_streambuf);
    }
private:
    ios& dest_stream;
    streambuf* saved_streambuf;
};

set<void*> addrs;
bool recordaddrs = false;

void* operator new(size_t n)
{
    void* p = malloc(n);
    if (recordaddrs  &&  n == sizeof(Martian))
    {
        recordaddrs = false;
        addrs.insert(p);
        recordaddrs = true;
    }
    return p;
}

void operator delete(void* p) noexcept
{
    if (recordaddrs)
    {
        recordaddrs = false;
        set<void*>::iterator it = addrs.find(p);
        if (it != addrs.end())
            addrs.erase(it);
        recordaddrs = true;
    }
    free(p);
}

void operator delete(void* p, std::size_t s) noexcept
{
    s = addrs.size();   // these two lines do nothing other than
    s+=s;               // getting rid of unused var warning on g++
    if (recordaddrs)
    {
        recordaddrs = false;
        set<void*>::iterator it = addrs.find(p);
        if (it != addrs.end())
            addrs.erase(it);
        recordaddrs = true;
    }
    free(p);
}

void testone(int n)
{
    ostringstream strCout;
    streambuf_switcher sso(cout, strCout); // this is a problem
    istringstream in1("Edwin Ambrosio 9234\nMassoud Ghyam 8345\nSolomon Russell 7876\nSatish Singhal 3456");
    istringstream empty("");
//    Martian m; m.fname_ = "Joe"; m.lname_ = "Smith"; m.SSN_ = "55555";
//cerr << "Martian creation" << endl;
    Martian m {"Joe", "Smith", "55555"};  // if you used a constructor for Martian I used this
    switch (n)
    {
        default: {
            cout << "Bad argument" << endl;
        } break; case  1: {
            MartianDatabase md(empty);
        } break; case  2: {
            MartianDatabase md(in1);
            md.printMartians(Criteria::first);
            string str = strCout.str();
            regex e("1[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e = "3[^[:w:]]+Satish[^[:w:]]+Singhal";
            assert(regex_search(str, e));
        } break; case  3: {
            MartianDatabase md(in1);
            md.printMartians(Criteria::last);
            string str = strCout.str();
            regex e("1[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e = "4[^[:w:]]+Satish[^[:w:]]+Singhal";
            assert(regex_search(str, e));
        } break; case  4: {
            MartianDatabase md(in1);
            md.printMartians(Criteria::ssn);
            string str = strCout.str();
            regex e("4[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e =("1[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
        } break; case  5: {
            MartianDatabase md(in1);
            md.addMartian(m);
            md.printMartians(Criteria::first);
            string str = strCout.str();
            regex e("1[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e =("3[^[:w:]]+Massoud[^[:w:]]+Ghyam");
            assert(regex_search(str, e));
            e =("4[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
        } break; case  6: {
            MartianDatabase md(in1);
            md.addMartian(m);
            md.printMartians(Criteria::last);
            string str = strCout.str();
            regex e("1[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e =("2[^[:w:]]+Massoud[^[:w:]]+Ghyam");
            assert(regex_search(str, e));
            e =("5[^[:w:]]+Joe[^[:w:]]+Smith");
            assert(regex_search(str, e));
        } break; case  7: {
            MartianDatabase md(in1);
            md.addMartian(m);
            md.printMartians(Criteria::ssn);
            string str = strCout.str();
            regex e("5[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e =("4[^[:w:]]+Massoud[^[:w:]]+Ghyam");
            assert(regex_search(str, e));
            e =("2[^[:w:]]+Joe[^[:w:]]+Smith");
            assert(regex_search(str, e));
        } break; case  8: {
            MartianDatabase md(in1);
            md.deleteMartian("8345");
            md.printMartians(Criteria::first);
            string str = strCout.str();
            regex e ("2[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
            e =("3[^[:w:]]+Solomon[^[:w:]]+Russell");
            assert(regex_search(str, e));
        } break; case  9: {
            MartianDatabase md(in1);
            md.deleteMartian("8345");
            md.printMartians(Criteria::last);
            string str = strCout.str();
            regex e ("3[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
            e =("2[^[:w:]]+Solomon[^[:w:]]+Russell");
            assert(regex_search(str, e));
        } break; case  10: {
            MartianDatabase md(in1);
            md.deleteMartian("8345");
            md.printMartians(Criteria::ssn);
            string str = strCout.str();
            regex e ("1[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
            e =("2[^[:w:]]+Solomon[^[:w:]]+Russell");
            assert(regex_search(str, e));
        } break; case  11: {
            MartianDatabase md(in1);
            assert(!md.deleteMartian("5555"));
        } break; case  12: {
            MartianDatabase md(in1);
            assert(md.deleteMartian("8345"));
            assert(md.deleteMartian("3456"));
            assert(md.deleteMartian("7876"));
            assert(md.deleteMartian("9234"));
            assert(md.isEmpty());
        } break; case  13: {
            MartianDatabase md(in1);
            assert(md.deleteMartian("8345"));
            assert(md.deleteMartian("3456"));
            assert(md.deleteMartian("7876"));
            assert(md.deleteMartian("9234"));
            assert(!md.isFull());
        } break; case  14: {
            recordaddrs = true;
            int oldn = addrs.size();
            {
                MartianDatabase md(in1);
                md.addMartian(m);
            }
            recordaddrs = false;
            assert(addrs.size() == oldn);
        } break; case  15: {
            recordaddrs = true;
            int oldn = addrs.size();
            {
                MartianDatabase md(in1);
                assert(md.deleteMartian("3456"));
            }
            recordaddrs = false;
            assert(addrs.size() == oldn);
        } break;
    }
}


int main() {
//    ifstream infile;
//    string filename = "/Users/junnymac/Desktop/Fall 2021/CSCI 02 0130/Assignment/Project5/input.txt";
////    string filename = "input.txt";
//    infile.open(filename);  //Reading ok
//    MartianDatabase db = *new MartianDatabase(infile);
//
//    int selectMenu = -1;
//    while (selectMenu != 6){
//        selectMenu = choiceMenu();
//        orderMenu(selectMenu, db);
//    }

    //Test cases
    cout << "Enter test number: ";
    int n;
    cin >> n;
    testone(n);
    cout << "Passed" << endl;

    // It was passed except for 14, 15

    return 0;
}
