#pragma once

ulong kitten_memtest_single(vu_long *buf, ulong start_addr, ulong end_addr, ulong seed, int soak);
int kitten_memtest(int megabytes, int iteration_limit, int soak);