#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include "aws_sigv4.h"

static inline int empty_str(aws_sigv4_str_t str)
{
    return (str.data == NULL || str.len == 0) ? 1 : 0;
}

int get_credential_scope(aws_sigv4_params_t* sigv4_params, aws_sigv4_str_t* credential_scope)
{
    int rc = AWS_SIGV4_OK;
    if (credential_scope == NULL
        || credential_scope->data == NULL
        || sigv4_params == NULL
        || empty_str(sigv4_params->x_amz_date)
        || empty_str(sigv4_params->region)
        || empty_str(sigv4_params->service))
    {
        rc = AWS_SIGV4_INVALID_INPUT_ERROR;
        goto finished;
    }
    char* str = credential_scope->data;
    /* get date in yyyymmdd format */
    strncpy(str, sigv4_params->x_amz_date.data, 8);
    str += 8;
    *str = '/';
    str++;

    strncpy(str, sigv4_params->region.data, sigv4_params->region.len);
    str += sigv4_params->region.len;
    *str = '/';
    str++;

    strncpy(str, sigv4_params->service.data, sigv4_params->service.len);
    str += sigv4_params->service.len;
    *str = '/';
    str++;

    strncpy(str, "aws4_request", 12);
    str += 12;

    credential_scope->len = str - credential_scope->data;
finished:
    return rc;
}

int aws_sigv4_sign(aws_sigv4_params_t* sigv4_params, aws_sigv4_str_t* auth_header)
{
    if (auth_header == NULL)
    {
        goto err;
    }

    /* TODO: Support custom memory allocator */
    auth_header->data = malloc(AWS_SIGV4_AUTH_HEADER_MAX_LEN);
    if (auth_header->data == NULL)
    {
        goto err;
    }
    int len = 0;
    return len;
err:
    /* deallocate memory in case of failure */
    if (auth_header && auth_header->data)
    {
        free(auth_header->data);
        auth_header->data = NULL;
    }
    return 0;
}

int test()
{
    return 0;
}
