
#include "manager.h"


int main(){
    Manager manager;
    
    manager.UpdateRequests();
    manager.ProcessUpdateRequests();
    manager.ReadRequests();
    manager.ProcessReadRequests();
    
    return 0;
}