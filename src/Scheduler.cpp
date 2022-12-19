#include <iostream>
using namespace std;

void Scheduling_Method();
void Preemptive_Mode();
void Show_Result();



int main(void)
{
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