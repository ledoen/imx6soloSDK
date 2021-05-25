.global _start

_start:
	/* 一级中断向量表 */
	ldr pc, =Reset_Handler		/* 复位中断 */
	ldr pc, =Undefine_Handler	/* 未定义指令中断 */
	ldr pc, =SVC_Handler		/* SVC中断 */
	ldr pc, =PrefAbort_Handler	/* 预取中止中断 */
	ldr pc, =DataAbort_Handler	/* 数据中止中断 */
	ldr pc, =NotUsed_Handler	/* 未使用中断 */
	ldr pc, =IRQ_Handler		/* IRQ中断 */
	ldr pc, =FIQ_Handler		/* FRQ中断 */
	
/* 复位中断处理函数
 * cpu上电进入的中断入口
 * 如果不使用中断，只需在此处写代码并跳转到mian函数即可
 * 本程序只使用到了复位中断和IRQ中断
*/
Reset_Handler:
	cpsid if						/* 关闭IRQ中断 */
	
	/* 关闭I/D Cache以及MMU */
	mrc p15, 0, r0, c1, c0, 0	/* 读取CP15协处理器的c1寄存器值到r0中 */
	bic r0, r0, #(1 << 12)		/* 清除c1的I位，关闭I Cache */
	bic r0, r0, #(1 << 2)		/* 清除c1的C位，关闭D Cache */
	bic r0, r0, #(1 << 1)		/* 清除c1的A位，关闭对齐检查 */
	bic r0, r0, #(1 << 11)		/* 清除c1的Z位，关闭分支预测 */
	bic r0, r0, #(1 << 0)		/* 清除c1的M位，关闭MMU */
	mcr p15, 0, r0, c1, c0, 0  	/* 将r0的值写入到CP15协处理器的c1寄存器中 */
	
	/* 设置中断向量表偏移 */
	ldr r0, =0x27800000
	
	dsb
	isb
	mcr p15, 0, r0, c12, c0, 0	/*将r0的值写入到CP15协处理器的c12寄存器中*/
	dsb
	isb
	
	/*关闭TLB*/
	mcr p15, 0, r0, c8, c7, 0
	
	/* 进入IRQ模式并设置其栈指针 */	
	cps #0x12					/* CPU进入IRQ运行模式 */
	ldr sp, =0x10600000			/* 设置IRQ模式下栈指针，大小为2MB */

	/* 进入System运行模式 */
    cps #0x1f       			/* SoC进入到SYS运行模式 */
    ldr sp, =0x10400000     	/* 设置IRQ模式下栈指针0x80400000，大小为2MB */
	
    /* SoC进入Supervisor运行模式 */
    cps #0x13       			/* SoC进入到SVC运行模式 */
    ldr sp, =0x10200000     	/* 设置IRQ模式下栈指针0x80200000，大小为2MB */

	cpsie i						/* 使能IRQ中断 */

	b main						/* 跳转到main函数执行 */
	
Undefine_Handler:
	ldr r0, =Undefine_Handler
	bx r0
	
SVC_Handler:
	ldr r0, =SVC_Handler
	bx r0
	
PrefAbort_Handler:
	ldr r0, =PrefAbort_Handler
	bx r0

DataAbort_Handler:
	ldr r0, =DataAbort_Handler
	bx r0
	
NotUsed_Handler:
	ldr r0, =NotUsed_Handler
	bx r0
	
IRQ_Handler:
	/* 中断发生时保护现场 */
    push {lr}           		/* 保存lr寄存器(中断发生时保存PC的值) */
    push {r0-r3, r12}   		/* 保存r0-r3,r12寄存器中的值 */

    mrs r0, spsr        		/* 读取spsr寄存器的值到R0寄存器中 */
    push {r0}           		/* 保存spsr寄存器 */
	
	mrc p15, 4, r1, c15, c0, 0  /* 读取CP15协处理器的c15寄存器值(保存GIC基地址) */
								/* r1=r1+0x100，得到GIC的CPU Interface端基地址 */
    add r1, r1, #0x100

    /**
     * CPU Interface端基地址加上0x0C得到
     * GICC_IAR寄存器地址，该寄存器保存着当前
     * 发生IRQ中断的中断号，需要根据这个中断号
     * 调用相应的中断服务函数。
     */
    ldr r0, [r1, #0x0C]     	/* 将GICC_IAR寄存器中的值保存到R0,待会还要用到 */
    push {r0, r1}           	/* 保存r0(发生IRQ的中断号)，r1(GIC的CPU Interface基地址) */
	
	cps #0x13               	/* SoC进入到SVC模式，允许其它中断再次进入 */

    push {lr}               	/* 保存SVC模式下的lr寄存器 */
    ldr r2, =system_irqhandler  /* 将C中断服务处理函数地址加载到R2寄存器 */
    blx r2                  	/* 调用C中断服务处理函数(参数为r0) */
	
	pop {lr}                	/* 执行完C中断服务处理函数，lr出栈 */
    cps #0x12               	/* SoC进入到IRQ模式 */
    pop {r0, r1}
    str r0, [r1, #0x10]     	/* 中断执行完成，写GICC_EOIR寄存器 */

    /* 中断执行完成需要恢复现场 */
    pop {r0}
    msr spsr_cxsf, r0   		/* 恢复spsr寄存器 */

    pop {r0-r3, r12}    		/* r0-r3,r12出栈 */
    pop {lr}            		/* lr出栈 */
    subs pc, lr, #4     		/* 将lr-4赋给pc寄存器 */
	
FIQ_Handler:
	ldr r0, =FIQ_Handler
	bx r0

