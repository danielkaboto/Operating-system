//importing the libraries needed for this project
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <getopt.h>

using namespace std;

//----------------------- prototypes of all the define function used --------------------------------// 
void Scheduling_Method();
void Preemptive_Mode();
void Show_Result();

// ----------------------- Variable definition and Declaration --------------------------------------//
int choice;

// link list prototypes
struct List_process *createNode(int , int , int );
struct List_process *insertBack(struct List_process *, int, int);
void Display(struct  List_process *);
struct List_process *Builder(ifstream& , char * , struct List_process *);   // read the file and take the data fills up the linked list

//----------------------- Definition of the linked list ----------------------------------------//
struct List_process
{
    int Id;     // store the ID of a process
    int Burst_time;
    int Arrival_time;
    int Priority;
    int Waiting_time;
    struct List_process *next;

};


void openFile(ifstream& inFile , char *fname)
{
    inFile.open(fname);
    if(inFile.is_open())
    {
        cout << "successfully opened file" << endl;
    }
    else
    {
        cout <<"Failed to open file" <<endl;
        exit(-1);
    }

}
void processFile(ifstream& inFile)
{
   
    string line,word;
    istringstream iss; // process the string 
    while(!inFile.eof())
    {
        ///read line by line
        getline(inFile,line);
        if (inFile.good()) // check if the process of getline was sucessfull
        {
          //  cout <<line <<endl;
        //read word by word form the line
         iss.clear(); // clear out the state
         iss.str(line);
         while(iss.good())
         {
            iss >> word ;
           // cout << word << endl;
           // read char by char from the word
        
           for( int i =0 ; i< word.length();i++)
           { 
                // check each character
                if(word[i]==':')
                    continue;
                else if(word[i] == '\n')
                    continue;
                else
                    cout << word[i] << endl;
           }
          
         }


        }
    }

}
int main(int argc , char *argv[])
{
    // reading the argument pass on the command line
    int counter;          // getopt variable holder
    char *File_in;      //store the input file
    char *File_out;    // store the ouput file

    // make sure the number of arguments given by the user is not less than 4
    if(argc < 4)
    {
        cout<<"Required: -f <fileInputName.txt> or -o <fileOutName.txt>" <<endl;
        return 1;
    }

    while((counter =getopt(argc,argv, "f:o:")) != -1)
    {
        switch (counter)
        {
        case 'f':
            File_in = optarg;   //assign the value of the optarg(content the file.txt used as input in the code) to File_in
            break;
        case 'o':
            File_out = optarg;   //assign the value of the optarg(content the file.txt used as output the code) to File_out
            break;
        // if the argument are not given prompt a error message 
        default :
            cout<<"Required: -f <fileInputName.txt> or -o <fileOutName.txt>" <<endl;
            return 1;
        }
    }

    // 1.open the file
    ifstream inFile;
    openFile(inFile, File_in);

    //2. Process the file
    processFile(inFile);
    inFile.close();
    //header = Builder(inFile,File_in)


    int choice;
    do
    {
        cout<< "-------------------- CPU SCHEDULING SIMULATOR-------------------------------"<< endl;
        cout<<" 1. SCHEDULING METHOD(NONE)\n 2. PREEMPTIVE MODE(OFF)\n 3. SHOW RESULT\n 4.END PROGRAM\n";
        cout<<" Option > ";
        cin >> choice;
        switch (choice)
        {
            case 1:
              //  inside_File_in(File_in, File_out);
                Scheduling_Method();
                
                break;
            case 2:
                Preemptive_Mode();
                break;
            case 3:
                Show_Result();
                break;
            case 4:
                return 0;
                break;
            
            default:
                cout <<"please enter the correct choice !!" << endl;
                break;
        }

    }
    while(choice>0 && choice <=4);

    return 0;

}
struct List_process *createNode( int Burst, int Arrival , int Priority)
{
    int num = 0;  // keep track on the Id number of new created process node
    struct List_process *temp;
    temp = (struct List_process *)malloc(sizeof(struct List_process));
    temp->Id = ++num;
    temp->Burst_time = Burst;
    temp->Arrival_time = Arrival;
    temp->Priority = Priority;
    temp->next = NULL;

    return temp;
}

struct List_process *insertBack(struct List_process *header, int Burst, int Arrival, int Priority)
{
    // create node
    struct List_process *temp = createNode(Burst, Arrival,Priority);
    struct List_process *headertemp;
    if(header == NULL)
    {
        header = temp;
        return header;
    }

    // find the end of the list
    headertemp = header;
    while(headertemp->next != NULL)
    {
        headertemp = headertemp->next;
    }

    headertemp->next = temp;
    return header;
}

void Display(struct List_process *header)
{
    if(header == NULL)
        puts("the list is empty");
    struct List_process *temp= header;
    while(temp != NULL)
    {
        cout <<"Process " << temp->Id << " : " << temp->Burst_time <<":"<<temp->Arrival_time
             <<":" << temp->Priority;
        temp= temp->next;
    }
    puts("");
}
void Scheduling_Method()
{

}

void Preemptive_Mode()
{
   

}

void Show_Result()
{
    // Read the input file and Write to the output file

  

}