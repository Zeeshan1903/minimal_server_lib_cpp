#include <bits/stdc++.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

using namespace std;

#include "html_builder.h"
#include "html_req_res.h"

/*
    What all it should have is a port and socket addrss and all those mappings of the functions 
    Like which fn should go in which part
    
    This is interesting problem so what i think of right now is how we have in golang as one wrapper would be there
    which would be like kind of callback which will be called onthose path 
    so we can kind of store callback fn for all; those paths like let say a use want to register a path "/about" then just add entry to it and add one callback fn which 
    should be called whenever a user enters the path "/about"
*/
class SimpleServer{
    
}

int main(){

}