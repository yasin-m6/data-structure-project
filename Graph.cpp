#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

class Graph
{
    private:
        int number_of_nodes; // number of vertexes
        vector<string> vertex; // name of vertexes
        vector<int> destination_number; //number of destination of each vertex
        vector<vector<string>> destinations; // name of destinations of each vertex
        vector<vector<double>> distances; // distance of destinations of each vertex
        vector<vector<int>> trafics; // trafic status of destinations for each vertex

        
        int pathes_number; // number of pathes
        
        //this tow lines are for another soloution
        /*vector<vector<string>> pathes;
        vector<double> pathes_distance;*/

        vector<string> best_path;
        double min_distance;
        
        
        void set_data();
        void find_all_pathes(string  , vector<string> , bool* , double &);
        double distance_calculater(string , string);
    public:
    Graph(int);
    ~Graph();
    void print_best_path();
};



int main()
{
    Graph places(16);
    places.print_best_path();
}




Graph::Graph(int number)
{
    number_of_nodes = number;
    pathes_number = 0;

    set_data();    
}

Graph::~Graph()
{
}

void Graph::print_best_path()
{
    vector<string> path;
    bool visited[number_of_nodes];
    double distance = 0;
    for (size_t i = 0; i < number_of_nodes; i++)
    {
        visited[i] = false;
    }

    find_all_pathes("Graph_0" , path , visited , distance);


    cout << "min dis : " << min_distance << endl << "best path : ";
    for (size_t i = 0; i < best_path.size(); i++)
    {
        cout << best_path[i] << " ";
    }
    
}

void Graph::set_data()
{
    ifstream myFile("data.txt" , ios::in);
    if (!myFile)
    {
        cerr << "file does not open !!!!" << endl;
    }
    cout << "file is open" << endl;

    string v_num;
    int dest_num;
    string dest_sub;
    string dis_sub;
    string tra_sub;
    int pos;

    for (size_t i = 0; i < 16; i++)
    {
        myFile >> v_num >> dest_num >> dest_sub >> dis_sub >> tra_sub;
        
        destinations.push_back(vector<string>());
        distances.push_back(vector<double>());
        trafics.push_back(vector<int>());

        for (size_t j = 0; j < dest_num; j++)
        {
            if (j == dest_num-1)
            {

                destinations.at(i).push_back(dest_sub);
                distances.at(i).push_back(stod(dis_sub));
                trafics.at(i).push_back(stoi(tra_sub));
            }
            else
            {
               
                pos = dest_sub.find(',');
                destinations.at(i).push_back(dest_sub.substr(0 , pos));
                dest_sub = dest_sub.substr(pos+1);
            
                pos = dis_sub.find(',');
                distances.at(i).push_back(stod(dis_sub.substr(0 , pos)));
                dis_sub = dis_sub.substr(pos+1);

                pos = tra_sub.find(',');
                trafics.at(i).push_back(stoi(tra_sub.substr(0 , pos)));
                tra_sub = tra_sub.substr(pos+1);
            }
            
            
        }
        vertex.push_back(v_num);
        destination_number.push_back(dest_num);

    }

    myFile.close();
}

void Graph::find_all_pathes(string v_name  , vector<string> path , bool * visited , double & distance)
{
    int v_num = stoi(v_name.substr(v_name.find("_")+1));
    visited[v_num] = true;
    path.push_back(v_name);

    if(v_name != "Graph_15")
    {
        for (size_t i = 0; i < destinations[v_num].size(); i++)
        {
            if (visited[ stoi(destinations[v_num][i].substr(destinations[v_num][i].find("_")+1)) ] == false)
            {
                distance = distance + distance_calculater(v_name , destinations[v_num][i]);
                find_all_pathes(destinations[v_num][i] , path , visited , distance);
                distance = distance - distance_calculater(v_name , destinations[v_num][i]);
            }   
        }
    }
    else
    {
        
        if (distance < min_distance || pathes_number == 0)
        {
            min_distance = distance;
            best_path.clear();
            
            for (size_t i = 0; i < path.size(); i++)
            {
                best_path.push_back(path.at(i));
            }
            pathes_number++;
        }
        
    }
    visited[v_num] = false;
    path.pop_back();
}

double Graph::distance_calculater(string node_1 , string node_2)
{
    int node_number = stoi(node_1.substr(node_1.find("_")+1 ));

    int position_in_destinations = 0;
    for (size_t i = 0; i < destination_number[node_number]; i++)
    {
        if(destinations[node_number][i] == node_2)
        {
            position_in_destinations = i;
            break;
        }
    }
    
    return distances[node_number][position_in_destinations] * static_cast<double>(trafics[node_number][position_in_destinations]);
}