#include "btstack.h"
 
int btstack_main(int argc, const char * argv[]){
 
    l2cap_init();
    sdp_init();
 
    gap_discoverable_control(1);
    gap_set_local_name("Hello world");
 
    hci_power_control(HCI_POWER_ON);
 
    return 0;
}
