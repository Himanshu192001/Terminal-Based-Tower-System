#include<bits/stdc++.h>
using namespace std;
#define endl "\n"
// structure for distance between towers 
struct Tower_distance
{
	char tower;
	int distance;
	bool chk;
};

class NETWORK
{
	private:
		vector<char> Towers;//saving the tower 
		vector<bool> Tower_chk;//tower is working or down 
		map< char,vector<Tower_distance> > Connection;//connection between towers and their distance
		fstream file;
	public:
		//using constructor for file reader 
		NETWORK()
		{
			char str;
			char key;
			bool chk;
			ifstream in("filename.txt");//object of ifstream
			in.seekg(0);//file pointer to the begining
			if(in.eof()!=0)//if file is empty
			{
				cout<<"\n FILE IS EMPTY \n";
			}
			else
			{
				while(1)//loop for all the tower
				{
					in>>str;
					if(str=='*')//* as marker for connection
					{
						break;
					}
					else
					{
						Towers.push_back(str);
						in>>chk;
						Tower_chk.push_back(chk);
					}	
				}
				while(in.eof()==0)
				{
					
					in>>key;//key of map
					vector<Tower_distance> TD;
					struct Tower_distance temp;//structure variable
					while( in.eof()!=1)//loop untill the file end
					{
						
						in>>str;
						if(str=='*')
						{
							break;
						}
						else
						{
							temp.tower=str;
							in>>temp.distance;
							in>>temp.chk;
							TD.push_back(temp);
						}	
					}
					Connection.insert({key,TD});//inserting in the map	
				}	
			}
		}
		
		//checking the name is letter or not 
		bool isLetter(char ch)
		{
			return ( ch >= 97 && ch <= 122 ) || ( ch >= 65 && ch <= 90 );
		}
		
		int minDistance(int *dist, bool *sptSet)
		{
		    // Initialize min value
		    int min = INT_MAX;
			int min_index;		  
		    for (int v = 0; v < Towers.size(); v++)
		    {
		    	if (sptSet[v] == false && dist[v] <= min)
		        {
		        	min = dist[v];
		        	min_index = v;
				}
			}
		    return min_index;
		}
		//returning the index of the tower or if tower didn't exist return -1
		int isValid(char v);
			
		void addTowers();
		
		void display();
		
		void addConnections();
		
		void delete_Tower();
		
		void towerActivity();
		
		void deleteConnection();
		
		void connectionActivity();
		
		void writingInFile();
		
		void dijikstra();
		
};

int NETWORK::isValid(char tower)
{
	int index=-1;
	for(int i=0;i<Towers.size();i++)
	{
		if(Towers[i]==tower)
		{
			index=i;
			return index;
		}
	}
	return index;
}

void NETWORK::addTowers()
{
	int activity;
	char tower;
	cout<<"\n Enter the tower Name:";
	cin.ignore();
	cin>>tower;
	cout<<"\n ENTER THE ACTIVITY OF TOWER 0 FOR DOWN 1 FOR ACTIVE:";
	cin>>activity;
	
	if(isLetter(tower))
	{
		//lower case to upper case
		if( tower >= 97 && tower <= 122 )
		{
			tower-= 32;
		}
		//checking tower exist or not 
		//if not then pushback in vector
		if(isValid(tower)!=-1)
		{
			cout<<endl<<"THE TOWER IS ALREADY EXISTS"<<endl;
		}
		else
		{
			if(activity)
			{
				Towers.push_back(tower);//list of towers
				Tower_chk.push_back(true);//list of towers working or not
				vector<Tower_distance>TD;//vector of edge and weight
				Connection.insert({tower,TD});//insert in the map key and vector
				cout<<"\n THE TOWER IS ADDED SUCCESFULLY \n";
			}
			else
			{
				Towers.push_back(tower);//list of towers
				Tower_chk.push_back(false);//list of towers working or not
				vector<Tower_distance>TD;//vector of edge and weight
				Connection.insert({tower,TD});//insert in the map key and vector
				cout<<"\n THE TOWER IS ADDED SUCCESFULLY \n";
			}
				
		}
	}
	
}

void NETWORK::display()
{
	cout<<"\n TOWER NETWORK INFORMATION \n";
	if(Towers.empty())
	{
		cout<<"\n NO TOWER EXIST \n";
	}
	for(int i=0;i<Towers.size();i++)
	{
		cout<<"\n TOWER :";
		cout<<"[ "<<Towers[i]<<" ]";
		if(Tower_chk[i])
		{
			cout<<"(*)";
		}
		else
		{
			cout<<"(!)";
		}
		cout<< " \n CONNECTIONS:";
		//making an iterator of map 
		map< char,vector<Tower_distance> >::iterator it=Connection.find(Towers[i]);
	
		if(it!=Connection.end())
		{
			vector<Tower_distance> conn=it->second;
			
			if(conn.size()==0)
			{
				cout<<"{ No Connections ";
			}
			else
			{
				//printing all the towers connected to the tower and their distances 
				cout<<" { ";
				for(int j=0;j<conn.size();j++)
				{
					cout<<conn[j].tower<<"("<<conn[j].distance<<")";
					if(conn[j].chk)
					{
						cout<<"(*), ";
					}
					else
					{
						cout<<"(!), ";
					}
				}
			}
		}
		
		cout<<" } "<<endl;
	}
}

void NETWORK::addConnections()
{
//	cout<<"yes"<<endl;
	char T1,T2;
	int distance,activity;
	cin.ignore();
	cout<<"\n ENTER THE NAME OF FIRST TOWER :";
	cin>>T1;
	cout<<"\n ENTER THE NAME OF THE SECOND TOWER:";
	cin>>T2;
	if(T1==T2)
	{
		cout<<"\n CIRCULAR CONNECTION NOT POSSISBLE \n";
		return;
	}
	
	cout<<"\n ENTER THE DISTANCE BETWEEN THEM :";
	cin>>distance;
	if(distance<0)
	{
		cout<<"\n DISTANCE CANNOT BE NEGATIVE \n";
		return;
	}
	
	cout<<"\n ENTER THE ACTIVITY OF EDGE 0 FOR DOWN 1 FOR ACTIVE:";
	cin>>activity;
	
	if(isLetter(T1) && isLetter(T2))
	{
		int index_T1=isValid(T1);
		int index_T2=isValid(T2);
		if(index_T1!=-1 && index_T2!=-1)
		{
			if(index_T1!=index_T2)
			{
				bool find=false;
				map< char,vector<Tower_distance> >::iterator it=Connection.find(T1);
				if(it!=Connection.end())
				{
					if(!it->second.empty())
					{
						vector<Tower_distance>::iterator it2=it->second.begin();//making iterator of vector
						while(it2!=it->second.end())//loop for check if there has connection between these two tower or not
						{
							if(it2->tower==T2)
							{
								find=true;
								break;
							}
							it2++;
						}
					}
				}
				if(find)
				{
					cout<<"\n THE CONNECTION BETWEEN THESE TWO TOWER ALREADY EXIST \n";
				}
				else
				{
					if(it!=Connection.end())
					{
						if(Tower_chk[index_T1]==false || Tower_chk[index_T2]==false )
						{
							Tower_distance t1={T2,distance,false};
							it->second.push_back(t1);
						}
						else
						{
							if(activity)
							{
								Tower_distance t1={T2,distance,true};
								it->second.push_back(t1);
							}
							else
							{
								Tower_distance t1={T2,distance,false};
								it->second.push_back(t1);
							}
						}
						
					}
					//for Bi-directional we need to add edge in both 
					map< char,vector<Tower_distance> >::iterator it2=Connection.find(T2);
					if(it!=Connection.end())
					{
						if(Tower_chk[index_T1]==false || Tower_chk[index_T2]==false)
						{
							Tower_distance t2={T1,distance,false};
							it2->second.push_back(t2);
						}
						else
						{
							if(activity)
							{
								Tower_distance t2={T1,distance,true};
								it2->second.push_back(t2);
							}
							else
							{	
								Tower_distance t2={T1,distance,false};
								it2->second.push_back(t2);
							}
						}
						cout<<"\n THE CONNECTION IS ADDED IN BETWEEN "<<T1 <<" AND "<<T2<<endl;
					}
					
				}
				}
			
		}
		else
		{
			cout<<"\n There is no Tower Exist \n";
		}
		
	}
	else
	{
		cout<<"\n Tower only represented by Letter \n";
	}
}

void NETWORK::delete_Tower()
{
	char T;
	cin.ignore();
	cout<<"\n ENTER THE TOWER NAME:";
	cin>>T;
	if(isLetter(T))
	{
		int index_T=isValid(T);//TAKING INDEX OF THE TOWER
		if(index_T!=-1)//CONDITION IF WE FIND THE TOWER
		{
			Towers.erase(Towers.begin()+index_T);
			map< char,vector<Tower_distance> >::iterator it=Connection.find(T);
			if(it!=Connection.end())
			{
				if(Connection.erase(T))
				{
					cout<<"\n THE TOWER IS REMOVED FROM THE LIST \n";
				}
				else
				{
					cout<<"\n THE TOWER IS NOT REMOVED FROM THE LIST \n";
				}
				//checking the if towers exist or not 
				if(!Towers.empty())
				{
					for(int i=0;i<Towers.size();i++)//loop for checking all towers
					{
						//finding the connection with the deleting tower checking all tower one by one 
						map< char,vector<Tower_distance> >::iterator it=Connection.find(Towers[i]);
						
						if(it!=Connection.end())
						{
							//making a iterator of vector of structure to find the connection
							vector<Tower_distance>::iterator it2=it->second.begin();
							while(it2!=it->second.end())
							{
								if(it2->tower==T)
								{
									break;
								}
								it2++;
							}
							
							//if connection was found
							if(it2!=it->second.end())
							{
								it->second.erase(it->second.begin()+(it2-it->second.begin()));
							}
						}
					}
				}
			}
		}
		else
		{
			cout<<"\n THERE IS NO TOWER EXIST WITH THIS NAME \n";
		}
		}
		else
		{
			cout<<"\n THERE IS NO TOWER EXIST WITH THIS NAME IN LIST \n";
		}
}

void NETWORK::towerActivity()
{
	int activity;
	char tower;
	cout<<"\n Enter the tower Name:";
	cin.ignore();
	cin>>tower;
	cout<<"\n ENTER THE ACTIVITY OF TOWER 0 FOR DOWN 1 FOR ACTIVE:";
	cin>>activity;
	
	if(isLetter(tower))
	{
		//lower case to upper case
		if( tower >= 97 && tower <= 122 )
		{
			tower-= 32;
		}
		int index_t=isValid(tower);
		if(activity)
		{
			Tower_chk[index_t]=true;
			cout<<"\n TOWER ACTIVITY STATUS HAS BEEN UPDATED \n";
		}
		else
		{
			Tower_chk[index_t]=false;
			cout<<"\n TOWER ACTIVITY STATUS HAS BEEN UPDATED \n";
		}
		
		//finding the connection with the deleting tower checking all tower one by one 
		map< char,vector<Tower_distance> >::iterator it=Connection.find(Towers[index_t]);
		
		if(it!=Connection.end())
		{
			//making a iterator of vector of structure to find the connection
			vector<Tower_distance>::iterator it2=it->second.begin();
			while(it2!=it->second.end())
			{
				
				if(activity)
				{
					it2->chk=true;
				}
				else
				{
					it2->chk=false;
				}
				it2++;
			}
		}
		
		for(int i=0;i<Towers.size();i++)//loop for checking all towers
		{
			//finding the connection with the deleting tower checking all tower one by one 
			map< char,vector<Tower_distance> >::iterator it=Connection.find(Towers[i]);
			
			if(it!=Connection.end())
			{
				//making a iterator of vector of structure to find the connection
				vector<Tower_distance>::iterator it2=it->second.begin();
				while(it2!=it->second.end())
				{
					if(it2->tower==tower)
					{
						if(activity)
						{
							it2->chk=true;
						}
						else
						{
							it2->chk=false;
						}
						
						break;
					}
					it2++;
				}
				
			}
		}
				
	}
}

void NETWORK::deleteConnection()
{
	char T1,T2;
	int activity;
	cin.ignore();
	cout<<"\n ENTER THE NAME OF FIRST TOWER :";
	cin>>T1;
	cout<<"\n ENTER THE NAME OF THE SECOND TOWER:";
	cin>>T2;
	if(T1==T2)
	{
		cout<<"\n CIRCULAR CONNECTION NOT POSSISBLE \n";
		return;
	}
	
	if(isLetter(T1) && isLetter(T2))
	{
		int index_T1=isValid(T1);
		int index_T2=isValid(T2);
		if(index_T1!=-1 && index_T2!=-1)
		{
			if(index_T1!=index_T2)
			{
				map< char,vector<Tower_distance> >::iterator it=Connection.find(T1);
				if(it!=Connection.end())
				{
					if(!it->second.empty())
					{
						vector<Tower_distance>::iterator vec=it->second.begin();//making iterator of vector
						while(vec!=it->second.end())//loop for check if there has connection between these two tower or not
						{
							if(vec->tower==T2)
							{
								it->second.erase(it->second.begin()+(vec-it->second.begin()));
								break;
							}
							vec++;
						}
					}
				}
				
				map< char,vector<Tower_distance> >::iterator it2=Connection.find(T2);
				if(it2!=Connection.end())
				{
					if(!it2->second.empty())
					{
						vector<Tower_distance>::iterator vec2=it2->second.begin();//making iterator of vector
						while(vec2!=it2->second.end())//loop for check if there has connection between these two tower or not
						{
							if(vec2->tower==T1)
							{
								it2->second.erase(it2->second.begin()+(vec2-it2->second.begin()));
								break;
							}
							vec2++;
						}
					}
				}
				cout<<"\n CONNECTION REMOVED SUCCESSFULLY \n";
			}
		}
	}
}

void NETWORK::connectionActivity()
{
	char T1,T2;
	int activity;
	cin.ignore();
	cout<<"\n ENTER THE NAME OF FIRST TOWER :";
	cin>>T1;
	cout<<"\n ENTER THE NAME OF THE SECOND TOWER:";
	cin>>T2;
	if(T1==T2)
	{
		cout<<"\n CIRCULAR CONNECTION NOT POSSISBLE \n";
		return;
	}
	cout<<"\n ENTER THE ACTIVITY OF TOWER 0 FOR DOWN 1 FOR ACTIVE:";
	cin>>activity;
	
	if(isLetter(T1) && isLetter(T2))
	{
		int index_T1=isValid(T1);
		int index_T2=isValid(T2);
		if(index_T1!=-1 && index_T2!=-1)
		{
			if(index_T1!=index_T2)
			{
				map< char,vector<Tower_distance> >::iterator it=Connection.find(T1);
				if(it!=Connection.end())
				{
					if(!it->second.empty())
					{
						vector<Tower_distance>::iterator vec=it->second.begin();//making iterator of vector
						while(vec!=it->second.end())//loop for check if there has connection between these two tower or not
						{
							if(vec->tower==T2)
							{
								if(activity)
								{
									vec->chk=true;
								}
								else
								{
									vec->chk=false;
								}
							}
							vec++;
						}
					}
				}
				
				map< char,vector<Tower_distance> >::iterator it2=Connection.find(T2);
				if(it2!=Connection.end())
				{
					if(!it2->second.empty())
					{
						vector<Tower_distance>::iterator vec2=it2->second.begin();//making iterator of vector
						while(vec2!=it2->second.end())//loop for check if there has connection between these two tower or not
						{
							if(vec2->tower==T1)
							{
								if(activity)
								{
									vec2->chk=true;
								}
								else
								{
									vec2->chk=false;
								}
							}
							vec2++;
						}
					}
				}
				cout<<"\n CONNECTION UPDATED SUCCESSFULLY \n";
			}
		}
	}
}

void NETWORK::writingInFile()
{
	ofstream out("filename.txt");
	for(int i=0;i<Towers.size();i++)//loop for accessing all towers 
	{
		//writing the tower name in file
		out<<Towers[i]<<endl;
		//writing the tower status in file
		out<<Tower_chk[i]<<endl;
	}
	 
	for(int i=0;i<Towers.size();i++)//loop for accesing all towers connection
	{
		//map iterator
		map< char,vector<Tower_distance> >::iterator it=Connection.find(Towers[i]);
		out<<"*"<<endl;//kind of marker for connections 
		//writing key in file
		out<<it->first<<endl; 
		//iterator of vector
		vector<Tower_distance>::iterator vec_it=it->second.begin();
		//loop for accessing all connections 
		while(vec_it!=it->second.end())
		{
			//writing the connection in file
			out<<" "<<vec_it->tower;
			out<<" "<<vec_it->distance;
			out<<" "<<vec_it->chk<<endl;
			//increasing iterator by one for next connection in vector 
			vec_it++;
		}
		
	}
	out<<"*"<<endl;
	out.close();
	cout<<"\n CONTENT SAVED IN FILE \n";
}

void NETWORK::dijikstra()
{
	char v;
	cout<<"\n ENTER THE SOURCE TOWER:";
	cin>>v;
	if( isLetter(v))
	{
		if( v >= 97 && v <= 122 )
		{
			v -= 32;
		}
		int index = isValid(v);
		if( index != -1 )
		{
			// Logic Of Dijikstra 
			bool *sptSet = new bool[Towers.size()]; // Array for Shortest Path is included or not
			int *dist = new int[Towers.size()]; // Array for storing Distance
			// Initialize all distances as INFINITE and stpSet[] as false
    		for (int i = 0; i < Towers.size(); i++)
    		{
    			dist[i] = INT_MAX;
				sptSet[i] = false;
			}
        	dist[index] = 0; // Source Vertex distance of reaching source vertex
  			for( int count = 0; count < Towers.size()-1; count++ )
  			{
  				int u = minDistance(dist, sptSet);
  				sptSet[u] = true;
  				map<char,vector<Tower_distance> >::iterator it = Connection.find(Towers[u]);
  				if( it != Connection.end())
				{
					vector<Tower_distance> edge = it->second;
					if( edge.size() > 0 )
					{
						for( int j = 0; j < edge.size(); j++ )
						{
							index = isValid(edge[j].tower);
							if(!sptSet[index] && dist[u] != INT_MAX && dist[u] + edge[j].distance < dist[index] ) 
							{
								dist[index] = dist[u] + edge[j].distance;
							}
						}
					}
				}
			}
			cout<<"\n tower \t\t Distance from Source\n";
    		for (int i = 0; i < Towers.size(); i++)
    		{
    			cout<<" [ "<<Towers[i]<<" -> ";
    			if(dist[i]==INT_MAX)
    			{
    				cout<<"\t NO PATH"<<endl;
				}
				else
				{
					cout<<"\t"<<dist[i]<<endl;
				}
				
			}        
			cout<<endl;
		}
		else
		{
			cout<<"\n There is no such TOWER present in Graph \n";
		}
	}
	else
	{
		cout<<"\n Please Enter Valid TOWER name, It must be a Letter \n";
	}
}

int main()
{
	int choice;
	NETWORK obj;//object of NETWORK class
	bool run=true;
	while(run)
	{
		cout<<"\n ************************************"<<endl;
		cout<<"\n PRESS 1 FOR ADD TOWER      \n ";
		cout<<"\n PRESS 2 FOR ADD CONNECTION \n ";
		cout<<"\n PRESS 3 FOR DISPLAY TOWERS \n ";
		cout<<"\n PRESS 4 FOR DELETE TOWER   \n ";
		cout<<"\n PRESS 5 FOR UPDATE ACTIVITY STATUS OF TOWER \n";
		cout<<"\n PRESS 6 FOR DELETE CONNECTION BETWEEN TOWERS \n";
		cout<<"\n PRESS 7 FOR UPDATE ACTIVITY STATUS OF CONNECTION \n";
		cout<<"\n PRESS 8 FOR WRITE CONTENT IN FILE \n";
		cout<<"\n PRESS 9 FOR SHORTEST PATH USING DIJIKSTRA \n";
		cout<<"\n PRESS 10 FOR EXIT \n";
		cout<<"\n ENTER YOUR CHOICE :";
		cin>>choice;
		cout<<"\n ************************************"<<endl;
		switch(choice)
		{
			case 1:
				obj.addTowers();
				break;
			case 2:
				obj.addConnections();
				break;
			case 3:
				obj.display();
				break;
			case 4:
				obj.delete_Tower();
				break;
			case 5:
				obj.towerActivity();
				break;
			case 6:
				obj.deleteConnection();
				break;
			case 7:
				obj.connectionActivity();
				break;
			case 8:
				obj.writingInFile();
				break;
			case 9:
				obj.dijikstra();
				break;
			case 10:
				run=false;
				break;
			default:
				cout<<"\n Invalid choice \n";
		}
	}
	return 0;
	
}







