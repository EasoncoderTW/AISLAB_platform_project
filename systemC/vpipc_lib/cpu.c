#include <stdio.h>
#include <stdlib.h>
#include "vpipc_pipe.h"


int main()
{
    struct vp_ipc_module vpm = create_vp_module(MODULE_TYPE_SERVER);
    struct vp_transfer vpt[3];
    struct vp_transfer_data vpt_send, vpt_resp;
    int num;
    char c;
    while(1)
    {   
        vp_wait(&vpm, vpt, 1);
        if(!client_is_connect(vpm)) 
        {
            printf("Waiting...\r");
            continue;
        }
        printf("[R / W] [address] ([data])");
        scanf(" %c %ld %ld", &c,&(vpt_send.addr),&(vpt_send.data));
        vpt_send.status = VP_OK; 
        if(c == 'W')
        {
            vpt_send.type = VP_WRITE;
            
        }
        else
        {
            vpt_send.type = VP_READ;
        }
        printf("send - Type: %s, Status: %ld, Address: 0x%016lx, Data: 0x%016lx\n", VP_Type_str[vpt_send.type], vpt_send.status, vpt_send.addr, vpt_send.data);

        vpt_resp = vp_b_transfer(&vpm, vpt_send);

        printf("recv - Type: %s, Status: %ld, Address: 0x%016lx, Data: 0x%016lx\n", VP_Type_str[vpt_resp.type], vpt_resp.status, vpt_resp.addr, vpt_resp.data);
    }
    cleanup_vp_module(vpm);
    return 0;
}