//importing the libraries needed for this project

#include <iostream>
#include <unistd.h>
#include <fstream>
using namespace std;

// prototypes of all the define function 
void Scheduling_Method();
void Preemptive_Mode();
void Show_Result();



int main(int argc , char *argv[])
{
    // reading the argument pass on the command line
    int counter;
    string File_in;      //store the input file
    string File_out;    // store the ouput file

    while((counter =getopt(argc,argv, "i:o:")) != -1)
    {
        switch (counter)
        {
        case 'i':
            File_in = optarg;   //assign the value of the optarg(content the file.txt used as input in the code) to File_in
            break;
        case 'o':
            File_out = optarg;   //assign the value of the optarg(content the file.txt used as output the code) to File_out
            break;
        
        case '?':
            break;
        }
    }
// for debuging process checking if the input file open perfectly wihout problem
    ifstream infile(File_in);
    if(!infile.is_open())
    {
        cout<<"Error: the input file cannot be open !!" <<endl;
        return 1;
    }

// Open the output file
    ofstream outfile(File_out);
    if(!outfile.is_open())
    {
        cout <<"Error : the output file cannot be open !!"<<endl;
        return 1;
    }

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


    // Read the input file and Write to the output file

    string Line;    // keep the string value came form the input values
    while(getline(infile,Line))
    {
      outfile << Line;
    }



    return 0;

}

void Scheduling_Method()
{

}

void Preemptive_Mode()
{

}

void Show_Result()
{

}