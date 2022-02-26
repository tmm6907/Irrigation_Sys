/*irrigation_sys.cpp

Author: Terrence Moore
Purpose: Recreate Python attempt at a mock irrigation system for a given 2-D array to
practice C++ skills. This attempt will also include use of multithreading for increased
performance.

*/


#include <stdlib.h> //srand, rand
#include <time.h> //time
#include <thread> //thread
#include <math.h> //round
#include <iostream> //cout
using namespace std;



//decay characteristics
const int MAX_DECAY = 30;
const int MAX_TIME = 3;
const int MIN_TIME = 1;

//paramaters of farm size
const int MAX_WIDTH = 5;
const int MAX_HEIGHT = 5;
const int MAX_SIZE = MAX_HEIGHT * MAX_WIDTH;

//class to handle dehydration, rehydration and analysis of each spot in a farm
class Sector{
    protected:
        //hydration types
        char hydrationchoices[2] = {'R','Y'};
    public:
        char state = 'G';

        //irrigation and irrigation time handler
        int irrigate_sector(){
            // sleep_for, sleep_until
            using namespace std::this_thread;
            using namespace std::chrono; 
            char s = 'G';

            switch (getState())
            {
            case 'Y':
                sleep_for(seconds(MIN_TIME));
                change_state(s);
                break;
            case 'R':
                sleep_for(seconds(MAX_TIME));
                change_state(s);
                break;
            
            default:
                break;
            }
            return 0;
        }

        //creates a dehydrated sector with random level of dehydration
        int dehydrate_sector(){
            srand (rand()); //initialize serail randomizer
            change_state(hydrationchoices[rand()% 2 + 0]);
            return 0;
        }

        //get hydration state of plot sector
        char getState(){return state;}

    private:
        //private sector state changer
        int change_state(char s){
            state = s;
            return 0;
        }
};

//display plot
int print_plot(Sector plot[][MAX_HEIGHT]){
    for(int i = 0; i < MAX_HEIGHT; i++){
        for(int j = 0; j<MAX_WIDTH; j++){cout << plot[i][j].getState()<< " ";}
        cout << "\n";
    }
    cout << "\n";
    return 0;
}

//handler for randomly selecting sector to dehydrate
void crop_dehydration(Sector plot[][MAX_HEIGHT]){
    srand (time(NULL)); //initialize serail randomizer
    for(int i = 0;i < MAX_DECAY; i++){
        int x = rand()% (MAX_WIDTH-1) + 0;
        int y = rand()% (MAX_HEIGHT-1) + 0;
        
        if(plot[x][y].getState() == 'G'){plot[x][y].dehydrate_sector();}
    }
}

//calls Sector class irrigation function on each sector in current row
int irrigate(Sector plot[][MAX_HEIGHT], int i, int j){
    for(int x=0; x < j; x++){
        if(plot[i][x].getState() != 'G'){plot[i][x].irrigate_sector();}
        print_plot(plot);
    }
    return 0;
}

//handler for irrigation of plot which is completed in 3 threads
int irrigate_plot(Sector plot[][MAX_HEIGHT]){
    int m4 = round(MAX_HEIGHT/4);
    int m2 = round(MAX_HEIGHT/2);

    //splits irrigation job into three separate threads
    for(int i = 0; i < m4; i++){
        thread t1(irrigate, plot, i, MAX_WIDTH);
        t1.join();
    }

    for(int i = m4; i < m2; i++){
        thread t2(irrigate, plot, i , MAX_WIDTH);
        t2.join();    
    }

    for(int i = m2; i < MAX_HEIGHT; i++){
        thread t3(irrigate, plot, i , MAX_WIDTH);
        t3.join();
    }
    return 0;
}

//main function
int main(){
    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono; // seconds
    Sector plot[MAX_WIDTH][MAX_HEIGHT];
    while(1){
        print_plot(plot);
        sleep_for(seconds(3));
        crop_dehydration(plot);

        print_plot(plot);
        irrigate_plot(plot);
    }
    return 0;
}
