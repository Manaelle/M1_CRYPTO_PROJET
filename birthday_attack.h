#ifndef B_ATTACK
#define B_ATTACK

//-----------------------------------------------
int64_t attack(uint8_t *pt, uint8_t *ct, size_t ctlen);

uint8_t *creation_message();

int XOR(uint8_t *pt, uint8_t *ct, int x, int y );

uint64_t calcul_Xor(uint8_t *ct, int i, int j);

//-----------------------------------------------

#endif
