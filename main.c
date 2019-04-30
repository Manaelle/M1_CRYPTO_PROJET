#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "tczero.h"
#include "cbc.h"
#include "birthday_attack.h"

#define SIZE_MESSAGE_BITS 512   //nombre de bits total (SIZE_MESSAGE*8)
#define SIZE_MESSAGE 64       //nombre d'octets total

//-----------------------------------------------
uint64_t key_generator(){

    uint64_t num;

    //ATTENTION : Ces fonctions ne marchent pas sous les linux <= version 3.17 (je suis à la 3.16...)
    //getentropy((void*)&num, sizeof(uint64_t));
    //getrandom((void*)&num, sizeof(uint64_t),0);
    //syscall(__NR_getrandom, (void*)&num, sizeof(uint64_t),0);

    num = rand();
    num = (num << 32) | rand();

    return num;
}
//-----------------------------------------------
//Used to test encryption and decryption
int main(){

    //GENERATION DU MESSAGE + INITIALISATION
    cbc_init();
    uint8_t M[SIZE_MESSAGE] = "coucou, je suis un message très important à crypter !"; //rappel : les caractères sont sur 8bits.
	uint64_t key[2];
	uint8_t* pt = (uint8_t*)M;
    uint8_t* ct = malloc(SIZE_MESSAGE*sizeof(uint8_t)+2); //rappel : allocation de 512bits, le +2 est pour y stocker IV
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
    cbc_dec(key, ct, pt_dec, SIZE_MESSAGE_BITS+2);
    printf("Message décrypté : %s\n", (char*)pt_dec);


    //ATTAQUE ANNIVERSAIRE
    printf("-------------------------------------\n");
    long long int taille_message = (pow(2,HALF_BLOCK_SIZE)+1)*HALF_BLOCK_SIZE*2 ;
    printf("Nouvelle taille de message : %lld\n", taille_message);
    uint8_t* ct_ack = malloc(taille_message*sizeof(uint8_t)+2*sizeof(uint64_t)); //On ajoute +2*sizeof(uint64_t) pour y stocker IV (nécessaire pour que cbc_enc fonctionne)
    
    printf("Message pour l'attaque :\n");
    uint8_t* pt_ack = creation_message();
    print_only_the_printable((char*)pt_ack, taille_message);

    cbc_enc(key, pt_ack, ct_ack, taille_message/8);
    printf("le xor = %hhn\n",pt_ack);
    
    free(ct);
    free(pt_dec);
    free(ct_ack);
    free(pt_ack);

    return 0;

}
