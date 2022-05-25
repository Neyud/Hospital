#pragma once
#include <bits/stdc++.h>
#include "Instance.h"
#include "Solution.h"

#define N 14

using namespace std;

class Solver {
   private:
    Instance *instance;
    Solution *bestSolution;
    vector<vector<int>> flag_doctors;
    vector<vector<pair<int, int>>> capacity_room_shifts;

   public:
    Solver() {}

    Solver(Instance *instance) {
        this->instance = instance;
        bestSolution = new Solution(instance);
        for(int i=0; i<N; ++i){
            flag_doctors.push_back(vector<int>(instance->getNumDoctor(), 0));
            capacity_room_shifts.push_back(vector<pair<int, int>>());
        }

    }

    void StartConstruction() {
        vector<pair<int, int>> capacity_rooms;
        vector<Room> listRoom = instance->getListRoom();
        for(auto i : listRoom){
            capacity_rooms.push_back({i.getRoomCapacity().size(), i.getId()});
        }
        for(auto &i: capacity_room_shifts){
            i = capacity_rooms;
        }

        //sort(capacity_rooms.begin(), capacity_rooms.end());
//        vector<int> id_doctors;
//        for(int i=0; i<instance->getNumDoctor(); ++i){
//            id_doctors.push_back({});
//        }
        vector<int> id_rooms;
        for(int i=0; i<instance->getNumRoom(); ++i){
            id_rooms.push_back({});
        }

        for(int shift=0; shift<N; shift+=2){
            int flag = instance->getNumRoom();
            flag_doctors[shift].assign(flag_doctors[shift].size(), 0);
//            id_doctors.assign(id_doctors.size(), 0);
            id_rooms.assign(id_rooms.size(), 0);
            for(auto id : bestSolution->getLichNghi(shift)){
//                id_doctors[id] = -1;
                flag_doctors[shift][id] = -1;
                for(auto &i : listRoom){
                    if(i.checkIdDoctor(id)){
                        for(auto &c : capacity_room_shifts[shift]){
                            if(c.second == i.getId()){
                                c.first --;
                                break;
                            }
                        }
                    }
                }
            }
            for(auto id : bestSolution->getLichTruc(shift)){
//                id_doctors[id] = -1;
                flag_doctors[shift][id] = -1;
                for(auto &i : listRoom){
                    if(i.checkIdDoctor(id)){
                        for(auto &c : capacity_room_shifts[shift]){
                            if(c.second == i.getId()){
                                c.first --;
                                break;
                            }
                        }
                    }
                }
            }
            for(auto id:bestSolution->getLichLamViec(shift)){
//                id_doctors[id] = -1;
                flag_doctors[shift][id] = -1;
                for(auto &i : listRoom){
                    if(i.checkIdDoctor(id)){
                        for(auto &c : capacity_room_shifts[shift]){
                            if(c.second == i.getId()){
                                c.first --;
                                break;
                            }
                        }
                    }
                }
            }

            for(auto id_room:bestSolution->getListRoomIsNotEmpty(shift)){
                id_rooms[id_room] = -1;
                flag--;
            }



            for(;flag>0; flag--){
                sort(capacity_room_shifts[shift].begin(), capacity_room_shifts[shift].end());
                int index_min_room = -1;
                for(auto i:capacity_room_shifts[shift]){
                    if(id_rooms[i.second] >= 0){
                        index_min_room = i.second;
                        break;
                    }
                }

                if(index_min_room<0) break;
//                vector<int> id_doctors_ran = listRoom[index_min_room].randListDoctor(id_doctors);
                vector<int> id_doctors_ran = listRoom[index_min_room].randListDoctor(flag_doctors[shift]);
                //if(id_doctors_ran.size()==0)
                //    id_doctors_ran = vector<int> (1, -1);
                bestSolution->setAt(shift, index_min_room, id_doctors_ran);
                bestSolution->setAt(shift+1, index_min_room,id_doctors_ran);

                // xoa bac si ra khoi phong va list bac si chua xep lich
                for(auto id_ : id_doctors_ran){
//                    id_doctors[id_] = -1;
                    flag_doctors[shift][id_] = -1;
                    for(auto &i : listRoom){
                        if(i.checkIdDoctor(id_)){
                            for(auto &c : capacity_room_shifts[shift]){
                                if(c.second == i.getId()){
                                    c.first --;
                                    break;
                                }
                            }
                        }
                    }
                }

                //xoa phong
                id_rooms[index_min_room] = -1;
            }
//            bestSolution->addNotWorking(id_doctors);
//            bestSolution->addNotWorking(id_doctors);
            bestSolution->addNotWorking(flag_doctors[shift]);
            bestSolution->addNotWorking(flag_doctors[shift]);
        }

//          cout << "132\n";
    }

    void Destroy(Solution* CurrentSol){
         int rand_ = (rand()%21 + 10) * N/2 * instance->getNumRoom() / 100;
         cout << rand_ << "\n" << "------------\n";
         while(rand_){
             int index_row = rand() % CurrentSol->getInfoSchedule().size();
             index_row -= index_row % 2;
             vector<list_D> shift = CurrentSol->getInfoSchedule()[index_row];
             int index_col = rand() % shift.size();
             if(shift[index_col].size()>0 && shift[index_col][0] != -1 && CurrentSol->fixed_position[index_row][index_col] == false){
                 for(auto i : shift[index_col]){
                     flag_doctors[index_row][i] = 0;
                     for(auto &cap_id_room : capacity_room_shifts[index_row]){
                         if(cap_id_room.second == index_col){
                             cap_id_room.first ++;
                             break;
                         }
                     }
                 }
                 vector<int> trc = CurrentSol->getInfoSchedule()[index_row][index_col];
                 CurrentSol->setAt(index_row, index_col,vector<int> (1, -1));
                 vector<int> sau = CurrentSol->getInfoSchedule()[index_row][index_col];
                 CurrentSol->setAt(index_row+1, index_col,vector<int> (1, -1));
                 --rand_;

             }

         }
  //       CurrentSol->XuatFile("C:/Users/Asus/Documents/QTProject/Hospital_Schedule/destroy.csv");
  //       cout << "destroy\n";
    }

    void Repair(Solution* CurrentSol){
        vector<Room> listRoom = instance->getListRoom();

        vector<int> id_rooms;
        for(int i=0; i<instance->getNumRoom(); ++i){
            id_rooms.push_back({});
        }

        for(int shift=0; shift<N; shift+=2){
            int flag = instance->getNumRoom();
            id_rooms.assign(id_rooms.size(), 0);


            for(auto id_room:CurrentSol->getListRoomIsNotEmpty(shift)){
                id_rooms[id_room] = -1;
                flag--;
            }


            for(;flag>0; flag--){
                sort(capacity_room_shifts[shift].begin(), capacity_room_shifts[shift].end());
                int index_min_room = -1;
                for(auto i:capacity_room_shifts[shift]){
                    if(id_rooms[i.second] >= 0){
                        index_min_room = i.second;
                        break;
                    }
                }

                if(index_min_room<0) break;
                vector<int> id_doctors_ran = listRoom[index_min_room].randListDoctor(flag_doctors[shift]);
                //if(id_doctors_ran.size()==0)
                //    id_doctors_ran = vector<int> (1, -1);
                CurrentSol->setAt(shift, index_min_room, id_doctors_ran);
                CurrentSol->setAt(shift+1, index_min_room,id_doctors_ran);

                // xoa bac si ra khoi phong va list bac si chua xep lich
                for(auto id_ : id_doctors_ran){
                    flag_doctors[shift][id_] = -1;
                    for(auto &i : listRoom){
                        if(i.checkIdDoctor(id_)){
                            for(auto &c : capacity_room_shifts[shift]){
                                if(c.second == i.getId()){
                                    c.first --;
                                    break;
                                }
                            }
                        }
                    }
                }

                //xoa phong
                id_rooms[index_min_room] = -1;
            }
            CurrentSol->addNotWorking(flag_doctors[shift]);
            CurrentSol->addNotWorking(flag_doctors[shift]);
        }

//          cout << "132\n";
    }

    void DataFromFile(string linkFile){
        Solution s_data = Solution();
        s_data.readFile(linkFile);
        bestSolution->MixSolution(s_data);
    }

    void SolutionConstruction() {
    }

    void display(){
        bestSolution->display();
    }
    void WriteFile(string linkFile_result, string linkFile_error){
        //bestSolution->XuatFile(linkFile_result);
        bestSolution->XuatFile_1(linkFile_result);
        bestSolution->display_error(linkFile_error);

    }

    void LocalSearch(){
        StartConstruction();
        int sc = bestSolution->Score();
        cout << "Score start: " << sc << "\n";
        int step = 10000;
        Solution CurrentSol = *bestSolution;
        float eps = 0.995;

        do{
            Solution tmp = CurrentSol;
            Destroy(&tmp);
            Repair(&tmp);
            if(tmp.Score() <= CurrentSol.Score()){
                CurrentSol = tmp;
                if(tmp.Score() < bestSolution->Score()){
                    *bestSolution = tmp;
                    tmp.display();
                }
            }
            cout <<"tmp score: " <<  tmp.Score() << " - best score: " << bestSolution->Score() << "\n";
            //CurrentSol.display();
            cout << "\n=====================================\n";
            eps *= 0.995;

        }while(--step);
    }
};

