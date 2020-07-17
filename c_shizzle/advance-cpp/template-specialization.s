	.file	"template-specialization.cpp"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"a: "
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB1567:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rsi, %rbp
	movl	$10, %edx
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movq	8(%rsi), %rdi
	xorl	%esi, %esi
	call	strtol@PLT
	movq	16(%rbp), %rdi
	movl	$10, %edx
	xorl	%esi, %esi
	movq	%rax, %rbx
	call	strtol@PLT
	pxor	%xmm1, %xmm1
	cvtsi2sd	%eax, %xmm1
#APP
# 55 "template-specialization.cpp" 1
	# add_op begin
# 0 "" 2
#NO_APP
	pxor	%xmm0, %xmm0
	movsd	%xmm1, 8(%rsp)
	cvtsi2sd	%ebx, %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, (%rsp)
#APP
# 57 "template-specialization.cpp" 1
	# add_op end
# 0 "" 2
#NO_APP
	leaq	.LC0(%rip), %rsi
	leaq	_ZSt4cout(%rip), %rdi
	movl	$3, %edx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movsd	(%rsp), %xmm0
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZNSo9_M_insertIdEERSoT_@PLT
	movq	%rax, %rdi
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@PLT
#APP
# 61 "template-specialization.cpp" 1
	# sub_op begin
# 0 "" 2
#NO_APP
	movsd	(%rsp), %xmm0
	movsd	8(%rsp), %xmm1
	subsd	%xmm1, %xmm0
	movsd	%xmm0, (%rsp)
#APP
# 63 "template-specialization.cpp" 1
	# sub_op begin
# 0 "" 2
#NO_APP
	leaq	.LC0(%rip), %rsi
	leaq	_ZSt4cout(%rip), %rdi
	movl	$3, %edx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movsd	(%rsp), %xmm0
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZNSo9_M_insertIdEERSoT_@PLT
	movq	%rax, %rdi
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1567:
	.size	main, .-main
	.p2align 4,,15
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB2059:
	.cfi_startproc
	leaq	_ZStL8__ioinit(%rip), %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE2059:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
