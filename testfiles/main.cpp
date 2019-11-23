#include <fstream>


int		main()
{
	std::ofstream fs;
	
	const std::string fname = "test";
	std::string tmpname, istr;
	for (int i = 0; i < 100; i++)
	{
		istr = std::to_string(i);
		tmpname = fname + istr;
		fs.open(tmpname.c_str(), std::fstream::out);
		fs << "key" << istr << ":" << "value" << istr;
		fs.close();
	}
	return (0);
}
