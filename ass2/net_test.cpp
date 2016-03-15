#include "train.hpp"
#include <iostream>

//using Coach::Class;
//using Coach::SType;
//using Berth::Btype;

int main(int argc, char **argv) {
    Train t1(1, "aloo");
    t1.add_coaches(2, Coach::Class::AC, Coach::SType::FOUR, 2);
    //t1.add_coaches(2,Coach::Class::SLEEPER,Coach::SType::SIX,2);
    std::cout << t1.get_empty_seats(Coach::Class::AC) << "_" << t1.get_booked_seats() <<"\n";
    BerthPref bp1, bp2;
    bp1.type = Berth::Btype::SU;
    bp2.type = Berth::Btype::SL;
    bp1.coach_type=(int)Coach::Class::AC;
    bp2.coach_type=(int)Coach::Class::AC;
    printf("%d..\n",bp1.coach_type);

    std::vector<BerthPref> bpv;
    bpv.push_back(bp1);
    bpv.push_back(bp2);

    std::vector<std::vector<Berth>::iterator> berths;
    bool success = t1.book(bpv, berths);
    if (success) {
        for (auto it: berths) {
            std::cout << it->get_coach_name() << " "<< it->no << "\n";
        }
    }
    std::cout << t1.get_empty_seats(Coach::Class::AC) << "_" << t1.get_booked_seats(Coach::Class::AC) <<"\n";
    return 0;
}
