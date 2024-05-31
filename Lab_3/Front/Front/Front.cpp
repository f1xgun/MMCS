#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "Enclave3_u.h"
#include "sgx_urts.h"
#include "sgx_tseal.h"
#define ENCLAVE_FILE L"Enclave3.signed.dll"

int main() {
    sgx_enclave_id_t eid;
    sgx_status_t ret = SGX_SUCCESS;
    sgx_launch_token_t token = { 0 };
    int updated = 0;

    ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);
    if (ret != SGX_SUCCESS) {
        printf("App: error %#x, failed to create enclave.\n", ret);
        return -1;
    }


    int req_index = -1;
    char outbuffer[1024] = { 0 };
    printf_s("Input array index: ");
    int result = scanf_s("%d", &req_index);
    if (result != 1 || req_index < 0) {
        printf("Incorrect index\n");
    }
    else {
        getPD(eid, outbuffer, sizeof(outbuffer), req_index);

        if (outbuffer[0] == '\0') {
            printf("Incorrect index\n");
        }
        else {
            printf("PersonalData[%d]: %s\n", req_index, outbuffer);
        }
    }
    std:system("pause");


   if (SGX_SUCCESS != sgx_destroy_enclave(eid))
        return -1;

    return 0;
}