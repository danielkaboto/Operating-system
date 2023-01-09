//------------------------- Importing the libraries needed for this project---------------------------//
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <climits>
#define  size  1024         //string input data size

using namespace std;

//------------------------Prototypes function of  all  Scheduling method --------------------------------//
void First_come();
void Shortest_jobNP();              // shortest_jobs for the Non-preemptive
void Shortest_jobP();               // shortest_jobs for the Preemptive
void Priority_schNP();              // Priority scheduling for the Non-preemptive
void Priority_schP() ;               // Priority scheduling for the Preemptive
void Round_robin();

//----------------------- prototypes of all the define function used --------------------------------// 
int Menu(string );   // Prompt the menu to the user on the screen
void Second_Menu(string);
void Scheduling_Method(string);
void Preemptive_Mode(string,string);
int Show_Result(struct List_process * , string, double);
int process_counter(struct List_process *);

//------------------------------------- link list prototypes ---------------------------------------//
struct List_process *createNode(int , int , int );
struct List_process *insertBack(struct List_process *, int, int,int);
void Display(struct  List_process *);
struct List_process *cloneList(struct List_process *);       
struct List_process *Merge_sort(struct List_process *);
struct List_process *merge(struct List_process *, struct List_process *);
struct List_process *mid_point(struct List_process *);
//----------------------------................ FCFS Algorithm --------------------------------------------------//
int waiting_time(struct List_process *);
int Turn_around_time(struct List_process *);
void Average_time(struct List_process *, string);
//--------------------------------------------Sorted Algorithm --------------------------------------------------------//
void mysort_Br(struct List_process *&,struct List_process *,struct List_process *); //sorting according to burst time
void mysort_Pr(struct List_process *&,struct List_process *,struct List_process *);  // sorting according to Priority
void mysort_Ar(struct List_process *&,struct List_process *,struct List_process *);  // sorting according to the Arrival time

//----------------------- Definition of the linked list ----------------------------------------//
struct List_process
{
    int Id;     // store the ID of a process
    int Burst_time;
    int Arrival_time;
    int Priority;
    int Waiting_time;
    int Turn_around_time;
    bool is_completed = false;
    int bt_remaining;
    int completion_time;
    bool is_first = false;
    struct List_process *next;

};

// ----------------------- Variable definition and Declaration --------------------------------------//
static int choice;
static string choice_str ="OFF";
int counter;          // getopt variable holder
char *File_in;      //store the input file
char *File_out;    // store the ouput file
struct List_process *header=NULL;     //original linked list root

int num = 0;  // keep track on the Id number of new created process node


//---------------------------- function to open the input input ----------------------------------//
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

    // 1.open the file and check if it's open properly without error
    ifstream inFile;
    openFile(inFile, File_in);
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
            << "3. Shortest-Job-First Scheduling\n 4.Priority Scheduling\n 5. Round-Robin Scheduling\n";
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
                Shortest_jobP();
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
                    Shortest_jobP();
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



int Show_Result(struct List_process *h , string S, double avg)
{
    int choice;
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
                    Display(h);
                    cout<<"The Avarage Waiting is :" << avg <<endl;
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
    cout<<"             "<<"Burst_time\t\t"<<"Arrival_time\t\t"<<"Priority"<<endl;
    cout<<"--------------------------------------------------------------------------"<<endl;
    while(temp != NULL)
    {
        cout <<"Process " << temp->Id << "\t " << temp->Burst_time <<"\t\t      "<<temp->Arrival_time
             <<"\t\t\t   " << temp->Priority<<endl ;
       /* cout<<"--"<<temp->Waiting_time;
        cout << " ----" << temp->Turn_around_time<<"     " << temp->is_completed<<endl;
        */
        temp= temp->next;
    
    }
    puts("");
}

void First_come()
{
    struct List_process *headtemp = cloneList(header);
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
    struct List_process *head = cloneList(header);
    struct List_process *temp = head;
    system("clear");
    Show_Result(head,"RR",0);
    
}
void Shortest_jobP()
{
    cout<<"You are in the Preemptive Mode for the SJF"<<endl;
    struct List_process *head = cloneList(header);
    // Initialize the current time to the earliest arrival time of any process
    mysort_Ar(head,head,head->next);
    int current_time = head->Arrival_time;

    // Initialize the total waiting time and turnaround time to 0
    double total_waiting_time = 0.0;
    double total_turnaround_time = 0.0;

    // Initialize a pointer to the head of the linked list
    struct List_process *p = head;
    // While there are still processes remaining to be completed
    while (p != NULL) {
        // Select the process with the shortest burst time from the list of processes that have arrived
        struct List_process* selected = NULL;
        int shortest_burst_time = INT_MAX;
        struct List_process *q = head;
        while (q != NULL) {
        if (q->Arrival_time <= current_time && q->Burst_time < shortest_burst_time) {
            selected = q;
            shortest_burst_time = q->Burst_time;
        }
        q = q->next;
        }

        // Calculate the waiting time and turnaround time for the selected process
        selected->Waiting_time = current_time - selected->Burst_time ;
        selected->Turn_around_time = current_time - selected->Arrival_time;

        // Update the current time to the completion time of the selected process
        current_time += selected->Burst_time;

        // Add the waiting time and turnaround time of the selected process to the total
        total_waiting_time += selected->Waiting_time;
        total_turnaround_time += selected->Turn_around_time;

        // Remove the selected process from the linked list
        if (selected == head) 
        {
            head = head->next;
        } 
        else 
        {
        struct List_process* prev = head;
        while (prev->next != selected) 
        {
            prev = prev->next;
        }
        prev->next = selected->next;
        }
        delete selected;

        // Update the pointer to the head of the linked list
        p = head;
    }

    // Calculate the average waiting time and average turnaround time
    // Calculate the average waiting time and average turnaround time
    double num_processes = process_counter(header);  // number of processes in the linked list
    double average_waiting_time = total_waiting_time / num_processes;
    double average_turnaround_time = total_turnaround_time / num_processes;

    // Print the results
    std::cout << "Average waiting time: " << average_waiting_time << std::endl;
    std::cout << "Average turnaround time: " << average_turnaround_time << std::endl;


}
void Priority_schP()
{
    cout <<"you are in the Priority scheduling preemptive"<<endl;

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
    while(temp3!=NULL)
    {
        temp3->Turn_around_time = temp3->Waiting_time + temp3->Burst_time;
        temp3 = temp3->next;
    }
    return 0;

}
//--------------------------------------------Funtion to calculate the Average time--------------------------------------//
void Average_time(struct List_process *header,string Al)
{
     //---------------------------------------  Average waiting Time ---------------------------------------//
    struct List_process *Final=header;
    int counter = 0;
    double Total_waiting=0;
    double Total_burst=0;
    double avg;
    for(int i=0 ; header!=NULL; i++)
    {
        Total_burst += header->Burst_time;
        Total_waiting +=(header->Turn_around_time - header->Burst_time);
        counter ++;
        header= header->next;
    }
    avg = Total_waiting/counter;
    system("clear");
    Show_Result(Final,Al,avg);

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
		Arr+=temp->Arrival_time;
		temp = temp->next;
	}

	return counter;
}



