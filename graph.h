#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <queue>
#include <stack>

//Evan Burriola

class Graph {
private:
    struct Vertex{
        size_t id;
        std::list<Vertex*> adj; //contains only verticies that THIS vertex POINTS to (has a directed edge to)

        struct TableInfo{
            bool known;
            double cost;
            int path; //TO allow -1

            TableInfo():known(false),cost(INFINITY),path(-1){}
        };

        TableInfo ti;
        

        Vertex():id(-1){}
    };



    std::unordered_map<size_t, Vertex*> verticies;
    std::unordered_map<size_t, std::unordered_map<size_t,double>> edges;

    void make_empty(){
        for(auto v: verticies){
            delete v.second; //use each vertex id to remove(needed for delete)
        } 
        verticies.clear();
        edges.clear();
    }


public:
    Graph(){}
    Graph(const Graph& other){
        for(auto v : other.verticies){ //for every vertex in the other graph
            add_vertex(v.first); // add a vertex <size_t, Vertex*>
        }
        for(auto src : other.edges){ //for each vertex(id) in edge map
            for(auto dest : src.second){ //for each nested map
                add_edge(src.first,dest.first,dest.second); //add an edge with the src.first vertex(size_t), dest.first (destination vertex), of weight dest.second(double)
            }
        }
    }
    Graph& operator=(const Graph& other){
        if(this != &other){
            make_empty();
            for(auto v : other.verticies){ //for every vertex in the other graph
                add_vertex(v.first); // add a vertex <size_t, Vertex*>
            }
            for(auto src : other.edges){ //for each vertex(id) in edge map
                for(auto dest : src.second){ //for each nested map
                    add_edge(src.first,dest.first,dest.second); //add an edge with the src.first vertex(size_t), dest.first (destination vertex), of weight dest.second(double)
                }
            }
        }
        return *this;
    }
    ~Graph(){
        make_empty();
    } //Dealloc
    size_t vertex_count() const{
        return verticies.size();
    }
    size_t edge_count() const{
        size_t count = 0;
        for(auto itr = edges.begin(); itr != edges.end(); itr++){
            count += itr->second.size(); //count each inner map entry
        }
        return count;
    }

    bool contains_vertex(size_t id) const{
        if(verticies.count(id) == 1) //map function
            return true;
        return false;

    }
    bool contains_edge(size_t src, size_t dest) const{
        if(!contains_vertex(src) || !contains_vertex(dest))
             return false;

        if(edges.count(src) == 1 && edges.at(src).count(dest) == 1)
            return true;
        return false;
        
    }
    double cost(size_t src, size_t dest) const{
        if(!contains_edge(src,dest)) //if the source or dest vertex doesn't exist
             return INFINITY;

        return edges.at(src).at(dest);
        
    }

    bool add_vertex(size_t id){
        if(contains_vertex(id)) //The id should be the next value in the sequence(no dupes)
             return false;
        // size_t newSize = vertex_count() + 1;
        Vertex* v = new Vertex;
        v->id = id;

        verticies[id] = v;
        //edges[id],std::unordered_map<size_t,double>()});
        return true;
    }

    bool add_edge(size_t src, size_t dest, double weight=1){
         if(contains_edge(src,dest) || !contains_vertex(src) || !contains_vertex(dest)) //if the source or dest vertex doesn't exist or the edge already exists
             return false;

        verticies[src]->adj.push_back(verticies[dest]); //push the destination Vertex into the Src Vertex adj list
        edges[src][dest] = weight; //update the edges map
        return true;

    }
    bool remove_vertex(size_t id){
        if(!contains_vertex(id))
             return false;

        for(auto pair : edges){
            if(contains_edge(pair.first,id)){
                edges.at(pair.first).erase(id);
                verticies.at(pair.first)->adj.remove(verticies.at(id));
            }
        }

        delete verticies.at(id);
        verticies.erase(id);
        edges.erase(id);
        

        return true;
    } 

    bool remove_edge(size_t src, size_t dest){
        if(!contains_edge(src,dest))
             return false;

          edges[src].erase(dest); //remove the edge in the edge structure
          verticies[src]->adj.remove(verticies[dest]); //remove the vertex from the adj list of the source
          return true;
    }

    // //USER
    // void print(std::ostream& os = std::cout) const{
    //     for(auto it = edges.begin(); it != edges.end(); it++){ //for every linked list
    //         for(auto edge = it->second.begin(); edge != it->second.end(); edge++){ //for all elems in linked list
    //             os << it->first << " -> {" << edge->second << "} " << edge->first << "   "; //Output: sourceVertex -> {COST} DestinationVertex
    //         }
    //         os << '\n';
    //     }
    // }

    void prim(size_t source_id){
        for(auto itr = verticies.begin(); itr != verticies.end(); itr++){ //Clear list
            itr->second->ti.cost = INFINITY;
            itr->second->ti.known = false;
            itr->second->ti.path = -1;
        } 

        if(!contains_vertex(source_id)) //Make sure target node exists
            return;



        auto cmp = [](Vertex* left, Vertex* right) { return (left->ti.cost) > (right->ti.cost); }; //Compare condition for Vertex objects (ordered by cost)
        std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(cmp)> pq(cmp); //create priority q for Vertex pointers

        verticies.at(source_id)->ti.cost = 0; // initialize target node cost
        pq.push(verticies.at(source_id));

        while(!pq.empty()){ 
            Vertex* cur = pq.top();
            pq.pop();//pointer to reference the Cheapest Unkown Vertex
            cur->ti.known = true;

            //Look through all neighbors (directed edges from THIS vertex to others)
            for(auto n : cur->adj){
                double newWeight = edges.at(cur->id).at(n->id); //+ cur->cost
                //if the edge from cur to the neighbor costs lest than what the neighbor has in its table.
                if(!(n->ti.known) && newWeight < n->ti.cost){
                    n->ti.cost = newWeight; //Assign cost
                    n->ti.path = cur->id; //Assign path
                    pq.push(n);
                }
            }

        }
    
// //LOOP condition?(while known nodes != size of MSTtable):
//         //Cheapest Unkown Vertex (call it V0)
//         //set V0.known = true;
//         //Look to each neighbor of V0
//         //Compare cost listed in MSTtable to cost in edgeCost table
//         //Change cost and path if this edge cost(V0 to target neighbor) is less than MSTtable cost
    }
    bool is_path(size_t id) const{
        if(!contains_vertex(id))
            return false;
        return verticies.at(id)->ti.known; //if the destination vertex is -1, there is no path
    }

    void print_help(size_t dest_id, std::ostream& os=std::cout) const{
        if(!contains_vertex(dest_id) || !is_path(dest_id)){
            os << "<no path>";
            return;
        }
        
        //Find source
        int source = -1;
        for(auto itr = verticies.begin(); itr != verticies.end(); itr++){
            if(itr->second->ti.path == -1)
                source = itr->second->id;
        }

        if(source == -1){
            os << "<no path>";
            return;
        }

        std::stack<size_t> path; //stack to get correct ordered output


        int vert = dest_id;
        //out.push_back(source);
        while(vert != -1){
            //out.push_back(vert);
            path.push(vert);
            vert = verticies.at(vert)->ti.path;
        }
        while(path.size() != 1){
            os << path.top() << " --> ";
            path.pop();
        }
        os << path.top();
    }



    void print_path(size_t dest_id, std::ostream& os=std::cout) const{
        print_help(dest_id,os);
        os << '\n';
    }

    //USER (assumming prim is ran)
    // void printMST(std::ostream& os = std::cout) const{
    //     std::cout << "Vertex|Known|Cost|Path\n"; 
    //     for(auto itr = verticies.begin(); itr != verticies.end(); itr++){
    //         os << itr->second->id << "|" << itr->second->ti.known << "|" << itr->second->ti.cost << "|" << itr->second->ti.path << '\n';
    //     }
    // }


    void dijkstra(size_t source_id){
        for(auto itr = verticies.begin(); itr != verticies.end(); itr++){
            itr->second->ti.cost = INFINITY;
            itr->second->ti.known = false;
            itr->second->ti.path = -1;
        } 

        if(!contains_vertex(source_id)) //Make sure target node exists
            return;

        auto cmp = [](Vertex* left, Vertex* right) { return (left->ti.cost) > (right->ti.cost); };
        std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(cmp)> pq(cmp);

        verticies.at(source_id)->ti.cost = 0; // initialize target node cost
        pq.push(verticies.at(source_id));

        while(!pq.empty()){ 
            Vertex* cur = pq.top();
            pq.pop();//pointer to reference the Cheapest Unkown Vertex
            cur->ti.known = true;

            //Look through all neighbors (directed edges from THIS vertex to others)
            for(auto n : cur->adj){
                double newWeight = edges.at(cur->id).at(n->id) + cur->ti.cost; //+ cur->cost
                //if the edge from cur to the neighbor costs lest than what the neighbor has in its table.
                if(!(n->ti.known) && newWeight < n->ti.cost){
                    n->ti.cost = newWeight; //Assign cost
                    n->ti.path = cur->id; //Assign path
                    pq.push(n);
                }
            }

        }
    }
    double distance(size_t id) const{
        if(!is_path(id))
            return INFINITY;
        return verticies.at(id)->ti.cost;
    }
    void print_shortest_path(size_t dest_id, std::ostream& os=std::cout) const{
        if(!contains_vertex(dest_id) || !is_path(dest_id)){
            os << "<no path>\n";
            return;
        }
        print_help(dest_id,os);
        os << " distance: " << distance(dest_id) <<'\n';

    }
};

#endif  // GRAPH_H
