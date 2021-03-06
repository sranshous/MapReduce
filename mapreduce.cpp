#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include "mapreduce.h"

void mapper(const long numReducers,
            const char* filename,
            std::string jobname,
            mapFunc map)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);

    if (!in)
    {
        std::cerr << "Could not open input file. Exiting." << std::endl;
        exit(1);
    }

    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();

    MapperWriter outwriter;
    std::string fn = std::string(filename);
    map(fn, contents, outwriter);

    /* Write to the intermediate file */
    std::stringstream outfile;
    outfile << jobname << ".intermediate";
    std::ofstream out(outfile.str(), std::ios::out);

    if (!out)
    {
        std::cerr << "Could not open output file. Exiting." << std::endl;
        exit(1);
    }

    out << outwriter.str();
}

void reducer(const char* filename,
             std::string jobname,
             redFunc reduce)
{
    std::stringstream infile;
    infile << jobname << ".intermediate";

    std::ifstream in(infile.str(), std::ios::in | std::ios::binary);

    if (!in)
    {
        std::cerr << "Could not open intermediate input file. Exiting." << std::endl;
        exit(1);
    }

    /* build a map where the key is the node key from the file and the
     * value is a vector which gets each value from the file pushed back */
    std::map<std::string, std::vector<std::string>> kv;
    std::string key, value;

    /*
     * This turns out to be much slower than the while loop approach

    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();

    std::vector<std::string> lines = split(contents, '\n');
    for (std::vector<std::string>::size_type i = 0; i < lines.size(); i++)
    {
        std::stringstream line(lines[i]);
        std::getline(line, key, '\t');
        std::getline(line, value, '\t');

        kv[key].push_back(value);
    }
    */

    while(!in.eof())
    {
        getline(in, key, '\t');
        getline(in, value, '\n');

        if (key != "")
        {
            kv[key].push_back(value);
        }
    }

    /* The final file to write to */
    std::stringstream outfile;
    outfile << jobname << ".final";
    std::ofstream out(outfile.str(), std::ios::out);

    if (!out)
    {
        std::cerr << "Could not open final output file. Exiting." << std::endl;
        exit(1);
    }

    ReducerWriter outwriter;

    /* call reduce for each kv in the map */
    for (std::map<std::string, std::vector<std::string>>::iterator it = kv.begin(); it != kv.end(); ++it)
    {
        /* first is the key, second is the value */
        reduce(it->first, it->second, outwriter);

        out << it->first << "\t" << outwriter.str();

        /* empty the outwriter */
        outwriter.reset();
    }
}

/* Private function to split the string */
static std::vector<std::string> &split(const std::string &s,
                                       char delim,
                                       std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }

    return elems;
}

/* Public function to split a given string according to a certain delimeter.
 * Returns the values from the split in a vector of strings. */
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
