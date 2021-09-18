#include "rsdl.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

#define WIDTH 800
#define HEIGHT 600
#define START_X_BEGIN 245
#define START_X_END 558
#define START_Y_BEGIN 512
#define START_Y_END 581
#define ZERO 0
#define BLOCK_X_START 200
#define BLOCK_Y_START 84
#define BLOCK_X_SIZE 64
#define BLOCK_Y_SIZE 96
#define ROW_NUMBER 5
#define COLUMN_NUMBER 9
#define SUN_COUNTER_SIZE 100
#define LABLE_X_START 10
#define LABLE_Y_START 70
#define LABLE_X_SIZE 80
#define LABLE_Y_SIZE 30
#define R_BROWN 212
#define G_BROWN 213
#define B_BROWN 139
#define SUN_COUNTER_X 30
#define SUN_COUNTER_Y 72
#define FONT_SIZE 23
#define SUNFLOWER_COST 50
#define SUNFLOWER_TOUGHNESS 5
#define SUN_MAKING_TIME 25
#define SUN_WIDTH 116
#define SUN_HEIGHT 116
#define SECOND_UNITS 20
#define PEASHOOTER_COST 100
#define PEASHOOTER_TOUGHNESS 5
#define SHOOTING_TIME 1.5
#define PEA_WIDTH 35
#define PEA_HEIGHT 35
#define PEA_X_START_DIF 50
#define PEA_y_START_DIF 11
#define PEA_SPEED 25
#define WALLNUT_COST 50
#define WALLNUT_TOUGHNESS 72
#define WALLNUT_TOUGHNESS2 48
#define WALLNUT_TOUGHNESS3 24
#define INIT_SUNS 100
#define SUN_UNIT 25
#define SKY_SUN_SPEED 5
#define YARD_WIDTH 440
#define YARD_HEIGHT 380
#define ZOMBIE_BITE_RATE 1
#define ZOMBIE_SPEED 0.65
#define LEVEL_LABLE_X_START 685
#define LEVEL_LABLE_Y_START 575

struct Plant{
  int cost;
  int toughness;
  int plant_time;
};

struct Yard_block{
  int start_x;
  int start_y;
  char plant_type; /*E:empty / W:wallnut / P:peashooter / S:sunflower*/
  Plant plant;
};

struct Sun{
  int x;
  int init_y;
  int final_y;
};

struct Pea{
  int x;
  int y;
};

struct Zombie{
  int x;
  int y;
  int health;
};

bool is_start_clicked (int , int );
void initialize_yard_blocks (vector<vector<Yard_block> > &yard);
void check_enter_frontyard (int ,int ,bool &is_frontyard,
  vector<vector<Yard_block> > &yard);
void update_game_window (window &game_window , int cur_block_column ,
  int cur_block_row , vector<vector<Yard_block> > yard , string suns_counter ,
  int cur_level);
void set_plant_type (vector<vector<Yard_block> > &yard , char plant_type ,
  int cur_block_column , int cur_block_row , int &suns_bank);
void update_plants (vector<vector<Yard_block> > &yard , window & game_window ,
  vector <Sun> &suns , vector <vector<Pea> > &peas);
void check_set_sunflower(char plant_type , vector <vector <Yard_block> >&yard
, int cur_block_column , int cur_block_row , int &suns_bank);
void check_create_new_sun(vector <Sun> &suns,vector <vector<Yard_block> > &yard,
  int column , int row);
void draw_suns(window &game_window,vector <Sun> &suns,int tick);
void check_set_peashooter (char plant_type , vector <vector <Yard_block> >&yard
  , int cur_block_column , int cur_block_row , int &suns_bank);
void check_add_pea(vector <vector<Pea> > &peas ,
  vector <vector <Yard_block> > &yard , int column , int row);
void draw_peas(vector<vector<Pea> > peas , window &game_window);
void shoot_pea(vector<vector<Pea> > &peas);
void check_set_wallnut (char plant_type , vector <vector <Yard_block> >&yard
  , int cur_block_column , int cur_block_row , int &suns_bank);
void draw_wallnut_mode(vector<vector<Yard_block> > yard ,window & game_window,
  int column , int row);
void check_is_eated(vector<vector<Yard_block> > &yard , int column , int row);
void sun_clicked(int x , int y , vector<Sun> &suns , int &suns_bank);
void making_sky_sun(int moment , vector <Sun> &suns);
void new_zombie (vector <vector<Zombie> > &zombies ,
  vector<vector<Yard_block> > yard , int &cur_wave , int moment ,
  vector <int> &zombies_sequence ,vector <int> &waves_length , int waves);
void draw_zombies(vector <vector<Zombie> > &zombies , window & game_window);
void check_zombie_death(vector <vector<Zombie> > &zombies);
void move_zombies(vector <vector<Zombie> > &zombies);
bool check_loose (vector <vector<Zombie> > zombies , int cur_level);
void check_win (vector <vector<Zombie> > zombies , int cur_wave , int &waves ,
  int &cur_level,vector <int> &zombies_sequence , vector <int> &waves_length);
bool open_level_file(string file_name , int &waves ,
  vector <int> &zombies_sequence , vector <int> &waves_length);
void read_level_file(ifstream &level , int &waves ,
  vector <int> &zombies_sequence , vector <int> &waves_length);
void next_wave(vector <int> zombies_sequence , int &cur_wave);
void next_level(int &cur_level,int &waves ,vector <int> &zombies_sequence ,
  vector <int> &waves_length);
bool open_savedata_file(string file_name , int &cur_level);
void read_savedata_file(ifstream &savedata , int &cur_level);
bool write_savedata_file(string file_name , int cur_level , string name);
void zombie_hit(vector <vector<Zombie> > &zombies , vector<vector<Pea> > &peas);
void zombie_bite(vector <vector<Zombie> > &zombies ,
  vector<vector<Yard_block> > &yard , int moment);

int main(){
  srand(time(NULL));
  bool is_frontyard = false;
  vector <vector<Yard_block> > yard(COLUMN_NUMBER,vector <Yard_block>(ROW_NUMBER));
  window game_window(WIDTH,HEIGHT);

  game_window.draw_png("./Firstpage.png" , ZERO , ZERO , WIDTH , HEIGHT);
  game_window.update_screen();

  bool quit=false;
  int cur_block_column = 0 , cur_block_row = 0;
  int moment = 0;
  int tick = 0;
  int suns_bank = INIT_SUNS;
  int cur_level = 0;
  int waves = 0;
  int cur_wave = 0;
  string suns_counter;
  string file_name;
  vector <Sun> suns;
  vector <int> zombies_sequence;
  vector <int> waves_length;
  vector <vector<Pea> > peas(ROW_NUMBER);
  vector <vector<Zombie> > zombies(ROW_NUMBER);

  if(!open_savedata_file("savedata" , cur_level)){
    return 0;
  }

  file_name= to_string(cur_level) + ".level";
  if(!open_level_file (file_name , waves , zombies_sequence , waves_length)){
    return 0;
  }

  while (!quit){
    if(is_frontyard){
      tick++;
      suns_counter= to_string(suns_bank);
      update_game_window(game_window , cur_block_column , cur_block_row ,
      yard , suns_counter , cur_level);
      update_plants(yard , game_window , suns , peas);
      draw_suns(game_window , suns , tick);
      draw_peas(peas , game_window);
      shoot_pea(peas);
      making_sky_sun(moment , suns);
      new_zombie(zombies , yard , cur_wave , moment , zombies_sequence ,
        waves_length , waves);
      draw_zombies(zombies , game_window);
      check_zombie_death(zombies);
      move_zombies(zombies);
      zombie_hit(zombies , peas);
      zombie_bite(zombies , yard , moment);
    }
    HANDLE(
      LCLICK({
         check_enter_frontyard(mouse_x , mouse_y , is_frontyard , yard);
         sun_clicked(mouse_x , mouse_y , suns , suns_bank);
      })
      KEY_PRESS(w,{
        set_plant_type(yard , 'W' , cur_block_column , cur_block_row, suns_bank);
      })
      KEY_PRESS(p,{
        set_plant_type(yard , 'P' , cur_block_column , cur_block_row, suns_bank);
      })
      KEY_PRESS(s,{
        set_plant_type(yard , 'S' , cur_block_column , cur_block_row, suns_bank);
      })
      KEY_PRESS(d,{
        yard[cur_block_column][cur_block_row].plant_type = 'E' ;
      })
      KEY_PRESS(RIGHT,{
        if (cur_block_column < COLUMN_NUMBER - 1)
          cur_block_column++;
      })
      KEY_PRESS(LEFT,{
        if (cur_block_column > ZERO)
          cur_block_column--;
      })
      KEY_PRESS(DOWN,{
        if (cur_block_row < ROW_NUMBER - 1)
          cur_block_row++;
      })
      KEY_PRESS(UP,{
        if (cur_block_row > ZERO)
          cur_block_row--;
      })
      KEY_PRESS(q,quit=true);
      QUIT({
        quit=true;
        cout<<"Please enter your name:"<<endl;
        string name;
        cin>>name;
        write_savedata_file("savedata" , cur_level , name);
      })
    );
    if (is_frontyard){
      game_window.update_screen();
      if (check_loose(zombies , cur_level))
        break;
      check_win(zombies , cur_wave , waves , cur_level , zombies_sequence ,
        waves_length);
    }
    DELAY(50);
    moment++;
  }
  return 0;
}

bool is_start_clicked(int mouse_x , int mouse_y){
  if ( ( (mouse_x > START_X_BEGIN) && (mouse_x < START_X_END) ) &&
     ( (mouse_y > START_Y_BEGIN) && (mouse_y < START_Y_END) ) )
     return true;
     return false;
}

void initialize_yard_blocks (vector<vector<Yard_block> > &yard){
  int x= BLOCK_X_START;
  int y= BLOCK_Y_START;
  for (int i=0; i < 9; i++){
    for (int j=0; j < 5; j++){
      yard[i][j].start_x = x + (i*BLOCK_X_SIZE);
      yard[i][j].start_y = y + (j*BLOCK_Y_SIZE);
      yard[i][j].plant_type = 'E' ;
      yard[i][j].plant.cost = 0;
      yard[i][j].plant.toughness = 0;
      yard[i][j].plant.plant_time = 0;
    }
   }
 }

void check_enter_frontyard(int mouse_x,int mouse_y,bool &is_frontyard,
   vector<vector<Yard_block> > &yard){
   if ( (is_start_clicked(mouse_x , mouse_y) ) && (is_frontyard==false) ){
     initialize_yard_blocks(yard);
     is_frontyard=true;
   }
 }

void update_game_window(window &game_window , int cur_block_column ,
  int cur_block_row , vector<vector<Yard_block> > yard , string suns_counter ,
  int cur_level){
  game_window.draw_png("./Frontyard1.png" , ZERO , ZERO , WIDTH , HEIGHT);
  game_window.draw_png("./sun_conter.png" , ZERO , ZERO , SUN_COUNTER_SIZE ,
    SUN_COUNTER_SIZE );
  game_window.fill_rect(LABLE_X_START , LABLE_Y_START , LABLE_X_SIZE ,
    LABLE_Y_SIZE , RGB(R_BROWN,G_BROWN,B_BROWN));
  game_window.show_text(suns_counter ,SUN_COUNTER_X , SUN_COUNTER_Y ,
    BLACK , "./FreeSans.ttf" , FONT_SIZE );
  game_window.draw_rect(yard[cur_block_column][cur_block_row].start_x ,
    yard[cur_block_column][cur_block_row].start_y , BLOCK_X_SIZE ,
    BLOCK_Y_SIZE , BLACK);
  game_window.show_text(("level "+to_string(cur_level)),LEVEL_LABLE_X_START,
   LEVEL_LABLE_Y_START , RED , "./FreeSans.ttf" , FONT_SIZE );
}

void set_plant_type (vector<vector<Yard_block> > &yard , char plant_type ,
  int cur_block_column , int cur_block_row , int &suns_bank){
  if (yard[cur_block_column][cur_block_row].plant_type == 'E'){
    yard[cur_block_column][cur_block_row].plant_type = plant_type ;
    check_set_sunflower(plant_type , yard , cur_block_column , cur_block_row ,
      suns_bank);
    check_set_peashooter(plant_type , yard , cur_block_column , cur_block_row ,
      suns_bank);
    check_set_wallnut(plant_type , yard , cur_block_column , cur_block_row ,
      suns_bank);
  }
};

void update_plants (vector<vector<Yard_block> > &yard , window & game_window ,
  vector <Sun> &suns , vector<vector<Pea> > &peas){
  for (int i=0; i < COLUMN_NUMBER; i++){
    for (int j=0; j < ROW_NUMBER; j++){
      if (yard[i][j].plant_type == 'W' ){
        draw_wallnut_mode(yard , game_window , i , j);
        check_is_eated(yard , i ,j);
      }
      else if (yard[i][j].plant_type == 'P'){
        game_window.draw_png("./Peashooter.png" ,yard[i][j].start_x ,
          yard[i][j].start_y , BLOCK_X_SIZE ,BLOCK_Y_SIZE );
        check_add_pea(peas , yard , i , j);
        check_is_eated(yard , i ,j);
      }
      else if (yard[i][j].plant_type == 'S'){
        game_window.draw_png("./Sunflower.png" , yard[i][j].start_x ,
          yard[i][j].start_y , BLOCK_X_SIZE , BLOCK_Y_SIZE );
        check_create_new_sun (suns , yard , i , j);
        check_is_eated(yard , i ,j);
      }
    }
  }
}

void check_set_sunflower (char plant_type , vector <vector <Yard_block> >&yard
, int cur_block_column , int cur_block_row , int &suns_bank){
  if (plant_type == 'S'){
    suns_bank -= SUNFLOWER_COST;
    if (suns_bank >= ZERO){
      yard[cur_block_column][cur_block_row].plant.cost = SUNFLOWER_COST;
      yard[cur_block_column][cur_block_row].plant.toughness=SUNFLOWER_TOUGHNESS;
    }
    else {
      suns_bank += SUNFLOWER_COST;
      yard[cur_block_column][cur_block_row].plant_type = 'E';
    }
  }
}

void check_create_new_sun (vector <Sun> &suns ,
  vector <vector <Yard_block> >&yard , int column , int row){
  yard[column][row].plant.plant_time++;
  if ( (yard[column][row].plant.plant_time)%(SUN_MAKING_TIME*SECOND_UNITS) == 0){
    Sun s;
    s.x = yard[column][row].start_x ;
    s.init_y = yard[column][row].start_y ;
    s.final_y = yard[column][row].start_y ;
    suns.push_back(s);
  }
};

void draw_suns(window &game_window,vector <Sun> &suns,int tick ){
  for (int i=0; i < suns.size(); i++){
    int x = suns[i].x;
    int y = suns[i].init_y;
    if (y < suns[i].final_y)
      suns[i].init_y += SKY_SUN_SPEED;
    game_window.draw_png("./Sun3.png", x , y , SUN_WIDTH , SUN_HEIGHT , tick);
    game_window.draw_png("./Sun2.png", x+20 , y+20 , 77 , 79 , -1*tick%360);
    game_window.draw_png("./Sun1.png", x+43 , y+43 , 32 , 32);
  }
}

void check_set_peashooter (char plant_type , vector <vector <Yard_block> >&yard
  , int cur_block_column , int cur_block_row , int &suns_bank){
  if (plant_type == 'P'){
    suns_bank -= PEASHOOTER_COST;
    if (suns_bank >= ZERO){
      yard[cur_block_column][cur_block_row].plant.cost = PEASHOOTER_COST;
      yard[cur_block_column][cur_block_row].plant.toughness=PEASHOOTER_TOUGHNESS;
    }
    else {
      suns_bank += PEASHOOTER_COST;
      yard[cur_block_column][cur_block_row].plant_type = 'E';
    }
  }
}

void check_add_pea(vector <vector<Pea> > &peas ,
  vector <vector <Yard_block> > &yard , int column , int row){
  yard[column][row].plant.plant_time++;
  if ((yard[column][row].plant.plant_time)%((int)(SHOOTING_TIME*SECOND_UNITS))==0){
    Pea p;
    p.x = yard[column][row].start_x + PEA_X_START_DIF;
    p.y = yard[column][row].start_y + PEA_y_START_DIF;
    peas[row].push_back(p);
  }
}

void draw_peas(vector <vector<Pea> > peas , window &game_window){
  for (int i=0; i< ROW_NUMBER; i++){
    for (int j=0; j< peas[i].size(); j++){
      int x = peas[i][j].x;
      int y = peas[i][j].y;
      game_window.draw_png("./pea.png" , x , y , PEA_WIDTH , PEA_HEIGHT);
    }
  }
}

void shoot_pea(vector<vector<Pea> > &peas){
  for (int i=0; i< ROW_NUMBER; i++){
    for (int j=0; j< peas[i].size(); j++){
      peas[i][j].x += PEA_SPEED;
    }
  }
}

void check_set_wallnut (char plant_type , vector <vector <Yard_block> >&yard
  , int cur_block_column , int cur_block_row , int &suns_bank){
  if (plant_type == 'W'){
    suns_bank -= WALLNUT_COST;
    if (suns_bank >= ZERO){
      yard[cur_block_column][cur_block_row].plant.cost = WALLNUT_COST;
      yard[cur_block_column][cur_block_row].plant.toughness = WALLNUT_TOUGHNESS;
    }
    else {
      suns_bank += WALLNUT_COST;
      yard[cur_block_column][cur_block_row].plant_type = 'E';
    }
  }
}

void draw_wallnut_mode(vector<vector<Yard_block> > yard ,window & game_window,
  int column , int row){
  if (yard[column][row].plant.toughness > WALLNUT_TOUGHNESS2)
    game_window.draw_png("./Wallnut_body.png" , yard[column][row].start_x ,
      yard[column][row].start_y , BLOCK_X_SIZE , BLOCK_Y_SIZE );
  else if (yard[column][row].plant.toughness > WALLNUT_TOUGHNESS3)
    game_window.draw_png("./Wallnut_cracked1.png" , yard[column][row].start_x ,
      yard[column][row].start_y , BLOCK_X_SIZE , BLOCK_Y_SIZE );
  else if (yard[column][row].plant.toughness > ZERO)
    game_window.draw_png("./Wallnut_cracked2.png" , yard[column][row].start_x ,
      yard[column][row].start_y , BLOCK_X_SIZE , BLOCK_Y_SIZE );
}

void check_is_eated(vector<vector<Yard_block> > &yard , int column , int row){
  if (yard[column][row].plant.toughness <= ZERO)
    yard[column][row].plant_type = 'E';
}

void sun_clicked(int x , int y , vector<Sun> &suns , int &suns_bank){
  for (int i = 0; i < suns.size() ;i++){
    int left_threshold = suns[i].x;
    int right_threshold = suns[i].x + SUN_WIDTH - 10;
    int top_threshold = suns[i].final_y ;
    int bottom_threshold = suns[i].final_y + SUN_HEIGHT - 10;
    if(x>left_threshold && x<right_threshold && y<bottom_threshold &&
      y>top_threshold){
      suns.erase(suns.begin()+i);
      suns_bank += SUN_UNIT ;
    }
  }
}

void making_sky_sun(int moment , vector <Sun> &sky_suns){
  if ( (moment % (SECOND_UNITS) == 0) && (moment % 5 == 0)){
    Sun s;
    s.x = BLOCK_X_START + (rand() % YARD_WIDTH);
    s.init_y = ZERO;
    s.final_y = BLOCK_Y_START + (rand() % YARD_HEIGHT);
    sky_suns.push_back(s);
  }
}

void new_zombie (vector <vector<Zombie> > &zombies ,
  vector<vector<Yard_block> > yard , int &cur_wave , int moment ,
  vector <int> &zombies_sequence ,vector <int> &waves_length , int waves){
  if (cur_wave <= waves){
    next_wave(zombies_sequence , cur_wave);
    int zombie_making_rate = ((waves_length[cur_wave]*SECOND_UNITS)/
      zombies_sequence[cur_wave]);
    if (moment% zombie_making_rate == 0){
      Zombie z;
      int random_row = rand() % 5;
      z.x = (yard[8][random_row].start_x) + BLOCK_X_SIZE ;
      z.y = yard[8][random_row].start_y;
      z.health = 10;
      zombies[random_row].push_back(z);
      zombies_sequence[cur_wave]--;
      next_wave(zombies_sequence , cur_wave);
    }
  }
}

void draw_zombies(vector <vector<Zombie> > &zombies , window & game_window){
  for (int i=0; i < ROW_NUMBER; i++){
    for ( int j=0; j < zombies[i].size(); j++){
      game_window.draw_png("./zombie.png" , zombies[i][j].x , zombies[i][j].y
        , BLOCK_X_SIZE , BLOCK_Y_SIZE);
    }
  }
}

void check_zombie_death(vector <vector<Zombie> > &zombies){
  for (int i=0; i < ROW_NUMBER; i++){
    for ( int j=0; j < zombies[i].size(); j++){
      if (zombies[i][j].health <= 0 ){
        zombies[i].erase(zombies[i].begin()+j);
      }
    }
  }
}

void move_zombies(vector <vector<Zombie> > &zombies){
  for (int i=0; i < ROW_NUMBER; i++){
    for ( int j=0; j < zombies[i].size(); j++){
      zombies[i][j].x -= ZOMBIE_SPEED;
    }
  }
}

bool check_loose (vector <vector<Zombie> > zombies , int cur_level){
  for (int i=0; i < ROW_NUMBER; i++){
    for ( int j=0; j < zombies[i].size(); j++){
      if (zombies[i][j].x == BLOCK_X_START){
        cout << "YOU LOOSE!" << endl;
        cout<<"Please enter your name:"<<endl;
        string name;
        cin>>name;
        write_savedata_file("savedata" , cur_level , name);
        return 1;
      }
    }
  }
  return 0;
}

void check_win (vector <vector<Zombie> > zombies , int cur_wave , int &waves ,
  int &cur_level,vector <int> &zombies_sequence ,
  vector <int> &waves_length){
  if ((zombies.size() == 0) && (cur_wave == waves)){
    cout <<"YOU WIN!"<<endl;
    cout <<"LET'S GO TO NEXT LEVEL ..."<<endl;
    next_level(cur_level , waves , zombies_sequence , waves_length);
  }
}

bool open_level_file(string file_name , int &waves , vector <int> &zombies_sequence ,
  vector <int> &waves_length){
	ifstream level(file_name.c_str());
	if(!level) {
		cerr << "can't open file [" << file_name << "]\n";
	   return false;
	}
  read_level_file(level , waves , zombies_sequence , waves_length);
	return true;
}

void read_level_file(ifstream &level , int &waves , vector <int> &zombies_sequence ,
  vector <int> &waves_length){
  string trash;
  level>>trash;
  level>> waves;
  zombies_sequence.resize(waves);
  waves_length.resize(waves);
  level>> trash >> trash >> trash;
  for (int i=0; i < waves; i++){
    level>>zombies_sequence[i];
  }
  level>> trash >> trash;
  for (int i=0; i < waves; i++){
    level>>waves_length[i];
  }
  level.close();
}

void next_wave(vector <int> zombies_sequence , int &cur_wave ){
  if(zombies_sequence[cur_wave] == 0){
    cur_wave++;
  }
}

void next_level(int &cur_level,int &waves ,vector <int> &zombies_sequence ,
  vector <int> &waves_length){
  cur_level++;
  string file_name= to_string(cur_level) + ".level";
  open_level_file (file_name , waves , zombies_sequence , waves_length);
}

bool open_savedata_file(string file_name , int &cur_level){
  ifstream savedata(file_name.c_str());
	if(!savedata) {
		cerr << "can't open file [" << file_name << "]\n";
	  return false;
	}
  read_savedata_file(savedata , cur_level);
	return true;
}

void read_savedata_file(ifstream &savedata , int &cur_level){
  savedata>>cur_level;
  savedata.close();
}

bool write_savedata_file(string file_name , int cur_level , string name){
  ofstream w_savedata(file_name.c_str());
  if (!w_savedata){
    cerr << "can't open file [" << file_name << "]\n";
    return false;
  }
  w_savedata << cur_level <<endl;
  w_savedata << name <<endl;
  w_savedata.close();
  return true;
}

void zombie_hit(vector <vector<Zombie> > &zombies , vector<vector<Pea> > &peas){
  for (int i=0; i< ROW_NUMBER; i++){
    if (peas[i].size()!=0 && zombies[i].size()!=0){
      if ((zombies[i][0].x - peas[i][0].x) < PEA_SPEED){
        zombies[i][0].health--;
        peas[i].erase(peas[i].begin());
      }
    }
  }
}

void zombie_bite(vector <vector<Zombie> > &zombies ,
  vector<vector<Yard_block> > &yard , int moment){
  for (int i=0; i< ROW_NUMBER; i++){
    for (int j = 0; j < zombies[i].size(); j++){
      int next_block_column=((zombies[i][j].x-BLOCK_X_START) / BLOCK_X_SIZE);
      if (yard[next_block_column][i].plant_type!='E'){
        zombies[i][j].x += 2*ZOMBIE_SPEED;
        if (moment % (ZOMBIE_BITE_RATE*SECOND_UNITS) == 0)
          yard[next_block_column][i].plant.toughness--;
      }
    }
  }
}
