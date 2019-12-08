#include<fstream>
#include<iostream>
#include<typeinfo>
#include<process.h>


using std::cout;
using std::cin;
using std::endl;
using std::ios;
using std::fstream;
using std::cerr;
using std::ofstream;
using std::ifstream;

const int LEN = 32;
const int MAXEM = 100;

enum employee_type{tmanager, tscientist, tlaborer};
////////////////////////////////////////////////////////////
class employee
{
private:
    char name[LEN];
    unsigned long number;
    static int n;
    static employee* arrap[];
public:
    virtual void getdata()
    {
        cin.ignore(10, '\n');
        cout << " Enter last name: ";
        cin >> name;
        cout << " Enter ID: ";
        cin >> number;
    }
    virtual void putdata()
    {
        cout << "\n Last name: " << name;
        cout << "\n ID: " << number;
    }
    virtual employee_type get_type();
    static void add();
    static void display();
    static void read();
    static void write();
};
//-------------------------------------------------------------
int employee::n;
employee* employee::arrap[MAXEM];
////////////////////////////////////////////////////////////
class manager : public employee
{
private:
    char title[LEN];
    double bonus;
public:
    void getdata()
    {
        employee::getdata();
        cout << "Enter a position: ";
        cin >> title;
        cout << "Enter the bonus amount: ";
        cin >> bonus;
    }
    void putdata()
    {
        employee::putdata();
        cout << "\n Title: " << title;
        cout << "\n Bonus: " << bonus;
    }
};
////////////////////////////////////////////////////////////
class scientist : public employee
{
private:
    int pubs;
public:
    void getdata()
    {
        employee::getdata();
        cout << " Enter the number of publications: ";
        cin >> pubs;
    }
    void putdata()
    {
        employee::putdata();
        cout << "\n Number of publications: " << pubs;
    }
};
////////////////////////////////////////////////////////////
class laborer : public employee
{
};
////////////////////////////////////////////////////////////
void employee::add()
{
    char ch;
    cout << "'m' to add a Manager"
            "\n's' to add a scientist"
            "\n'l' to add a worker"
            "\nYour choice: ";
     cin >> ch;
     switch(ch)
     {
         case 'm': arrap[n] = new manager;
         break;
         case 's': arrap[n] = new scientist;
         break;
         case 'l': arrap[n] = new laborer;
         break;
         default: cout << "\nUnknown type of employee\n";
         return;
     }
     arrap[n++]->getdata();
}
//-----------------------------------------------------------
void employee::display()
{
    for(int j = 0; j < n; j++)
    {
        cout << (j + 1);
        switch(arrap[j]->get_type())
        {
            case tmanager: cout << ". Type: Manager";
            break;
            case tscientist: cout << ". Type: Scientist";
            break;
            case tlaborer: cout << ". Type: Worker";
            break;
            default: cout << ". Unknown type";
        }
        arrap[j]->putdata();
        cout << endl;
    }
}
//----------------------------------------------------------
employee_type employee::get_type()
{
    if(typeid(*this) == typeid(manager))
        return tmanager;
    else if(typeid(*this) == typeid(scientist))
        return tscientist;
    else if(typeid(*this) == typeid(laborer))
        return tlaborer;
    else
    {
        cerr << "\nWrong type of worker";
        exit(1);
    }
    return tmanager;
}
//----------------------------------------------------------
void employee::write()
{
    int size;
    cout << "Recording in progress " << n << " of workers.\n";
    ofstream ouf;
    employee_type etype;

    ouf.open("EMPLOY.DAT", ios::trunc | ios::binary);
    if(!ouf)
    {
        cout << "\nIt is impossible to open file\n";
        return;
    }
    for(int j = 0; j < n; j++)
    {
        etype = arrap[j]->get_type();

        ouf.write((char*)&etype, sizeof(etype));

        switch(etype)
        {
            case tmanager : size = sizeof(manager);
            break;
            case tscientist : size = sizeof(scientist);
            break;
            case tlaborer: size = sizeof(laborer);
            break;
        }
        ouf.write((char*)(arrap[j]), size);
        if(!ouf)
            cout << "\nWriting to file is not possible\n";
        return;
    }
}
//------------------------------------------------------------
void employee::read()
{
    int size;
    employee_type etype;
    ifstream inf;
    inf.open("EMPLOY.DAT", ios::binary);
    if(!inf)
    {
        cout << "\nUnable to open file\n";
        return;
    }

    n = 0;

    while(true)
    {
        inf.read((char*)&etype, sizeof(etype));
        if(inf.eof())
            break;
        if(!inf)
        {
            cout << "\nCould not read the type\n";
            return;
        }
    switch(etype)
    {
    case tmanager:
        arrap[n] = new manager;
        size = sizeof(manager);
        break;
    case tscientist:
        arrap[n] = new scientist;
        size = sizeof(scientist);
        break;
    case tlaborer:
        arrap[n] = new laborer;
        size = sizeof(laborer);
        break;
    default:
        cout << "\nUnknown file type\n";
        return;
    }
    inf.read((char*)arrap[n], size);
    if(!inf)
    {
        cout << "\nData cannot be read from the file\n";
        return;
    }
    n++;  //увеличить счетчик работников
    }
    cout << "Goes reading " << n << " of employees\n";
}
////////////////////////////////////////////////////////////
int main()
{
    char ch;

    while(true)
    {
        cout << "'a' - add information about the employee"
        "\n'd' - display information about all employees"
        "\n'w' - write all data to a file"
        "\n'r' - read all data from file"
        "\n'x' - exit"
        "\nYour choise: ";
        cin >> ch;
        switch(ch)
        {
        case 'a':
            employee::add();
            break;
        case 'd':
            employee::display();
            break;
        case 'w':
            employee::write();
            break;
        case 'r':
            employee::read();
            break;
        case 'x':
            exit(0);
            default:cout << "Unknown command";
        }
    }

    return 0;
}
