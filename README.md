# Projet Cryptologie

## PREMIERE PARTIE [Manal]
### Cryptage

![alt text](https://upload.wikimedia.org/wikipedia/commons/thumb/8/80/CBC_encryption.svg/601px-CBC_encryption.svg.png "Cryptage par CBC")

Soit un message M que nous souhaitons crypter, morceau par morceau. Ce message M, codé sur x bits sera divisé en plusieurs bouts (les plaintext) de 64 bits (le dernier plaintext à traiter contiendra les bits restants.) Pour chaque bloc de chiffrement (fourni dans tczero.h), on donne en paramètre des entrées de 64 bits : 
- Un plaintext qui aura subit au préalable un XOR avec un vecteur de 64 bits aussi. Ce vecteur sera IV pour le premier bloc, et le cyphertext i-1 pour les autres blocs i.
- La clef
Elle donnera en sortie un cyphertext contenant IV sur ses 64x2 premiers bits, et le plaintext chiffré sur les autres bits. Pour chaque bloc i, on donne le plaintext i, qui, une fois tous chiffrés, donnera le message complet M', chiffré. On remarquera que le plaintext subit au préalable un XOR avec un vecteur de 64 bits aussi. Ce vecteur sera IV (généré aléatoirement) pour le premier bloc, et le cyphertext du bloc d'avant pour les autres blocs.


### Décryptage

![alt text](https://upload.wikimedia.org/wikipedia/commons/thumb/2/2a/CBC_decryption.svg/601px-CBC_decryption.svg.png "Cryptage par CBC")

Là encore, pas plus compliqué que le cryptage : Le message chiffré M' est divisé en part de 64 bits. Chaque part est donnée en entrée (avec la clef) au bloc de chiffrement de décryptage. De ce bloc en ressort :
- le (plaintext XOR IV) dans le cas du premier bloc.
- le (plaintext XOR le cyphertext du bloc d'avant) dans le cas des autres blocs.

Pour retrouver le premier plaintext, il nous faut IV, récupéré dans les 64x2 premiers bits du cyphertext.

### Concernant le random
Je suis sous un linux 3.16 (version installée sur les ordinateurs de l'im2ag), qui ne semble pas équipé des fonctions de randomisation plus sophistiqués (comme getrandom() ou le syscall SYS_getrandom, qui ne sont disponibles que depuis la version 3.17). J'utilise donc rand() à défaut d'avoir mieux, malgré qu'il soit pseudo-aléatoire et donc prévisible. 

## DEUXIEME PARTIE [Jules]
### Question 1
Une birthday attack est une attaque cryptographique qui, au lieu de chercher la clef, utilise le nombre de blocks pour trouver les collisions entre les blocs de plaintext et les blocs du cyphertext. Afin d'avoir des relations xor identiques entre les deux blocs du plaintext et les deux blocs cryptés, il faut récupérer deux cyphertext et avec eux essayer de retrouver le XOR des deux plaintext.

Le cbc est donc sécure pour 2^n/2 blocs avec la taille des blocs (dans la 1er question 32).

### Question 2
Ici n est la taille du bloc, soit 2*HALF_BLOCK_SIZE. Nous avons :
- hashtable qui décompose le message en (2^n/2)+1 blocs, et qui récupère les blocs cryptés dès qu'il y a un collision dans cette hashtable.
- Au moins (2^n/2)+1 blocs.
- Oui, cbc affirme avoir une sécurité de 2^n/2.

### Application numérique
Q2. n=32
	-hashtable qui décompose le message en (2^n/2)+1=65537 block et donc des qu'il y a un collision dans cette hashtable on récupère les block crypté
	-au moin un plain text de ((2^n/2)+1)*32=2097184 bit.
	-oui un cbc avec des block de 32bit affirme avoir une sécurité des plaint text de (2^n/2)*32=65536 block=2097152 bit.

Q3. n=50
	-hashtable qui décompose le message en (2^n/2)+1=33554432 block et donc des qu'il y a un collision dans cette hashtable on récupère les block crypté
	-au moin un plain text de ((2^n/2)+1)*50=1677721600 bit.
	-oui un cbc avec des block de 32bit affirme avoir une sécurité des plaint text de (2^n/2)*50 bit=33554431 block=1677721550 bit.

Q4. n=64
	-hashtable qui décompose le message en (2^n/2)+1=4294967297 block et donc des qu'il y a un collision dans cette hashtable on récupère les block crypté
	-au moin un plain text de ((2^n/2)+1)*64=274877907008 bit.
	-oui un cbc avec des block de 32bit affirme avoir une sécurité des plaint text de (2^n/2)*64=4294967296 block=274877906944 bit.

Q5. n=80
	-il faudrait une hashtable de l'ordre de 10^12 case ce qui n'est pas accecible comme mémoire sur un ordinateur classique.
	n=128
	-il faudrait une hashtable de l'ordre de 10^19 case ce qui n'est pas accecible comme mémoire sur un ordinateur quel qu'il soit.




