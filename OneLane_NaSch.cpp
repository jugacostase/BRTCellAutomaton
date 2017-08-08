// Malla de Trafico
#include <iostream>
#include<fstream>
#include<cstdlib>
#include <cmath>
#include <cstdlib> 
#include <ctime> 
#include <vector>
#include "Random64.h"
using namespace std;

class Car{

private:                                              // Class Car is introduced.
  int Vel,Pos,l;                                      // Position, velocity and length of the car set as private variables. 
public:
  void create_car(int Pos0, int Vel0,int l0);         // initializes a car with initial Pos, Vel and l;
  void move_car(void);
  friend class Lane;                                  // to allow class Lane get Pos, Vel and Length
};

void Car::create_car(int Pos0, int Vel0,int l0){     
  Pos=Pos0; Vel=Vel0; l=l0;
}

void Car::move_car(void){                           // car moves Vel*t but t=1 so just Vel
  Pos+=Vel;
}

class Lane{                                         // I introduce the class Lane, which will place objects of the class Car on it.
private:                                            // L is the lane length.
  int L,n_cars;
  vector<Car>Cars;                                  // This array will contain the cars.
  vector<int>Street;                                // This array will contain the position info. 
public:
  void build_lane(int L0);                      
  void place_car(Car Auto);
  void show_lane(void);
  void update_lane(void);
  void move_cars(void);
};

void Lane::build_lane(int L0){                                  // Sets a lane of 0's of length L and number of cars=0;
  L=L0; n_cars=0;
  for(int i=0; i<L; i++){Street.push_back(0);}
}

void Lane::place_car(Car Auto){                                 // Puts a car in the array of cars
  Cars.push_back(Auto);
  n_cars++;
 
}

void Lane::show_lane(void){                                     // Prints rows of 0s with 1s where the cars are
  for(int i=0; i<L; i++){cout<<Street[i];}
  cout<<endl;
}

void Lane::update_lane(void){                                    // Refresh cars position on the lane
for(int i=0; i<L; i++){Street[i]=0;}
  for(int i=0; i<n_cars; i++){
    for(int j=Cars[i].Pos; j>(Cars[i].Pos-Cars[i].l);j--){
      Street[j%L]=1;
    }
  }
}

void Lane::move_cars(void){                                   
  for(int i=0; i<n_cars; i++){
    Cars[i].move_car();
  }
}

int main(void){

  Lane Gold;
  Car Volvo,MB;
  
  Gold.build_lane(10);
  Volvo.create_car(10,1,2);
  MB.create_car(15,1,2);
  Gold.place_car(Volvo);
  Gold.place_car(MB);
  for(int t=0;t<15;t++){
    Gold.update_lane();
    Gold.show_lane();
    Gold.move_cars();
  }

  return 0;
}
