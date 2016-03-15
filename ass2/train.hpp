//booking request struct
//coaches struct
//class att
//berth struct

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>

struct Berth {
    friend struct Coach;

    enum StatusEnum {
        OCCUPIED=1, UNOCCUPIED=0
    };

    private:
    StatusEnum status;

    public:
    enum Btype {
        SU, SL, UB, MB, LB, NA
    };

    const int no;
    const Btype type;
    const std::string coach_name;

    Berth(const int id, const Btype btype, std::string c_str): no(id), type(btype), status(UNOCCUPIED), coach_name(c_str) {};

    bool is_reserved() {
        return status == OCCUPIED ? true : false;
    }

    std::string get_coach_name() {
        return coach_name;
    }

    std::string get_berth_place() {
        switch (type) {
            case SU: return "SU";
            case SL: return "SL";
            case UB: return "UB";
            case MB: return "MB";
            case LB: return "LB";
        }
    }

    bool operator==(const Berth& b) {
        return (no == b.no && type == b.type && status == b.status);
    }

    private:
    bool reserve() {
        if (status == OCCUPIED) return false;
        status = OCCUPIED;
        return true;
    }

    bool unreserve() {
        if (status == UNOCCUPIED) return false;
        status = UNOCCUPIED;
        return true;
    }
};

struct BerthPref {
    Berth::Btype type;
    int coach_type;
};

struct Coach {
    private:
    std::vector<Berth> berths;
    int booked_seats;
    int empty_seats;

    public:
    enum SType {
        FOUR=4, SIX=6
    };

    enum Class {
        AC, SLEEPER
    };

    const int no;
    const Class ctype;
    const SType stype;

    Coach(int id, Class class_type, SType seating_type, unsigned int num_compartments): no(id), ctype(class_type), stype(seating_type), booked_seats(0), empty_seats(0) {
        std::string cname = this->name();
        if (seating_type == FOUR) {
            for (unsigned int i = 1; i <= 6*num_compartments; i++) {
                switch (i%6) {
                    case 1: {
                                Berth b(i, Berth::Btype::LB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 2: {
                                Berth b(i, Berth::Btype::UB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 3: {
                                Berth b(i, Berth::Btype::LB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 4: {
                                Berth b(i, Berth::Btype::UB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 5: {
                                Berth b(i, Berth::Btype::SL, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 0: {
                                Berth b(i, Berth::Btype::SU, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                }
            }
        } else {
            for (unsigned int i = 1; i <= 8*num_compartments; i++) {
                switch (i%8) {
                    case 1: {
                                Berth b(i, Berth::Btype::LB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 2: {
                                Berth b(i, Berth::Btype::MB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 3: {
                                Berth b(i, Berth::Btype::UB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 4: {
                                Berth b(i, Berth::Btype::LB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 5: {
                                Berth b(i, Berth::Btype::MB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 6: {
                                Berth b(i, Berth::Btype::UB, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 7: {
                                Berth b(i, Berth::Btype::SL, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                    case 0: {
                                Berth b(i, Berth::Btype::SU, cname);
                                berths.push_back(b);
                                empty_seats++;
                                continue;
                            }
                }
            }
        }
    }

    std::string name() const {
        std::stringstream ss;
        if (ctype == AC) {
            ss << "A-" << no;
            return ss.str();
        } else if (ctype == SLEEPER) {
            ss << "S-" << no;
            return ss.str();
        } else {
            ss << "N.A.-" << no;
            return ss.str();
        }
    }

    int get_empty_seats() const {
        return this->empty_seats;
    }

    int get_booked_seats() const {
        return this->booked_seats;
    }

    bool reserve_berth(const int b_id) {
        for (auto it = berths.begin(); it != berths.end(); it++) {
            if (it->no == b_id) {
                if (!it->is_reserved()) {
                    bool success = it->reserve();
                    if (!success) {
                        return false;
                    } else {
                        empty_seats--;
                        booked_seats++;
                        return true;
                    }
                } else {
                    return false;
                }
                break;
            }
        }
        return false;
    }

    bool check_availability(const Berth::Btype t) const {
        for (auto it: berths) {
            if (it.type == t && !it.is_reserved()) {
                return true;
            }
        }
        return false;
    }

    bool accomodate(std::vector<BerthPref> prefs, std::vector<std::vector<Berth>::iterator>& reservations) {
        std::vector<std::vector<Berth>::iterator> found_ones;
        unsigned int pref_size = prefs.size();
        if (pref_size > empty_seats) { return false; }
        for (auto it = berths.begin(); it != berths.end(); it++) {
            if (!prefs.empty()) {
                for (std::vector<BerthPref>::iterator prit = prefs.begin(); prit != prefs.end(); prit++) {
                    if (prit->coach_type != (int) ctype) {
                        return false;
                    }
                    if ((it->type == prit->type || prit->type == Berth::Btype::NA) && !it->is_reserved()) {
                        bool already_found = false;
                        for (auto af = found_ones.begin(); af != found_ones.end(); af++) {
                            if (it->no == (*af)->no) {
                                already_found = true;
                                break;
                            }
                        }
                        if (!already_found) {
                            prit = prefs.erase(prit);
                            found_ones.push_back(it);
                            break;
                        }
                    }
                }
            } else {
                break;
            }
        }
        if (prefs.empty() && pref_size == found_ones.size()) {
            for (auto it: found_ones) {
                it->reserve();
                empty_seats--;
                booked_seats++;
            }
            reservations.insert(reservations.end(), found_ones.begin(), found_ones.end());
            return true;
        }
        if (!prefs.empty()) {
            for (auto fit: found_ones) {
                fit->reserve();
                empty_seats--;
                booked_seats++;
            }
            int pref_left_seats = prefs.size();
            for (auto bit = berths.begin(); bit != berths.end(); bit++) {
                if (!bit->is_reserved()) {
                    found_ones.push_back(bit);
                    bit->reserve();
                    empty_seats--;
                    booked_seats++;
                    pref_left_seats--;
                    if (pref_left_seats == 0) {
                        break;
                    }
                }
            }
            reservations.insert(reservations.end(), found_ones.begin(), found_ones.end());
            return true;
        }
        return false;
    }
};

class Train {
    std::string name_;
    unsigned int number_;
    int ac_coach_counter;
    int sleeper_coach_counter;
    std::vector<Coach> coaches;

    public:

    Train(unsigned int num, std::string tname): name_(tname), number_(num) {
        ac_coach_counter=0;//MADE CHANGES
        sleeper_coach_counter=0;//MADE CHANGES
    };

    void add_coaches(int num, Coach::Class ctype, Coach::SType stype, int num_comparts_per_coach) {
        for (int i = 1; i <= num; i++) {
            Coach c((ctype == Coach::Class::AC ? ac_coach_counter : sleeper_coach_counter)+1, ctype, stype, num_comparts_per_coach);
            coaches.push_back(c);
            if (ctype == Coach::Class::AC) {
                ac_coach_counter++;
            } else {
                sleeper_coach_counter++;
            }
        }
    }

    std::string name() {
        return name_;
    }

    unsigned int number() {
        return number_;
    }

    int get_empty_seats(Coach::Class cl_type) {
        int sum = 0;
        for (auto cit: coaches) {
            if (cit.ctype == cl_type) {
                sum += cit.get_empty_seats();
            }
        }
        return sum;
    }

    int get_booked_seats(Coach::Class cl_type) {
        int sum = 0;
        for (auto cit: coaches) {
            if (cit.ctype == cl_type) {
                sum += cit.get_booked_seats();
            }
        }
        return sum;
    }

    int get_empty_seats() {
        int sum = 0;
        for (auto cit: coaches) {
            sum += cit.get_empty_seats();
        }
        return sum;
    }

    int get_booked_seats() {
        int sum = 0;
        for (auto cit: coaches) {
            sum += cit.get_booked_seats();
        }
        return sum;
    }
    bool book(std::vector<BerthPref> prefs, std::vector<std::vector<Berth>::iterator>& reservations) {
        bool success = false;
        if (prefs.size() > this->get_empty_seats()) return false;
        for (auto& cit: coaches) {
            success = cit.accomodate(prefs, reservations);
            if (success) return true;
        }
        while (prefs.size() > 0) {
            int max = 0;
            Coach * c_ptr = NULL;
            for (auto& cit: coaches) {
                int temp = cit.get_empty_seats();
                if (temp > max) {
                    max = temp;
                    c_ptr = &cit;
                }
            }
            std::vector<BerthPref> pref_chunk;
            if (c_ptr == NULL || max == 0) return false;
            for (int i = 0; i < max; i++) {
                pref_chunk.push_back(prefs.back());
                prefs.pop_back();
            }
            bool success = c_ptr->accomodate(pref_chunk, reservations);
            if (!success) {
                return false;
            }
        }
        return true;
    }
};
