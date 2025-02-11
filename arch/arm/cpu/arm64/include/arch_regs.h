/**
 * Copyright (c) 2013 Sukanto Ghosh.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * @file arch_regs.h
 * @author Sukanto Ghosh (sukantoghosh@gmail.com)
 * @brief common header file for CPU registers
 */
#ifndef _ARCH_REGS_H__
#define _ARCH_REGS_H__

#include <vmm_types.h>
#include <vmm_compiler.h>
#include <vmm_spinlocks.h>
#include <vmm_cpumask.h>
#include <cpu_defines.h>

struct arch_regs {
	/* X0 - X29 */
	u64 gpr[CPU_GPR_COUNT];
	/* Link Register (or X30) */
	u64 lr;
	/* Stack Pointer */
	u64 sp;
	/* Program Counter */
	u64 pc;
	/* PState/CPSR */
	u64 pstate;
} __packed;

typedef struct arch_regs arch_regs_t;

/* Note: This structure is accessed from assembly code
 * hence any change in this structure should be reflected
 * in relevant defines in cpu_defines.h
 */
struct arm_priv_ptrauth {
	u64 apiakeylo_el1;			/* 0x0 Pointer Authentication Key A for Instruction */
	u64 apiakeyhi_el1;			/* 0x8 Pointer Authentication Key A for Instruction */
	u64 apibkeylo_el1;			/* 0x10 Pointer Authentication Key B for Instruction */
	u64 apibkeyhi_el1;			/* 0x18 Pointer Authentication Key B forInstruction */
	u64 apdakeylo_el1;			/* 0x20 Pointer Authentication Key A for Data */
	u64 apdakeyhi_el1;			/* 0x28 Pointer Authentication Key A for Data */
	u64 apdbkeylo_el1;			/* 0x30 Pointer Authentication Key B for Data */
	u64 apdbkeyhi_el1;			/* 0x38 Pointer Authentication Key B for Data */
	u64 apgakeylo_el1;			/* 0x40 Pointer Authentication Key A for Code */
	u64 apgakeyhi_el1;			/* 0x48 Pointer Authentication Key A for Code */
};

/* Note: This structure is accessed from assembly code
 * hence any change in this structure should be reflected
 * in relevant defines in cpu_defines.h
 */
struct arm_priv_vfp {
	/* 64bit EL1/EL0 registers */
	u32 mvfr0;				/* 0x0 Media and VFP Feature Register 0 */
	u32 mvfr1;				/* 0x4 Media and VFP Feature Register 1 */
	u32 mvfr2;				/* 0x8 Media and VFP Feature Register 2 */
	u32 fpcr;				/* 0xC Floating-point Control Register */
	u32 fpsr;				/* 0x10 Floating-point Status Register */
	u32 fpexc32;			/* 0x14 32bit only registers, isn't suit for aarch64 */
	u64 fpregs[64];			/* 0x18 32x128bit floating point registers */
} __packed;

/* Note: This structure is accessed from assembly code
 * hence any change in this structure should be reflected
 * in relevant defines in cpu_defines.h
 */
struct arm_priv_sysregs {
	/* 64bit EL1/EL0 registers */
	u64 sp_el0;					/* 0x0 Stack Pointer (EL0) */
	u64 sp_el1;					/* 0x8 Stack Pointer (EL1) */
	u64 elr_el1;				/* 0x10 Exception Link Register (EL1) */
	u64 spsr_el1;				/* 0x18 Saved Program Status Register (EL1) */
	u64 midr_el1;				/* 0x20 Main ID Register */
	u64 mpidr_el1;				/* 0x28 Multiprocessor Affinity Register */
	u64 sctlr_el1;				/* 0x30 System Control Register (EL1) */
	u64 actlr_el1;				/* 0x38 Auxiliary Control Register (EL1) */
	u64 cpacr_el1;				/* 0x40 Architectural Feature Access Control Register */
	u64 ttbr0_el1;				/* 0x48 Translation Table Base Register 0 */
	u64 ttbr1_el1;				/* 0x50 Translation Table Base Register 1 */
	u64 tcr_el1;				/* 0x58 Translation Control Register (EL1) */
	u64 esr_el1;				/* 0x60 Exception Syndrome Register (EL1) */
	u64 far_el1;				/* 0x68 Fault Address Register (EL1) */
	u64 par_el1;				/* 0x70  Physical Address Register */
	u64 mair_el1;				/* 0x78 Memory Attribute Indirection Register(EL1) */
	u64 vbar_el1;				/* 0x80 Vector Base Address Register (EL1)*/
	u64 contextidr_el1;			/* 0x88 Context ID Register (EL1) */
	u64 tpidr_el0;				/* 0x90 EL0 Read/Write Software Thread ID Register*/
	u64 tpidr_el1;				/* 0x98 EL1 Software Thread ID Register*/
	u64 tpidrro_el0;			/* 0xA0 EL0 Read-Only Software Thread ID Register */
	u32 spsr_abt;				/* 0xA8 Saved Program Status Register (Abort mode) */
	u32 spsr_und;				/* 0xAC Saved Program Status Register (Undefined mode) */
	u32 spsr_irq;				/* 0xB0 Saved Program Status Register (IRQ mode) */
	u32 spsr_fiq;				/* 0xB4 Saved Program Status Register (FIQ mode) */
	u32 dacr32_el2;				/* 0xB8 Domain Access Control Register */
	u32 ifsr32_el2;				/* 0xBC Instruction Fault Status Register (EL2) */
	/* 32bit only ThumbEE registers */
	u32 teecr32_el1;			/* 0xC0 */
	u32 teehbr32_el1;			/* 0xC4 */
} __packed;

struct arm_priv {
	/* Internal CPU feature flags. */
	u32 cpuid;
	u64 features;
	/* Hypervisor context */
	vmm_spinlock_t hcr_lock;
	u64 hcr;	/* Hypervisor Configuration */
	u64 cptr;	/* Architectural Feature Trap Register */
	u64 hstr;	/* Hypervisor System Trap Register */
	/* EL1/EL0 sysregs */
	struct arm_priv_sysregs sysregs;
	vmm_cpumask_t dflush_needed;
	/* VFP & SMID context */
	struct arm_priv_vfp vfp;
	/* Pointer Authentication context */
	struct arm_priv_ptrauth ptrauth;
	/* Last host CPU on which this VCPU ran */
	u32 last_hcpu;
	/* Generic timer context */
	void *gentimer_priv;
	/* VGIC context */
	bool vgic_avail;
	void (*vgic_save)(void *vcpu_ptr);
	void (*vgic_restore)(void *vcpu_ptr);
	bool (*vgic_irq_pending)(void *vcpu_ptr);
	void *vgic_priv;
};

struct arm_guest_priv {
	/* Stage2 table */
	struct mmu_pgtbl *ttbl;
	/* PSCI version
	 * Bits[31:16] = Major number
	 * Bits[15:0] = Minor number
	 */
	u32 psci_version;
};

#define arm_regs(vcpu)		(&((vcpu)->regs))
#define arm_priv(vcpu)		((struct arm_priv *)((vcpu)->arch_priv))
#define arm_guest_priv(guest)	((struct arm_guest_priv *)((guest)->arch_priv))

#define arm_cpuid(vcpu)		(arm_priv(vcpu)->cpuid)
#define arm_set_feature(vcpu, feat) \
			(arm_priv(vcpu)->features |= (0x1ULL << (feat)))
#define arm_clear_feature(vcpu, feat) \
			(arm_priv(vcpu)->features &= ~(0x1ULL << (feat)))
#define arm_feature(vcpu, feat)	\
			(arm_priv(vcpu)->features & (0x1ULL << (feat)))

/**
 *  Instruction emulation support macros
 */
#define arm_cpsr(regs)		((u32)((regs)->pstate & 0xffffffff))
#define arm_pc(regs)		((regs)->pc)

/**
 *  Generic timers support macro
 */
#define arm_gentimer_context(vcpu)	(arm_priv(vcpu)->gentimer_priv)

/**
 *  VGIC support macros
 */
#define arm_vgic_setup(vcpu, __save_func, __restore_func, \
		       __irq_pending_func, __priv) \
	do { \
		arm_priv(vcpu)->vgic_avail = TRUE; \
		arm_priv(vcpu)->vgic_save = __save_func; \
		arm_priv(vcpu)->vgic_restore = __restore_func; \
		arm_priv(vcpu)->vgic_irq_pending = __irq_pending_func; \
		arm_priv(vcpu)->vgic_priv = __priv; \
	} while (0)
#define arm_vgic_cleanup(vcpu)	\
	do { \
		arm_priv(vcpu)->vgic_avail = FALSE; \
		arm_priv(vcpu)->vgic_save = NULL; \
		arm_priv(vcpu)->vgic_restore = NULL; \
		arm_priv(vcpu)->vgic_irq_pending = NULL; \
		arm_priv(vcpu)->vgic_priv = NULL; \
	} while (0)
#define arm_vgic_avail(vcpu)	(arm_priv(vcpu)->vgic_avail)
#define arm_vgic_save(vcpu)	\
	if (arm_vgic_avail(vcpu)) { \
		arm_priv(vcpu)->vgic_save(vcpu); \
	}
#define arm_vgic_restore(vcpu)	\
	if (arm_vgic_avail(vcpu)) { \
		arm_priv(vcpu)->vgic_restore(vcpu); \
	}
#define arm_vgic_irq_pending(vcpu)	\
	({ \
		bool __r = FALSE; \
		if (arm_vgic_avail(vcpu)) { \
			__r = arm_priv(vcpu)->vgic_irq_pending(vcpu); \
		} \
		__r; \
	})
#define arm_vgic_priv(vcpu)	(arm_priv(vcpu)->vgic_priv)

#endif
