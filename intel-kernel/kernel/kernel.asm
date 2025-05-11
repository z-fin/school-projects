; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start


; COMPLETAR - Agreguen declaraciones extern según vayan necesitando

extern A20_enable
extern GDT_DESC
extern screen_draw_layout

extern idt_init
extern IDT_DESC
extern pic_enable
extern pic_reset
extern mmu_init_kernel_dir
extern mmu_init_task_dir
extern tss_init
extern sched_init
extern tasks_init
extern tasks_screen_draw


; COMPLETAR - Definan correctamente estas constantes cuando las necesiten
%define GDT_IDX_CODE_0 1
%define GDT_IDX_DATA_0 3

%define CS_RING_0_SEL 0x08  
%define DS_RING_0_SEL 0x18    

%define C_FG_LIGHT_MAGENTA (0xD)

%define GDT_IDX_TASK_INITIAL  11
%define GDT_IDX_TASK_IDLE     12
%define SEL_TSS_INITIAL (GDT_IDX_TASK_INITIAL << 3) 
%define SEL_TSS_IDLE (GDT_IDX_TASK_IDLE << 3) 


BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; COMPLETAR - Deshabilitar interrupciones

    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; COMPLETAR - Imprimir mensaje de bienvenida - MODO REAL
    ; (revisar las funciones definidas en print.mac y los mensajes se encuentran en la
    ; sección de datos)

    print_text_rm start_rm_msg, start_rm_len, C_FG_LIGHT_MAGENTA, 20, 20

    ; COMPLETAR - Habilitar A20
    ; (revisar las funciones definidas en a20.asm)

    call A20_enable

    ; COMPLETAR - Cargar la GDT
    
    lgdt [GDT_DESC]
    
    ; COMPLETAR - Setear el bit PE del registro CR0

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; COMPLETAR - Saltar a modo protegido (far jump)
    ; (recuerden que un far jmp se especifica como jmp CS_selector:address)
    ; Pueden usar la constante CS_RING_0_SEL definida en este archivo

    jmp CS_RING_0_SEL:modo_protegido

BITS 32
modo_protegido:
    ; COMPLETAR - A partir de aca, todo el codigo se va a ejectutar en modo protegido
    ; Establecer selectores de segmentos DS, ES, GS, FS y SS en el segmento de datos de nivel 0
    ; Pueden usar la constante DS_RING_0_SEL definida en este archivo

    mov ax, DS_RING_0_SEL ;por cuestiones de seguridad hay que hacerlo de esta manera
    mov DS, ax
    mov ES, ax
    mov GS, ax
    mov FS, ax
    mov SS, ax

    ; COMPLETAR - Establecer el tope y la base de la pila
    mov ebp, 0x25000
    mov esp, 0x25000

    ; COMPLETAR - Imprimir mensaje de bienvenida - MODO PROTEGIDO

    print_text_pm start_pm_msg, start_pm_len, C_FG_LIGHT_MAGENTA, 54, 45


    ; COMPLETAR - Inicializar pantalla

    call screen_draw_layout
   
;; -------------------------------------------------------------------------- ;;
; Interrupciones

    call idt_init ; Para inicializar las entradas en la IDT
    lidt [IDT_DESC]

; Inicialización correspondiente para los PICS
    call pic_reset   ; // remapea el PIC
    call pic_enable  ; // habilita el PIC
    sti              ; // habilita las interrupciones (setea el flag)

; acelera el clock
    ; El PIT (Programmable Interrupt Timer) corre a 1193182Hz.
    ; Cada iteracion del clock decrementa un contador interno, cuando éste llega
    ; a cero se emite la interrupción. El valor inicial es 0x0 que indica 65536,
    ; es decir 18.206 Hz
    mov ax, 4000
    out 0x40, al
    rol ax, 8
    out 0x40, al

; Inicializacion del directorio
    call mmu_init_kernel_dir
    mov eax, 0x25000
    mov cr3, eax

; Activacion de paginacion
    mov eax, cr0       
    or eax, 0x80000000 
    mov cr0, eax

; Tasks
    call tss_init
    call tasks_screen_draw ; Prepara la pantalla para nuestras tareas
    mov ax, SEL_TSS_INITIAL
    ltr ax ; cargamos el registro task register con el selector de segmento de la tarea inicial
    ; forzamos el cambio de contexto.
    ; es decir, manualmente le decimos "Estas ejecutando esta tarea".
    ; Así al saltar a la tarea idle tiene una TSS en la que guardar el contexto.
    call sched_init
    call tasks_init

    sti ; habilitar interrupciones
    
    jmp SEL_TSS_IDLE:0 ; saltamos a la tarea idle

   ; Ciclar infinitamente 
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

%include "a20.asm"
