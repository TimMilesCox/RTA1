
#include <stdio.h>
#include <net/bpf.h>

#include "portal.h"

static struct bpf_program filter;

int main(int argc, char *argv[])
{
   portal(1, "172.29.7.10+172.29.7.11+172.29.7.12:8080", &filter);
}

