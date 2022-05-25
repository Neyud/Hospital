#pragma once

#include <bits/stdc++.h>
#include "Instance.h"
#include "Library.h"
#define N 14

using namespace std;
typedef vector<int> list_D;

class Solution{
    private:
        // buoi a phong b do bs 1, 2, 3 ... lam
        vector<vector<list_D>> info_Schedule;
        Instance instance;
        vector<int> id_hard_room;
        vector<list_D> not_Working;
        vector<vector<bool>> fixed_position;
        vector<string> name_shift = {"thu 2", "thu 3", "thu 4", "thu 5", "thu 6", "thu 7", "chu nhat"};

        friend class Solver;

    public:
        Solution(){
            info_Schedule =  vector<vector<list_D>>();
            instance = Instance();
            id_hard_room = vector<int>();

        }
        Solution(Instance *instance){
            this->instance = *instance;
            for(int index_shift = 0; index_shift < N; ++index_shift){
                info_Schedule.push_back({});
                for(int index_room=0; index_room < instance->getNumRoom()+2; ++index_room){
                    info_Schedule[index_shift].push_back({});
                }
                fixed_position.push_back(vector<bool>(instance->getListRoom().size()+2, false));
            }
            for(auto &i : instance->getListRoom()){
                if(i.getLevel()==1){
                    id_hard_room.push_back(i.getId());
                }
            }
        }


        Solution(vector<vector<list_D>> info_Schedule){
            this->info_Schedule = info_Schedule;
        }

        vector<vector<list_D>> getInfoSchedule(){
            return info_Schedule;
        }

        void setInfoSchedule(vector<vector<list_D>> info_Schedule){
            this->info_Schedule = info_Schedule;
        }

        list_D getLichNghi(int shift){
               return info_Schedule[shift][info_Schedule[0].size()-1];
        }

        void setLichNghi( vector<list_D> lich_nghi){
               info_Schedule[info_Schedule.size() - 1] = lich_nghi;
        }

        list_D getLichTruc(int shift){
               return info_Schedule[shift][info_Schedule[0].size()-2];
        }

        void setLichTruc( vector<list_D> lich_truc){
               info_Schedule[info_Schedule.size() - 2] = lich_truc;
        }

        void setAt(int index_shift, int id_room, list_D id_doctors){
            info_Schedule[index_shift][id_room] = id_doctors;
        }

        list_D getLichLamViec(int shift){
            list_D result;
            for(const auto &i : info_Schedule[shift]){
                for(auto &j : i){
                    if(j < 0) break;
                    result.push_back(j);
                }
            }
            return result;
        }

        vector<int> getListRoomIsNotEmpty(int shift){
            list_D result;
            for(int i=0; i< info_Schedule[shift].size()-2; ++i){
                if(info_Schedule[shift][i].size() > 0 && info_Schedule[shift][i][0] > 0 ){
                    result.push_back(i);
                }
            }
            return result;
        }

        void addNotWorking(list_D list_doctor){
            not_Working.push_back(list_doctor);
        }

        void display(){
            cout << "Thong tin chi tiet: \n";
            for(const auto &row : info_Schedule){
                for(const auto &col : row){
                    for(auto id_doctor : col){
                        printf("%4d\t",id_doctor+1);
                    }
                    cout << "; ";
                }
                cout << "\n";
            }
            cout << "So vi pham: " << Score() << "\n";
        }
        void XuatFile(string linkFile){
            ofstream out(linkFile);
//            out << "Phong 1, Phong 2, Phong 3, Phong 4, Phong 5, Phong 6, Phong 7, Phong 8, Phong 9, Phong 10, Lich truc, Lich nghi \n";
//            for(const auto &row : info_Schedule){
//                for(const auto &col : row){
//                    for(auto id_doctor : col){
//                       // fprintf(out, "%4d\t",id_doctor+1);
//                        out << id_doctor+1 << "; ";
//                    }
//                    out << ", ";
//                }
//                out << "\n";
//            }
            out << ", Th2 S, Th2 C, Th3 S, Th3 C, Th4 S, Th4 C, Th5 S, Th5 C, Th6 S, Th6 C, Th7 S, Th7 C, CN S, CN C\n";
            vector<Doctor> listDoctor = instance.getListDoctor();
            for(int room=0; room<info_Schedule[0].size(); ++room){
                if(room < info_Schedule[0].size() - 2)
                    out << instance.getListRoom()[room].getName();
                else if(room == info_Schedule[0].size() - 2)
                    out << "Lich truc";
                else
                    out << "Lich nghi";
                out << ", ";
                for(int shift =0; shift < info_Schedule.size(); ++shift){
                    for(int ind_id_doctor=0; ind_id_doctor < info_Schedule[shift][room].size(); ++ind_id_doctor){
                        //fprintf(out, "%4d\t",id_doctor+1);
                        out << listDoctor[info_Schedule[shift][room][ind_id_doctor]].getName();
                        if(ind_id_doctor< info_Schedule[shift][room].size() - 1)
                            out << "; ";
                    }
                    out << ", ";
                }
                out << "\n";
            }

            out << "Not Working ,";
            for(int i=0; i<info_Schedule.size(); ++i){
                for(int j=0; j < not_Working[i].size(); ++j){
//                    out << not_Working[i][j] << "; ";
                    if(not_Working[i][j] == 0){
                        out << listDoctor[j].getName() ;
                        if(j < not_Working[i].size() -1)
                            out << "; ";
                    }


                }
                out << ", ";
            }
            out << "\n";

            out << "So vi pham 1:" << CountSub1() << "\n";
            out << "So vi pham 2:" << CountSub2() << "\n";
            out << "So vi pham 3:" << CountSub3() << "\n";
            out << "So vi pham 4:" << CountSub4()*2 << "\n";
            out << "So vi pham 5:" << CountSub5()*0.1 << "\n";
            out << "So vi pham: " << Score();
            out.close();
        }

        void XuatFile_1(string linkFile){
            ofstream out(linkFile);
//            out << "Phong 1, Phong 2, Phong 3, Phong 4, Phong 5, Phong 6, Phong 7, Phong 8, Phong 9, Phong 10, Lich truc, Lich nghi \n";
//            for(const auto &row : info_Schedule){
//                for(const auto &col : row){
//                    for(auto id_doctor : col){
//                       // fprintf(out, "%4d\t",id_doctor+1);
//                        out << id_doctor+1 << "; ";
//                    }
//                    out << ", ";
//                }
//                out << "\n";
//            }
            out << "Phong, Th2, Th3, Th4, Th5, Th6, Th7, CN\n";
            vector<Doctor> listDoctor = instance.getListDoctor();
            for(int room=0; room<info_Schedule[0].size(); ++room){
                if(room < info_Schedule[0].size() - 2)
                    out << instance.getListRoom()[room].getName();
                else if(room == info_Schedule[0].size() - 2)
                    out << "Lich truc";
                else
                    out << "Lich nghi";
                out << ", ";
                for(int shift =0; shift < info_Schedule.size(); shift++){
                    string names = "";
                    for(int ind_id_doctor=0; ind_id_doctor < info_Schedule[shift][room].size(); ++ind_id_doctor){
                        //fprintf(out, "%4d\t",id_doctor+1);
                        names += listDoctor[info_Schedule[shift][room][ind_id_doctor]].getName();
                        if(ind_id_doctor< info_Schedule[shift][room].size() - 1)
                            names += "; ";
                    }
                    shift++;
                    string names_ = "";
                    for(int ind_id_doctor=0; ind_id_doctor < info_Schedule[shift][room].size(); ++ind_id_doctor){
                        //fprintf(out, "%4d\t",id_doctor+1);
                        names_ += listDoctor[info_Schedule[shift][room][ind_id_doctor]].getName();
                        if(ind_id_doctor< info_Schedule[shift][room].size() - 1)
                            names_ += "; ";
                    }
                    if(names == names_){
                        out << names;
                    }else{
                        out << "S: " << names << "/" << "C: " << names_;
                    }

                    out << ", ";
                }
                out << "\n";
            }

            out << "Not Working ,";
            for(int i=0; i<info_Schedule.size(); ++i){
                string names = "";
                for(int j=0; j < not_Working[i].size(); ++j){
//                    out << not_Working[i][j] << "; ";
                    if(not_Working[i][j] == 0){
                        names += listDoctor[j].getName() ;
                        if(j < not_Working[i].size() -1)
                            names += "; ";
                    }
                }
                i++;
                string names_ = "";
                for(int j=0; j < not_Working[i].size(); ++j){
//                    out << not_Working[i][j] << "; ";
                    if(not_Working[i][j] == 0){
                        names_ += listDoctor[j].getName() ;
                        if(j < not_Working[i].size() -1)
                            names_ += "; ";
                    }
                }
                if(names == names_){
                    out << names;
                }else{
                    out << "S: " << names << "/" << "C: " << names_;
                }
                out << ", ";
            }
            out << "\n";

            out << "So vi pham 1:" << CountSub1() << "\n";
            out << "So vi pham 2:" << CountSub2() << "\n";
            out << "So vi pham 3:" << CountSub3() << "\n";
            out << "So vi pham 4:" << CountSub4()*2 << "\n";
            out << "So vi pham 5:" << CountSub5()*0.1 << "\n";
            out << "So vi pham: " << Score();
            out.close();
        }

        //điều kiện 1: 1 tuần không làm quá 4 buổi phòng nặng
            int CountSub1(){
                int result = 0;
                vector<int> count;
                int index = 0;
                for(int i = 0; i < instance.getNumDoctor(); i++){
                    count.push_back(0);
                }
                vector<Room> listRoom = instance.getListRoom();
                for(auto shift : info_Schedule){
                    for(auto id_room : id_hard_room){
                        for(auto id_doctor : shift[id_room]){
                            if(id_doctor < 0) continue;

                                if(id_doctor >= count.size()){
                                    std::cout << id_doctor << "\n";
                                    continue;
                                }
                                count[id_doctor] ++;
                                if(count[id_doctor] > 4){
                                  //  cout << index << " - " << id_doctor << "\n";
                                    result ++;
                                }

                        }
                    }
                   index++;
                }

                return result;

            }

        //điều kiện 2: 2 ngày liền không làm phòng nặng quá 2 buổi
            int CountSub2(){
                int result = 0;
                vector<int> count;
                int index = 0;
                for(int i = 0; i < instance.getNumDoctor(); i++){
                    count.push_back({});
                }
                for(int i=0; i<info_Schedule.size() - 3; i += 2){
                    for(int j=i; j<i+3; ++j){
                        for(auto id_room : id_hard_room){
                            for(auto id_doctor : info_Schedule[j][id_room]){
                                if(id_doctor < 0) continue;
                                if(id_doctor >= count.size()){
                                    std::cout << id_doctor << "\n";
                                    continue;
                                }
                                count[id_doctor] ++;
                                if(count[id_doctor] > 2)
                                {
                                    // cout << index << " - " << id_doctor << "\n";
                                     result ++;
                                }

                            }
                        }
                    }
                    count.assign(count.size(), 0);
                }
                return result;
            }

        //điều kiện 3: 1 người không được làm ở quá 1 phòng
            int CountSub3(){
                int result = 0;
                vector<int> count;
                for(int i = 0; i < instance.getNumDoctor(); i++){
                    count.push_back({0});
                }
                for(const auto &i:info_Schedule){
                    for(int j=0; j<i.size()-2; j+=2){
                        for(auto id_doctor : i[j]){
                            if(id_doctor < 0) continue;
                            if(id_doctor >= count.size()){
                                std::cout << id_doctor << "\n";
                                continue;
                            }
                            count[id_doctor] ++;
                            if(count[id_doctor] > 1){
                                result++;
                            }
                        }
                    }
                    count.assign(count.size(), 0);
                }
                return result;
            }
            //điều kiện 4: ô trống
            int CountSub4(){
                int result = 0;
                for(const auto &i : info_Schedule){
                    for(int j=0; j < i.size()-2; j++){
                        if((i[j].size() > 0 && i[j][0] == -1) || (i[j].size() == 0)){
                            result++;
                        }
                    }
                }
                return result;
            }

            //điều kiện 5: sự công bằng giữa các bác sĩ
            int CountSub5(){
                vector<int> count_shift = vector<int>(instance.getNumDoctor(), 0);
                for(const auto &i : info_Schedule){
                    for(int j=0; j < i.size()-2; j++){
                        for(auto k : i[j]){
                            count_shift[k]++;
                        }
                    }
                }
                sort(count_shift.begin(), count_shift.end());

                return (count_shift.back() - count_shift.front())/2;
            }


            double Score(){
                return CountSub1() + CountSub2() + CountSub3() + CountSub4()*2 + CountSub5()*0.1;
            }

            void readFile(string link_file){
                vector<vector<list_D>> dt;
                vector<vector<string>> data = readData(link_file);
                for(int row=1; row<data.size(); ++row){
                    while(dt.size()<row){
                        dt.push_back({});
                    }
                    for(int col=1; col<data[row].size(); ++col){
                        while(dt[row-1].size() < col){
                            dt[row-1].push_back({});
                        }
                        vector<string> token = tokenize(data[row][col], ';');
                        for(const auto &tmp:token){
                            dt[row-1][col-1].push_back(stoi(tmp)-1);
                        }
                    }
                }


                for(int j=0; j<dt[0].size(); j++){
                    info_Schedule.push_back({});
                    for(int i=0; i<dt.size(); ++i){
                        info_Schedule[j].push_back({});
                        info_Schedule[j][i] = dt[i][j];
                    }
                }
                cout << "Finish read file !\n";
            }

            void display_error_1(string linkFile_error){
                ofstream out(linkFile_error);
                out << "error 1\n";
                int result = 0;
                vector<int> count;
                int index = 0;
                for(int i = 0; i < instance.getNumDoctor(); i++){
                    count.push_back(0);
                }
                vector<Room> listRoom = instance.getListRoom();
                int index_shift = -1;
                for(auto shift : info_Schedule){
                    index_shift++;
                    for(auto id_room : id_hard_room){
                        for(auto id_doctor : shift[id_room]){
                            if(id_doctor < 0) continue;

                                if(id_doctor >= count.size()){
                                    std::cout << id_doctor << "\n";
                                    continue;
                                }
                                count[id_doctor] ++;
                                if(count[id_doctor] > 4){
                                  //  cout << index << " - " << id_doctor << "\n";
                                    out << name_shift[(int)(index_shift)/2] << ", " << instance.getListRoom()[id_room].getName() << ", " <<
                                           instance.getListDoctor()[id_doctor].getName() << "\n";
                                }

                        }
                    }
                   index++;
                }
                out.close();
            }

            void display_error_2(string linkFile_error){
                ofstream out(linkFile_error, std::ofstream::app);
                out << "error 2\n";
                int result = 0;
                vector<int> count;
                int index = 0;
                for(int i = 0; i < instance.getNumDoctor(); i++){
                    count.push_back({});
                }
                int index_shift = -1;
                for(int i=0; i<info_Schedule.size() - 3; i += 2){
                    index_shift++;
                    for(int j=i; j<i+3; ++j){
                        for(auto id_room : id_hard_room){
                            for(auto id_doctor : info_Schedule[j][id_room]){
                                if(id_doctor < 0) continue;
                                if(id_doctor >= count.size()){
                                    std::cout << id_doctor << "\n";
                                    continue;
                                }
                                count[id_doctor] ++;
                                if(count[id_doctor] > 2)
                                {
                                    out << name_shift[(int)(index_shift)/2] << ", " << instance.getListRoom()[id_room].getName() << ", " <<
                                           instance.getListDoctor()[id_doctor].getName() << "\n";
                                }

                            }
                        }
                    }
                    count.assign(count.size(), 0);
                    index++;
                }
                out.close();
            }

            void display_error_3(string linkFile_error){
                ofstream out(linkFile_error, std::ofstream::app);
                out << "error 3\n";
                int result = 0;
                vector<int> count;
                for(int i = 0; i < instance.getNumDoctor(); i++){
                    count.push_back({0});
                }
                int index_shift = -1;
                for(const auto &i:info_Schedule){
                    index_shift++;
                    for(int j=0; j<i.size()-2; j+=2){
                        for(auto id_doctor : i[j]){
                            if(id_doctor < 0) continue;
                            if(id_doctor >= count.size()){
                                std::cout << id_doctor << "\n";
                                continue;
                            }
                            count[id_doctor] ++;
                            if(count[id_doctor] > 1){
                                out << name_shift[(int)(index_shift)/2] << ", " << instance.getListRoom()[j].getName() << ", " <<
                                       instance.getListDoctor()[id_doctor].getName() << "\n";
                            }
                        }
                    }
                    count.assign(count.size(), 0);
                }
                out.close();
            }

            void display_error_4(string linkFile_error){
                ofstream out(linkFile_error, std::ofstream::app);
                 out << "error 4\n";
                 int result = 0;
                 int index_shift = -1;
                 for(const auto &i : info_Schedule){
                     index_shift++;
                     for(int j=0; j < i.size()-2; j++){
                         if((i[j].size() > 0 && i[j][0] == -1) || (i[j].size() == 0)){
                             out << name_shift[(int)(index_shift)/2] << ", " << instance.getListRoom()[j].getName() << ", " <<
                                    instance.getListDoctor()[j].getName() << "\n";
                         }
                     }
                 }
                 out.close();
             }

             void display_error(string linkFile_error){
                 display_error_1(linkFile_error);
                 display_error_2(linkFile_error);
                 display_error_3(linkFile_error);
                 display_error_4(linkFile_error);
             }

            void MixSolution(Solution s){
                vector<vector<list_D>> s_info_Schedule = s.getInfoSchedule();
                for(int row=0; row<s_info_Schedule.size(); ++row){
                    for(int col=0; col<s_info_Schedule[0].size(); ++col){
                        if(s_info_Schedule[row][col].size()>0 && s_info_Schedule[row][col][0] != -1){
                            info_Schedule[row][col] = s_info_Schedule[row][col];
                            fixed_position[row][col] = true;

                        }
                    }
                }
            }

};



