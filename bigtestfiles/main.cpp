#include <fstream>


int		main()
{
	std::ofstream fs;
	
	const std::string fname = "bigtest";
	const std::string bigdata = "Lorem ipsum dolor sit amet, ex suas nonumy sanctus his. Eum cetero option euismod cu, pri hinc honestatis disputationi an. Stet legimus per ne, cu modus dolor recusabo qui. Vis nulla nostrum partiendo id, eam ea detracto quaerendum, liber altera numquam sed ei. Amet illum assum est ad, labores mediocritatem ius te."
"Ne quo copiosae salutandi maluisset, impetus appetere splendide sed ad. Aperiri adversarium id vim, viderer propriae persequeris vel an. Pro no scripta debitis, id probo mutat mel. Duo causae audiam placerat no. Noster delenit at pro. Reque ponderum no qui. At appareat aliquando pro, id sit impedit deserunt reprehendunt."
"Pro meliore apeirian intellegebat no, no per gloriatur instructior. Ei copiosae mandamus qui. At velit novum aliquam eum, harum apeirian inimicus has an. Vitae decore ut cum, vim in facete tibique, phaedrum consectetuer ne nec. Singulis iracundia at pri, tollit decore percipit an mei. Ea eum omnis delicata, elit conceptam disputando sit id."
"Tollit ancillae has et. Ea pro idque forensibus elaboraret, est ex sint quodsi interesset. Ne nam feugait propriae, ius ut alii aperiri. Cu minimum repudiare persecuti vel. Ne legere copiosae nam, forensibus interesset quo at."
"Habeo summo necessitatibus ne his, ex eum minim aeque noluisse. Congue decore convenire per ut, ad eos illum commune, ei putent timeam audire duo. At graece oblique per. Soluta malorum inimicus ad eos, amet integre albucius ne vel, ex pro utroque recteque maiestatis. Eos ad fuisset suscipit adversarium.";
	std::string tmpname, istr;
	for (int i = 0; i < 100; i++)
	{
		istr = std::to_string(i);
		tmpname = fname + istr;
		fs.open(tmpname.c_str(), std::fstream::out);
		fs << "bigdata" << istr << bigdata << ":" << "value" << istr << bigdata;
		fs.close();
	}
	return (0);
}
