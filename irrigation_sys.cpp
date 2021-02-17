#include <stdlib.h> //srand, rand
#include <time.h> //time
#include <thread> //thread
#include <math.h> //round
#include <iostream> //cout
using namespace std;

//decay characteristics
const int MAX_DECAY = 5;
const int MAX_TIME = 15;
const int MIN_TIME = 5;

//paramaters of farm size
const int MAX_WIDTH = 10;
const int MAX_HEIGHT = 10;
const int MAX_SIZE = MAX_HEIGHT * MAX_WIDTH;

//red and yellow sector initializations
class Sector{
    protected:
        char state = 'G';
        char hydrationchoices[2] = {'R','Y'};
    public:
        //irrigation of dehydrated sectors
        int irrigate_sector(){
            using namespace std::this_thread;     // sleep_for, sleep_until
            using namespace std::chrono; // seconds
            char s = 'G';

            cout << "Entering Irrigation function";

            if(getState() == 'Y'){
                sleep_for(seconds(MIN_TIME));
                change_state(s);
                return 0;
            }
            else if(getState() == 'R'){
                sleep_for(seconds(MAX_TIME));
                change_state(s);
                return 0;
            }
            else if(getState() == 'G'){return 0;}
            return 1;
        }

        //creates a dehydrated sector with random level of decay
        int dehydrate_sector(){
            srand (time(NULL)); //initialize serail randomizer
            change_state(hydrationchoices[rand()% 1 + 0]);
            return 0;
        }

        //get hydration state of plot sector
        char getState(){return state;}

    private:
        //private sector state changer
        int change_state(char s){
            if(state != s){state = s;}
            return 0;
        }
};

//display plot
void print_plot(Sector plot[][MAX_HEIGHT]){
    for(int i = 0; i++; i < MAX_HEIGHT){
        for(int j = 0; j++; j<MAX_WIDTH){cout << plot[i][j].getState()<< " ";}
        cout << "\n";
    }
}

//handler for randomly selecting sector to dehydrate
void crop_dehydration(Sector plot[][MAX_HEIGHT]){
    srand (time(NULL)); //initialize serail randomizer
    for(int i = 0; i++; i < MAX_DECAY){
        int x = rand()% (MAX_WIDTH-1) + 0;
        int y = rand()% (MAX_HEIGHT-1) + 0;
        
        if(plot[x][y].getState() == 'G'){plot[x][y].dehydrate_sector();}
    }
}

//handler for irrigation of plot which is completed in 3 threads
int irrigate_plot(Sector plot[][MAX_HEIGHT]){
    for(int i = 0; i++; i < round(MAX_HEIGHT/4)){
        for(int j = 0; j++; j < MAX_WIDTH){
            if(plot[i][j].getState() != 'G'){
                thread t1(plot[i][j].irrigate_sector());
                t1.join();
            }
        }
    }

    for(int i = round(MAX_HEIGHT/4); i++; i < round(MAX_HEIGHT/2)){
        for(int j = 0; j++; j < MAX_WIDTH){
            if(plot[i][j].getState() != 'G'){
                thread t2(plot[i][j].irrigate_sector());
                t2.join();
            }
        }
    }

    for(int i = round(MAX_HEIGHT/2); i++; i < MAX_HEIGHT){
        for(int j = 0; j++; j < MAX_WIDTH){
            if(plot[i][j].getState() != 'G'){
                thread t3(plot[i][j].irrigate_sector());
                t3.join();
            }
        }
    }
    return 0;
}

//main function
int main(){
    Sector plot[MAX_WIDTH][MAX_HEIGHT];
    while(1){
        print_plot(plot);
        crop_dehydration(plot);

        print_plot(plot);
        irrigate_plot(plot);
    }
    return 0;
}
