#pragma once

const int TAM_KERNEL = 3;

#ifdef DEFINE_KERNEL
double window[TAM_KERNEL][TAM_KERNEL] = {
	{ 1, 2, 1 },
	{ 2, 8, 2},
	{ 1, 2, 1}
 };
#endif