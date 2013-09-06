#ifndef MAPREDUCE_H
#define  MAPREDUCE_H

#include <vector>
#include <tr1/unordered_map>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

class InputType
{
public:
    
};

class EmitType
{
public:
    
};

class ReduceResult
{
public:
    int64_t key;
    EmitType value;
};

class BatchAccessor
{
public:
    virtual bool end() = 0;
    virtual InputType& getNextInput() = 0;
};

class MapReduce
{
    
    std::string m_job_name;
    std::string m_node_name;
protected:
    boost::function<void(int64_t, EmitType*)> emit; 
    
public:
    
    MapReduce (std::string job_name, std::string node_name);
    
    void setEmitF(boost::function<void(int64_t, EmitType*)> emitf);
    virtual void map(InputType &object) = 0;   
    virtual EmitType* reduce(int64_t emit_key, EmitType* a, EmitType* b) = 0;
};

#endif