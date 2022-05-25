#include "Instance.h"
#include "Solution.h"
#include "Solver.h"

using namespace std;

int main() {
    srand(time(NULL));
    vector<string> link_file = {"C:/Users/Asus/Documents/QTProject/Hospital_Schedule/Doctor_1.csv",
                                "C:/Users/Asus/Documents/QTProject/Hospital_Schedule/Room_1.csv",
                                "C:/Users/Asus/Documents/QTProject/Hospital_Schedule/AbilityMatrix_1.csv",
                                "C:/Users/Asus/Documents/QTProject/Hospital_Schedule/JobSchedule_1.csv"
                               };


    Instance instance = Instance();
    instance.readNameDoctors(link_file[0]);
    instance.readInforRooms(link_file[1]);
    instance.readCapacityDoctors(link_file[2]);

    
    Solver Solver(&instance);

    Solver.DataFromFile(link_file[3]);
    //Solver.StartConstruction();
    Solver.LocalSearch();
    Solver.display();
    Solver.WriteFile("C:/Users/Asus/Documents/QTProject/Hospital_Schedule/test1.csv",
                     "C:/Users/Asus/Documents/QTProject/Hospital_Schedule/error.csv");




    return 0;
}


