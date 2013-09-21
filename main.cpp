#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "mapreduce.h"

#define DEBUG 0

/* key is the filename, value is the contents as a vector */
void map(std::string key, std::string value, MapperWriter &out)
{
    std::vector<std::string> lines = split(value, '\n');

    long nNodes = 0, nEdges = 0;
    std::vector<std::string> graph_info = split(lines[0], ' ');
    nNodes = std::stol(graph_info[0]);
    nEdges = std::stol(graph_info[1]);

#if DEBUG
    std::cout << "nNodes = " << nNodes << "\tnEdges = " << nEdges << std::endl;
#endif

    /* start at 1 since 0 was the graph info */
    std::string v1, v2;
    for (long i = 1; i <= nEdges; i++)
    {
        std::stringstream edge(lines[i]);
        std::getline(edge, v1, ' ');
        std::getline(edge, v2, ' ');

#if DEBUG
        std::cout << "edge ( " << v1 << ", " << v2 << " )" << std::endl;
#endif

        out.write(v1, "1");
        out.write(v2, "1");
    }
}

/* key is the node, value is the list of degrees */
void reduce(std::string key, std::vector<std::string> value, ReducerWriter &out)
{
    long count = 0;

    for(unsigned long i = 0; i < value.size(); i++)
    {
        count += std::stol(value[i]);
    }

#if DEBUG
    std::cout << "deg(" << key << ") = " << count << std::endl;
#endif

    out.write(std::to_string(count));
}

int main(int argc, char *argv[])
{
    // input check
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <number of mappers>"\
                                             " <number of reducers>"\
                                             " <jobname>"\
                                             " <input file>" << std::endl;
        exit(1);
    }

    int nMappers = 0, nReducers = 0;

    nMappers = std::atoi(argv[1]);
    nReducers = std::atoi(argv[2]);
    std::string jobname(argv[3]);
    const char* filename = argv[4];

#if DEBUG
    std::cout << "nMappers = " << nMappers << "\tnReducers = " << nReducers << std::endl;
#endif

    mapper(nReducers, filename, jobname, map);
    reducer(filename, jobname, reduce);
}
