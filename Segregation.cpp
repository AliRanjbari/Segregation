/*
    Advanced Programming
    Assignment 03
    By Ali Ranjbari
    started farvardin,20 ,1400
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

const int neighbors_direction[4][2] = {{1,0},{0,-1},{-1,0},{0,1}};
typedef char Type;
typedef int Id;
#define RED 'R'
#define BLUE 'B'
#define EMPTY 'E'
#define DEFAULT_HAPP_THRESH 0.3;
#define TILL_ALL_HAPPY -1;

struct Place{
    Type type = EMPTY;
    Id id;
};

typedef vector<Place> Row_of_places;
typedef vector<Place*> Row_of_places_pointer;
typedef vector<Row_of_places> World;
typedef vector<Place> Creatures_list;

void print_chracter_map(const World& world){
    for(int row = 0 ; row < world.size() ; row++){
        for(int column =  0 ; column < world[row].size() ;column++ ){
            cout << world[row][column].type;
        }
        cout << endl;
    }
}

bool is_place_empty(Place current){
    if(current.type == EMPTY)
        return true;
    return false;
}

bool is_position_in_world(const World& world, int row, int column){
    if(row < 0 || row >= world.size())
        return false;
    if(column < 0 || column >= world[0].size())
        return false;
    return true;
}

bool is_creature_happy (const World& world, int row , int column ,double happiness_threshold){
    double neighbors_count = 4;
    double similar_neighbor = 0;
    Type type = world[row][column].type;
    for(int position = 0 ; position < 4 ; position++){
        int neighbor_row = row+neighbors_direction[position][0];
        int neighbor_column = column+neighbors_direction[position][1];
        if(!is_position_in_world(world,neighbor_row,neighbor_column)){
            neighbors_count--;
        }
        else{
            Place neighbor = world[neighbor_row][neighbor_column];
            if(neighbor.type == EMPTY || neighbor.type == type)
                similar_neighbor++;
        }
    }

    double proportion = similar_neighbor/neighbors_count;
    return (proportion >= happiness_threshold);
}

int number_of_unhappy_creatures(const World& world , double  happiness_threshold){
    int unhappy_creatures_count = 0;
    for(int row = 0 ; row < world.size() ; row++)
        for(int column = 0 ; column < world[row].size() ; column++){
            if(!is_place_empty(world[row][column]))
                if(!is_creature_happy(world , row , column , happiness_threshold)){
                    unhappy_creatures_count++;
                }
        }
    return unhappy_creatures_count;
}

void move_out_unhappy_creatures(World& world,Creatures_list unhappy_creatures){
    if(unhappy_creatures.size() == 0 ){
        return;
    }
    for(int row = 0 ; row<world.size(); row++)
        for(int column = 0 ; column < world[row].size() ; column++){
            if(world[row][column].id == unhappy_creatures[0].id){
                world[row][column].type = EMPTY;
                unhappy_creatures.erase(unhappy_creatures.begin());
                move_out_unhappy_creatures(world , unhappy_creatures);
                return ;
            }
        }
}

Creatures_list unhappy_creatures(World& world , double happiness_threshold){
    Creatures_list unhapp_crt;
    for(int row = 0 ; row < world.size() ; row++)
        for(int column = 0 ; column < world[row].size() ; column++)
            if(!is_place_empty(world[row][column]))
                if(!is_creature_happy(world,row,column,happiness_threshold))
                    unhapp_crt.push_back(world[row][column]);
    move_out_unhappy_creatures(world, unhapp_crt);
    return unhapp_crt;
}

Row_of_places_pointer list_of_available_places(World& world){
    Row_of_places_pointer available_places;
    for(int row = 0 ; row < world.size() ; row++)
        for(int column = 0 ; column < world[row].size() ; column++){
            if(world[row][column].type == EMPTY)
                available_places.push_back(&world[row][column]);
        }
    return available_places;
}

void jump_creature_to_place(Place& dest , Place source){
    dest.type = source.type;
}

void segregate_one_creature(Row_of_places_pointer& available_places , Place unhappy_creature){
    int size_of_available_places = available_places.size();
    int random_place_number;
    do{
        random_place_number = rand()%size_of_available_places;
    }while (available_places[random_place_number]->id == unhappy_creature.id);
    jump_creature_to_place(*available_places[random_place_number] , unhappy_creature);
    available_places.erase(available_places.begin()+random_place_number);
}

bool is_last_place_of_creature_exist(Row_of_places_pointer& available_places , Place unhappy_person){
    for(int place=0 ; place < available_places.size(); place++)
        if(available_places[place]->id == unhappy_person.id)
            return true;
    return false;
}

void segregate_two_creature_with_two_places(Row_of_places_pointer& available_places , Creatures_list& unhappy_persons){
    if(is_last_place_of_creature_exist(available_places ,unhappy_persons[1])){
            segregate_one_creature(available_places , unhappy_persons[1]);
            unhappy_persons.erase(unhappy_persons.begin()+1);
            segregate_one_creature(available_places ,unhappy_persons[0]);
            unhappy_persons.erase(unhappy_persons.begin());
        }
    else if(is_last_place_of_creature_exist(available_places ,unhappy_persons[0])){
            segregate_one_creature(available_places ,unhappy_persons[0]);
            unhappy_persons.erase(unhappy_persons.begin());
            segregate_one_creature(available_places , unhappy_persons[0]);
            unhappy_persons.erase(unhappy_persons.begin());
    }
}

void empty_all_remain_places(Row_of_places_pointer& available_places){
    for(int plc = 0 ; plc < available_places.size() ; plc++){
        available_places[plc]->type = EMPTY;
    }
    for(int plc = 0 ; plc < available_places.size() ; plc++)
        available_places.erase(available_places.begin());
}

void segregate_one_time(Row_of_places_pointer& available_places , Creatures_list& unhappy_creatures){
    if(unhappy_creatures.size() == 2 && available_places.size() == 2){
        segregate_two_creature_with_two_places(available_places , unhappy_creatures);
    }
    if(unhappy_creatures.size() == 0){
        empty_all_remain_places(available_places);
        return;
    }
    segregate_one_creature(available_places , unhappy_creatures[0]); 
    unhappy_creatures.erase(unhappy_creatures.begin());
    segregate_one_time(available_places , unhappy_creatures);
}

bool can_go_on_segregating(Row_of_places_pointer& available_places,Creatures_list& unhappy_creatures_list){
    if(unhappy_creatures_list.size() == 1 && available_places.size() == 1){
        available_places[0]->type = unhappy_creatures_list[0].type;
        return false;
    }
    if(unhappy_creatures_list.size() == 0){
        return false;
    }
    return true;
}

void segregate_several_time(World& world , double happiness_threshold , int repeat_number){

    for(int t = 0 ; t<repeat_number ; t++){
        Creatures_list unhappy_creatures_list = unhappy_creatures(world , happiness_threshold);
        Row_of_places_pointer available_places = list_of_available_places(world);
        if(!can_go_on_segregating(available_places , unhappy_creatures_list)){
            return;
        }
        segregate_one_time(available_places , unhappy_creatures_list);
    }
       
}

void segregate_until_everyone_is_happy(World& world , double happiness_threshold){

    while(number_of_unhappy_creatures(world , happiness_threshold) != 0){
        Creatures_list unhappy_creatures_list = unhappy_creatures(world , happiness_threshold);
        Row_of_places_pointer available_places = list_of_available_places(world);
        if(!can_go_on_segregating(available_places , unhappy_creatures_list)){
            return;
        }
        segregate_one_time(available_places , unhappy_creatures_list);
    }
}

World get_character_map(string file_name){
    Id id = 1;
    World world;
    ifstream map(file_name);
    string one_row;
    while(getline(map , one_row)){
        world.push_back(Row_of_places(one_row.size()));
       for(int column = 0 ; column < one_row.size() ; column++){
           world.back()[column].type = one_row[column];
            world.back()[column].id = id;
            id++;
        }
    }
    map.close();
    return world;
}

double get_happiness_threshold(string happiness_threshold){
    double happ_tresh = stod(happiness_threshold)/100;
    return happ_tresh;
}

int get_iterations(string number_of_iterations){
    int iter = stoi(number_of_iterations);
    return iter;
}

void get_input(int argc, char* argv[], World& world, double& happiness_threshold, int& iterations){
    happiness_threshold = DEFAULT_HAPP_THRESH;
    iterations = TILL_ALL_HAPPY;
    int prefix_number = 2;
    while(argc > prefix_number){
        string command = argv[prefix_number-1];
        string information = argv[prefix_number];
        if(command ==  "-f")
            world = get_character_map(information);
        else if(command ==  "-p")
            happiness_threshold = get_happiness_threshold(information);
        else if(command == "-s")
            iterations = get_iterations(information);

        prefix_number = prefix_number+2;
        }
        
}

void segregate(World& world, double happiness_threshold, int iteration){
    if(iteration == -1)
        segregate_until_everyone_is_happy(world, happiness_threshold);
    else
        segregate_several_time(world, happiness_threshold, iteration);
    
}

void generate_ppm_color_map(const World& world){
    ofstream color_map("out.ppm");
    int row_count = world.size();
    int column_count = world[0].size(); 
    color_map << "P3 " << column_count << ' ' << row_count << " 1\n";
    for(auto row : world){
        for(auto place : row){
            if(place.type == RED)
                color_map << "1 0 0 ";
            else if(place.type == BLUE)
                color_map << "0 0 1 ";
            else if (place.type == EMPTY)
                color_map << "1 1 1 ";
        }
        color_map << endl;
    }
    color_map.close();
}

void output(const World& world, double happiness_threshold){
    cout << number_of_unhappy_creatures(world, happiness_threshold) << endl ;
    print_chracter_map(world);
    generate_ppm_color_map(world);
}


int main(int argc, char* argv[]){

    srand(time(0));
    World world;
    double happiness_threshold;
    int iteration;
    get_input(argc,argv,world,happiness_threshold , iteration);
    segregate(world, happiness_threshold, iteration);
    output(world, happiness_threshold);
    return 0;
}