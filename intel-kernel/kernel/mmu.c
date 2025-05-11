/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

static pd_entry_t* kpd = (pd_entry_t*)KERNEL_PAGE_DIR;
static pt_entry_t* kpt = (pt_entry_t*)KERNEL_PAGE_TABLE_0;

static const uint32_t identity_mapping_end = 0x003FFFFF;
static const uint32_t user_memory_pool_end = 0x02FFFFFF;

static paddr_t next_free_kernel_page = 0x100000;
static paddr_t next_free_user_page = 0x400000;

/**
 * kmemset asigna el valor c a un rango de memoria interpretado
 * como un rango de bytes de largo n que comienza en s
 * @param s es el puntero al comienzo del rango de memoria
 * @param c es el valor a asignar en cada byte de s[0..n-1]
 * @param n es el tamaño en bytes a asignar
 * @return devuelve el puntero al rango modificado (alias de s)
*/
static inline void* kmemset(void* s, int c, size_t n) {
  uint8_t* dst = (uint8_t*)s;
  for (size_t i = 0; i < n; i++) {
    dst[i] = c;
  }
  return dst;
}

/**
 * zero_page limpia el contenido de una página que comienza en addr
 * @param addr es la dirección del comienzo de la página a limpiar
*/
static inline void zero_page(paddr_t addr) {
  kmemset((void*)addr, 0x00, PAGE_SIZE);
}


void mmu_init(void) {}


/**
 * mmu_next_free_kernel_page devuelve la dirección física de la próxima página de kernel disponible. 
 * Las páginas se obtienen en forma incremental, siendo la primera: next_free_kernel_page
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de kernel
 */
paddr_t mmu_next_free_kernel_page(void) {
  paddr_t actual = next_free_kernel_page;
  next_free_kernel_page += PAGE_SIZE;
  return actual;
}

/**
 * mmu_next_free_user_page devuelve la dirección de la próxima página de usuarix disponible
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de usuarix
 */
paddr_t mmu_next_free_user_page(void) {
  paddr_t actual = next_free_user_page;
  next_free_user_page += PAGE_SIZE;
  return actual;
}

/**
 * mmu_init_kernel_dir inicializa las estructuras de paginación vinculadas al kernel y
 * realiza el identity mapping
 * @return devuelve la dirección de memoria de la página donde se encuentra el directorio
 * de páginas usado por el kernel
 */
paddr_t mmu_init_kernel_dir(void) {

  zero_page(KERNEL_PAGE_DIR);
  zero_page(KERNEL_PAGE_TABLE_0);

  kpd[0].attrs = MMU_W | MMU_P;
  kpd[0].pt = KERNEL_PAGE_TABLE_0 >> 12;

  for (int i = 0; i < 1024; i++) {
    kpt[i].attrs =  MMU_W | MMU_P;
    kpt[i].page = i;
  }

  return KERNEL_PAGE_DIR;
}

/**
 * mmu_map_page agrega las entradas necesarias a las estructuras de paginación de modo de que
 * la dirección virtual virt se traduzca en la dirección física phy con los atributos definidos en attrs
 * @param cr3 el contenido que se ha de cargar en un registro CR3 al realizar la traducción
 * @param virt la dirección virtual que se ha de traducir en phy
 * @param phy la dirección física que debe ser accedida (dirección de destino)
 * @param attrs los atributos a asignar en la entrada de la tabla de páginas
 */
void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {
  pd_entry_t* page_dir = CR3_TO_PAGE_DIR(cr3);
  int32_t pd_index = VIRT_PAGE_DIR(virt);
  int32_t pt_index = VIRT_PAGE_TABLE(virt);

  // si no está presente, es decir P == 0, tenemos que pedir la página
  if (page_dir[pd_index].attrs & MMU_P == 0x0) {
    paddr_t next_page_table = mmu_next_free_kernel_page();
    zero_page(next_free_kernel_page);
    page_dir[pd_index].pt = (next_page_table >> 12); // Me importaban los 20 bits de arriba  

  }
  // Ahora si podemos mapearla
  pt_entry_t* actual_page_table = (page_dir[pd_index].pt << 12); 
  page_dir[pd_index].attrs |= attrs |MMU_P; 
  
  actual_page_table[pt_index].page = (phy >> 12); // también importaban los 20 de arriba
  actual_page_table[pt_index].attrs = attrs | MMU_P; 
  tlbflush(); // limpiamos caché de direcciones viejas
}

/**
 * mmu_unmap_page elimina la entrada vinculada a la dirección virt en la tabla de páginas correspondiente
 * @param virt la dirección virtual que se ha de desvincular
 * @return la dirección física de la página desvinculada
 */
paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {
  pd_entry_t* page_dir = CR3_TO_PAGE_DIR(cr3);
  int32_t pd_index = VIRT_PAGE_DIR(virt);
  int32_t pt_index = VIRT_PAGE_TABLE(virt);

  // si la page directory no está presente, es decir P == 0, no hay nada por borrar
  if ((page_dir[pd_index].attrs & MMU_P) == 0x0) {
    return 0;
  }
    
  pt_entry_t* actual_page_table = (page_dir[pd_index].pt << 12); 
  pt_entry_t* actual_entry_table = actual_page_table[pt_index].attrs;

    // si la entry table no está presente, tampoco hay nada por eliminar
  if ((actual_entry_table->attrs & MMU_P)== 0x0) {
    return 0;
  }
  
  paddr_t un_map = MMU_ENTRY_PADDR(actual_entry_table->page);
  actual_entry_table->attrs = 0x0; // ya no está presente
  paddr_t res = un_map;
  tlbflush(); // limpiamos cache de direcciones viejas

  return res;
}

#define DST_VIRT_PAGE 0xA00000
#define SRC_VIRT_PAGE 0xB00000

/**
 * copy_page copia el contenido de la página física localizada en la dirección src_addr a la página física ubicada en dst_addr
 * @param dst_addr la dirección a cuya página queremos copiar el contenido
 * @param src_addr la dirección de la página cuyo contenido queremos copiar
 *
 * Esta función mapea ambas páginas a las direcciones SRC_VIRT_PAGE y DST_VIRT_PAGE, respectivamente, realiza
 * la copia y luego desmapea las páginas. Usar la función rcr3 definida en i386.h para obtener el cr3 actual
 */
void copy_page(paddr_t dst_addr, paddr_t src_addr) {
  uint32_t attrs = MMU_P | MMU_W; // necesito pasarle los atributos a la funcion
  uint32_t cr3 = rcr3(); // me devuelve el cr3

  mmu_map_page(cr3, DST_VIRT_PAGE, dst_addr, attrs); // DST_VIRT_PAGE y SRC_VIRT_PAGE son las declaradas de arriba
  mmu_map_page(cr3, SRC_VIRT_PAGE, src_addr, attrs);

  // ya con las dos direcciones, podemos copiar
  uint32_t* virtual_dest = DST_VIRT_PAGE;
  uint32_t* virtual_src = SRC_VIRT_PAGE;

  for (int i = 0; i < PAGE_SIZE; i++) {
    virtual_dest[i] = virtual_src[i];
  }

  mmu_unmap_page(cr3, DST_VIRT_PAGE);
  mmu_unmap_page(cr3, SRC_VIRT_PAGE);
  
}

 /**
 * mmu_init_task_dir inicializa las estructuras de paginación vinculadas a una tarea cuyo código se encuentra en la dirección phy_start
 * @pararm phy_start es la dirección donde comienzan las dos páginas de código de la tarea asociada a esta llamada
 * @return el contenido que se ha de cargar en un registro CR3 para la tarea asociada a esta llamada
 */

// todas tienen la misma Identity  (kernel y área kernel libre), on demand, el shared (memoria compartida)
// luego cada una tiene su pila asignada
paddr_t mmu_init_task_dir(paddr_t phy_start){
  paddr_t cr3 = mmu_next_free_kernel_page(); // pedimos página para cr3
  zero_page(cr3); // lo limpiamos por si tiene basura

  // hacemos el identity mapping para el todo el kernel (incluido el área libre)
  for(uint32_t i = 0; i < 1024; i++){
    mmu_map_page(cr3, i * PAGE_SIZE, i * PAGE_SIZE, MMU_W | MMU_P);
  }
  // mapeamos las dos páginas de código como solo lectura. Son nivel usuario
  mmu_map_page(cr3, TASK_CODE_VIRTUAL, phy_start, MMU_U | MMU_P);
  mmu_map_page(cr3, TASK_CODE_VIRTUAL + PAGE_SIZE, phy_start + PAGE_SIZE, MMU_U | MMU_P);

  // mapeamos el stack como lectura-escritura. El stack debe tener nivel usuario
  paddr_t stack = mmu_next_free_user_page(); // pedimos una página libre para el stack
  mmu_map_page(cr3, TASK_STACK_BASE - PAGE_SIZE, stack, MMU_U | MMU_W | MMU_P);

  // mapeamos la página de memoria compartida como solo lectura de nivel 3
  // direccion fisica de la pagina de memoria compartida (SHARED es 0x0001D000)
  mmu_map_page(cr3, TASK_SHARED_PAGE, SHARED, MMU_U | MMU_P);

  return cr3;
}   

// COMPLETAR: devuelve true si se atendió el page fault y puede continuar la ejecución 
// y false si no se pudo atender
bool page_fault_handler(vaddr_t virt) {
  print("Atendiendo page fault...", 0, 0, C_FG_WHITE | C_BG_BLACK);
  
  // Chequeemos si el acceso fue dentro del area on-demand
  if (ON_DEMAND_MEM_START_VIRTUAL<=virt || virt<=ON_DEMAND_MEM_END_VIRTUAL) {
    // En caso de que si, mapear la pagina
    // Mapeamos como de lectura-escritura a nivel usuario
    mmu_map_page(rcr3(), virt, ON_DEMAND_MEM_START_PHYSICAL, MMU_U | MMU_W | MMU_P);
    return true;
  
  } else { 
    print("No en la memoria on demand", 0, 2, C_FG_WHITE | C_BG_BLACK);
  }

  return false;
}
