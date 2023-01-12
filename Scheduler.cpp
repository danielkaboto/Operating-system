//------------------------- Importing the libraries needed for this project---------------------------//
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <climits>
#define  size  1024  
#define TRUE 1
#define FALSE 0       

using namespace std;
//----------------------------------------- Structure using to keep values when working with Preemptive -----------------------//
struct _process
{
    int id;
    int arrival_time;
    int waiting_time;
    int return_time;
    int turnaround_time;
    int response_time;
    int burst;
    int priority;
    int completed;
}p[size];
//------------------------Prototypes function of  all  Scheduling method --------------------------------//
void First_come();
void Shortest_jobNP();              // shortest_jobs for the Non-preemptive
void Shortest_jobP(_process *);               // shortest_jobs for the Preemptive
void Priority_schNP();              // Priority scheduling for the Non-preemptive
void Priority_schP() ;               // Priority scheduling for the Preemptive
void Round_robin();

//----------------------- prototypes of all the define function used --------------------------------// 
int Menu(string );   // Prompt the menu to the user on the screen
void Scheduling_Method(string);
void Preemptive_Mode(string,string);
int Show_Result(struct List_process * , string, double);
void show_resultP(struct List_process *, string);
int process_counter(struct List_process *);

//------------------------------------- link list prototypes ---------------------------------------//
struct List_process *createNode(int , int , int );
struct List_process *insertBack(struct List_process *, int, int,int);
void Display(struct  List_process *);
struct List_process *cloneList(struct List_process *);  

//----------------------------................ FCFS Algorithm --------------------------------------------------//
int waiting_time(struct List_process *);
int Turn_around_time(struct List_process *);
void Average_time(struct List_process *, string);
//--------------------------------------------Sorted Algorithm --------------------------------------------------------//
void mysort_Br(struct List_process *&,struct List_process *,struct List_process *); //sorting according to burst time
void mysort_Pr(struct List_process *&,struct List_process *,struct List_process *);  // sorting according to Priority
void mysort_Ar(struct List_process *&,struct List_process *,struct List_process *);  // sorting according to the Arrival time
//--------------------------------------------Prototypes function Waiting time for SJF , PR, RR for Preemptive mode -------------// 
void Time_calP(struct List_process *);
void Time_cal(struct List_process *);
void Time_calR(struct List_process *, int);
//--------------------------------------------- Definition of the linked list ----------------------------------------//
struct List_process
{
    int Id;     // store the ID of a process
    int Burst_time;
    int Arrival_time;
    int Priority;
    int Waiting_time;
    int Turn_around_time;
    double Avg_wt;
    double Avg_tat;
    string mode;
    struct List_process *next;

};

// ------------------------------------------- Variable definition and Declaration --------------------------------------//
static int choice;
static string choice_str ="OFF";
int counter;          // getopt variable holder
char *File_in;      //store the input file
char *File_out;    // store the ouput file
struct List_process *header=NULL;     //original linked list root
int num = 0;  // keep track on the Id number of new created process node

 // 1.open the file and check if it's open properly without error
ifstream inFile;
ofstream outFile;
char buffer[size];
char Final_buffer[size * 6];

//---------------------------- function to open the input file-----------------------------------------------------------//
void openFileI(ifstream& inFile , char *fname)
{
    inFile.open(fname); // open the file
    // check if the file was opened 
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
void openFileO(ofstream& outFile , char *fname)
{
    outFile.open(fname); // open the file
    // check if the file was opened 
    if(outFile.is_open())
    {
        cout << "successfully opened file" << endl;
    }
    else
    {
        cout <<"Failed to open file" <<endl;
        exit(-1);
    }

}
//----------------------------- Function for filling the list with input text values ------------------------------------//
struct List_process *processFile(ifstream& inFile, struct List_process *hdr) // read the file and take the data fills up the linked list
{
    string line;  // Buffer for storing a line of the file
    char word[size];  // Buffer for storing a word from the file
    int Burst, Arrival, Priority;  // Variables for storing the values
    istringstream iss;  // Stream for processing the string

    if (inFile.good())  // Check if the file was opened successfully
    {
        while (!inFile.eof())  // Read until the end of the file
        {
            // Read a line from the file
            getline(inFile, line);
            // Clear the stream and assign the line to it
            iss.clear();
            iss.str(line);
            // Clear the stream and assign the line to it
            iss.clear();
            iss.str(line);
            // Initialize the variables to 0
            Burst = 0;
            Arrival = 0;
            Priority = 0;
            // Read the characters of the first column until the ':' character
            int i = 0;
            while (line[i] != ':' && line[i] != '\0')
            {
                word[i] = line[i];
                i++;
            }
            word[i] = '\0';  // Null terminate the string
            // If the ':' character was found, convert the first column to an integer
            if (line[i] == ':')
            {
                Burst = atoi(word);  // Convert the string to an integer
            }
            // Advance the counter past the ':' character
            i++;
            // Read the characters of the second column until the ':' character
            int j = 0;
            while (line[i] != ':' && line[i] != '\0')
            {
                word[j] = line[i];
                i++;
                j++;
            }
            word[j] = '\0';  // Null terminate the string
            // If the ':' character was found, convert the second column to an integer
            if (line[i] == ':')
            {
                Arrival = atoi(word);  // Convert the string to an integer
            }
            // Advance the counter past the ':' character
            i++;
            // Read the characters of the third column until the end of the line
            j = 0;
            while (line[i] != '\0')
            {
                word[j] = line[i];
                i++;
                j++;
            }
            word[j] = '\0';  // Null terminate the string
            // Convert the third column to an integer
            Priority = atoi(word);
            // Insert the values into the linked list
            hdr = insertBack(hdr, Burst, Arrival, Priority);
        }
    }
    
    return hdr;

}
void process_init(struct List_process *ht)
{
    int i=0;
    struct List_process *h= ht;
    if(header == NULL)
        puts("the list is empty");
    struct List_process *temp= ht;
    while(temp != NULL)
    {
        p[i].id = temp->Id;
        p[i].arrival_time = temp->Arrival_time ;
        p[i].burst = temp->Burst_time;
        p[i].priority = temp->Priority;
        p[i].waiting_time = 0;
        p[i].return_time =0;
        p[i].response_time =0;
        p[i].completed = FALSE;
        i++;
        temp= temp->next;
    
    }
    puts("");
    /* test if the transfer successed
    for(int i=0 ; h!=NULL ; i++)
    {
        cout<<p[i].id<<" "<<" "<<p[i].burst<<" "<<"" <<p[i].arrival_time<<" "<<p[i].priority<<endl;
        h=h->next;
    }*/

}

void output(ofstream& outFile , struct List_process *header)
{
    struct List_process *temp = header;
    char buffer_1[50];
    char buffer_2[50];

    while(temp!= NULL)
    {
        strcpy(buffer, "");
        strcat(buffer, "Scheduling Method: ");
        snprintf(buffer_1, 20 , "waiting time  %d \n", temp ->Waiting_time);
        strcat(buffer, buffer_1);
        temp = temp -> next;

    }
    while(header != NULL)
    {
        snprintf(buffer_2, 39, "Average Waiting Time: %.3f ms\n\n", header->Avg_tat);
        strcat(buffer, buffer_2);
        strcat(Final_buffer, buffer);
        header= header->next;
    }
    if(outFile.is_open())
    {
        outFile << Final_buffer;
        outFile.close();
    }
    
}
//------------------------------------------- Main Function -------------------------------------------------------------------------//
int main(int argc , char *argv[])
{
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
    openFileI(inFile, File_in);
    openFileO(outFile,File_out);
    header = processFile(inFile,header);
    //check if the linked list was successsfully create
    //Display(header);
    //call the function of prompting first the Menu on the screen
    Menu(choice_str);
   // head = cloneList(header);
    //check if we sucessfully create copy a linked with the data inside
   // Display(head);
    inFile.close();

    return 0;

}
//-------------------------------------------- First Menu -----------------------------------------------------------------------------//
int Menu(string choice_str)
{
    int choice;
    if(choice_str =="OFF")
    {
        do
        {
            system("clear");
            cout<< "-------------------- CPU SCHEDULING SIMULATOR-------------------------------"<< endl;
            cout<<" 1. SCHEDULING METHOD(NONE)\n 2. PREEMPTIVE MODE(OFF)\n 3. SHOW RESULT\n 4.END PROGRAM\n";
            cout<<" Option > ";
            cin >> choice;
            switch (choice)
            {
                case 1:
                //  inside_File_in(File_in, File_out);
                    Scheduling_Method(choice_str);
                    
                    break;
                case 2:
                    Preemptive_Mode(choice_str , "YES");
                    break;
                case 3:
                    cout <<"<< Please select Scheduling Method first >>"<<endl;
                    sleep(2);
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
    }
    else
    {
        do
        {
            system("clear");
            cout<< "-------------------- CPU SCHEDULING SIMULATOR-------------------------------"<< endl;
            cout<<" 1. SCHEDULING METHOD(NONE)\n 2. PREEMPTIVE MODE(ON)\n 3. SHOW RESULT\n 4.END PROGRAM\n";
            cout<<" Option > ";
            cin >> choice;
            switch (choice)
                {
                    case 1:
                    //  inside_File_in(File_in, File_out);
                        Scheduling_Method(choice_str);
                        
                        break;
                    case 2:
                        Preemptive_Mode(choice_str,"YES");
                        break;
                    case 3:
                        cout <<"<< Please select Scheduling Method first >>"<<endl;
                        sleep(2);
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
    }

}

struct List_process *createNode( int Burst, int Arrival , int Priority)
{
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
struct List_process *cloneList(struct List_process *head)
{
    struct List_process *current = head;
    struct List_process *newList = NULL;
    struct List_process *node = NULL;

    while(current != NULL)
    {
        if(newList == NULL)
        {
            newList = (struct List_process *)malloc(sizeof(struct List_process));
            newList->Id = current->Id;
            newList->Burst_time = current->Burst_time;
            newList->Arrival_time = current->Arrival_time;
            newList->Priority = current->Priority;

            newList->next = NULL;
            node = newList;
        }
        else{
            node->next = (struct List_process *)malloc(sizeof(struct List_process));
            node = node->next;
            node->Id = current->Id;
            node->Burst_time = current->Burst_time;
            node->Arrival_time = current->Arrival_time;
            node->Priority = current->Priority;
            node->next = NULL;
        }
        current = current->next;
    }
    return newList;

}
//---------------------------------------------- Second Menu ------------------------------------------------------------------//
void Scheduling_Method(string choice_str)
{
    int choice;
    if(choice_str == "OFF")
    {
    do
    {
        system("clear");
        cout<< "-------------------- Scheduling Method-------------------------------"<< endl;
        cout<<" 1. None: None of scheduling method chosen\n 2. First Come, First Served Scheduling\n "
            << "3. Shortest-Job-First Scheduling\n 4. Priority Scheduling\n 5. Round-Robin Scheduling\n";
        cout<<" Method > ";
        cin >> choice;
        switch (choice)
        {
            case 1:
                system("clear");
                char user;
                cout<<" ----------NO SCHEDULING METHOD WAS CHOOSE-----------"<<endl
                    <<" ******Do you want to go Back to the main Menu(Y/N)*******"<<endl;
                cout<<"Answers > ";
                cin >> user;
                if(user == 'Y' || user =='y')
                    Menu(choice_str);
                else if (user =='N' || user == 'n')
                    Scheduling_Method(choice_str);
                else
                    cout <<"You enter a invalid character use Y or N !!!"<<endl;
                break;
            case 2:
                First_come();
                break;
            case 3:
                Shortest_jobNP();
                break;
            case 4:
                Priority_schNP();
                break;
            case 5:
                Round_robin();
                break;
            default:
                cout <<"please enter the correct choice !!" << endl;
                break;
        }

    }
    while(choice>0 && choice <=5);
    }
    else
    {
        system("clear");
        do
     {
        cout<< "-------------------- Scheduling Method-------------------------------"<< endl;
        cout<<" 1. None: None of scheduling method chosen\n "
            << "2. Shortest-Job-First Scheduling\n 3. Priority Scheduling\n 4. Round-Robin Scheduling\n";
        cout<<" Method > ";
        cin >> choice;
        switch (choice)
        {
            case 1:
                system("clear");
                char user;
                cout<<" ----------NO SCHEDULING METHOD WAS CHOOSE-----------"<<endl
                    <<" ******Do you want to go Back to the main Menu(Y/N)*******"<<endl;
                cout<<"Answers > ";
                cin >> user;
                if(user == 'Y' || user =='y')
                    Menu(choice_str);
                else if (user =='N' || user == 'n')
                    Scheduling_Method(choice_str);
                else
                    cout <<"You enter a invalid character use Y or N !!!"<<endl;
                
                break;
            case 2:
                Shortest_jobP(p);
                break;
            case 3:
                Priority_schP();
                break;
            case 4:
                Round_robin();
                break;
            default:
                cout <<"please enter the correct choice !!" << endl;
                sleep(2);
                Scheduling_Method(choice_str);
                break;
        }

    }
    while(choice>0 && choice <=4);

    }
}

void Preemptive_Mode(string choice_str, string Main)
{
    system("clear");
    int choice;
    cout<<"-------------------------------------------------------------------------------------------------"<<endl;
    cout <<"<< WARNING !!! THE PREEMPITVE MODE WILL AFFECT WILL AFFECT THE SCHEDULING METHOD MENU !!!>>"<<endl;
    cout <<" Enable the Preemptive mode \n \t 1.OFF\n \t 2.ON" <<endl;
    cout<<"Answers > ";
    cin >> choice;
    if(Main == "YES")
    {
        switch(choice)
        {
            case 1:
                choice_str = "OFF";
                Menu(choice_str);
                break;
            case 2:
                choice_str ="ON";
                Menu(choice_str);
                break;
            default:
                cout<<"INCORRECT,Your choice should be a integer number between 1-2 !!!" <<endl;
                sleep(5);
                Preemptive_Mode(choice_str,"YES");
                break;
        }
    }   
    
    else
    {
        if(choice_str == "FCFS")
        {
            cout << "<< No preemptive mode for this Scheduling method >>" <<endl;
        }
        else if(choice_str == "SJF")
        {
             switch(choice)
            {
                case 1:
                    Shortest_jobNP();
                    break;
                case 2:
                    Shortest_jobP(p);
                    break;
                default:
                    cout<<"INCORRECT,Your choice should be a integer number between 1-2 !!!" <<endl;
                    sleep(5);
                    Preemptive_Mode(choice_str,"NO");
                    break;

            }

        }
        else if(choice_str == "RR")
        {
            cout<<"<< Round Robin is Preemptive >>" <<endl;
        }
        else if(choice_str == "PS")
        {
            switch(choice)
            {
                case 1:
                    Priority_schNP();
                    break;
                case 2:
                    Priority_schP();
                    break;
                default:
                    cout<<"INCORRECT,Your choice should be a integer number between 1-2 !!!" <<endl;
                    sleep(5);
                    Preemptive_Mode(choice_str,"NO");
                    break;

            }

        }
            
    }

}

void show_resultP(struct List_process *h , string S)
{
    int choice;
    system("clear");
    if(S == "SJF" || S == "RR" || S == "PS" )
    {
        do
        {
            cout<< "-------------------- CPU Scheduler Simlulator -------------------------------"<< endl;
            cout<<" 1. Scheduling("<<S<<")\n 2. Preemptive Mode\n "
                << "3. Show Result\n 4. END PROGRAM\n ";
            cout<<" OPTION > ";
            cin >> choice;
            switch (choice)
            {
                case 1:
                    Scheduling_Method("ON");
                    break;
                case 2:
                    Preemptive_Mode(S,"NO");
                    break;
                case 3:
                    puts(" ");
                    cout <<"\t   "<< h->mode << endl;
                    puts(" ");
                    Display(h);
                    cout<<"The Avarage Waiting is :" << h->Avg_wt<<endl;
                    break;
                case 4:
                    exit(0);
                    break;
            
                default:
                cout<<"INCORRECT,Your choice should be a integer number between 1-2 !!!" <<endl;
                sleep(5);
                break;
            }
        }
        while(choice>0 && choice <=4);
    }
    
}
int Show_Result(struct List_process *h , string S, double avg)
{
    int choice;
    system("clear");
    if(S == "FCFS" || S == "SJF" || S =="RR" || S == "PS" )
    {
        do
        {
            cout<< "-------------------- CPU Scheduler Simlulator -------------------------------"<< endl;
            cout<<" 1. Scheduling("<<S<<")\n 2. Preemptive Mode\n "
                << "3. Show Result\n 4. END PROGRAM\n ";
            cout<<" OPTION > ";
            cin >> choice;
            switch (choice)
            {
                case 1:
                    Scheduling_Method("OFF");
                    break;
                case 2:
                    Preemptive_Mode(S,"NO");
                    break;
                case 3:
                    puts(" ");
                    cout <<"\t   "<< h->mode << endl;
                    puts(" ");
                    Display(h);
                    cout<<"The Avarage Waiting is :" << avg <<endl;
                    //output(outFile , h);
                    break;
                case 4:
                    exit(0);
                    break;
            
                default:
                cout<<"INCORRECT,Your choice should be a integer number between 1-2 !!!" <<endl;
                sleep(5);
                break;
            }
        }
        while(choice>0 && choice <=4);
    }

}
void Display(struct List_process *header)
{
    if(header == NULL)
        puts("the list is empty");
    struct List_process *temp= header;
    cout<<"             "<<"Burst_time\t\t"<<"Arrival_time\t\t"<<"Priority\t\t"<<"Waiting_Time"<<endl;
    cout<<"-------------------------------------------------------------------------------------------------"<<endl;
    while(temp != NULL)
    {
        puts(" ");
        cout <<"Process " << temp->Id << "\t " << temp->Burst_time <<"\t\t      "<<temp->Arrival_time
             <<"\t\t\t   " << temp->Priority<<"\t\t\t\t"<<temp->Waiting_time<<endl;
        
        cout<<"__________|________________|____________________|_______________________|___________________________|"<<endl;    
        temp= temp->next;
    
    }
}
//------------------------------------------------- Scheduling Algorithm section -------------------------------------------------- //
void First_come()
{
    struct List_process *headtemp = cloneList(header);
    headtemp->mode = "<<You are in First_come Scheduling >>";
    mysort_Ar(headtemp,headtemp,headtemp->next);
    //calculate the Waiting 
    waiting_time(headtemp);
    //Turn_around_time
    Turn_around_time(headtemp);
    //Average_time
    Average_time(headtemp,"FCFS");
}
void Shortest_jobNP()
{
    struct List_process *head = cloneList(header);
    head->mode = "<<You are in Shortest_Job_Scheduling in Nom-preemptive >>";
    //call the sort funtion 
    mysort_Br(head,head,head->next);

    //calculate the Waiting 
    waiting_time(head);
    //Turn_around_time
    Turn_around_time(head);
    //Average_time
    Average_time(head,"SJF");
}
void Priority_schNP()
{   
    struct List_process *head = cloneList(header);
    head->mode = "<<You are in Priority_Scheduling in Nom-preemptive >>";
    //sort the linked list according to the priority time 
    mysort_Pr(head,head,head->next);
    //calculate the Waiting 
    waiting_time(head);
    //Turn_around_time
    Turn_around_time(head);
    //Average_time
    Average_time(head,"PS");

}
void Round_robin()
{
    int quantum;
    struct List_process *head = cloneList(header);
    struct List_process *temp= NULL;
    head->mode = "<<You are in Rond Robin in Preemptive Mode >>" ;
    cout<<"Please enter Quantum >";
    cin >> quantum;
   
    mysort_Ar(head,head,head->next);
    int total_waiting_time =0;
    int total_turnaround_time =0;
    int total_response_time =0;

    int len = process_counter(head);

    process_init(head);

    Time_calR(head,quantum);


    for(int i=0 ; i<len ; i++)
    {
        total_waiting_time +=p[i].waiting_time;

        total_turnaround_time += p[i].turnaround_time;

        total_response_time += p[i].response_time; 

    }
    // store the average waiting time and the turnaround time in the linked list
    head->Avg_wt = (double)total_waiting_time/(double)len;
    head->Avg_tat = (double)total_turnaround_time/(double)len;
    //temp = cloneList(head);
    show_resultP(head,"RR");
    
}
void Shortest_jobP(struct _process *p)
{
    struct List_process *head = cloneList(header);
    struct List_process *temp = NULL;
    head->mode = "<<You are in Shortest_Job_Scheduling in Preemptive Mode >>" ;
    // Initialize the current time to the earliest arrival time of any process
    mysort_Ar(head,head,head->next);
    int total_waiting_time =0;
    int total_turnaround_time =0;
    int total_response_time =0;

    int len = process_counter(head);

    process_init(head);

    Time_cal(head);

    for(int i=0 ; i<len ; i++)
    {
        p[i].turnaround_time = p[i].return_time -p[i].arrival_time;

        total_waiting_time += p[i].waiting_time;

        total_turnaround_time += p[i].turnaround_time;

        total_response_time += p[i].response_time; 
    }
    
    // store the average waiting time and the turnaround time in the linked list
    head->Avg_wt = (double)total_waiting_time/(double)len;
    head->Turn_around_time = (double)total_turnaround_time/(double)len;
   // temp = cloneList(head);
    show_resultP(head,"SJF");

}
void Priority_schP()
{
    struct List_process *head = cloneList(header);
    struct List_process *temp = NULL;
    head->mode = "<<You are in Priority_Scheduling (Preemptive Mode) >>" ;
    // Initialize the current time to the earliest arrival time of any process
    mysort_Pr(head,head,head->next);
    int total_waiting_time =0;
    int total_turnaround_time =0;
    int total_response_time =0;

    int len = process_counter(head);

    process_init(head);

    Time_calP(head);

    for(int i=0 ; i<len ; i++)
    {
        p[i].turnaround_time = p[i].return_time -p[i].arrival_time;

        total_waiting_time += p[i].waiting_time;

        total_turnaround_time += p[i].turnaround_time;

        total_response_time += p[i].response_time; 
    }
    
    // store the average waiting time and the turnaround time in the linked list
    head->Avg_wt = (double)total_waiting_time/(double)len;
    head->Turn_around_time = (double)total_turnaround_time/(double)len;
  //  temp = cloneList(head);
    show_resultP(head,"PS");
}
//----------------------------------------Function to calculate the waiting time -------------------------------------//
int waiting_time(struct List_process *header)
{
    //------------------------------------------ Waiting time calculation ---------------------------------------------------------//
    struct List_process *head = header;
    struct List_process *temp = NULL;
    temp = head;
    head = head->next;
    head->Waiting_time =0;

    while(head!=NULL)
    {
        head->Waiting_time = temp->Burst_time + temp->Waiting_time;
        head= head->next;
        temp=temp->next;
    }
    return 0;

}
//----------------------------------------Function to calculate the Turnaround time -----------------------------------//
int Turn_around_time(struct List_process *header)
{
    //------------------------------------------- Turn Around Time ------------------------------------------//
    struct List_process *temp3= header;
    int current=0;
    while(temp3!=NULL)
    {
        current+= temp3 -> Burst_time ;
        temp3->Turn_around_time = current- temp3->Arrival_time;
        temp3 = temp3->next;
    }
    return 0;

}
//--------------------------------------------Function to calculate the Average time--------------------------------------//
void Average_time(struct List_process *header,string Al)
{
     //---------------------------------------  Average waiting Time ---------------------------------------//
    struct List_process *Final=header;
    struct List_process *f = header ;
    struct List_process *t = NULL;
    int counter = 0;
    double Total_waiting=0;
    double Total_burst=0;
    double avg;
    for(int i=0 ; f!=NULL; i++)
    {
        Total_burst += f->Burst_time;
        Total_waiting +=(f->Turn_around_time - f->Burst_time);
        counter ++;
        f= f->next;
    }
    avg = Total_waiting/counter;
    Final ->Avg_wt = avg;
    t= Final;
    system("clear");
    Show_Result(t,Al,avg);
   // output(outFile , Final);


}
//-------------------------------------- Function to calculate the waiting and turnaround time for SJF preemptive -----------------------------//
void Time_cal(struct List_process *header)
{
    int current_time =0;  // variable to store the current time 
    int total_burst_time =0;    // variable to store the total execution time
    int shortest_remain_time ;   // variable to store the index with minimal work
    int len = process_counter(header); // store the number of element in  the linked list
    int k =0;       // variable store the currently running process number
    // dynamically allocate an array to store the remaining execution time for each process
    int *remain_burst_time = (int *)malloc(sizeof(int)*len);
    // dynamically allocate an array to be used to check the response time
    int *count = (int *)malloc(sizeof(int)*len);
    // repeat untill we reach the last process
    for(int i = 0; i <len ; i++)
    {
        // initialization of a count array and the remain_burst_time array
        count[i]=0;
        remain_burst_time[i] = p[i].burst;
        total_burst_time+= p[i].burst; // calculate  the total burst time by sum up all burst of each process
    }

    while(current_time < total_burst_time)
    {
        shortest_remain_time = INT_MAX;
        // if the current_time is less than the arrival time of the last process entered
        if(current_time <= p[len-1].arrival_time)
        {
            for(int i = 0 ; i<len ; i++)
            {
                if((p[i].completed == FALSE) && (p[i].arrival_time <= current_time)&&(shortest_remain_time > remain_burst_time[i]))
                {
                    //update the minimum work time
                    shortest_remain_time = remain_burst_time[i];
                    k=i; // update minimum working process index
                }
            }
        }
        else        // when there is no more a new process coming in
        {
            for(int i=0 ; i<len ; i++)
            {
                if((p[i].completed == FALSE)&&(shortest_remain_time > remain_burst_time[i]))
                {
                    shortest_remain_time = remain_burst_time[i];
                    k=i;
                }
            }
        }
        if(count[k] == 0) // when the selected process starts for the first time
        {
            count[k]++; // indicate that is not the initial run
            p[k].response_time = current_time; // save the response time of the running process
        }
        remain_burst_time[k]--; // decrease the remaining time of the executed process
        current_time ++;           // increment the current time
// When the remaing execution time of the process become 0
        if(remain_burst_time[k] ==0)
        {
            p[k].completed = TRUE;   // change the done state
            // calculate the waiting_time
            p[k].waiting_time = current_time - p[k].burst - p[k].arrival_time;
            // calculte the return time
            p[k].return_time = current_time;
        }
    }

    //back up all the values of the waiting time in the linked list 
    for(int i=0 ; i<len ; i++)
    {
        header->Waiting_time = p[i].waiting_time;
        header= header->next;
    }
   /* check
    cout<<p[0].waiting_time<<endl;
    cout<<p[1].waiting_time<<endl;
    cout<<p[2].waiting_time<<endl;*/
    

    
    //deallocate memory for dynamically allocates arrays
    free(count);
    free(remain_burst_time);

}
//----------------------------------------- Function to calculate the waiting and turnaround time for Priority Preemptive -------------------------//
void Time_calP(struct List_process *header)
{
    int current_time =0;  // variable to store the current time 
    int total_burst_time =0;    // variable to store the total execution time
    int Priority ;   // variable to store the index with minimal priority
    int len = process_counter(header); // store the number of element in  the linked list
    int k =0;       // variable store the currently running process number
    // dynamically allocate an array to store the remaining execution time for each process
    int *remain_burst_time = (int *)malloc(sizeof(int)*len);
    // dynamically allocate an array to be used to check the response time
    int *count = (int *)malloc(sizeof(int)*len);
    // repeat untill we reach the last process
    for(int i = 0; i <len ; i++)
    {
        // initialization of a count array and the remain_burst_time array
        count[i]=0;
        remain_burst_time[i] = p[i].burst;
        total_burst_time+= p[i].burst; // calculate  the total burst time by sum up all burst of each process
    }

    while(current_time < total_burst_time)
    {
        Priority = INT_MAX;
        // if the current_time is less than the arrival time of the last process entered
        if(current_time <= p[len-1].arrival_time)
        {
            for(int i = 0 ; i<len ; i++)
            {
                if((p[i].completed == FALSE) && (p[i].arrival_time <= current_time)&&(Priority > remain_burst_time[i]))
                {
                    //update the minimum work time
                    Priority = p[i].priority;
                    k=i; // update minimum working process index
                }
            }
        }
        else        // when there is no more a new process coming in
        {
            for(int i=0 ; i<len ; i++)
            {
                if((p[i].completed == FALSE)&&(Priority > p[i].priority))
                {
                    Priority = p[i].priority;
                    k=i;
                }
            }
        }
        if(count[k] == 0) // when the selected process starts for the first time
        {
            count[k]++; // indicate that is not the initial run
            p[k].response_time = current_time; // save the response time of the running process
        }
        remain_burst_time[k]--; // decrease the remaining time of the executed process
        current_time ++;           // increment the current time
// When the remaing execution time of the process become 0
        if(remain_burst_time[k] ==0)
        {
            p[k].completed = TRUE;   // change the done state
            // calculate the waiting_time
            p[k].waiting_time = current_time - p[k].burst - p[k].arrival_time;
            // calculte the return time
            p[k].return_time = current_time;
        }
    }

    //back up all the values of the waiting time in the linked list 
    for(int i=0 ; i<len ; i++)
    {
        header->Waiting_time = p[i].waiting_time;
        header= header->next;
    }
   /* check
    cout<<p[0].waiting_time<<endl;
    cout<<p[1].waiting_time<<endl;
    cout<<p[2].waiting_time<<endl;*/
    

    
    //deallocate memory for dynamically allocates arrays
    free(count);
    free(remain_burst_time);

}
//----------------------------------------- Function to calculate the waiting and turnaround time for Round Robin ------------------//
void Time_calR(struct List_process *header, int quantum)
{
    int lsize = process_counter(header);    // store number of element in the list
    int current_time = 0;
    int *remain_burst_time =(int *)malloc(sizeof(int)*lsize);
    int *resp_time = (int *)malloc(sizeof(int)*lsize);

    for(int i = 0; i<lsize; i++)
    {
        remain_burst_time[i] = p[i].burst;
        resp_time[i] = FALSE;
    }

    while(TRUE)
    {
        int check = TRUE;
        for(int i=0 ; i<lsize;i++)
        {
            if(remain_burst_time[i]>0)
            {
                check = FALSE;
                if(resp_time[i] == FALSE)
                {
                    p[i].response_time = current_time - p[i].arrival_time;
                    resp_time[i] = TRUE;
                }
                if(remain_burst_time[i] > quantum)
                {
                    current_time += quantum;
                    remain_burst_time[i]-=quantum;
                }
                else
                {
                    current_time += remain_burst_time[i];
                    p[i].waiting_time = current_time -p[i].burst;
                    remain_burst_time[i] = 0;

                }
            }
        }
        if(check == TRUE)
            break;

    }
     for(int i =0 ; i<lsize ; i++)
    {
        p[i].turnaround_time = p[i].burst + p[i].waiting_time -p[i].arrival_time;
    }

    //back up all the values of the waiting time in the linked list 
    for(int i=0 ; i<lsize ; i++)
    {
        header->Waiting_time = p[i].waiting_time;
        header->Turn_around_time = p[i].turnaround_time;
        header= header->next;
    }
  
    /*cout<<p[0].turnaround_time<<endl;
    cout<<p[1].turnaround_time<<endl;
    cout<<p[2].turnaround_time<<endl;*/
    free(remain_burst_time);
}
//------------------------------------Function to sort accorging to a burst time------------------------------------//
void mysort_Br(struct List_process *&head,struct List_process *ht,struct List_process *temp)
{
    if(temp==NULL)
    {
        return;
    }
   struct List_process *next_node=ht->next;
   while(next_node!=NULL)
    {
        if(ht->Burst_time > next_node->Burst_time)
        {
            swap(ht->Id,next_node->Id);
            swap(ht->Burst_time,next_node->Burst_time);
            swap(ht->Arrival_time,next_node->Arrival_time);
            swap(ht->Priority,next_node->Priority);
            swap(ht->Turn_around_time,next_node->Turn_around_time);
            swap(ht->Waiting_time, next_node->Waiting_time);

            
        }
        
       
        next_node=next_node->next;
    }
    mysort_Br(head,temp,ht->next);
}
//-----------------------------------Function to sort the Priority scheduling -------------------------------------------
void mysort_Pr(struct List_process *&head,struct List_process *ht,struct List_process *temp)
{
    if(temp==NULL)
    {
        return;
    }
   struct List_process *next_node=ht->next;
   while(next_node!=NULL)
    {
        if(ht->Priority > next_node->Priority)
        {
            swap(ht->Id,next_node->Id);
            swap(ht->Burst_time,next_node->Burst_time);
            swap(ht->Arrival_time,next_node->Arrival_time);
            swap(ht->Priority,next_node->Priority);
            swap(ht->Turn_around_time,next_node->Turn_around_time);
            swap(ht->Waiting_time, next_node->Waiting_time);

            
        }
       
        next_node=next_node->next;
    }
    mysort_Pr(head,temp,ht->next);
}
//-----------------------------------Funtion to sort according to the Arrival Time -----------------------------------------//
void mysort_Ar(struct List_process *&head,struct List_process *ht,struct List_process *temp)
{
    if(temp==NULL)
    {
        return;
    }
   struct List_process *next_node=ht->next;
   while(next_node!=NULL)
    {
        if(ht->Arrival_time > next_node->Arrival_time )
        {
            swap(ht->Id,next_node->Id);
            swap(ht->Burst_time,next_node->Burst_time);
            swap(ht->Arrival_time,next_node->Arrival_time);
            swap(ht->Priority,next_node->Priority);
            swap(ht->Turn_around_time,next_node->Turn_around_time);
            swap(ht->Waiting_time, next_node->Waiting_time);

            
        }
        else if(ht->Arrival_time == next_node->Arrival_time)
        {
            if(ht->Id > next_node->Id)
            {
                swap(ht->Id,next_node->Id);
                swap(ht->Burst_time,next_node->Burst_time);
                swap(ht->Arrival_time,next_node->Arrival_time);
                swap(ht->Priority,next_node->Priority);
                swap(ht->Turn_around_time,next_node->Turn_around_time);
                swap(ht->Waiting_time, next_node->Waiting_time);

            }
        }
        
        
        next_node=next_node->next;
    }
    mysort_Ar(head,temp,ht->next);

}
//-----------------------------------Count the number of element in the Linked List -----------------------------------------//
int process_counter(struct List_process *header)
{
	struct List_process *temp = header;
	int Arr = 0;
	while (temp != NULL)
	{
		Arr++;
		temp = temp->next;
	}
	return Arr;
}

