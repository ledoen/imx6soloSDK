#ifndef __CORE_CA7_H
#define __CORE_CA7_H
#include <stdint.h>
#include <stddef.h>

#define   __I     volatile const       /*!< Defines 'read only' permissions */
#define   __O     volatile             /*!< Defines 'write only' permissions */
#define   __IO    volatile             /*!< Defines 'read / write' permissions */

#if defined(__GNUC__)
  #define FORCEDINLINE  __attribute__((always_inline))
#else 
  #define FORCEDINLINE
#endif

#if   defined ( __CC_ARM )
  #define __ASM            __asm                                      /*!< asm keyword for ARM Compiler */
  #define __INLINE         __inline                                   /*!< inline keyword for ARM Compiler */
  #define __STATIC_INLINE  static __inline

#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #define __ASM            __asm                                      /*!< asm keyword for ARM Compiler */
  #define __INLINE         __inline                                   /*!< inline keyword for ARM Compiler */
  #define __STATIC_INLINE  static __inline

#elif defined ( __GNUC__ )
  #define __ASM            __asm                                      /*!< asm keyword for GNU Compiler */
  #define __INLINE         inline                                     /*!< inline keyword for GNU Compiler */
  #define __STATIC_INLINE  static inline

#elif defined ( __ICCARM__ )
  #define __ASM            __asm                                      /*!< asm keyword for IAR Compiler */
  #define __INLINE         inline                                     /*!< inline keyword for IAR Compiler. Only available in High optimization mode! */
  #define __STATIC_INLINE  static inline

#elif defined ( __TMS470__ )
  #define __ASM            __asm                                      /*!< asm keyword for TI CCS Compiler */
  #define __STATIC_INLINE  static inline

#elif defined ( __TASKING__ )
  #define __ASM            __asm                                      /*!< asm keyword for TASKING Compiler */
  #define __INLINE         inline                                     /*!< inline keyword for TASKING Compiler */
  #define __STATIC_INLINE  static inline

#elif defined ( __CSMC__ )
  #define __packed
  #define __ASM            _asm                                      /*!< asm keyword for COSMIC Compiler */
  #define __INLINE         inline                                    /*!< inline keyword for COSMIC Compiler. Use -pc99 on compile line */
  #define __STATIC_INLINE  static inline

#else
  #error Unknown compiler
#endif


#define __STRINGIFY(x) #x

#define __MCR(coproc, opcode_1, src, CRn, CRm, opcode_2)                          \
    __ASM volatile ("MCR " __STRINGIFY(p##coproc) ", " __STRINGIFY(opcode_1) ", " \
                    "%0, " __STRINGIFY(c##CRn) ", " __STRINGIFY(c##CRm) ", "      \
                    __STRINGIFY(opcode_2)                                         \
                    : : "r" (src) )

#define __MRC(coproc, opcode_1, CRn, CRm, opcode_2)                               \
  ({                                                                              \
    uint32_t __dst;                                                               \
    __ASM volatile ("MRC " __STRINGIFY(p##coproc) ", " __STRINGIFY(opcode_1) ", " \
                    "%0, " __STRINGIFY(c##CRn) ", " __STRINGIFY(c##CRm) ", "      \
                    __STRINGIFY(opcode_2)                                         \
                    : "=r" (__dst) );                                             \
    __dst;                                                                        \
  })
__attribute__((always_inline)) __STATIC_INLINE void __DSB(void)
{
  __ASM volatile ("dsb 0xF":::"memory");
}

__attribute__((always_inline)) __STATIC_INLINE void __ISB(void)
{
  __ASM volatile ("isb 0xF":::"memory");
}


FORCEDINLINE __STATIC_INLINE uint32_t __get_VBAR(void)
{
  return __MRC(15, 0, 12, 0, 0);
}

FORCEDINLINE __STATIC_INLINE void __set_VBAR(uint32_t vbar)
{
  __MCR(15, 0, vbar, 12, 0, 0);
}

FORCEDINLINE __STATIC_INLINE uint32_t __get_CBAR(void)
{
  return __MRC(15, 4, 15, 0, 0);
}





/*******************************************************************************
 *                 GIC Functions
 ******************************************************************************/


typedef struct
{
    uint32_t D_CTLR;              //!< Distributor Control Register.
    uint32_t D_TYPER;             //!< Interrupt Controller Type Register.
    uint32_t IIDR;              //!< Distributor Implementer Identification Register.
    uint32_t _reserved0[29];
    uint32_t IGROUPRn[8];       //!< Interrupt Group Registers.
    uint32_t _reserved1[24];
    uint32_t D_ISENABLER[32];    //!< Interrupt Set-Enable Registers.
    uint32_t D_ICENABLER[32];    //!< Interrupt Clear-Enable Registers.
    uint32_t D_ISPENDRn[32];      //!< Interrupt Set-Pending Registers.
    uint32_t D_ICPENDRn[32];      //!< Interrupt Clear-Pending Registers.
    uint32_t D_ICDABRn[32];       //!< Active Bit Registers.
    uint32_t _reserved2[32];
    uint8_t IPRIORITYRn[255 * sizeof(uint32_t)];  //!< Interrupt Priority Registers. (Byte accessible)
    uint32_t _reserved3;
    uint8_t D_ITARGETSRn[255 * sizeof(uint32_t)];   //!< Interrupt Processor Targets Registers. (Byte accessible)
    uint32_t _reserved4;
    uint32_t ICFGRn[64];        //!< Interrupt Configuration Registers.
    uint32_t _reserved5[128];
    uint32_t SGIR;              //!< Software Generated Interrupt Register
} GICD_Type;

typedef struct
{
    uint32_t C_CTLR;  //!< CPU Interface Control Register.
    uint32_t C_PMR;   //!< Interrupt Priority Mask Register.
    uint32_t C_BPR;   //!< Binary Point Register.
    uint32_t IAR;   //!< Interrupt Acknowledge Register.
    uint32_t EOIR;  //!< End of Interrupt Register.
    uint32_t RPR;   //!< Running Priority Register.
    uint32_t HPPIR; //!< Highest Priority Pending Interrupt Register.
    uint32_t ABPR;  //!< Aliased Binary Point Register. (only visible with a secure access)
    uint32_t _reserved[56];
    uint32_t IIDR;  //!< CPU Interface Identification Register.
} GICC_Type;

extern uint32_t regdata;


/* For simplicity, we only use group0 of GIC */
FORCEDINLINE __STATIC_INLINE void GIC_Init(void)
{
	uint32_t i;
	uint32_t irqRegs;
	GICD_Type *gic_d = (GICD_Type *)(__get_CBAR() + 0x1000UL);

	irqRegs = (gic_d->D_TYPER & 0x1FUL) + 1;

	//gic_d->D_ITARGETSRn[88] |= 1;	/*必须设置目标CPU*/
	
	/* On POR, all SPI is in group 0, level-sensitive and using 1-N model */

	/* Disable all PPI, SGI and SPI */
	for (i = 0; i < irqRegs; i++)
		gic_d->D_ICENABLER[i] = 0xFFFFFFFFUL;
	
	GICC_Type *gic_c = (GICC_Type *)(__get_CBAR() + 0x100UL);
	/* Make all interrupts have higher priority */
	gic_c->C_PMR = (0xFFUL << (8 - __GIC_PRIO_BITS)) & 0xFFUL;
	
	gic_c->C_BPR = 7 - __GIC_PRIO_BITS;

	/* Enable group0 signaling */
	gic_c->C_CTLR |= 1UL;
  
	/* Enable group0 distribution */
	gic_d->D_CTLR |= 1UL;
	
	//regdata = gic_d->D_ICPENDRn[2];
}

FORCEDINLINE __STATIC_INLINE void GIC_EnableIRQ(IRQn_Type IRQn)
{
	GICD_Type *gic_d = (GICD_Type *)(__get_CBAR() + 0x1000UL);
  
	gic_d->D_ISENABLER[((uint32_t)(int32_t)IRQn) >> 5] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
	gic_d->D_ITARGETSRn[IRQn] = 1;
}

#endif