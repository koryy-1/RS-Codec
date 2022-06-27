#pragma once

void calc_n_k(int mm, int tt, int& nn, int& kk);

void set_m_t_p(int mm, int tt, int* pp);

int *generate_gf(int* l_alpha_to, int* l_index_of);

int *gen_poly();

int *encode_rs(int* inf_data, int* b);

int *decode_rs(int* recd);