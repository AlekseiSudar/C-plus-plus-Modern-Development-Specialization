
#include "manager.h"


int main(){
    Manager manager;
    
    manager.UpdateRequests();
    manager.ReadRequests();
    manager.ProcessRequests();
    
    return 0;
}