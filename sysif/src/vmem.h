#ifndef VMEM_H0
#define VMEM_H

// Taille d'une page
#define PAGE_SIZE 4096
// Nombre d'entrées de la table des pages de N2
#define SECON_LVL_TT_COUN 256
// Taille de table N2 en Octets
#define SECON_LVL_TT_SIZE 1024
// Nombre d'entrées de la table des pages de N1
#define FIRST_LVL_TT_COUN 4096
// Taille de table N1 en Octets
#define FIRST_LVL_TT_SIZE 16384


unsigned int init_kern_translation_table(void);

#endif






 
