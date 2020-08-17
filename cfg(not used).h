，附带cfg
// class CFG_node
// {
// public:
// 	map<int, StatementAST*> statements;
// 	vector<int> linenums;
// 	int next_node_linenum = -1;
// 	void push(int linenum, StatementAST* statement)
// 	{
// 		statements[linenum] = statement;
// 		linenums.push_back(linenum);
// 	}
// 	CFG_node(){};
// };
// extern map<int, CFG_node*> mycfg;

// void generate_cfg_goto()
// 	{
// 		CFG_node *mycfg_node = new CFG_node();
// 		int cfg_node_start_linenum = it->first;
// 		while(1)
// 		{
// 			if(it == statements.end())
// 				break;
// 			else if(it->second->new_cfg_node_sign != 0)
// 				break;
// 			mycfg_node->push(it->first, it->second);
// 			cout << "cfg_node_push: line" << it->first << endl;
// 			it++;
// 			continue;
// 		}
// 		mycfg[cfg_node_start_linenum] = mycfg_node;
// 		cout << "cfg_node which starts from line " << cfg_node_start_linenum << " ends" << endl;
// 	}
// 	void generate_cfg()
// 	{
// 		it = statements.begin();
// 		while(1)
// 		{
// 			if(it == statements.end())
// 				break;
// 			else if(!it->second->new_cfg_node_sign)
// 			{
// 				generate_cfg_normal();
// 				continue;
// 			}
// 			else if(it->second->new_cfg_node_sign == sign_goto)
// 			{
// 				generate_cfg_goto();
// 				continue;
// 			}
// void generate_cfg()
	// {
	// 	auto it = statements.begin();
	// 	while(1)
	// 	{
	// 		if(it == statements.end())
	// 			break;
	// 		CFG_node *mycfg_node = new CFG_node();
	// 		int cfg_node_start_linenum = it->first;
	// 		while(1)
	// 		{
	// 			if(it == statements.end())
	// 			{
	// 				cout << "cfg_node which starts from line " << cfg_node_start_linenum << " ends" << endl;
	// 				mycfg[cfg_node_start_linenum] = mycfg_node;
	// 				break;
	// 			}
	// 			if(it->second->new_cfg_node_sign == sign_empty)
	// 			{
	// 				mycfg_node->push(it->first, it->second);
	// 				cout << "cfg_node_push: line" << it->first << endl;
	// 				it++;
	// 			}
	// 			else if(it->second->new_cfg_node_sign == sign_goto)
	// 			{
	// 				mycfg_node->push(it->first, it->second);
	// 				cout << "cfg_node_push: line" << it->first << "   ***goto, next node linenum is ";
	// 				mycfg_node->next_node_linenum = dynamic_cast<GotoStatementAST*>(it->second)->goto_linenum;
	// 				cout << mycfg_node->next_node_linenum << endl;
	// 				it++;
	// 				cout << "cfg_node which starts from line " << cfg_node_start_linenum << " ends" << endl;
	// 				mycfg[cfg_node_start_linenum] = mycfg_node;
	// 				break;
	// 			}
	// 			else if(it->second->new_cfg_node_sign == sign_if)
	// 			{
	// 				mycfg_node->push(it->first, it->second);
	// 				cout << "cfg_node_push: line" << it->first << "   ***if  , next node linenum is ";
	// 				mycfg_node->next_node_linenum = dynamic_cast<IfStatementAST*>(it->second)->goto_value();
	// 				cout << mycfg_node->next_node_linenum << endl;
	// 				cout << "cfg_node which starts from line " << cfg_node_start_linenum << " ends" << endl;
	// 				mycfg[cfg_node_start_linenum] = mycfg_node;
	// 				CFG_node *new_mycfg_node = new CFG_node();
	// 				int new_cfg_node_start_linenum = (++it)->first;
	// 				it--;
	// 				while(1)
	// 				{
	// 					it++;
	// 					if(it->first >= mycfg_node->next_node_linenum)
	// 					{
	// 						mycfg[new_cfg_node_start_linenum] = new_mycfg_node;
	// 						cout << "cfg_node which starts from line " << new_cfg_node_start_linenum << " ends" << endl;
	// 						cfg_node_start_linenum = it->first;
	// 						break;
	// 					}
	// 					new_mycfg_node->push(it->first, it->second);
	// 					cout << "cfg_node_push: line" << it->first << endl;
	// 				}
	// 			}
	// 		}
	// 	}
	// }