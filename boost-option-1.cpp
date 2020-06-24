#include <boost/program_options.hpp>
#include <iostream>
#include <exception>
#include <string>
#include <boost/version.hpp>

using namespace boost::program_options;

int main(int argc, char** argv) {
    try{
        boost::program_options::options_description desc("Generic options");
        int oranges_var;
        desc.add_options()
            ("apples", boost::program_options::value<int>(), "how many aplles do you have")
            ("bananas", boost::program_options::value<int>()->default_value(10), "how many aplles do you have")
            ("oranges,o", boost::program_options::value<int>(&oranges_var)->required(), "how many oranges do you have")
            ("name", boost::program_options::value<std::string>(), "your name")
            ("help", "produce help message")
        ;

        
        boost::program_options::variables_map vm;
        auto parsecmd = boost::program_options::parse_command_line(argc, argv, desc);
        
        boost::program_options::store(std::ref(parsecmd), std::ref(vm));
        boost::program_options::notify(vm);
        if(vm.count("help")){
            std::cout << desc << "\n";
            return 1;
        }
        std::cout << "Fruits count : " 
            << vm["apples"].as<int>() + vm["oranges"].as<int>()
            << std::endl;
    }
    catch(std::exception & e){
        std::cout << "exception occur.. " << std::endl;
        std::cerr << e.what();
    }   


    return 0;

}
