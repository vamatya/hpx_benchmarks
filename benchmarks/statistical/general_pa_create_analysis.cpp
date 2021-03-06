//  Copyright (c)      2012 Daniel Kogler
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "general_declarations.hpp"
#include "statstd.hpp"

///////////////////////////////////////////////////////////////////////////////
//The first test creates the packaged_actions
template <typename Vector, typename Package>
void create_packages(Vector& packages, uint64_t num, double ot){
    uint64_t i = 0;
    double mean;
    string message = "Measuring time required to create packaged_actions:";
    vector<double> time;
    packages.reserve(num);

    //the first measurement is of the amount of time it takes to generate
    //the specified num of packaged_actions
    high_resolution_timer t;
    for(; i < num; ++i)
        packages.push_back(new Package());
    mean = t.elapsed()/num;
    packages.clear();
    time.reserve(num);
    packages.reserve(num);
    for(i = 0; i < num; ++i){
        high_resolution_timer t1;
        packages.push_back(new Package());
        time.push_back(t1.elapsed());
    }
    printout(time, ot, mean, message);
}

///////////////////////////////////////////////////////////////////////////////

//this runs a series of tests for a plain_result_action
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t);
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t, T);
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t, T, T);
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t, T, T, T);
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t, T, T, T, T);

//this runs a series of tests for a plain_action
template <typename Vector, typename Package, typename Action>
void run_void(uint64_t);
template <typename Vector, typename Package, typename Action, typename T>
void run_void(uint64_t, T);
template <typename Vector, typename Package, typename Action, typename T>
void run_void(uint64_t, T, T);
template <typename Vector, typename Package, typename Action, typename T>
void run_void(uint64_t, T, T, T);
template <typename Vector, typename Package, typename Action, typename T>
void run_void(uint64_t, T, T, T, T);

///////////////////////////////////////////////////////////////////////////////

//decide which action type to use
void decide_action_type(bool rtype, uint64_t num, int type, int argc);

//parse the argument types
void parse_arg(string atype, bool rtype, uint64_t num, int argc){
    int type;
    if(atype.compare("int") == 0) type = 0;
    else if(atype.compare("long") == 0) type = 1;
    else if(atype.compare("float") == 0) type = 2;
    else if(atype.compare("double") == 0) type = 3;
    else{
        std::cerr<<"Error parsing input, see help entry for a list of ";
        std::cerr<<"available types\n";
        return;
    }
    decide_action_type(rtype, num, type, argc);
}

///////////////////////////////////////////////////////////////////////////////
int hpx_main(variables_map& vm){
    uint64_t num = vm["number-spawned"].as<uint64_t>();
    bool rtype = (vm.count("result-action") ? true : false);
    string atype = vm["arg-type"].as<string>();
    int c = vm["argc"].as<int>();
    csv = (vm.count("csv") ? true : false);
    parse_arg(atype, rtype, num, c);
    return hpx::finalize();
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){
    // Configure application-specific options.
    options_description
        desc_commandline("usage: " HPX_APPLICATION_STRING " [options]");

    desc_commandline.add_options()
        ("number-spawned,N",
            boost::program_options::value<uint64_t>()
                ->default_value(500000),
            "number of packaged_actions created and run")
        ("result-action,R",
            "specifies that we are also to use plain_result_actions instead of "
            "the default plain_actions.  Additional tests will be performed "
            "regarding futures and return values if this option is specified")
        ("arg-type,A",
            boost::program_options::value<string>()
                ->default_value("int"),
            "specifies the argument type of the action, as well as the result "
            "type if a plain_result_action is used, to be either int, long, "
            "float, or double. This argument has no effect if the number of "
            "arguments specified is 0")
        ("argc,C",
            boost::program_options::value<int>()
                ->default_value(1),
            "the number of arguments each action takes")
        ("csv",
            "output results as csv "
            "(format:count,mean,accurate mean,variance,min,max)");

    // Initialize and run HPX
    outname = argv[0];
    return hpx::init(desc_commandline, argc, argv);
}

///////////////////////////////////////////////////////////////////////////////
//Below is purely tedium

//decide which action type to use
void decide_action_type(bool rtype, uint64_t num, int type, int argc){
    if(rtype){
        if(type == 0){
            switch(argc){
                case 0: run_empty<vector<empty_packagei0*>, empty_packagei0, 
                    empty_actioni0, int> (num); break;
                case 1: run_empty<vector<empty_packagei1*>, empty_packagei1, 
                    empty_actioni1, int> (num, ivar); break;
                case 2: run_empty<vector<empty_packagei2*>, empty_packagei2, 
                    empty_actioni2, int> (num, ivar, ivar); break;
                case 3: run_empty<vector<empty_packagei3*>, empty_packagei3, 
                    empty_actioni3, int> (num, ivar, ivar, ivar); break;
                default: run_empty<vector<empty_packagei4*>, empty_packagei4, 
                    empty_actioni4, int> (num, ivar, ivar, ivar, ivar); break;
            }
        }
        else if(type == 1){
            switch(argc){
                case 0: run_empty<vector<empty_packagel0*>, empty_packagel0, 
                    empty_actionl0, long> (num); break;
                case 1: run_empty<vector<empty_packagel1*>, empty_packagel1, 
                    empty_actionl1, long> (num, lvar); break;
                case 2: run_empty<vector<empty_packagel2*>, empty_packagel2, 
                    empty_actionl2, long> (num, lvar, lvar); break;
                case 3: run_empty<vector<empty_packagel3*>, empty_packagel3, 
                    empty_actionl3, long> (num, lvar, lvar, lvar); break;
                default: run_empty<vector<empty_packagel4*>, empty_packagel4, 
                    empty_actionl4, long> (num, lvar, lvar, lvar, lvar); break;
            }
        }
        else if(type == 2){
            switch(argc){
                case 0: run_empty<vector<empty_packagef0*>, empty_packagef0, 
                    empty_actionf0, float> (num); break;
                case 1: run_empty<vector<empty_packagef1*>, empty_packagef1, 
                    empty_actionf1, float> (num, fvar); break;
                case 2: run_empty<vector<empty_packagef2*>, empty_packagef2, 
                    empty_actionf2, float> (num, fvar, fvar); break;
                case 3: run_empty<vector<empty_packagef3*>, empty_packagef3, 
                    empty_actionf3, float> (num, fvar, fvar, fvar); break;
                default: run_empty<vector<empty_packagef4*>, empty_packagef4, 
                    empty_actionf4, float> (num, fvar, fvar, fvar, fvar); break;
            }
        }
        else{
            switch(argc){
                case 0: run_empty<vector<empty_packaged0*>, empty_packaged0, 
                    empty_actiond0, double> (num); break;
                case 1: run_empty<vector<empty_packaged1*>, empty_packaged1, 
                    empty_actiond1, double> (num, dvar); break;
                case 2: run_empty<vector<empty_packaged2*>, empty_packaged2, 
                    empty_actiond2, double> (num, dvar, dvar); break;
                case 3: run_empty<vector<empty_packaged3*>, empty_packaged3, 
                    empty_actiond3, double> (num, dvar, dvar, dvar); break;
                default: run_empty<vector<empty_packaged4*>, empty_packaged4, 
                    empty_actiond4, double> (num, dvar, dvar, dvar, dvar); break;
            }
        }
    }
    else{
        if(type == 0){
            switch(argc){
                case 0: run_void<vector<void_package0*>, void_package0,
                    void_action0> (num); break;
                case 1: run_void<vector<void_packagei1*>, void_packagei1, 
                    void_actioni1, int> (num, ivar); break;
                case 2: run_void<vector<void_packagei2*>, void_packagei2, 
                    void_actioni2, int> (num, ivar, ivar); break;
                case 3: run_void<vector<void_packagei3*>, void_packagei3, 
                    void_actioni3, int> (num, ivar, ivar, ivar); break;
                default: run_void<vector<void_packagei4*>, void_packagei4, 
                    void_actioni4, int> (num, ivar, ivar, ivar, ivar); break;
            }
        }
        else if(type == 1){
            switch(argc){
                case 0: run_void<vector<void_package0*>, void_package0,
                    void_action0> (num); break;
                case 1: run_void<vector<void_packagel1*>, void_packagel1, 
                    void_actionl1, long> (num, lvar); break;
                case 2: run_void<vector<void_packagel2*>, void_packagel2, 
                    void_actionl2, long> (num, lvar, lvar); break;
                case 3: run_void<vector<void_packagel3*>, void_packagel3, 
                    void_actionl3, long> (num, lvar, lvar, lvar); break;
                default: run_void<vector<void_packagel4*>, void_packagel4, 
                    void_actionl4, long> (num, lvar, lvar, lvar, lvar); break;
            }
        }
        else if(type == 2){
            switch(argc){
                case 0: run_void<vector<void_package0*>, void_package0,
                    void_action0> (num); break;
                case 1: run_void<vector<void_packagef1*>, void_packagef1, 
                    void_actionf1, float> (num, fvar); break;
                case 2: run_void<vector<void_packagef2*>, void_packagef2, 
                    void_actionf2, float> (num, fvar, fvar); break;
                case 3: run_void<vector<void_packagef3*>, void_packagef3, 
                    void_actionf3, float> (num, fvar, fvar, fvar); break;
                default: run_void<vector<void_packagef4*>, void_packagef4, 
                    void_actionf4, float> (num, fvar, fvar, fvar, fvar); break;
            }
        }
        else{
            switch(argc){
                case 0: run_void<vector<void_package0*>, void_package0,
                    void_action0> (num); break;
                case 1: run_void<vector<void_packaged1*>, void_packaged1, 
                    void_actiond1, double> (num, dvar); break;
                case 2: run_void<vector<void_packaged2*>, void_packaged2, 
                    void_actiond2, double> (num, dvar, dvar); break;
                case 3: run_void<vector<void_packaged3*>, void_packaged3, 
                    void_actiond3, double> (num, dvar, dvar, dvar); break;
                default: run_void<vector<void_packaged4*>, void_packaged4, 
                    void_actiond4, double> (num, dvar, dvar, dvar, dvar); break;
            }
        }
    }
}

//this runs a series of tests for a plain_result_action
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t num){
    double ot = timer_overhead(num);
    Vector packages;

    //first test
    create_packages<Vector, Package>(packages, num, ot);
}
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t num, T a1){
    double ot = timer_overhead(num);
    Vector packages;
    create_packages<Vector, Package>(packages, num, ot);
}
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t num, T a1, T a2){
    double ot = timer_overhead(num);
    Vector packages;
    create_packages<Vector, Package>(packages, num, ot);
}
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t num, T a1, T a2, T a3){
    double ot = timer_overhead(num);
    Vector packages;
    create_packages<Vector, Package>(packages, num, ot);
}
template <typename Vector, typename Package, typename Action, typename T>
void run_empty(uint64_t num, T a1, T a2, T a3, T a4){
    double ot = timer_overhead(num);
    Vector packages;
    create_packages<Vector, Package>(packages, num, ot);
}

//this runs a series of tests for a plain_action
template <typename Vector, typename Package, typename Action>
void run_void(uint64_t num){
    double ot = timer_overhead(num);
    Vector packages;

    //first test
    create_packages<Vector, Package>(packages, num, ot);
}
template <typename Vector, typename Package, typename Action, typename T>
void run_void(uint64_t num, T a1){
    double ot = timer_overhead(num);
    Vector packages;
    create_packages<Vector, Package>(packages, num, ot);
}
template <typename Vector, typename Package, typename Action, typename T>
void run_void(uint64_t num, T a1, T a2){
    double ot = timer_overhead(num);
    Vector packages;
    create_packages<Vector, Package>(packages, num, ot);
}
template <typename Vector, typename Package, typename Action, typename T>
void run_void(uint64_t num, T a1, T a2, T a3){
    double ot = timer_overhead(num);
    Vector packages;
    create_packages<Vector, Package>(packages, num, ot);
}
template <typename Vector, typename Package, typename Action, typename T>
void run_void(uint64_t num, T a1, T a2, T a3, T a4){
    double ot = timer_overhead(num);
    Vector packages;
    create_packages<Vector, Package>(packages, num, ot);
}
