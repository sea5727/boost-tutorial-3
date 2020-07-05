#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define CONFIG_PATH "/home/jdin/workspace/boost-tutorial-3/config.json"

bool Read(const char *fileName)
{
	using boost::property_tree::ptree;

	try
	{
		ptree props;
		boost::property_tree::read_json(fileName, props);

		std::cout << "attribute1"  << props.get<int>("attribute1", 100) << std::endl;
		std::cout << "m_attribute2 " << props.get<std::string>("attribute2", "0 0 0 0") << std::endl;
		std::cout << "m_attribute3 " << props.get<std::string>("attribute3", "0 0 0 0") << std::endl;
		std::cout << "m_attribute4 " << props.get<int>("attribute4", 0) << std::endl;
		std::cout << "m_attribute5 " << props.get<int>("attribute5", 5) << std::endl;

		auto &children = props.get_child("attribute6");
		for (auto &vt : children)
		{
			std::cout << "subAttr1 " << vt.second.get<std::string>("sub-attr1", "") << std::endl;
			std::cout << "subAttr2 " << vt.second.get<std::string>("sub-attr2", "") << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::string err = e.what();
        std::cerr << err << std::endl;
		return false;
	}
	return true;
}


int main(int argc, char **argv)
{
    Read(CONFIG_PATH);
}
