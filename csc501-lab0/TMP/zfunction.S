		.text
		.globl zfunction
zfunction:
		pushl 	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %eax
		andl	$0xfffc01ff, %eax
		shr	$8, %eax
		orl	$0xff000000, %eax
		movl	%ebp, %esp
		popl	%ebp
		ret
