#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <unistd.h>
#include <syscall.h>
#include <errno.h>
#include <linux/random.h>

#include <math.h>

#include "tczero.h"
#include "cbc.h"
#include "birthday_attack.h"

#define SIZE_MESSAGE_BITS 512   //nombre de bits
#define SIZE_MESSAGE 64       //nombre d'octets (=1024/8)

//-----------------------------------------------
uint64_t key_generator(){

    uint64_t num;

    num = rand();
    num = (num << 32) | rand();

    return num;
}
//-----------------------------------------------
//Used to test encryption and decryption
int main(){

    //GENERATION DU MESSAGE + INITIALISATION
    srand(time(NULL));

    long long int taille_message=(pow(2,HALF_BLOCK_SIZE)+1)*HALF_BLOCK_SIZE*2 ;
    uint8_t M[SIZE_MESSAGE] = "coucou, je suis un message très important à crypter !"; //rappel : les caractères sont sur 8bits.

	uint64_t key[2];
	uint8_t* pt = (uint8_t*)M;
    
    //uint8_t* ct = malloc(SIZE_MESSAGE*sizeof(uint8_t)); //rappel : allocation de 512bits
    uint8_t* ct = malloc(taille_message*sizeof(uint8_t));
    uint8_t *pt_dec = malloc(SIZE_MESSAGE*sizeof(uint8_t));
	
    

    printf("-------------------------------------\n");
    printf("Message de base : %s (Taille : %d o)\n", pt, SIZE_MESSAGE);
    
    //GENERATION DE LA CLEF
    key[0] = key_generator();
    key[1] = key_generator();
    printf("Clé générée : %lu %lu\n", key[0], key[1]);

    //VERIFICATION 0 : le cryptage marche-il ?
    printf("-------------------------------------\n");
    cbc_enc(key, pt, ct, SIZE_MESSAGE_BITS);
    printf("Message crypté :\n");
    print_only_the_printable((char*)ct, SIZE_MESSAGE);

    //VERIFICATION 1 : le cryptage est-il non-déterministe ?
    printf("-------------------------------------\n");
    test_cbc_enc_determinism(key, pt, SIZE_MESSAGE) ? printf("[DETERMINISTE]\n") : printf("[NON-DETERMINISTE]\n");

    //VERIFICATION 2 : Le décryptage marche-il ?
    cbc_dec(key, ct, pt_dec, SIZE_MESSAGE_BITS);
    printf("Message décrypté : %s\n", (char*)pt_dec);
    printf("-------------------------------------\n");

    free(ct);
    free(pt_dec);

    //ATTACK
    
    
    pt = creation_message();
    printf("pt = %d\n",pt[1]);
    cbc_enc(key, pt, ct,taille_message/8 );
    printf("le xor = %hhn\n",pt);


}
