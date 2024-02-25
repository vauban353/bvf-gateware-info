// SPDX-License-Identifier: MIT
/*
 * BeagleV-Fire gateware information runtime retrieval.
 *
 * This is based on the following PolarFire SoC Linux example:
 * https://github.com/polarfire-soc/polarfire-soc-linux-examples/blob/master/system-services/system-services-example.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFSIZE 4096
#define DESIGN_NAME_LENGTH  31u

#define DESIGN_INFO_COMMAND 0x2u
#define DESIGN_INFO_MSG_SIZE 0u
#define DESIGN_INFO_RESP_SIZE 36u
#define DESIGN_INFO_OFFSET 0u
#define DESIGN_INFO_DEVICE "/dev/mpfs_generic_service"

#define SUCCESS                         0
#define SYS_SERVICE_FAILURE             -1
#define SYS_SERVICE_FILE_ACCESS_ERROR   -2
#define SYS_SERVICE_FILE_NOT_AVAILABLE  -3

static int get_design_info(unsigned char * design_info, char * design_name, unsigned int * design_version) {
    unsigned char inbuff[BUFFSIZE];
    unsigned int command, msg_size, resp_size, send_offset, response_offset;
    FILE *fptr;
    char chr;
    int i;
    int ret_val = SUCCESS;

    if (access(DESIGN_INFO_DEVICE, F_OK))
    {
        return SYS_SERVICE_FILE_NOT_AVAILABLE;
    }
    else if ((fptr = fopen(DESIGN_INFO_DEVICE, "w")) == NULL)
    {
        return SYS_SERVICE_FILE_ACCESS_ERROR;
    }
    command = DESIGN_INFO_COMMAND;
    msg_size = DESIGN_INFO_MSG_SIZE;
    resp_size = DESIGN_INFO_RESP_SIZE;
    send_offset = DESIGN_INFO_OFFSET;
    response_offset = send_offset;
    fprintf(fptr, "%c%c%c%c%c%c%c%c%c",
            command,
            (char)msg_size, (char)(msg_size >> 8),
            (char)resp_size, (char)(resp_size >> 8),
            (char)send_offset, (char)(send_offset >> 8),
            (char)response_offset, (char)(response_offset >> 8));
    fclose(fptr);
    if ((fptr = fopen(DESIGN_INFO_DEVICE, "r")) == NULL)
    {
        return SYS_SERVICE_FILE_ACCESS_ERROR;
    }
    fscanf(fptr, "%37s", design_info);
    if (*design_info){
        ret_val = SYS_SERVICE_FAILURE;
    }
    else {
    }
    fclose(fptr);

    memcpy(design_name, &design_info[3], DESIGN_NAME_LENGTH);
    design_name[30] = 0u;
    *design_version = (unsigned int)design_info[33] + (((unsigned int )design_info[34]) << 8u);

    return ret_val;
}

int main()
{
    unsigned char design_info[BUFFSIZE];
    char design_name[DESIGN_NAME_LENGTH];
    unsigned int design_version;
    int ret;

    ret = get_design_info(design_info, design_name, &design_version);
    if(ret == SUCCESS)
    {
        printf("Gateware Design Information: \n");
        printf("  Design name: %s\r\n", design_name);
        printf("  Design version: \r\n");
        printf("       raw: 0x%x\r\n", design_version);
        printf("       interpreted as unique-design-version: %02d.%02d.%d\r\n",
                            design_version / 1000u, 
                            (design_version / 10u) % 100u , 
                            design_version % 10u);
        printf("       interpreted as date and time: \r\n         day: %d \r\n         time: %d:%02d\r\n",
                            design_version / 1440u,
                            (design_version / 60u) %24u ,
                            design_version % 60u);
    }
    else {
        if (ret == SYS_SERVICE_FILE_ACCESS_ERROR) printf("Please use this utility as root.\n");
    }

    return ret;
}