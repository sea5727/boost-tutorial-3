// #include <iostream>
// #include <boost/property_tree/ptree.hpp>
// #include <boost/property_tree/ini_parser.hpp>



// int main(int argc, char** argv) {


//     boost::property_tree::ptree pt;
//     boost::property_tree::ini_parser::read_ini(CONFIG_PATH, pt);

//     // std::cout << pt.get<std::string>("CMN.WAIT_SLEEP_MS") << std::endl;

//     std::cout << "hello world" << std::endl;

//     return 0;

// }


#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#define CONFIG_PATH "/home/jdin/workspace/boost-tutorial-3/config.ini"

int main()
{
     namespace pt = boost::property_tree;
     pt::ptree propTree;

     read_ini(CONFIG_PATH, propTree);



// [CMN]
// WAIT_SLEEP_MS = 500
// HB_INTERVAL = 10

// [CMN.HAHA]
// WAIT_SLEEP_MS = 500


// [CMN.HOHO]
// WAIT_SLEEP_MS = 500

     boost::optional<uint32_t> someProp1 = propTree.get_optional<uint32_t>( pt::ptree::path_type( "my.section.subsection1/someProp1", '/'));
     boost::optional<uint32_t> anotherProp = propTree.get_optional<uint32_t>( pt::ptree::path_type( "my.section.subsection2/anotherProp", '/'));
     boost::optional<std::string> test = propTree.get_optional<std::string>( pt::ptree::path_type( "CMN/WAIT_SLEEP_MS", '/'));

     boost::optional<std::string> test2 = propTree.get_optional<std::string>( pt::ptree::path_type( "CMN/HB_INTERVAL", '/'));
     boost::optional<std::string> test3 = propTree.get_optional<std::string>( pt::ptree::path_type( "CMN.HAHA/WAIT_SLEEP_MS", '/'));
     boost::optional<std::string> test4 = propTree.get_optional<std::string>( pt::ptree::path_type( "CMN.HOHO/WAIT_SLEEP_MS", '/'));

    try
    {
        std::cout << propTree.get<std::string>("CMN.HAHA.WAIT_SLEEP_MS") << std::endl;    
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
     std::cout << "someProp1 = " << *someProp1 << std::endl
             << "\nanotherProp = " << *anotherProp << std::endl
             << *test << std::endl
             << *test2 << std::endl
             << *test3 << std::endl
             << *test4 << std::endl
             << std::endl;
}

