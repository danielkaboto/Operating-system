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
void Second_Menu();
void Scheduling_Method(string);
void Preemptive_Mode(string);
void Show_Result();
int process_counter(struct List_process *);

//------------------------------------- link list prototypes ---------------------------------------//
struct List_process *createNode(int , int , int );
struct List_process *insertBack(struct List_process *, int, int,int);
void Display(struct  List_process *);
struct List_process *Builder(ifstream& , struct List_process *);   // read the file and take the data fills up the linked list
struct List_process *cloneList(struct List_process *);       
struct List_process *Merge_sort(struct List_process *);
struct List_process *merge(struct List_process *, struct List_process *);
struct List_process *mid_point(struct List_process *);
void mysort_Br(struct List_process *&,struct List_process *,struct List_process *); //sorting according to burst time
void mysort_Pr(struct List_process *&,struct List_process *,struct List_process *);  // sorting according to Priority
void mysort_Ar(struct List_process *&,struct List_process *,struct List_process *);  // sorting according to the Arrival time

           // clone 
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
struct List_process *processFile(ifstream& inFile, struct List_process *hdr)
{
   
    string line;
    char word[size];
    int Burst, Arrival , Priority;
    istringstream iss;   // process the string 
    if(inFile.good())   // check if the process of getline was sucessfull
    while(!inFile.eof())
    {
        ///read line by line
        getline(inFile,line);
        //cout <<line <<endl;

        //read word by word form the line
         iss.clear(); // clear out the state
         iss.str(line);
         while(iss.good())
         {
            iss >> word ;
           // cout << word << endl;
           // read char by char from the word
        
           for( int i =0 ; i< strlen(word);i++)
           { 
                   // cout << word[i] << endl;
                    Burst = atoi(strtok(word, ":"));        //stores digits bfr the ':'
                    Arrival= atoi(strtok(NULL, ":"));       //stores digits bfr the ':' starting at the point it left off
                    Priority = atoi(strtok(NULL, "\n"));
                    hdr = insertBack(hdr,Burst,Arrival,Priority);
           }
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
    header = Builder(inFile,header);
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
                    Preemptive_Mode(choice_str);
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
    }
    else
    {
        do
        {
            
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
                        Preemptive_Mode(choice_str);
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
    }

}

struct List_process *Builder(ifstream& inFile, struct List_process *hdr)
{
     //2. Process the file
    hdr =processFile(inFile,hdr);
    return hdr;
    

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

void Display(struct List_process *header)
{
    if(header == NULL)
        puts("the list is empty");
    struct List_process *temp= header;
    while(temp != NULL)
    {
        cout <<"Process " << temp->Id << " : " << temp->Burst_time <<":"<<temp->Arrival_time
             <<":" << temp->Priority ;
        cout<<"--"<<temp->Waiting_time;
        cout << " ----" << temp->Turn_around_time<<"     " << temp->is_completed<<endl;
        temp= temp->next;
        
    }
    puts("");
}
void Scheduling_Method(string choice_str)
{
    int choice;
    if(choice_str == "OFF")
    {
    do
    {
        cout<< "-------------------- Scheduling Method-------------------------------"<< endl;
        cout<<" 1. None: None of scheduling method chosen\n 2. First Come, First Served Scheduling\n "
            << "3. Shortest-Job-First Scheduling\n 4.Priority Scheduling\n 5. Round-Robin Scheduling\n";
        cout<<" Method > ";
        cin >> choice;
        switch (choice)
        {
            case 1:
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

void Preemptive_Mode(string choice_str)
{
    int choice;
    cout<<"-------------------------------------------------------------------------------------------------"<<endl;
    cout <<"<< WARNING !!! THE PREEMPITVE MODE WILL AFFECT WILL AFFECT THE SCHEDULING METHOD MENU !!!>>"<<endl;
    cout <<" Enable the Preemptive mode \n \t 1.OFF\n \t 2.ON" <<endl;
    cout<<"Answers > ";
    cin >> choice;
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
            Preemptive_Mode(choice_str);
            break;

    }

}

void Show_Result()
{
    // Read the input file and Write to the output file

  

}


void First_come()
{
    struct List_process *headtemp = cloneList(header);
    struct List_process *temp1 = NULL;
    struct List_process *temp2 = NULL;
    
   // ------------------ we are calculating the waiting time for each process ---------------------// 
    //the first process the waitting is 0
    headtemp->Waiting_time = 0;
    temp1 = headtemp;           // the first node
    temp2= headtemp->next;     // the next node
    // waiting time = waiting[i] + burst[i+1]
    while(temp2!=NULL)
    {
        temp2->Waiting_time= temp1->Burst_time + temp1->Waiting_time;
        temp1= temp1->next;
        temp2=temp2->next;
        
    }
    //check if the waiting time calculation went well
  // Display(headtemp);
//---------------------------------TURN AROUND TIME -----------------------------------------------//
    struct List_process * T1=NULL;
    T1 = headtemp;
    while(T1!=NULL)
    {
        T1->Turn_around_time = T1->Waiting_time+T1->Burst_time;
        T1= T1->next;
    }
    //check if the calculation went well
    //Display(headtemp);
//----------------------------------- AVERAGE WAITING TIME -----------------------------------//
    // Average time = Total waitinti time / number of process
    struct List_process *Final = NULL;
    Final = headtemp;
    int counter =0;    // keep track on the number of the process 
    double Total_waiting=0;
    double Total_Burst =0;
    double avg;

    for(int i=0 ; headtemp != NULL ; i++)
    {
        Total_waiting += headtemp->Waiting_time;
        Total_Burst += headtemp-> Burst_time;
        //headtemp->Id =i++; // the title of the process
        counter++;
        headtemp = headtemp->next;
    }
   // Display(Final);
    avg = Total_waiting/counter;
    cout <<"the average time is : " << avg <<endl;

}
void Shortest_jobNP()
{
    struct List_process *head = cloneList(header);
    struct List_process *temp1 = head;
    struct List_process *temp2=NULL;
    //call the sort funtion 
    mysort_Br(head,head,head->next);

    //------------------------------ waiting time calculation -------------------------------- //
    head->Waiting_time =0;
    temp2 = head->next;

    while(temp2 !=NULL)
    {
        temp2->Waiting_time= temp1->Burst_time + temp1->Waiting_time;
        temp1= temp1->next;
        temp2=temp2->next;

    }
    //------------------------------------------- Turn Around Time ------------------------------------------//
    struct List_process *temp3= head;
    while(temp3!=NULL)
    {
        temp3->Turn_around_time = temp3->Waiting_time + temp3->Burst_time;
        temp3 = temp3->next;
    }
    //---------------------------------------  Average waiting Time ---------------------------------------//
    struct List_process *Final=head;
    int counter = 0;
    double Total_waiting=0;
    double Total_burst=0;
    double avg;
    for(int i=0 ; head!=NULL; i++)
    {
        Total_burst += head->Burst_time;
        Total_waiting +=head->Waiting_time;
        counter ++;
        head= head->next;
    }
    Display(Final);
    avg = Total_waiting/counter;
    cout <<"The average Waiting time is : "<< avg <<endl;
      
    cout<<" you are in the shortest job" << endl;
}
//----------------------------------- function to sort accorging to a burst time------------------------------------//
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
//*-------------function to sort the Priority scheduling -------------------------------------------
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

//------------------------------- Funtion to sort according to the Arrival Time -----------------------------------------//
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

void Priority_schNP()
{
    struct List_process *head = cloneList(header);
    struct List_process *temp = head;
    struct List_process *temp2= head->next;

    //-------------------------------- sort the linked list according to the priority time --------------------------------//
    mysort_Pr(head,temp,temp2);
    //--------------------------------- Waiting time calculation ---------------------------------------------------------//
     head->Waiting_time =0;

    while(temp2 !=NULL)
    {
        temp2->Waiting_time= temp->Burst_time + temp->Waiting_time;
        temp= temp->next;
        temp2=temp2->next;

    }
    //------------------------------------------- Turn Around Time ------------------------------------------//
    struct List_process *temp3= head;
    while(temp3!=NULL)
    {
        temp3->Turn_around_time = temp3->Waiting_time + temp3->Burst_time;
        temp3 = temp3->next;
    }
    //---------------------------------------  Average waiting Time ---------------------------------------//
    struct List_process *Final=head;
    int counter = 0;
    double Total_waiting=0;
    double Total_burst=0;
    double avg;
    for(int i=0 ; head!=NULL; i++)
    {
        Total_burst += head->Burst_time;
        Total_waiting +=head->Waiting_time;
        counter ++;
        head= head->next;
    }
    Display(Final);
    avg = Total_waiting/counter;
    cout <<"The average Waiting time is : "<< avg <<endl;

    cout <<" you are in the Priority job " << endl;
}
//-------------------------------- Count the number of element in the Linked List -----------------------------------------//
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

void Round_robin()
{
    cout <<"You are in the round robin " <<endl;
}

void Shortest_jobP()
{
    struct List_process *head = cloneList(header);
    struct List_process *temp1 = head;
    struct List_process *swapper =NULL:
    //call the sort funtion 
    mysort_Ar(head,head,head->next); // sort according to the arrival time
    int num = process_counter(head); // store the total amount of the arrival time 
    int current_time=0, completed=0;
    int ct_init = 0;
    
    cout <<fixed << setprecision(2);
    while(completed!=num) // until all the process are completed 
    {
        while(temp1!=NULL) 
        {
            if( temp1->Arrival_time == ct_init && temp1->is_completed == false)
            {
                current_time++;
                for(int i ; i < num ; i++)
                {
                    if(current_time == temp1->next->Arrival_time && temp1->next->is_completed ==  false)
                    {
                        if(temp1->Burst_time <= temp1->next->Burst_time)
                        {
                            temp1->bt_remaining= temp1->Burst_time --; // decrement the burst time
                            swapper = temp1->next;
                            //function to push the next proces (swapper) in the queue
                            if(temp1->is_first) // if its the first process
                            {
                                int wt  = 0;
                                int tat = temp1->Burst_time - temp1->Arrival_time;
                            }
                        }
                        else()
                        {
                            swapper = temp1;
                            // function to push the current process(swapper) to the queue

                        }
                    }
                    
                }
            }
            temp1 = temp->next;
        }
        
    }

}

void Priority_schP()
{
    cout <<"you are in the Priority scheduling preemptive"<<endl;

}