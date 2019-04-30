#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "tczero.h"
#include "cbc.h"
#include "birthday_attack.h"

//-----------------------------------------------
int64_t attack(uint8_t *pt, uint8_t *ct, size_t ctlen){
    printf("je suis dans attack\n");
	int nb_index_for_block=2*HALF_BLOCK_SIZE/8; //combien de cases des pt ou ct représente un block
	//vérifier ou est la collision:
	//regarder tous les block du plain text pt[i]
	//regarder un autre blog du plain text pt[j]
	//comparer : pt[i] xor ct[i-1] et pt[j] xor ct[j-1]
	//si il son égaux on a trouver la collision don on renvois ct[i-1] xor ct[j-1]
	for(int i = nb_index_for_block;i<pow(2,HALF_BLOCK_SIZE)+1;i=i+nb_index_for_block){
		for(int j = i+nb_index_for_block ;j<pow(2,HALF_BLOCK_SIZE)+1; j=j+nb_index_for_block){
				if(XOR(pt,ct,i,j)){
                    return calcul_Xor(ct,i,j);
                }
		}
	}
	return -1;
}
//----------------------------------------------------------------
//crée un message de taille (2^n/2)+1*taille d'un block qui est la taille minimal du CBC on des collision apparait 
uint8_t *creation_message(){
    printf("je suis dans creation-message\n");
	long long int taille_message=(pow(2,HALF_BLOCK_SIZE)+1)*HALF_BLOCK_SIZE*2;
    uint8_t *pt=malloc(taille_message);
    int i =0;
    while(i<=taille_message){
        pt[i%8]=pt[i%8]+1;
        pt[i%8]=pt[i%8]<<1;
        i++;
    }
    return pt;
}
//--------------------------------------------------------------------
//test l'égalité entre les xor des block de pt et ct
int XOR(uint8_t *pt, uint8_t *ct, int x, int y ){
    printf("je suis dans XOR\n");
    int xor1=0;
    int xor2=0;

    for(int i=x; i<x+(HALF_BLOCK_SIZE*2)/8;i++){
        xor1=xor1+(pt[i] ^ ct[i-1]);
        xor1=xor1<<8;
    }

    for(int i=y; i<y+(HALF_BLOCK_SIZE*2)/8;i++){
        xor1=xor1+(pt[i] ^ ct[i-1]);
        xor2=xor2<<8;
    }
    return xor1==xor2;
}
//---------------------------------------------------------------------
//calcul de xor des block
uint64_t calcul_Xor(uint8_t *ct, int x, int y){
    printf("je suis dans calcul-Xor\n");
    int xor1=0;
    int xor2=0;

    for(int i=x; i<x+(HALF_BLOCK_SIZE*2)/8;i++){
        xor1=xor1+ct[i-1];
        xor1=xor1<<8;
    }

    for(int i=y; i<y+(HALF_BLOCK_SIZE*2)/8;i++){
        xor2=xor2+ct[i-1];
        xor2=xor2<<8;
    }
    return (xor1 ^ xor2);
}


//-----------------------------------------------
