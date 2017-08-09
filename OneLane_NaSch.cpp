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


const int Vel_max=5;
Crandom ran(54);

class Car{

private:                                              // Class Car is introduced.
  int Vel,Pos,l;                                      // Position, velocity and length of the car set as private variables. 
public:
  void create_car(int Pos0, int Vel0,int l0);         // initializes a car with initial Pos, Vel and l;
  void move_car(void);
  void decide(int Pos_a, int Vel_a, int l_a);
  friend class Lane;                                  // to allow class Lane get Pos, Vel and Length
};

void Car::create_car(int Pos0, int Vel0,int l0){     
  Pos=Pos0; Vel=Vel0; l=l0; 
}

void Car::move_car(void){                              // car moves Vel*t but t=1 so just Vel
  Pos+=Vel;
}

void Car::decide(int Pos_a, int Vel_a, int l_a){             // deciding rules         
  double p=0.3;                                              // probability of reducing velocity
  int dist_a=Pos_a-Pos-l_a, v_temp;                                  // distance to car ahead  taking into account cars length

  Vel=min(Vel_max,Vel+1);                                    // acceleration
  Vel=min(Vel,dist_a);                                       // slowing down
  if(Vel>0 and ran.r()<p)Vel--;                              // randomization
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
  void interact(void);
  void put_N_cars(int N,int seed);
  double get_average_vel(void);
  int get_number_of_cars(void);
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

void Lane::update_lane(void){                                    // This function freshes cars position on the lane
for(int i=0; i<L; i++){Street[i]=0;}                             // Set all lane values to 0
  for(int i=0; i<n_cars; i++){
    for(int j=Cars[i].Pos; j>(Cars[i].Pos-Cars[i].l);j--){       // Place a 1s for every car according to its length
      Street[j%L]=1;                                             // The j%L holds for periodic boundary conditions
    }
  }
}

void Lane::move_cars(void){                                      // Makes every car on the cars arrange move according to its velocity
  for(int i=0; i<n_cars; i++){
    Cars[i].move_car();
  }
}

void Lane::interact(void){
  int P_old[n_cars], V_old[n_cars];
  for(int i=0; i<n_cars; i++){
    P_old[i]=Cars[i].Pos;  V_old[i]=Cars[i].Vel;
  }
  for(int i=0; i<n_cars-1; i++){
    Cars[i].decide(P_old[i+1],V_old[i+1],Cars[i+1].l);     // Makes every car decide according to the car ahead
  }
  Cars[n_cars-1].decide(P_old[0]+L,V_old[0],Cars[0].l);    // Makes the last car interact with the first so it's periodic.
}

void Lane::put_N_cars(int N,int seed){                           // This is supposed to put N cars randomly with random velocities aswell...
  Crandom rr(seed), rrr(seed+132);                               // ...but I haven't found an efficient way to do so.
  int N_current=0;                                               // This definitely needs to be improved.
  for(int i=1; i<L;i++){                                         // What I do is to go over the Lane and "throw a dice" on each cell to decide..
    if(rr.r()<0.9){                                              //..whether to put a car or not, and then move to the next cell. That's...
      Car R_Car;                                                 //..repeated until the desired number of cars is gotten. However,I run out of...
      R_Car.create_car(i,(int)(Vel_max*rrr.r()),2);              //... lane's cell before I get the desired number everytime. I chose I higher...
      place_car(R_Car);                                          // probability (90%) to fix it. Still don't like it though. Ther must be a better way
      N_current++; i+=1;
    }
    if(N_current==N) break;
  }
}

double Lane::get_average_vel(void){                             // This returns the average velocities over all the cars on the lane.
  double sum=0;
  for(int i=0; i<n_cars; i++){
    sum+=Cars[i].Vel;
  }
  return sum/n_cars;
}

int Lane::get_number_of_cars(void){return n_cars;}             // Returns number of cars.

int main(void){                   

  int Lane_size=1000; int N_CARS=Lane_size/2;
  for(double N=1;N<N_CARS;N+=10){
    
    Lane Gold;
    
    Gold.build_lane(Lane_size);
    Gold.put_N_cars(N,9);
    for(int t=0;t<5000;t++){
      Gold.update_lane();
      //Gold.show_lane();
      Gold.interact();
      Gold.move_cars();
    }
    
    cout<<(double)2*Gold.get_number_of_cars()/1000<<" "<<Gold.get_average_vel()<<endl;  //rho vs mean vel
    //cout<<(double)2*Gold.get_number_of_cars()/1000<<" "<<Gold.get_average_vel()*(double)2*Gold.get_number_of_cars()/1000<<endl; //fund. diagram.
    
 
  }
  return 0;
}
